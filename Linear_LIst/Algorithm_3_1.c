/*  线性表的静态单链表的存储结构 */
#include <string.h>
#include <ctype.h>
#include <malloc.h>	/* malloc() 等 */
#include <limits.h>	/* INT_MAX 等*/
#include <stdio.h>	/* EOF(=^Z或F6),NULL */
#include <stdlib.h>	/* atoi() */
#include <io.h>	/* eof() */
#include <math.h>	/* floor(),ceil(),abs() */
#include <process.h> 	/* exit() */
/* 函数结果状态代码 */
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
/* #define OVERFLOW -2 因为在math.h中已定义OVERFLOW的值为3,故去掉此行 */

typedef int Status; /* Status是函数的类型,其值是函数结果状态代码，如OK等 */
typedef int Boolean; /* Boolean是布尔类型,其值是TRUE或FALSE */
typedef int ElemType;  /* ElemType定义的数据类型 */

#define MAXSIZE 20 /* 链表的最大长度 */

typedef struct {
	ElemType data;
	int cur;
}component,SLinkList[MAXSIZE];

int Malloc(SLinkList space); /* 算法2.15 */
void Free(SLinkList space,int k); /* 算法2.16 */
void DestroyList();  /* 静态数组不能被销毁 */
void visit(ElemType e);

/* 一个数组只生成一个静态链表 */
void InitList(SLinkList L);
Status ClearList(SLinkList L);
Status ListEmpty(SLinkList L);
int ListLength(SLinkList L);
Status GetElem(SLinkList L,int i,ElemType *e);
int LocateELem(SLinkList L,ElemType e); /* 算法2.13有改动 */
Status PriorElem(SLinkList L,ElemType cur_e,ElemType *pre_e);
Status NextElem(SLinkList L,ElemType cur_e,ElemType *next_e);
Status ListInsert(SLinkList L,int i,ElemType e);
Status ListDelete(SLinkList L,int i,ElemType *e);
Status ListTraverse(SLinkList L,void(*vi)(ElemType));

void main(void) {
	// int c1;
	int j, k;
	Status i;
	ElemType e, e0;
	SLinkList L;
	InitList(L);
	for(j = 1;j <= 5; j++)
		i = ListInsert(L,1,j);
	printf("在L的表头依次插入1~5后：L=");
	ListTraverse(L,visit);
	i = ListEmpty(L);
	printf("L是否为空：i=%d(1:是 0:否)表L的长度=%d\n", i, ListLength(L));
	i = ClearList(L);
	printf("清空L后：L=");
	//测试原理使用
	// for(c1 = 0;c1 < MAXSIZE-1; c1++) {
	// 	printf("c1 = %d   cur = %d\n", c1, L[c1].cur);
	// }
	ListTraverse(L,visit);
	i = ListEmpty(L);
	printf("L是否为空：i=%d(1:是 0:否)表L的长度=%d\n", i, ListLength(L));
	for(j = 1;j <= 10; j++)
		ListInsert(L,j,j);
	printf("在L的表尾依次插入1~10后：L=");
	ListTraverse(L,visit);
	GetElem(L,5,&e);
	printf("第5个元素的值为：%d\n", e);
	for(j = 0;j <= 1; j++) {
		k = LocateELem(L,j);
		if(k)
			printf("值为%d的元素在静态链表中的位序为%d\n", j, k);
		else
			printf("没有值为%d的元素\n", j);
	}
	for(j = 1;j <= 2; j++) {  /* 测试头两个数据 */
		GetElem(L,j,&e0); /* 把第j个数据赋给e0 */
		i = PriorElem(L,e0,&e);  /* 求e0的前驱 */
		if(!i)
			printf("元素%d无前驱\n", e0);
		else
			printf("元素%d的前驱为：%d\n", e0, e);
	}
	for(j = ListLength(L)-1;j <= ListLength(L); j++) {  /* 最后两个数据 */
		GetElem(L,j,&e0); /* 把第j个数据赋给e0 */
		i = NextElem(L,e0,&e);  /* 求e0的后继 */
		if(!i)
			printf("元素%d无后继\n", e0);
		else
			printf("元素%d后继为：%d\n", e0, e);
	}
	k = ListLength(L); /* k为表长 */
	for(j = k+1;j >= k; j--) {
		i = ListDelete(L,j,&e); /* 删除第j个数据 */
		if(i)
			printf("删除的元素为：%d\n", e);
		else
			printf("删除第%d个数据失败\n", j);
	}
	printf("依次输出L的元素：");
	ListTraverse(L,visit);  /* 依次对元素调用visit(),输出元素的值 */
}

