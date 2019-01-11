#include "BTree.h"

extern int m;
//随机生成n个随机数老构造m阶B树
Status CreateBTree(BTree &T, int n, int rootNum)
{
	int  *num, index;
	result r;
	newRoot(T,NULL,rootNum,NULL);
	if (NULL == T)
		return OVERFLOW;
	num = (int *)malloc(sizeof(int)*n);
	if (NULL == num)
		return OVERFLOW;
	for (int i = 0; i < n; i++)
	{
		//获得随机数
		num[i] = rand() % 9999;
	}
	//开始插入
	for (int i = 0; i < n; i++)
	{
		SearchBTree(T,num[i],r);
		//printf(" %d,%d ",num[i],r.tag);
		if(r.tag==0)
			InsertBTree(T, num[i],r.pt,r.i);
	}
	return OK;
}
//分裂，以s为中点，把q分裂成俩部分，前半部分保留原节点，后半部分放在ap
void spilt(BTree &q, int s, BTree &ap)
{
	int i, j;
	//分配内存
	ap = (BTNode *)malloc(sizeof(BTNode));
	ap->ptr[0] = q->ptr[s];
	q->ptr[s] = NULL;
	//赋值新节点的关键字数组跟孩子指针数组
	//清空旧节点后半部分的关键字数组跟指针数组
	for (i = s + 1, j = 1; i <= q->keynum; i++, j++)
	{
		ap->key[j] = q->key[i];
		ap->ptr[j] = q->ptr[i];
		q->key[i] = 0;
		q->ptr[i] = NULL;
	}
	//给ap的孩子指针域赋值
	for (i = 0; i <= q->keynum - s; i++)
	{
		if (ap->ptr[i] != NULL)
			ap->ptr[i]->parent = ap;
	}
	ap->keynum = q->keynum - s;
	q->keynum = s-1;   
	ap->parent = q->parent;
}
void Insert(BTree &q,int i,int x,BTree ap)
{
	//在key[i]插入x,指针数组ptr[i]插入ap
	//插入操作就是将原来的值后移，然后在i位置填入值
	int j;
	for (j = q->keynum; j >= i; j--)
	{
		q->key[j + 1] = q->key[j];
		q->ptr[j + 1] = q->ptr[j];
	}
	q->key[i] = x;
	q->ptr[i] = ap;
	if (ap != NULL)
	{
		ap->parent = q;
	}
	//插入完成后关键字个数加一
	q->keynum++;
}
//新建根节点，其值为x，孩子数组ptr[0]为p,ptr[1]为ap
void newRoot(BTree &t,BTree p,int x,BTree ap)
{
	t = (BTree)malloc(sizeof(BTNode));
	t->ptr[0] = p;
	t->ptr[1] = ap;
	t->parent = NULL;
	t->key[1] = x;
	t->keynum = 1;
	if (p)
		p->parent = t;
	if (ap)
		ap->parent = t;
}
//在B树t中q节点的key[i-1]和key[i]之间插入关键字k
//若插入后节点关键字个数等于B树的阶，则沿双亲指针链进行节点分裂，使t仍是m阶B树
void InsertBTree(BTree &t,int k,BTree q,int i)
{
	int x, s, finish = 0, needNewRoot = 0;
	BTree ap=NULL;
	//只有当根节点为NULL的时候才会出现q为NULL的情况
	//所以新建一个根节点
	if (NULL == q)
		newRoot(t,NULL,k,NULL);		//生成新的根节点
	else
	{
		x = k, ap = NULL;
		while (finish == 0 && needNewRoot == 0)
		{
			//关键字数组在i位置插入k
			//孩子指针数组在i位置插入ap
			Insert(q, i, x, ap);
			//如果小于阶数m插入成功
			if (q->keynum < m)
				finish = 1;
			//否则需要进行分裂
			else
			{
				s = (m + 1) / 2;
				//以s为中间，分裂成俩部分
				spilt(q, s, ap);
				x = q->key[s];
				//寻找中间元素在父节点的插入位置
				if (q->parent != NULL)
				{
					i = Search(q->parent, x);
					q = q->parent;
				}
				else
					needNewRoot = 1;
			}
			
		}
		if (needNewRoot == 1)
			newRoot(t, q, x, ap);
	}
}
int Search(BTree p, int k) 
{
	int i = 1;
	while (i <= p->keynum&&k > p->key[i])
		i++;
	return i;
}
void SearchBTree(BTree T, int k, result &r)
{
	//在m阶B树T上查找关键字k，返回(pt,i,tag)  
	//若查找成功，则特征值tag=1,指针pt所致结点中第i个关键字等于k;否则  
	//特征值tag=0，等于k的关键字记录应插入在指针pt所指结点中第i-1个和第i个关键字间  
	int i = 0, found = 0;
	BTree p = T, q = NULL;
	while (p != NULL && 0 == found) 
	{
		i = Search(p, k);		//在p->key[1..keynum]中查找p->key[i-1]<k<=p->p->key[i]  
		if (i > 0 && p->key[i] == k)
			found = 1;			//找到待查关键字
		else {
			q = p;				//找不到，到其孩子节点去搜索
			p = p->ptr[i - 1];
		}
	}
	if (1 == found)        //查找成功
	{
		r.pt = p;
		r.i = i;
		r.tag = 1;
	}
	else                  //查找不成功，返回key的插入位置i 
	{
		r.pt = q;
		r.i = i;
		r.tag = 0;
	}
}
//在p的Ai子树下找到最小的 最下层非终端节点
void Successor(BTree &p,int i)
{
	BTree q = p->ptr[i];
	while (q->ptr[0])
		q= q->ptr[0];
	//赋值
	p->key[i] = q->key[1];
	p = q;
}
//删除p节点的第i个关键字
void Remove(BTree &p,int i)
{
	for (int j = i; j < p->keynum; j++)
		p->key[j] = p->key[j + 1];
	p->keynum--;
}
//把根节点从q换成P
void ChangeRoot(BTree &q,BTree &p)
{
	BTree temp;
	temp = q;
	q= p;
	p->parent = NULL;
	//free(temp);
}
//调整B树
void Restore(BTree &p,int i)
{ 
	int num = 0, j = 0;
	BTree q = p->parent, brother;
	//找出p节点位于q节点的哪个位置,赋值num
	while (num < q->keynum)
	{
		if (q->ptr[num] == p)
			break;
		num++;
	}
	//判断左兄弟是否有多余的节点
	if (num > 0 && q->ptr[num - 1]->keynum > m  / 2)
	{
		brother = q->ptr[num - 1];
		j = i-1;
		//从左边的父亲节点借的关键字比p节点任何关键字都小，所以后移所有元素
		while (j<=p->keynum)
			p->key[j+1] = p->key[j++];
		//再赋值给p节点第一个关键字
		p->key[1] = q->key[num ];
		p->keynum++;
		//把左兄弟最大的节点赋值给父节点，代替那个划分到p节点的关键字
		q->key[num] = brother->key[brother->keynum];
		Remove(brother,brother->keynum);
	}
	//判断右兄弟是否有多余的节点
	else if (num < q->keynum&&q->ptr[num + 1]->keynum>m/2)
	{
		brother = q->ptr[num + 1];
		//从右边的父节点借的关键字比p节点任何关键字都大，所以赋值最后给p的最后一个节点
		p->key[p->keynum+1] = q->key[num + 1];
		p->keynum++;
		//把右兄弟的最小关键字赋值给父节点，代替那个划分到p节点的关键字
		q->key[num + 1] = brother->key[1];
		//删除右兄弟的代替的关键字
		Remove(brother,1);
	}
	//左右兄弟都没多余的节点，只能向父节点借
	else
	{
		//合并关键字要注意合并的先后顺序
		//跟左兄弟以及区分他们的父节点合并
		if (num != 0)
		{
			brother = q->ptr[num - 1];
			//把父节点中区分p跟其左节点的关键字赋值到左兄弟的关键字数组
			//把右孩子的第一个孩子指针也随父节点的关键字一起插入到brother节点
			Insert(brother,brother->keynum+1,q->key[num],p->ptr[0]);
			//brother->key[++p->keynum] = q->key[num];
			j = 1;
			//把p中的节点也合并到borther节点中
			//孩子节点也合并(如果存在)
			while (j <= p->keynum)
			{
				//brother->key[++p->keynum] = p->key[j];
				Insert(brother, brother->keynum + 1, p->key[j], p->ptr[j]);
				j++;
			}
			//删除父节点该关键字
			Remove(q,num);
			for(j=num;j<=q->keynum;j++)
				q->ptr[j] = q->ptr[j+1];
			q->ptr[q->keynum + 1] = NULL;
			//释放p的内存
			free(p);
		}
		//合并父节点和其右孩子到p节点中
		else {
			brother = q->ptr[num + 1];
			//把父节点的关键字赋值给p
			Insert(p,p->keynum+1,q->key[num+1],brother->ptr[0]);
			//p->key[++p->keynum] = q->key[num + 1];
			Remove(q,num+1);
			j = 1;
			//合并p右节点的关键字
			//以及合并其孩子节点
			while (j <= brother->keynum)
			{
				//p->key[++p->keynum] = brother->key[j++];
				Insert(p,p->keynum+1,brother->key[j],brother->ptr[j]);
				j++;
			}
			for(j=num+1;j<=q->keynum;j++)
				q->ptr[j] = q->ptr[j+1];
			q->ptr[q->keynum+1] = NULL;
			free(brother);
		}
		//删除后如果父节点关键字数小于m/2需要调整
		if (q->keynum < m / 2)
		{
			//如果不是根节点，再进行调整
			if (q->parent != NULL)
				Restore(q, i);
			//如果到了根节点需要更换根节点
			else
				ChangeRoot(q,p);
		}
			
	}
}
//删除B树p节点的第i个关键字
void DeleteBTree(BTree &p,int i)
{
	
	if (p->ptr[i]!=NULL)			//若不是最下层的非终端节点
	{
		Successor(p,i);				//找到最下层的非终端节点
		DeleteBTree(p,1);			//删除其中最小的节点
	}
	else							//若是最下层的非终端节点
	{
		Remove(p,i);
		if (p->keynum < (m - 1) / 2)
			Restore(p,i);
	}
}
//采用缩进的显示效果打印出B树(先序遍历)
void ShowBTree(BTree t,int x)
{
	if (t == NULL)
		return;
	printf("     -------------------------------------------------------\n");
	for (int i = 0; i < 5*x; i++)
		printf(" ");
	//先打印出该节点的所有关键字的值
	printf("|");
	for (int i = 1; i <= t->keynum; i++)
		printf("%d|", t->key[i]);
	printf("\n");
	//每打印完一个节点就换行
	for (int i = 0; i <= t->keynum; i++)
	{
		ShowBTree(t->ptr[i],x+1);
	}

}
//中序遍历输出B树
void TravelBTreeInorder(BTree t)
{
	
	int i;
	if (t == NULL)
		return;
	for (i = 0; i <= t->keynum / 2; i++)
		TravelBTreeInorder(t->ptr[i]);
	for (i = 1; i <= t->keynum; i++)
		printf(" %d ",t->key[i]);
	for(i= t->keynum / 2+1;i<=t->keynum;i++)
		TravelBTreeInorder(t->ptr[i]);
}
//后序遍历输出B树
void TravelBTreePosterorder(BTree t)
{
	int  i;
	if (t == NULL)
		return;
	for (i = 0; i <= t->keynum; i++)
		TravelBTreePosterorder(t->ptr[i]);
	for (i = 1; i <= t->keynum; i++)
		printf(" %d ",t->key[i]);
	
}
//计算B树关键字数
int CountBTreeKeyNum(BTree t)
{
	int num=0;
	if (t == NULL)
		return 0;
	else
	{
		num += t->keynum;
		for (int i = 0; i <= t->keynum; i++)
		{
			if (t->ptr[i] == NULL)
				continue;
			num+=CountBTreeKeyNum(t->ptr[i]);
		}
	}
	return num;
}
//计算B树节点数
void CountBTreeNodeNum(BTree t,int& num)
{
	int i = 0;
	if (t == NULL)
		return;
	for (; i <= t->keynum; i++)
	{
		if (t->ptr[i] != NULL)
			num++;
		CountBTreeNodeNum(t->ptr[i],num);
	}
}
void DestroyBTree(BTree T) 
{
	//对B树进行摧毁
	int i = 1;
	if (NULL != T)
	{
		for (; i <= T->keynum + 1; i++)
		{
			DestroyBTree(T->ptr[i - 1]);
		}
		free(T);   //释放结点
	}
}

