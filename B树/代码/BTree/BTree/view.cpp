#include "BTree.h"

int menu() {//菜单  
	int choice;
	printf("\n\n\t\t\t〓〓〓〓〓〓〓〓〓〓〓〓B树测试〓〓〓〓〓〓〓〓〓〓〓\n");
	printf("\n\n");
	printf("\t\t\t    __________________B树功能菜单_________________\n");
	printf("\t\t\t   |                                              |\n");
	printf("\t\t\t   |                                              |\n");
	printf("\t\t\t   |   1.创建B树            2.输出B树             |\n");
	printf("\t\t\t   |                                              |\n");
	printf("\t\t\t   |   3.B树结点的插入      4.B树结点的删除       |\n");
	printf("\t\t\t   |                                              |\n");
	printf("\t\t\t   |   5.B树的中序遍历      6.B树的后序遍历       |\n");
	printf("\t\t\t   |                                              |\n");
	printf("\t\t\t   |   7.B树的节点查找      8.B树节点数           |\n");
	printf("\t\t\t   |                                              |\n");
	printf("\t\t\t   |   9.B树的摧毁          0.退出                |\n");
	printf("\t\t\t   |______________________________________________|\n");
	printf("\n\n");
	printf("\t\t\t〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓\n\n");
	printf("\t\t\t\t    章世楠 3117004958 软件工程(4)班\n");
	printf("\n\t\t\t〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓\n");
	printf("\n\t\t\t请选择菜单中的相应操作【输入0-9】:");
	while (scanf("%d", &choice) != 1 || choice < 0 || choice>9) {
		while (getchar() != '\n')	//吸收错误输入
			continue;
		printf("\n\t\t\t输入有误，重新输入");
		printf("\n\t\t\t请选择菜单中的相应操作【输入0-9】:");
	}//避免非法输入  
	return choice;
}