int Malloc(SLinkList space) {
	/* 若备用链表非空,则返回分配的结点下标(备用链表的第一个结点),否则返回0 */
	int i = space[0].cur;
	if(i)  /* 备用链表非空 */
		space[0].cur = space[i].cur;  /* 备用链表的头结点指向原备用链表的第二个结点 */
	return i;  /* 返回新开辟结点的坐标 */
}

void Free(SLinkList space,int k) {
	/* 将下标为k的空闲结点回收到备用链表(成为备用链表的第一个结点) */
	space[k].cur = space[0].cur;  /* 回收结点的"游标"指向备用链表的第一个结点 */
	space[0].cur = k;  /* 备用链表的头结点指向新回收的结点 */
}

void visit(ElemType e) {
	printf("%d ", e);
}

/* 一个数组只生成一个静态链表 */
void InitList(SLinkList L) {
	/* 构造一个空的链表,表头为L的最后一个单元L[MAXSIZE-1],其余单元链成 */
	/* 一个备用链表，表头为L的第一个单元L[0]，"0"表示空指针 */
	int i;
	L[MAXSIZE-1].cur = 0;  /* L的最后一个单元为空链表的表头 */
	for(i = 0;i < MAXSIZE-2; i++)  /* 将其余单元连接成以L[0]为表头的备用链表 */
		L[i].cur = i+1;
	L[MAXSIZE-2].cur = 0;
	for(i = 0;i < MAXSIZE; i++) {
		printf("i=%d L[i].cur=%d\n", i, L[i].cur);
	}
}

Status ClearList(SLinkList L) {
	/* 初始条件：线性表L已存在。操作结果：将L重置为空表 */
	int i, j, k;
	i = L[MAXSIZE-1].cur;   /* 链表第一个节点的位置 */
	L[MAXSIZE-1].cur = 0;   /* 链表空 */
	k = L[0].cur;  /* 备用链表第一个结点的位置 */
	L[0].cur = i;  /* 把链表的结点连到备用链表的表头 */
	while(i) {  /* 没到链表尾 */
		j = i;
		i = L[i].cur;  /* 指向下一个元素 */
	}
	L[j].cur = k; /* 备用链表的第一个结点接到链表的尾部 */
	return OK;
}

Status ListEmpty(SLinkList L) {
	/* 若L是空表，返回TRUE；否则返回FALSE */
	if(L[MAXSIZE-1].cur == 0)  /* 空表 */
		return TRUE;
	else
		return FALSE;
}

int ListLength(SLinkList L) {
	/* 返回L中数据元素个数 */
	int j = 0,i = L[MAXSIZE-1].cur; /* i指向第一个元素 */
	while(i) {  /* 没到静态链表尾 */
		i = L[i].cur;  /* 指向下一个元素 */
		j++;
	}
	return j;
}

Status GetElem(SLinkList L,int i,ElemType *e) {
	/* 用e返回L中第i个元素的值 */
	int l, k = MAXSIZE-1; /* k指向表头序号 */
	if(i<1 || i>ListLength(L))
		return ERROR;
	for(l = 1;l <= i; l++)  /* 移动到第i个元素处 */
		k = L[k].cur;
	*e = L[k].data;
	return OK;
}

