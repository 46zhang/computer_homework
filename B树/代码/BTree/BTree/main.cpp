#include "BTree.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int m=10;			//阶数全局变量 一开始默认为10
void CreateBTreeByInput(BTree &T);
void InsertBTreeByInput(BTree &T);
void CountKeyNum(BTree t);
void CountNode(BTree t);
void FineKeyInBTree(BTree t);
void DeleteBTreeNode(BTree &BT);
int main()
{
	int choice, m;
	
	BTree BT = NULL;
	while (1)
	{
		choice = menu();
		switch (choice)
		{
		case 0:	exit(0);					 //退出
				break;
		case 1:	CreateBTreeByInput(BT);		//创建B树 
				break;
		case 2:	ShowBTree(BT, 1);			//显示B树
				break;
		case 3:	InsertBTreeByInput(BT);		//插入B树
				break;
		case 4:	DeleteBTreeNode(BT);		//删除B树的节点
				break;
		case 5:	TravelBTreeInorder(BT);		//B树的中序遍历
				break;
		case 6:	TravelBTreePosterorder(BT);	//B树的后序遍历
				break;
		case 7:	FineKeyInBTree(BT);			//查找关键字
				break;
		case 8:	CountNode(BT);				//节点数
				break;
		case 9:	DestroyBTree(BT);			//销毁树 
				break;
		}
		
	}
	return 0;
}
void CreateBTreeByInput(BTree &T)
{
	int  n, rootNum;
	printf("输入你想要输入的创建的B树的阶数(1<m<=10):");
	while (scanf("%d", &m) != 1 || m < 1 || m>10)
	{
		printf("输入有误重新输入\n");
		while (getchar() != '\n')
			continue;
		printf("输入你想要输入的创建的B树的阶数(1<m<=10):");
	}
	printf("请设置B树的初始化关键字个数(0<=个数<=10000)\n个数=");
	while (scanf("%d", &n) != 1 || n < 0 || n>10000)
	{
		printf("输入有误重新输入\n");
		while (getchar() != '\n')
			continue;
		printf("请设置B树的初始化关键字个数(0<=个数<=10000)\n个数=");
	}
	printf("请设置B树的的根节点的值");
	while (scanf("%d", &rootNum) != 1 )
	{
		printf("输入有误重新输入\n");
		while (getchar() != '\n')
			continue;
		printf("请设置B树的的根节点的值");
	}
	if (OK == CreateBTree(T, n, rootNum))
		printf("\t\t创建成功\n");
	else
		printf("\t\t创建失败\n");
}

void InsertBTreeByInput(BTree &T)
{
	int num;
	result r;
	printf("\t请输入你想插入的节点的数值：");
	while (scanf("%d", &num)!=1)
	{
		printf("\t输入有误，重新输入\n");
		while (getchar() != '\n')
			continue;
		printf("\t请输入你想插入的节点的数值：");
	}
	if (T == NULL)
		newRoot(T, NULL, num, NULL);
	else {
		SearchBTree(T, num, r);
		if (r.tag == 0)
		{
			InsertBTree(T, num, r.pt, r.i);
		}
	}
}
void CountKeyNum(BTree t)
{
	int num = 0;
	num = CountBTreeKeyNum(t);
	printf("\t\t该B树的关键字个数为%d\n",num);
}
void CountNode(BTree t)
{
	int num = 1;
	CountBTreeNodeNum(t, num);
	printf("\t\t该B树的节点数为%d\n",num);
}
void FineKeyInBTree(BTree t)
{
	int num;
	result r;
	printf("\t输入你想查找的关键字：");
	while (scanf("%d", &num) != 1)
	{
		printf("输入有误\n\t输入你想查找的关键字：");
		while (getchar() != '\n')
			continue;
	}
	SearchBTree(t, num, r);
	
	if (r.tag == 0)
		printf("\t找不到该关键字\n");
	else
		printf("\t待查找的关键字位置：关键字为%d的节点的第%d个",(r.pt)->key[1],r.i);
}
void DeleteBTreeNode(BTree &BT)
{
	int num;
	result r;
	printf("\t\t请输入你想要删除的关键字");
	while (scanf("%d", &num) != 1)
	{
		printf("输入有误\n\t输入你想查找的关键字：");
		while (getchar() != '\n')
			continue;
	}
	SearchBTree(BT, num, r);
	if (r.tag == 0)
		printf("\t找不到该关键字\n");
	else
		DeleteBTree(r.pt,r.i);
	//调整后如果根节点为空，则重新赋值根节点
	if (BT->keynum == 0)
		BT = BT->ptr[0];
}