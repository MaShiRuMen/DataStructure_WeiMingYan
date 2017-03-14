/*  设立尾指针的单循环链表 */
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

struct LNode {
	ElemType data;
	struct LNode *next;
};
typedef struct LNode *LinkList;

Status compare(ElemType c1,ElemType c2);
void visit(ElemType c);
Status InitList_CL(LinkList *L);
Status DestroyList_CL(LinkList *L);
Status ClearList_CL(LinkList *L);	/* 改变L */
Status ListEmpty_CL(LinkList L);
Status ListLength_CL(LinkList L);
Status GetElem_CL(LinkList L,int i,ElemType *e);
int LocateElem_CL(LinkList L,ElemType e,Status(*compare)(ElemType,ElemType));
Status PriorElem_CL(LinkList L,ElemType cur_e,ElemType *pre_e);
Status NextElem_CL(LinkList L,ElemType cur_e,ElemType *next_e);
Status ListInsert_CL(LinkList *L,int i,ElemType e);		/* 改变L */
Status ListDelete_CL(LinkList *L,int i,ElemType *e);	/* 改变L */
Status ListTraverse_CL(LinkList L,void(*vi)(ElemType));

void main() {
	LinkList L;
	ElemType e;
	int j;
	Status i;
	i = InitList_CL(&L);  /* 初始化单循环链表L */
	printf("初始化单循环链表L i=%d (1:初始化成功)\n",i);
	i = ListEmpty_CL(L);
	printf("L是否空 i=%d(1:空 0:否)\n",i);
	ListInsert_CL(&L,1,3);	/* 在L中依次插入3,4,5 */
	ListInsert_CL(&L,2,4);
	ListInsert_CL(&L,3,5);
	i = GetElem_CL(L,1,&e);
	j = ListLength_CL(L);
	printf("L中数据元素个数=%d,第1个元素的值为%d。\n",j,e);
    printf("L中的数据元素依次为：");
    ListTraverse_CL(L,visit);
    PriorElem_CL(L,5,&e);	/* 求元素5的前驱 */
    printf("5前面的元素的值为%d。\n",e);
    NextElem_CL(L,3,&e);	/* 求元素3的后继 */
    printf("3后面的元素的值为%d。\n",e);
    printf("L是否空 %d(1:空 0:否)\n",ListEmpty_CL(L));
    j = LocateElem_CL(L,5,compare);
    if(j)
    	printf("L的第%d个元素为5。\n",j);
    else
    	printf("不存在值为5的元素\n");
    i = ListDelete_CL(&L,2,&e);
    printf("删除L的第2个元素：\n");
    if(i) {
    	printf("删除的元素值为%d,现在L中的数据元素依次为：",e);
    	ListTraverse_CL(L,visit);
    }
    else
    	printf("删除不成功！\n");
    printf("清空L：%d(1: 成功)\n",ClearList_CL(&L));
    printf("清空L后，L是否空：%d(1:空 0:否)\n",ListEmpty_CL(L));
    printf("销毁L：%d(1: 成功)\n",DestroyList_CL(&L));
}

Status compare(ElemType c1,ElemType c2) {
	if(c1 == c2) return TRUE;
	else return FALSE;
}

void visit(ElemType c) {
	printf("%d ", c);
}

Status InitList_CL(LinkList *L) {
	/* 操作结果：构造一个空的线性表L */
	*L = (LinkList)malloc(sizeof(struct LNode));	/* 产生头结点,并使L指向此头结点 */
	if(!*L)		/* 存储分配失败 */
		exit(OVERFLOW);
	(*L)->next = *L;	/* 指针域指向头结点 */
		return OK;
}

Status DestroyList_CL(LinkList *L) {
	/* 操作结果：销毁线性表L */
	LinkList q, p=(*L)->next;	/* p指向头结点 */
	while(p != *L) {	/* 没到表尾 */
		q = p->next;
		free(p);
		p = q;
	}
	free(*L);
	*L = NULL;
	return OK;
}

Status ClearList_CL(LinkList *L) {
	/* 初始条件：线性表L已存在。操作结果：将L重置为空表 */
	LinkList p, q;
	*L = (*L)->next; /* L指向头结点 */
	p = (*L)->next;	/* p指向第一个结点 */
	while(p != *L) {	/* 没到表尾 */
		q = p->next;
		free(p);
		p = q;
	}
	(*L)->next = *L;	/* 头结点指针域指向自身 */
	return OK;
}

