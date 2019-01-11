#include<stdio.h>
#include"elevator.h"
#include <stdlib.h>
#include <conio.h>

void InputMaxTime();
void init(Elevator * E, int i);
void CheckGiveUp(Elevator E[]);

int MaxClientNum = 7;					//最大乘客人数
int MaxRunTime;							//最大运行时间
int NextClientInterTime;				//下一个乘客进入系统的时间
int TotalClient=1;						//当前总共有多少个乘客(用来产生不重复的序号)
WaitQueue FloorWaitQueue[2][5];			//每层楼有俩个等待队列，0向上,1向下
int CallUp[5], CallDown[5], CallCar[5];	//楼层的向上向下按钮,以及目标楼层,取值为1表示按下,-1表示电梯已经响应						
void main()
{
	//分别对应每层的电梯向上按钮，向下按钮，以及目标楼层
	int  i, Time = 0, t[2];
	Elevator E[2];
	srand((unsigned)time(NULL));
	init(E,2);										//全局初始化操作
	t[1] = t[0] = 0;
	ShowTitle();
	InputMaxTime();		
	system("cls");
	PrintElevatorFrame();							//输出电梯框架
	PrintfHelp();
	ShowElevatorBody(E[0],0,1);
	ShowElevatorBody(E[1], 1,1);						//输出电梯主体
	while (Time++ < MaxRunTime)
	{
		//产生新的乘客
		if (NextClientInterTime == 0)
			NewClient();
		else
			NextClientInterTime--;
		//检测是否乘客放弃搭乘
		CheckGiveUp(E);
		//调用电梯调度函数
			Controler(E);
		for (i = 0; i < 2; i++)
		{
			//如果处于开门状态，每隔一定的时间周期允许进出乘客
			if (E[i]->move == Opened)
			{
				//每隔25s进一个人
				if (t[i] == InOutTime)
				{
					t[i] = 0;
					//等待的乘客进入电梯，应该消隐后重新显示在等待的乘客
					DisappearWaitingClient(FloorWaitQueue[1][E[i]->floor], 0, 0);
					DisappearWaitingClient(FloorWaitQueue[0][E[i]->floor], 1, FloorWaitQueue[1][E[i]->floor].WaitClientNum);
					//乘客进出,电梯的显示情况发生改变，显示出来
					if (TRUE == ClientInOrOut(E[i], i))
					{
						//消隐之前的电梯，显示乘客进入后的电梯
						DisappearEvelatorBody(E[i],i,E[i]->floor);
						ShowElevatorBody(E[i],i,E[i]->floor);
					}//消隐后再显示
					ShowWaitingClient(FloorWaitQueue[1][E[i]->floor], 0, 0);
					ShowWaitingClient(FloorWaitQueue[0][E[i]->floor], 1, FloorWaitQueue[1][E[i]->floor].WaitClientNum);
				}
				else
					t[i]++;
			}
			//如果当前状态的时间周期到了，需要改变状态
			if (TimeUp(E[i]) == TRUE)
			{
				ElevatorChangeMove(E[i], i);
				PrintfElevatorMove(E[i],i);
			}
			else 
				E[i]->StateCount--;			//计时
		}
		if (Time % 10 == 0)
		{
			gotoxy(20,2);
			printf("时间为： %d", Time);
		}
		if(Time%100==0)
			PrintElevatorFrame();							//输出电梯框架
		Sleep(80);
	}
	gotoxy(60,30);
	printf("演示结束，按下任意键退出\n");
	getch();
}
void init(Elevator * E,int num)
{
	int i;
	//初始化等待队列操作
	for (i = 0; i < 5; i++)
	{
		InitQueue(FloorWaitQueue[0][i]);
		InitQueue(FloorWaitQueue[1][i]);
	}
	//初始化俩台电梯
	for (i = 0; i < num; i++)
	{
		E[i] = (Elevator)malloc(sizeof(ElevatorNode));
		E[i]->ClientNum = 0;
		//初始状态在1楼停留
		E[i]->floor = 1;
		E[i]->State = Idle;
		E[i]->StateCount = 0;
		E[i]->move = Waiting;
		for (int j = 0; j < 5; j++)
		{
			//目标楼层初始化为0
			E[i]->CallCar[j] = 0;
			//初始化该电梯的乘客栈
			InitStack(E[i]->Stack[j]);
		}
		//初始化乘客ID数组
		for (int j = 0; j <= 8; j++)
			E[i]->ClientID[j] = 0;
	}
	//初始化三个电梯按钮数组
	for (i = 0; i < 5; i++)
	{
		CallUp[i] = 0;
		CallDown[i] = 0;
	}
}
void CheckGiveUp(Elevator E[])
{
	int i;
	//检测是否乘客放弃搭乘
		for (i = 0; i < 5; i++)
		{
			
			ClientGiveUp(FloorWaitQueue[0][i], E, i);
				
			ClientGiveUp(FloorWaitQueue[1][i], E, i);
				
		}
}
void InputMaxTime()
{
	printf("\n\t\t\t请输入电梯的最大运行时间，达到该时间，电梯系统将会自动停止\n");
	printf("\t\t\t请输入:");
	while (scanf("%d", &MaxRunTime) != 1 || MaxRunTime <= 500 || MaxRunTime > 10000)
	{
		printf("\t\t\t输入有误，重新输入（500=<x<10000）\n");
		while (getchar() != '\n')
			continue;
		printf("\t\t\t请输入:");
	}
}