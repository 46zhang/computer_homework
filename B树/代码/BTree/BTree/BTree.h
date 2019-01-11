#pragma once
#include<stdio.h>
#include<stdlib.h>
#include <conio.h>

#define M 10			//默认10阶B树
#define KeyType int

//定义状态
typedef enum {
	ERROR, OK, OVERFLOW
}Status;

typedef struct {  //记录的结构定义  
	KeyType key;
	char data;
}Record;

typedef struct BTNode {
	int keynum;					//节点当前的关键字个数
	KeyType key[M + 1];			//关键字数组 ，key[0]未用
	struct BTNode * parent;		//双亲节点指针
	struct BTNode * ptr[M+ 1];	//孩子节点指针数组
	Record * recptr[M + 1];		//记录指针向量，0号单元未用
}BTNode,* BTree;

typedef struct {
	BTree pt;
	int i;
	int tag;
}result;

//函数声明
int menu();
Status CreateBTree(BTree &T, int n,  int rootNum);
void InsertBTree(BTree &t, int k, BTree q, int i);
int Search(BTree p, int k);
void SearchBTree(BTree T, int k, result &r);
void ShowBTree(BTree t, int x);
void newRoot(BTree &t, BTree p, int x, BTree ap);
int CountBTreeKeyNum(BTree t);
void CountBTreeNodeNum(BTree t, int& num);
void DestroyBTree(BTree T);
void TravelBTreePosterorder(BTree t);
void TravelBTreeInorder(BTree t);
void DeleteBTree(BTree &p, int i);