int LocateELem(SLinkList L,ElemType e) {
	/* 在静态单链线性表L中查找第1个值为e的元素。若找到，则返回它在L中的 */
	/* 位序，否则返回0。(与其它LocateElem()的定义不同) */
	int i = L[MAXSIZE-1].cur;  /* i指示表中第一个结点 */
	while(i && L[i].data != e)  /* 在表中顺链查找(e不能是字符串) */
		i = L[i].cur;
	return i;
}

Status PriorElem(SLinkList L,ElemType cur_e,ElemType *pre_e) {
	/* 初始条件：线性表L已存在 */
	/* 操作结果：若cur_e是L的数据元素,且不是第一个，则用pre_e返回它的前驱， */
	/* 			 否则操作失败，pre_e无定义 */
	int j, i = L[MAXSIZE-1].cur;  /* i指示链表第一个结点的位置 */
	do {  /* 向后移动结点 */
		j = i;
		i = L[i].cur;
	}while(i && cur_e!=L[i].data);
	if(i) {/* 找到该元素 */
		*pre_e = L[j].data;
		return OK;
	}
	return ERROR;
}

Status NextElem(SLinkList L,ElemType cur_e,ElemType *next_e) {
	/* 初始条件：线性表L已存在 */
    /* 操作结果：若cur_e是L的数据元素，且不是最后一个，则用next_e返回它的后继， */
    /*           否则操作失败，next_e无定义 */
    int j, i = LocateELem(L,cur_e);  /* 在L中查找第一个值为cur_e的元素的位置 */
    if(i) {/* L中存在元素cur_e */
		j = L[i].cur;  /* cur_e的后继的位置 */
		if(j) {/* cur_e有后继 */
			*next_e = L[j].data;
			return OK;  /* cur_e的后继存在 */
		}
	}
	return ERROR;  /* L不存在cur_e元素,cur_e元素无后继 */
}

Status ListInsert(SLinkList L,int i,ElemType e) {
	/* 在L中第i个元素之前插入新的数据元素e */
	int l, j, k = MAXSIZE-1;  /* k指向表头 */
	if(i<1 || i>ListLength(L)+1)
		return ERROR;
	j = Malloc(L);  /* 申请新单元 */
	// printf("j = %d k = %d\n", j, k);
	if(j) {/* 申请成功 */
		L[j].data = e; /* 赋值给新单元 */
		for(l=1;l<i;l++) { /* 移动i-1个元素 */
			k = L[k].cur;
			// printf("k = %d\n", k);
		}
		L[j].cur = L[k].cur;
		L[k].cur = j;
		// printf("L[j].cur = %d  L[k].cur = %d L[0].cur = %d\n", L[j].cur,L[k].cur,L[0].cur);
		return OK;
	}
	return ERROR;
}

Status ListDelete(SLinkList L,int i,ElemType *e) {
	/* 删除在L中第i个数据元素e，并返回其值 */
	int j, k = MAXSIZE-1;  /* k指向表头 */
	if(i<1 || i>ListLength(L))
		return ERROR;
	for(j = 1;j < i; j++)  /* 移动i-1个元素 */
		k = L[k].cur;
	j = L[k].cur;
	L[k].cur = L[j].cur;
	*e = L[j].data;
	Free(L,j);
	return OK;
}

Status ListTraverse(SLinkList L,void(*vi)(ElemType)) {
	/* 初始条件:线性表L已存在 */
    /* 操作结果:依次对L的每个数据元素调用函数vi()。一旦vi()失败,则操作失败 */
    int i=L[MAXSIZE-1].cur;  /* 指向第一个元素 */
    while(i) {  /* 没到静态链表尾 */
		// printf("%d: %d ", i, L[i].cur);
		vi(L[i].data);  /* 调用vi() */
		// printf("\n");
		i = L[i].cur; /* 指向下一个元素 */
    }
    printf("\n");
    return OK;
}