#include "elevator.h"

Status InitStack(ClientStack &S) {
	//构造一个空栈
	S.base = (ElemType *)malloc(STACK_INIT_SIZE * sizeof(ElemType));
	if (!S.base) return OVERFLOW;
	S.top = S.base;
	S.stacksize = STACK_INIT_SIZE;
	return OK;
}//InitStack


Status DestroyStack(ClientStack &S) {
	//销毁栈S
	ElemType	*p;
	if (S.base) {
		for (p = S.base; p < S.top; p++)
//			DestoryClient(*p);
		free(S.base);
	}
	return OK;
}

Status StackEmpty(ClientStack S) {
	//若栈S为空，则返回TRUE，否则返回FALSE
	if (S.top == S.base)
		return TRUE;
	else
		return FALSE;
}
Status Push(ClientStack &S, ElemType e) {
	//入栈
	if (!S.base)
		return ERROR;
	if (S.top - S.base >= S.stacksize) {
		S.base = (ElemType *)realloc(S.base, (S.stacksize + STACKINCREMENT) * sizeof(ElemType));
		if (!S.base)
			return OVERFLOW;
		S.top = S.base + S.stacksize;
		S.stacksize += STACKINCREMENT;
	}
	*S.top++ = e;
	return OK;
}//Push
Status Pop(ClientStack &S, ElemType &e) {   //出栈并将出栈信息返回
	//出栈
	if (S.top == S.base)
		return ERROR;
	e = *(--S.top);
	return OK;
}//Pop

