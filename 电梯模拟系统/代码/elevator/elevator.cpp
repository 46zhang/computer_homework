#include"elevator.h"

extern  int MaxClientNum ;						//最大乘客人数
extern  int MaxRunTime;							//最大运行时间
extern  int NextClientInterTime;				//下一个乘客进入系统的时间
extern  WaitQueue FloorWaitQueue[2][5];			//每层楼有俩个等待队列，0向上,1向下
extern  int CallUp[5], CallDown[5];				//楼层的向上向下按钮

//判断该动作的时间周期是否到了
Status TimeUp(Elevator e)
{
	if (e->State == Idle && e->floor == 1)
		return FALSE;
	else
		if (e->StateCount<=0)
			return TRUE;
}

//没有人进入电梯
Status NobodyIn(Elevator e)
{
	if (e->State == GoingUp && FloorWaitQueue[0][e->floor].WaitClientNum == 0)
		return TRUE;
	else if (e->State == GoingDown && FloorWaitQueue[1][e->floor].WaitClientNum == 0)
		return TRUE;
	else
		return FALSE;
}
//没有人出电梯
Status NobodyOut(Elevator e)
{
	//没有 乘客坐这个楼层的电梯
	if (!e->CallCar[e->floor])
		return TRUE;
	//该层没有乘客或者走光
	else if (e->State == GoingUp && StackEmpty(e->Stack[e->floor]) == TRUE )
		return TRUE;
	//该层没有乘客出乘客
	else if (e->State == GoingDown && StackEmpty(e->Stack[e->floor]) == TRUE )
		return TRUE;
	else
		return FALSE;
}
//没人进出电梯
Status NobodyInOrOut(Elevator e)
{
	//没有 乘客坐这个楼层的电梯
	if (NobodyIn(e) == TRUE && NobodyOut(e) == TRUE)
		return TRUE;
	else
		return FALSE;
}

//是否要在下一层停留
Status StopNextFloor(Elevator e)
{
	//三种情况需要在下一层停留
	//下一层有乘客，下一层为目的层，下一层为终点
	if (e->State == GoingUp)
	{
		if (e->floor == MaxFloor)
			return TRUE;
		else if (CallUp[e->floor] == 1)
			return TRUE;
		else if (e->CallCar[e->floor] == 1)
			return TRUE;
		else
			return FALSE;
	}
	else
	{
		if (e->floor  == MinFloor)
			return TRUE;
		else if (CallDown[e->floor ] == 1)
			return TRUE;
		else if (e->CallCar[e->floor ] == 1)
			return TRUE;
		else
			return FALSE;
	}
}
//改变电梯的行为
void ElevatorChangeMove(Elevator e,int k)
{
	switch (e->move)
	{
	case Opening:
		e->move = Opened;
		e->StateCount = CloseTestTime;
		break;
	case Opened:
		//如果没有人进出或者电梯人数达到上限，则关门
		if (NobodyInOrOut(e) == TRUE || e->ClientNum == MaxClientNum)
		{
			e->move = Closing;
			e->StateCount = DoorTime;
		}
		else
			e->StateCount = CloseTestTime;
		break;
	case Closing:
		if (NobodyInOrOut(e) == TRUE)
		{
			e->move = Closed;
		}
		//如果关门期间有人进入，则开门
		else if(e->ClientNum<MaxClientNum)
		{	
			e->move = Opening;
			e->StateCount = DoorTime;
		}
		break;
	case Closed:
		//关门完成后判断是要向上移动还是向下移动还是停止
		if (e->State == GoingUp)
		{
			//清空响应事件
			if (-1 == CallUp[e->floor])
				CallUp[e->floor] = 0;
			int j;
			//判断是否需要继续往上
			for ( j = e->floor; j <= MaxFloor; j++)
			{
				//如果上层有目标楼层，则需要往上
				if (e->CallCar[j] == 1)
				{
					e->move = Accelerate;
					e->StateCount = AccelerateTime;
					break;
				}
			}//说明没有往上的目标楼层
			if (j > MaxFloor)
				e->move = Waiting, e->State = Idle;
		}
		else if (e->State == GoingDown)
		{
			//清空响应事件
			if (-1 == CallDown[e->floor])
				CallDown[e->floor] = 0;
			int j;
			//还有更底的目标楼层，加速启动
			for ( j = e->floor; j >=0; j--)
			{
				if (1 == e->CallCar[j])
				{
					e->move = Accelerate;
					e->StateCount = AccelerateTime;
					break;
				}
			}
			if (j < 0)
				e->move = Waiting, e->State = Idle;
		}
		else
			e->move = Waiting;
		break;
	case Accelerate:
		e->move = Moving;
		//上升时间跟下降时间不一样,要区分开
		if (e->State == GoingUp)
			e->StateCount = UpTime;
		else
			e->StateCount = DownTime;
		break;
	case Moving:	//完成移动
		//先消隐电梯再显示电梯
		DisappearEvelatorBody(e,k,e->floor);
		if (e->State == GoingUp)
		{
			e->floor++;
			if (TRUE == StopNextFloor(e))
			{
				e->move = SlowDown;
				e->StateCount = UpSlowTime;
			}
			//否则继续上升
			else
				e->StateCount = UpTime;
		}
		else if(GoingDown==e->State)
		{
			e->floor--;
			if (TRUE == StopNextFloor(e))
			{
				e->move = SlowDown;
				e->StateCount = DownSlowTime;
			}
			//继续下降
			else
				e->StateCount = DownTime;
		}
		ShowElevatorBody(e,k,e->floor);
		break;
	case SlowDown:
		//到达该层，清空电梯内的目的楼层标志位
		e->CallCar[e->floor] = 0;
		//判断是否需要转变方向
		if (e->State == GoingUp)
		{
			int need = 1;
			//还没到达目标层，无需转向
			for (int j = e->floor; j < MaxFloor; j++)
			{
				if (1 == e->CallCar[j])
				{
					need = 0;
					break;
				}
			}
			//否则改变方向,判断高层是否有请求，如果没有，改变方向
			if (1==need&& -1==HigherRequires(e) )
				e->State = GoingDown;;

		}
		else if (e->State = GoingDown)
		{
			int need = 1;
			//没达到最低目标层无需改向
			for (int j = e->floor; j > 0; j--)
			{
				if (1 == e->CallCar[j])
				{
					need = 0;
					break;
				}
			}
			//否则，如若底层还没响应改变状态
			if (1 == need && -1 == LowerRequires(e))
				e->State = GoingUp;
		}
		
		e->move = Opening;
		e->StateCount = DoorTime;
		
		break;
	case Waiting: //等待状态有Control函数进行下一行动的判断
		break;
	}
}

