#include "elevator.h"
//等待队列的基本操作
Status InitQueue(WaitQueue &Q) {
	//构造一个空队列Q
	Q.front=Q.rear= new ClientQueueNode;
	if (!Q.front)
		return OVERFLOW;//分配存储失败
	Q.front->next = NULL;
	Q.front->data = NULL;
	Q.WaitClientNum = 0;
	return OK;
}

Status DestroyQueue(WaitQueue &Q) {
	//销毁队列Q
	while (Q.front) {
		Q.rear = Q.front->next;
		if (Q.front->data) free(Q.front->data);
		delete Q.front;
		Q.front = Q.rear;
	}
	return OK;
}

Status EnterQueue(WaitQueue &Q, ElemType e) {
	//插入元素e为Q的新的队尾元素
	ClientQueuePtr p;
	p = new ClientQueueNode;
	if (!p)
		return OVERFLOW;
	p->data = e; p->next = NULL;
	Q.rear->next = p;
	Q.rear = p;
	Q.WaitClientNum++;
	return OK;
}
Status DeleteQueue(WaitQueue &Q, ElemType &e) { //带头结点的队列
	//若队列不空,则删除Q的队头元素,用e返回其值,并返回OK;
	//否则返回ERROR
	ClientQueuePtr p;
	if (Q.front == Q.rear)  //如果队列为空，则返回错误
		return ERROR;
	p = Q.front->next;    //绕过头结点
	e = p->data;
	Q.front->next = p->next;
	if (Q.rear == p)
		Q.rear = Q.front;
	delete p;
	Q.WaitClientNum--;
	return OK;
}

Status IsQueueEmpty(WaitQueue Q) {
	//判断队列是否为空
	if (Q.front == Q.rear)
		return TRUE;
	else
		return FALSE;
}

Status DeleteQueueNode(WaitQueue &Q, ClientQueuePtr p) {
	//删除队列中p指向的结点的下一个结点
	ClientQueuePtr q;
	ClientQueuePtr *temp=&Q.rear;
	if (p == NULL || p->next == NULL)
		return ERROR;
	q = p->next;
	p->next = q->next;
	if (p->next == NULL)
		*(temp) = p;
	//free(q->data);
	//free(q);
	Q.WaitClientNum--;
	return OK;
}

void PrintQueue(WaitQueue Q) {
	//输出队列
	ClientQueuePtr q;
	int count = 0;
	if (Q.front->next == NULL)  //如果队列为空，跳转到end
		goto end;
	q = Q.front->next;
	while (q != NULL) {
		cout << setw(3) << q->data->ClientID << ' ';
		q = q->next;
		count++;
	}
end:
	while (count++ <= 4) //输出空格补全占位，每次占3个字符位
		cout << "    ";
}