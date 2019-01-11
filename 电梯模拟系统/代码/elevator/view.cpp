#include <stdio.h>
#include <windows.h>
#include <iostream>
using namespace std;
#include <iomanip>
#include"elevator.h"

extern int MaxClientNum;
extern int  MaxRunTime;
void ShowTitle()
{
	printf("\n\n\n");
	printf("\n\t\t\t *********************************************************");
	printf("\n\t\t\t _________________________________________________________");
	printf("\n\t\t\t 〓〓〓〓〓〓〓〓〓欢迎来到电梯管理系统 〓〓〓〓〓〓〓〓〓\n");
	printf("\t\t\t ---------------------------------------------------------");
	printf("\n\t\t\t _________________________________________________________");
	printf("\n\t\t\t 〓〓〓〓〓〓〓〓author: 软件四班 章世楠〓〓〓〓〓〓〓〓〓\n");
	printf("\t\t\t ---------------------------------------------------------");
	printf("\n");
	printf("\n\t\t\t *********************************************************\n");
	
}
//用于确定信息的输出位置
void gotoxy(short x, short y)
{
	COORD coord = { x, y };
	/*COORD是Windows API中定义的一种结构，表示一个字符在控制台屏幕上的坐标。其定义为：

	typedef struct _COORD {

	SHORT X; // horizontal coordinate

	SHORT Y; // vertical coordinate
	} COORD;*/

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
//输出电梯的整体轮廓
void PrintElevatorFrame(void) {
	int j, i, k;
	for (i = 0; i <= 1; i++)
	{
		for (j = MaxFloor+1; j >= 0; j--)
		{
			if (j == MaxFloor)
			{
				gotoxy(6+i*27,9);
				printf("--------------------------");
			}
			gotoxy(6+i*27,9+j* FloorHight);
			printf("--------------------------");
			
		}
		j= MaxFloor + 1;
		for (k = 9; k <= 9 + j * FloorHight; k ++)
		{
			gotoxy(5+i*27,k);
			printf("|");
			gotoxy(5+i*27+26,k);
			printf("|");
		}
	}

}
//隐藏在x层的电梯
void DisappearEvelatorBody(Elevator e,int k,int x)
{
	int i, j;
	//先设置字体为背景颜色(黑色)
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
	for (i = LeftElevatorX + k * 27; i <= LeftElevatorX + k * 26 + 24; i++)
	{
		gotoxy(i, LeftElevatorY - 1 + (4 - x)*FloorHight);
		printf(" ");
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}
//在x层显示电梯
void ShowElevatorBody(Elevator e, int k,int x)
{
	int i, j;
	Client *p;
	gotoxy(LeftElevatorX+k*27,LeftElevatorY-1+(4-x)*FloorHight);
	printf("[");
	for (i = 0; i < e->ClientNum; i++)
	{
		if(e->ClientID[i]!=0)
			printf(" %d", e->ClientID[i]);
	}
	gotoxy(LeftElevatorX+k*26+24,LeftElevatorY - 1 + (4 - x)*FloorHight);
	printf("]");
}
//显示等待队列的乘客,flag 为0 表示该队列的乘客要往下，flag为1表示往上
//num表示同层中另一个队列的人数，显示必须在其之后，不然会覆盖上一个队列的数据
void ShowWaitingClient(WaitQueue Q, int flag,int num)
{
	ClientQueueNode * p;
	p = Q.front->next;
	int j=num;
	while (p != NULL)
	{
		gotoxy(RightElevatorX + 26+j*6, RightElevatorY-1 + (4-p->data->InFloor)*FloorHight);
		printf(" %d", p->data->ClientID);
		if (flag)
			printf("↑");
		else
			printf("↓");
		printf("%d",p->data->OutFloor);
		p = p->next;
		j++;
	}
}
//消隐处于等待状态的乘客
//当有新乘客加入或者有乘客入电梯，乘客离开时，需要消隐
void DisappearWaitingClient(WaitQueue Q, int flag,int num)
{
	//设置字体的颜色，用背景颜色再写一遍就能消去字体 0是黑色 7是白色
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
	ShowWaitingClient(Q, flag,num);
	//消隐完后再重新恢复字体颜色
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

//显示电梯的动作
void PrintfElevatorMove(Elevator e,int k)
{
	//Opening, Opened, Closing, Closed, Moving, SlowDown, Waiting, Accelerate
	gotoxy(10 + 30 * k, 4);
	printf("                         ");
	gotoxy(10+30*k,4);
	switch (e->move)
	{
	case Opening:printf(" %d电梯 门正在打开", k); break;
	case Opened:printf(" %d电梯 门已经打开", k); break;
	case Closing:printf(" %d电梯 正在关门", k); break;
	case Closed:printf(" %d电梯 门已经关上", k); break;
	case Moving:printf(" %d电梯 正在移动", k); break;
	case SlowDown:printf(" %d电梯 正在减速", k); break;
	case Waiting:printf(" %d电梯 正在等待", k); break;
	case Accelerate:printf(" %d电梯 正在加速", k); break;
	}
}
//显示说明信息
void PrintfHelp()
{
	gotoxy(60, 7);
	printf("说明：前面的数字表示乘客ID，箭头表示该乘客向上或下");
	gotoxy(60, 9);
	printf("后面的数字代表该乘客的目标楼层");
}