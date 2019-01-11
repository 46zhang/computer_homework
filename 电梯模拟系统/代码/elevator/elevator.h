#ifndef ELEVATOR_H_INCLUDED
#define ELEVATOR_H_INCLUDED

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
using namespace std;
#include <iomanip>
#include <time.h>

//所有的宏定义
#define TRUE		1
#define	FALSE		0
#define OK			1
#define ERROR		0
#define INFEASIBLE	-1
#define OVERFLOW	-2
#define	STACK_INIT_SIZE	8	//存储空间初始分配量
#define	STACKINCREMENT	5	//存储空间分配增量
#define Run 1;
//#define Debug 1
#ifdef Run
#define	CloseTestTime 40	//电梯关门测试时间,有人进出时，电梯每隔40t测试一次，若无人进出，则关门
#define OverTime  300		//电梯停候超时时间,如果电梯在某层静止时间超过300t，则驶回1层候命
#define AccelerateTime 15	//加速时间
#define UpTime	51			//上升时间
#define DownTime 61			//下降时间
#define UpSlowTime 14		//上升减速
#define DownSlowTime 23		//下降减速
#define DoorTime	20		//开门关门时间,关门和开门各需要20t
#define InOutTime	25		//进出电梯时间,每个人进出电梯均需要25t
#define MaxGiveUpTime 300		//乘客最长忍耐时间
#define MaxInterTime 100	//下一个乘客最长的出现时间
#endif

#define MaxWaitNumInFloor 7	//每一层的最大等待人数
#define MaxFloor	4		//最高层
#define MinFloor	0		//最低层
#define MaxElevator   10
#define Empty	0
#define FloorHight 3		//楼层高度		
#define LeftElevatorX 6		//左边电梯的x起始坐标
#define LeftElevatorY 12	//左边电梯的y起始坐标
#define RightElevatorX 34	//右边电梯的x起始坐标
#define RightElevatorY 12	//右边电梯的y起始坐标

#ifdef Debug
//调试时设置时间短一点
#define	CloseTestTime 3		//电梯关门测试时间,有人进出时，电梯每隔40t测试一次，若无人进出，则关门
#define OverTime  300		//电梯停候超时时间,如果电梯在某层静止时间超过300t，则驶回1层候命
#define AccelerateTime 1	//加速时间
#define UpTime	1			//上升时间
#define DownTime 1			//下降时间
#define UpSlowTime 1		//上升减速
#define DownSlowTime 1		//下降减速
#define DoorTime	1		//开门关门时间,关门和开门各需要20t
#define InOutTime	1		//进出电梯时间,每个人进出电梯均需要25t
#define MaxInterTime 8		//下一个乘客最长的出现时间
#define MaxGiveUpTime 4		//乘客最长忍耐时间
#endif // DEBUG
//------------------------------------------------------

//状态的定义
enum	ElevatorMove { Opening, Opened, Closing, Closed, Moving, SlowDown, Waiting, Accelerate };  //电梯的8种动作
enum	ElevatorState{ GoingUp, GoingDown,Idle };												  //电梯的状态
//---------------------------------------------------------------------------------------------------------------



//乘客相关的结构体定义
typedef struct ClientNode {
	int ClientID;			//乘客的ID
	int InFloor;			//进入的楼层
	int OutFloor;			//目标楼层
	int GiveupTime;			//放弃时间
}*Client;

typedef int Status;					
typedef Client ElemType;			

//乘客队列
typedef struct ClientQueueNode {
	Client data;
	ClientQueueNode * next;
}* ClientQueuePtr;

//乘客的等待队列
typedef struct WaitQueueNode {
	ClientQueuePtr front;
	ClientQueuePtr rear;
	int WaitClientNum;
}WaitQueue;

//乘客栈
typedef struct ClientStackNode {
	ElemType *base;				//栈底指针
	ElemType *top;				//栈顶指针
	int			stacksize;	    //当前已分配存储空间，以元素为单位
}ClientStack;
//------------------------------------------------------

//电梯相关的结构体定义
typedef struct ElevatorNode {
	int ClientNum;			//乘客数量
	ElevatorState State;	//电梯的状态
	int StateCount;			//当前状态的计时
	int floor;				//当前所在的楼层
	ElevatorMove move;		//当前电梯的动作
	int CallCar[5];			//目标楼层数据
	ClientStack Stack[5];	//乘客栈
	int ClientID[8];		//该数组用来存储电梯内的乘客编号
}*Elevator;
//------------------------------------------------------


void PrintElevatorFrame(void);
void ShowTitle();
void gotoxy(short x, short y);
void ShowWaitingClient(WaitQueue Q, int flag, int x);
void DisappearWaitingClient(WaitQueue Q, int flag, int num);
void ShowElevatorBody(Elevator e, int k, int x);
void DisappearEvelatorBody(Elevator e, int k, int x);
void PrintfElevatorMove(Elevator e, int k);
void PrintfHelp();

Status InitQueue(WaitQueue &Q);
Status DestroyQueue(WaitQueue &Q);
Status EnterQueue(WaitQueue &Q, ElemType e);
Status DeleteQueue(WaitQueue &Q, ElemType &e);
Status IsQueueEmpty(WaitQueue Q);
Status DeleteQueueNode(WaitQueue &Q, ClientQueuePtr p);
void PrintQueue(WaitQueue Q);

Status InitStack(ClientStack &S);
Status DestroyStack(ClientStack &S);
Status StackEmpty(ClientStack S);
Status Push(ClientStack &S, ElemType e);
Status Pop(ClientStack &S, ElemType &e);

Status TimeUp(Elevator e);
void ElevatorChangeMove(Elevator e, int k);
void Controler(Elevator E[]);
int HigherRequires(Elevator e);
int LowerRequires(Elevator e);

void NewClient();
Status ClientGiveUp(WaitQueue &Q, Elevator E[], int floor);
Status ClientInOrOut(Elevator e, int k);

#endif