Status ListEmpty_CL(LinkList L) {
	/* 初始条件：线性表L已存在。操作结果：若L为空表，则返回TRUE，否则返回FALSE */
	if(L->next == L)	/* 空 */
		return TRUE;
	else
		return FALSE;
}

int ListLength_CL(LinkList L) {
	int i = 0;
	LinkList p = L->next;	/* p指向头结点 */
	while(p != L) {	/* 没到表尾 */
		i++;
		p = p->next;
	}
	return i;
}

Status GetElem_CL(LinkList L,int i,ElemType *e) {
	/* 当第i个元素存在时,其值赋给e并返回OK,否则返回ERROR */
	int j = 1;	/* 初始化,j为计数器 */
	LinkList p = L->next->next;	/* p指向第一个结点 */
	if(i<=0||i>ListLength_CL(L))	/* 第i个元素不存在 */
		return ERROR;
	while(j < i) {	/* 顺指针向后查找,直到p指向第i个元素 */
		p = p->next;
		j++;
	}
	*e = p->data;	/* 取第i个元素 */
	return OK;
}

int LocateElem_CL(LinkList L,ElemType e,Status(*compare)(ElemType,ElemType)) {
	/* 初始条件：线性表L已存在，compare()是数据元素判定函数 */
    /* 操作结果：返回L中第1个与e满足关系compare()的数据元素的位序。 */
    /*           若这样的数据元素不存在，则返回值为0 */
    int i = 0;
    LinkList p = L->next->next;	/* p指向第一个结点 */
    while(p != L->next) {
    	i++;
    	if(compare(p->data,e))	/* 满足关系 */
    		return i;
    	p = p->next;
    }
    return 0;
}

Status PriorElem_CL(LinkList L,ElemType cur_e,ElemType *pre_e) {
	/* 初始条件：线性表L已存在 */
    /* 操作结果：若cur_e是L的数据元素，且不是第一个，则用pre_e返回它的前驱， */
    /*           否则操作失败，pre_e无定义 */
    LinkList q, p = L->next->next;	/* p指向第一个结点 */
    q = p->next;
    while(q != L->next) {	/* p没到表尾 */
    	if(q->data == cur_e) {
    		*pre_e = p->data;
    		return TRUE;
    	}
    	p = q;
    	q = q->next;
    }
    return FALSE;
}

Status NextElem_CL(LinkList L,ElemType cur_e,ElemType *next_e) {
	/* 初始条件：线性表L已存在 */
    /* 操作结果：若cur_e是L的数据元素,且不是最后一个,则用next_e返回它的后继， */
    /*           否则操作失败，next_e无定义 */
    LinkList p = L->next->next;	/* p指向第一个结点 */
    while(p != L) {	/* p没到表尾 */
		if( cur_e == p->data) {
			*next_e = p->data;
			return TRUE;
		}
		p = p->next;
    }
    return FALSE;
}

Status ListInsert_CL(LinkList *L,int i,ElemType e) {
	/* 在L的第i个位置之前插入元素e */
	LinkList p = (*L)->next, s;	/* p指向头结点 */
	int j = 0;
	if(i <= 0 || i > ListLength_CL(*L)+1)	/* 无法在第i个元素之前插入 */
		return ERROR;
	while(j < i-1) {	/* 寻找第i-1个结点 */
		p = p->next;
		j++;	
	}
	s = (LinkList)malloc(sizeof(struct LNode));	/* 生成新结点 */
	s->data = e;	/* 插入L中 */
	s->next = p->next;
	p->next = s;
	if(p == *L) /* 改变尾结点 */
		*L = s;
	return OK;
}

Status ListDelete_CL(LinkList *L,int i,ElemType *e) {
	/* 删除L的第i个元素,并由e返回其值 */
	LinkList p = (*L)->next, q; /* p指向头结点 */
	int j = 0;
	if(i <= 0 || i > ListLength_CL(*L))	/* 第i个元素不存在 */
		return ERROR;
	while(j < i-1) {	/* 寻找第i-1个结点 */
		p = p->next;
		j++;
	}
	q = p->next;	/* q指向待删除结点 */
	p->next = q->next;
	*e = q->data;
	if(*L == q)	/* 删除的是表尾元素 */
		*L = p;
	free(q);	/* 释放待删除结点 */
	return OK;
}

Status ListTraverse_CL(LinkList L,void(*vi)(ElemType)) {
	/* 初始条件:L已存在。操作结果:依次对L的每个数据元素调用函数vi()。一旦vi()失败,则操作失败 */
	LinkList p = L->next->next;
	while(p != L->next) {
		vi(p->data);
		p = p->next;
	}
	printf("\n");
	return OK;
}