//有向上或者向下的请求，返回最近一层的电梯请求
int HasUpOrDownRequires(Elevator e)
{
	int i, j;
	for (j = e->floor-1, i = e->floor+1; j >= 0 || i <= MaxFloor; j--, i++)
	{
		if (j>=0&&(CallDown[j]==1 || CallUp[j]==1))
			return j;
		if (i <= MaxFloor && (CallDown[i] == 1 || CallUp[i] == 1))
			return i;
	}
}
//高层是否有请求，有返回该楼层，没有返回-1
int HigherRequires(Elevator e)
{
	int floor;
	for (floor = e->floor; floor <= MaxFloor; floor++)
	{
		if (CallUp[floor])
			return floor;
	}

	return -1;
}
//底层是否有请求,有返回该楼层，没有返回-1
int LowerRequires(Elevator e)
{
	int floor;
	for (floor = e->floor; floor >= 0; floor--)
	{
		if (CallDown[floor])
			return floor;
	}
	return -1;
}
//电梯向下移动
Status ElevatorMoveDown(Elevator &e)
{
	int floor = LowerRequires(e);
	if (floor != -1)
	{
		//如果请求的是该层电梯，直接打开电梯门
		if (e->floor == floor)
			e->move = Opening;
		else
		{
			e->CallCar[floor] = 1;
			e->move = Accelerate;
			//响应该层的请求
			CallDown[floor] = -1;
		}
		return TRUE;
	}
	return FALSE;
}
//电梯向上移动
Status ElevatorMoveUp(Elevator &e)
{
	int floor = HigherRequires(e);
	//高楼有请求
	if (floor != -1)
	{
		//如果响应的电梯为本层，则直接开门
		if (floor == e->floor)
			e->move = Opening;
		else
		{
			e->CallCar[floor] = 1;
			e->move = Accelerate;
			//响应标志
			CallUp[floor] = -1;
		}
		return TRUE;
	}
	else
		return FALSE;
	

}
//控制函数，用来判断电梯的下一个移动方向
//该步骤的思想为先让0号电梯去响应最近的一台电梯
//再让1号电梯去响应最近的一台电梯
//通过响应事件，把响应层的电梯按钮设置为-1，避免俩台电梯竞争同一资源
void Controler(Elevator E[])
{
	for (int i = 0; i <= 1; i++)
	{//如果i号电梯位于等待状态
		if (E[i]->move == Waiting)
		{
			//计算最近的上下电梯的花费
			int  floor;
			//如果在本层有乘客，直接开门
			if (CallUp[E[i]->floor] == 1)
				E[i]->move = Opening, CallUp[E[i]->floor] = -1, E[i]->State = GoingUp;
			else if (CallDown[E[0]->floor] == 1)
				E[i]->move = Opening, CallDown[E[i]->floor] = -1, E[i]->State = GoingDown;
			//否则找到最近的请求楼层
			else
			{
				floor = HasUpOrDownRequires(E[i]);
				//没有乘客响应
				if (floor<0 || floor>MaxFloor)
					return;
				else if (1==CallUp[floor])
				{
					//向上运动
					if (floor >= E[i]->floor)
					{
						E[i]->move = Accelerate;
						E[i]->CallCar[floor] = 1;
						//设置楼层响应标志
						CallUp[floor] = -1;
						E[i]->State = GoingUp;
					}
					else
					{
						E[i]->move = Accelerate;
						E[i]->CallCar[floor] = 1;
						//设置楼层响应标志
						CallUp[floor] = -1;
						E[i]->State = GoingDown;
					}
				}
				else if (CallDown[floor] == 1)
				{
					//电梯向上运动
					if (floor >= E[i]->floor)
					{
						E[i]->move = Accelerate;
						E[i]->CallCar[floor] = 1;
						//设置楼层响应标志
						CallDown[floor] = -1;
						E[i]->State = GoingUp;
					}
					else
					{
						E[i]->move = Accelerate;
						E[i]->CallCar[floor] = 1;
						//设置楼层响应标志
						CallDown[floor] = -1;
						E[i]->State = GoingDown;
					}
				}

			}
		}
	}
	
}