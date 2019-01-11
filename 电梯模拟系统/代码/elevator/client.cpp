#include"elevator.h"

extern int NextClientInterTime;							//下一个乘客进入系统的时间
extern int TotalClient;									//总共有多少个乘客
extern WaitQueue FloorWaitQueue[2][5];					//每层楼有俩个等待队列，0向上,1向下
extern int CallUp[5], CallDown[5];			//楼层的向上向下按钮,以及目标楼层
extern int MaxClientNum;
void NewClient()
{
	Client c;
	c = (Client)malloc(sizeof(ClientNode));
	srand((unsigned)time(NULL));
	//产生所在楼层，目前楼层，忍耐时间
	c->ClientID = TotalClient++;
	c->GiveupTime = rand()%MaxGiveUpTime;
	//忍耐时间在50t-300t之间
	while(c->GiveupTime<=50)
		c->GiveupTime= rand() % MaxGiveUpTime;
	c->InFloor = rand()*11 % (MaxFloor+1);
	c->OutFloor = rand()*17 % (MaxFloor+1);
	//如果目前楼层于现在楼层一样，则重新赋值
	while (c->OutFloor == c->InFloor)
		c->OutFloor= rand() % MaxFloor;
	NextClientInterTime = rand() % MaxInterTime;
	//下一 乘客出现的时间在5-120s之间
	while(NextClientInterTime<5)
		NextClientInterTime = rand() % MaxInterTime;
	//进入等待队列
	//先判断其是向上还是先下，然后消隐原来的队列显示，加入乘客到队列
	//再重新显示新的乘客队列
	if (c->InFloor > c->OutFloor)
	{
		DisappearWaitingClient(FloorWaitQueue[1][c->InFloor], 0, 0);
		DisappearWaitingClient(FloorWaitQueue[0][c->InFloor], 1, FloorWaitQueue[1][c->InFloor].WaitClientNum);
		//进入等待队列
		EnterQueue(FloorWaitQueue[1][c->InFloor], c);
		//按下该层的电梯按钮
		if(CallDown[c->InFloor] !=-1)
			CallDown[c->InFloor] = 1;
		ShowWaitingClient(FloorWaitQueue[1][c->InFloor], 0,0);
		ShowWaitingClient(FloorWaitQueue[0][c->InFloor], 1,FloorWaitQueue[1][c->InFloor].WaitClientNum);
	}
	else
	{
		DisappearWaitingClient(FloorWaitQueue[1][c->InFloor], 0, 0);
		DisappearWaitingClient(FloorWaitQueue[0][c->InFloor], 1, FloorWaitQueue[1][c->InFloor].WaitClientNum);
		EnterQueue(FloorWaitQueue[0][c->InFloor], c);
		//按下该层的电梯按钮
		if (CallUp[c->InFloor] != -1)
			CallUp[c->InFloor] = 1;
		//显示向上跟向下的乘客
		ShowWaitingClient(FloorWaitQueue[1][c->InFloor], 0,0);
		ShowWaitingClient(FloorWaitQueue[0][c->InFloor], 1,FloorWaitQueue[1][c->InFloor].WaitClientNum);
	}
}
Status ClientOut(Elevator &e, int k)
{
	Client p;
	int i, j;
	//说明有乘客的目标楼层为这个,并且该层乘客栈不为空,让乘客出栈,每出去一个乘客需要等待25个t
	if (StackEmpty(e->Stack[e->floor]) != TRUE)
	{
		Pop(e->Stack[e->floor], p);
		for (i = 0; i < MaxClientNum; i++)
		{
			if (e->ClientID[i] == p->ClientID)
				break;
		}
		e->ClientNum--;
		//覆盖掉乘客ID数组的数据
		for (j = i; j < 7; j++)
			e->ClientID[j] = e->ClientID[j + 1];
		gotoxy(10 + 30 * k, 6);
		printf("                         ");
		gotoxy(10 + 30 * k, 6);
		printf(" %d乘客出电梯 %d", p->ClientID, k);
		free(p);
		return TRUE;
	}
	else
		return FALSE;
}
Status ClientIn(Elevator &e, int k)
{
	Client p;
	if (e->ClientNum >= 8)
		return FALSE;
	switch (e->State)
	{
		//如果该电梯向下，只接收向下的乘客
	case GoingDown:
		if (FloorWaitQueue[1][e->floor].WaitClientNum != 0)
		{	//出队成功
			if (OK == DeleteQueue(FloorWaitQueue[1][e->floor], p))
			{
				if (OK == Push(e->Stack[p->OutFloor], p))
				{
					e->CallCar[p->OutFloor] = 1;
					e->ClientID[e->ClientNum++] = p->ClientID;
					gotoxy(10 + 30 * k, 6);
					printf("                         ");
					gotoxy(10 + 30 * k, 6);
					printf(" %d乘客成功进入电梯%d", p->ClientID, k);
				}
				else
					printf(" %d乘客进入电梯%d失败", p->ClientID, k);
			}

		}
		else
			return FALSE;
		break;
	case GoingUp:
		//如果该电梯向下只接收向下的乘客
		if (FloorWaitQueue[0][e->floor].WaitClientNum != 0)
		{
			if (OK == DeleteQueue(FloorWaitQueue[0][e->floor], p))
			{
				//压入栈中
				if (OK == Push(e->Stack[p->OutFloor], p))
				{
					e->CallCar[p->OutFloor] = 1;
					e->ClientID[e->ClientNum++] = p->ClientID;
					gotoxy(10 + 30 * k, 6);
					printf("                         ");
					gotoxy(10 + 30 * k, 6);
					printf(" %d乘客成功进入电梯%d", p->ClientID, k);
				}
				else
					printf(" %d乘客进入电梯%d失败", p->ClientID, k);
			}
			else
				return FALSE;
		}
		else
			return FALSE;
		break;
		//如果是处于等待状态需要谨慎考虑
	case Idle:
		break;
	}
}
//乘客进出电梯处理函数，先出后进
Status ClientInOrOut(Elevator e, int k)
{
	if (ClientOut(e, k) == FALSE)
	{
		if (ClientIn(e, k) == FALSE)
			return FALSE;
		//有人进入电梯，需要重新显示等待队列
		DisappearWaitingClient(FloorWaitQueue[1][e->floor], 0, 0);
		DisappearWaitingClient(FloorWaitQueue[0][e->floor], 1, FloorWaitQueue[1][e->floor].WaitClientNum);
		ShowWaitingClient(FloorWaitQueue[1][e->floor], 0, 0);
		ShowWaitingClient(FloorWaitQueue[0][e->floor], 1, FloorWaitQueue[1][e->floor].WaitClientNum);
	}
	return TRUE;
}
//检测队列中的乘客是否要放弃搭乘电梯
//floor表示当前楼层，flag表示向下或者向上
Status ClientGiveUp(WaitQueue &Q,Elevator E[],int floor)
{
	Client node;
	ClientQueuePtr p=Q.front;
	if (Q.WaitClientNum <= 0)
		return FALSE;
	else if (NULL != Q.front->next)
		node = Q.front->next->data;
	else
		return FALSE;
	//如果忍耐时间到了并且该层没有电梯
	if (node != NULL && node->GiveupTime <= 0 && E[0]->floor != floor && E[1]->floor != floor)
	{
		gotoxy(10 + 30, 6);
		printf("                         ");
		gotoxy(10 + 30, 6);
		printf(" %d号乘客放弃搭乘电梯\n", node->ClientID);
		//等待的乘客离开电梯，应该消隐后重新显示在等待的乘客
		DisappearWaitingClient(FloorWaitQueue[1][floor], 0, 0);
		DisappearWaitingClient(FloorWaitQueue[0][floor], 1, FloorWaitQueue[1][floor].WaitClientNum);
		DeleteQueueNode(Q,p);
		ShowWaitingClient(FloorWaitQueue[1][floor], 0, 0);
		ShowWaitingClient(FloorWaitQueue[0][floor], 1, FloorWaitQueue[1][floor].WaitClientNum);
	}
	else
		node->GiveupTime--;
	/*ClientQueuePtr p=Q.front;
	//找到第一个等待的乘客，判断其是否要放弃等待
	if (Q.WaitClientNum <= 0)
		return FALSE;
	if (p->next != NULL)
		node =p->next->data;
	else
		return FALSE;
	//如果忍耐时间到了并且该层没有电梯
	if (node->GiveupTime <= 0 && E[0]->floor != floor && E[1]->floor != floor)
	{
		gotoxy(10 + 30 , 6);
		printf("                         ");
		gotoxy(10 + 30 , 6);
		printf(" %d号乘客放弃搭乘电梯\n", node->ClientID);
		//乘客出队
		DeleteQueueNode(Q, p);//将放弃等待的人删除
	}
	else
		node->GiveupTime--;*/
	return TRUE;
}
