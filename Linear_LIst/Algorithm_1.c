/*  �������õ�ͷ�ļ��� */
#include <string.h>
#include <ctype.h>
#include <malloc.h>	/* malloc() �� */
#include <limits.h>	/* INT_MAX ��*/
#include <stdio.h>	/* EOF(=^Z��F6),NULL */
#include <stdlib.h>	/* atoi() */
#include <io.h>	/* eof() */
#include <math.h>	/* floor(),ceil(),abs() */
#include <process.h> 	/* exit() */
/* �������״̬���� */
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
/* #define OVERFLOW -2 ��Ϊ��math.h���Ѷ���OVERFLOW��ֵΪ3,��ȥ������ */
#define LIST_INIT_SIZE 10 /* ���Ա�洢�ռ�ĳ�ʼ������ */
#define LISTINCREMENT 2  /* ���Ա�洢�ռ�ķ������� */

typedef int Status; /* Status�Ǻ���������,��ֵ�Ǻ������״̬���룬��OK�� */
typedef int Boolean; /* Boolean�ǲ�������,��ֵ��TRUE��FALSE */
typedef int ElemType;  /* ElemType�������������*/

typedef struct {
	ElemType *elem;  /* �洢�ռ��ַ */
	int length; /* ��ǰ���� */
	int listsize; /* ��ǰ����Ĵ洢����(��sizeof(ElemType)Ϊ��λ) */
}SqList;

Status InitList(SqList *L); /* Algorithm 2.3 */
Status DestroyList(SqList *L);
Status ClearList(SqList *L);
Status ListEmpty(SqList L);
Status ListLength(SqList L);
Status GetElem(SqList L,int i,ElemType *e);
Status PriorElem(SqList L,ElemType cur_e,ElemType *pre_e);
Status NextElem(SqList L,ElemType cur_e,ElemType *next_e);
Status ListInsert(SqList *L,int i,ElemType e); /* Algorithm 2.4 */
Status ListDelete(SqList *L,int i,ElemType *e); /* Algorithm 2.5 */
Status ListTraverse(SqList L,void(*vi)(ElemType*));
Status equal(ElemType c1,ElemType c2);
Status comp(ElemType c1,ElemType c2);
int LocateElem(SqList L,ElemType e,Status(*compare)(ElemType,ElemType));
void visit(ElemType *c);
void dbl(ElemType *c);
void Union(SqList *La,SqList Lb); /* Algorithm 2.1 */
void print(ElemType *c);


// void main(void) {
// 	SqList La, Lb;
// 	Status i;
// 	int j;
// 	i = InitList(&La);
// 	if (i == 1) /* creat an empty list La successfully */ {
// 		for (j = 1;j <= 5; j++) /* insert five elements to a list */
// 			i = ListInsert(&La, j, j);
// 	}
// 	printf("La= "); /* print the content of list La */	
// 	ListTraverse(La,print);
// 	InitList(&Lb); /* do it or not depends on you */
// 	for (j = 1;j <= 5; j++) /* insert five elements to list Lb */
// 		i = ListInsert(&Lb,j,2*j);
// 	printf("Lb= "); /* print the content of list Lb */
// 	ListTraverse( Lb, print);
// 	Union(&La,Lb);
// 	printf("new La= "); /* print the content of new list la */
// 	ListTraverse(La,print);
// }

void main() {
	SqList L;
	ElemType e, e0;
	Status i;
	int j, k;
	i = InitList(&L);
	printf("��ʼ��L��L.elem=%u L.length=%d L.listsize=%d\n", L.elem,L.length,L.listsize);
	for (j = 1;j <= 5; j++)
		i = ListInsert(&L,1,j);
	printf("��L�ı�ͷ���β���1~5��*L.elem=");
	for (j = 1;j <= 5; j++)
		printf("%d ", *(L.elem+j-1));
	printf("\n");
	printf("L.elem=%u L.length=%d L.listsize=%d\n", L.elem, L.length, L.listsize);
	i = ListEmpty(L);
	printf("L�Ƿ�Ϊ�գ�i=%d(1:�� 0:��)\n", i);
	for (j = 1; j <= 10; j++)
		ListInsert(&L,j,j);
	printf("��L�ı�β���β���1~10��*L.elem=");
	for (j = 1;j <= 10; j++)
		printf("%d ", *(L.elem+j-1));
	printf("\n");
	printf("L.elem=%u L.length=%d L,listsize=%d\n", L.elem, L.length, L.listsize);
	ListInsert(&L,1,0);
	printf("��L�ı�ͷ����0��*L.elem=");
	for (j = 1;j <= ListLength(L); j++) /* ListLength(L)ΪԪ�ظ��� */
		printf("%d ", *(L.elem+j-1));
	printf("\n");
	printf("L.elem=%u(�п��ܸı�) L.length=%d(�ı�) L.listsize=%d(�ı�)\n", L.elem, L.length, L.listsize);
	GetElem(L,5,&e);
	printf("��5��Ԫ�ص�ֵΪ��%d\n", e);
	for (j = 3;j <= 4; j++) {
		k = LocateElem(L,j,comp);
		if(k) printf("��%d��Ԫ�ص�ֵΪ%d��ƽ��\n", k, j);
		else  printf("û��ֵΪ%d��ƽ����Ԫ��\n",j);
	}
	for (j = 1;j <= 2; j++) { /* ����ͷ�������� */
		GetElem(L,j,&e); /* ����ͷ�������� */
		i = PriorElem(L,e0,&e); /* ��e0��ǰ�� */
		if (i == INFEASIBLE ) printf("Ԫ��%d��ǰ��\n", e0);
		else printf("Ԫ��%d��ǰ��Ϊ��%d\n", e0, e);
	}
	for (j = ListLength(L)-1;j <= ListLength(L); j++) {  /* ����������� */
		GetElem(L,j,&e0);  /* �ѵ�j�����ݸ���e0 */
		i = NextElem(L,e0,&e); /* ��e0�ĺ�� */
		if (i == INFEASIBLE) printf("Ԫ��%d�޺��\n", e0);
		else printf("Ԫ��%d�ĺ��Ϊ��%d\n", e0, e);
	}
	k = ListLength(L); /* kΪ�� */
	for (j = k+1;j >= k; j--) {
		i = ListDelete(&L,j,&e); /* ɾ����j������ */
		if (i == ERROR) printf("ɾ����%d������ʧ��\n", j);
		else printf("ɾ����Ԫ��ֵΪ��%d\n", e);
	}
	printf("�������L��Ԫ�أ�");
	ListTraverse(L,visit); /* ���ζ�Ԫ�ص���visit(),���Ԫ�ص�ֵ */
	printf("L��Ԫ��ֵ�ӱ���");
	ListTraverse(L,dbl); /* ���ζ�Ԫ�ص���dbl(),Ԫ��ֵx2 */
	ListTraverse(L,visit);
	DestroyList(&L);
	printf("����L��L.elem=%u L.length=%d L.listsize=%d\n", L.elem, L.length, L.listsize);
}

Status InitList(SqList *L) {
	/* �������������һ���յ�˳�����Ա� */
	(*L).elem = (ElemType*)malloc(LIST_INIT_SIZE*sizeof(ElemType));
	if (!(*L).elem)
		exit(OVERFLOW); /* ����洢ʧ�� */
	(*L).length = 0; /* �ձ���Ϊ0 */
	(*L).listsize = LIST_INIT_SIZE; /* ��ʼ�洢���� */
	return OK;
}

Status DestroyList(SqList *L) {
	/* ��ʼ������˳�����Ա�L�Ѵ��ڡ��������������˳�����Ա�L */
	free((*L).elem);
	(*L).elem = NULL;
	(*L).length = 0;
	(*L).listsize = 0;
	return OK;
}

Status ClearList(SqList *L) {
	/* ��ʼ������˳Ѱ���Ա�L�Ѵ��ڡ������������L����Ϊ�ձ� */
	(*L).length = 0;
	return OK;
}

Status ListEmpty(SqList L) {
	/* ��ʼ������˳Ѱ���Ա�L�Ѵ��ڡ������������LΪ�ձ��򷵻�TRUE�����򷵻�FALSE */
	if (L.length == 0) return TRUE;
	else return FALSE;
}

Status ListLength(SqList L) {
	/* ��ʼ������˳�����Ա�L�Ѵ��ڡ��������������L������Ԫ�ظ��� */
	return L.length;
}

Status GetElem(SqList L,int i,ElemType *e) {
	/* ��ʼ������˳�����Ա�L�Ѵ��ڣ�1��i��ListLength(L) */
	/* �����������e����L�е�i������Ԫ�ص�ֵ */
	if (i<1||i>L.length)
		exit(ERROR);
	*e = *(L.elem+i-1);
	return OK;
}

int LocateElem(SqList L,ElemType e,Status(*compare)(ElemType,ElemType)) {
	/* ��ʼ������˳�����Ա�L�Ѵ��ڣ�compare()������Ԫ���ж�����(����Ϊ1������Ϊ0) */
	/* �������������L�е�1����e�����ϵcompare()������Ԫ�ص�λ�� */
	/* ������������Ԫ�ز����ڣ��򷵻�ֵΪ0�� */
	ElemType *p;
	int i = 1;
	p = L.elem;
	while (i <= L.length && !compare(*p++,e))
		++i;
	if (i <= L.length)
		return i;
	else
		return 0;
}

Status PriorElem(SqList L,ElemType cur_e,ElemType *pre_e) {
	/* ��ʼ������˳�����Ա�L�Ѵ��� */
	/* �����������cur_e��L������Ԫ�أ��Ҳ��ǵ�һ��������pre_e��������ǰ�� */
	/* �������ʧ�ܣ�pre_e�޶��� */
	int i = 2;
	ElemType *p=L.elem+1;
	while (i <= L.length && *p!=cur_e) {
		p++;
		i++;
	}
	if (i > L.length)
		return INFEASIBLE;
	else {
		*pre_e = *--p;
		return OK;
	}
}

Status NextElem(SqList L,ElemType cur_e,ElemType *next_e) {
	/* ��ʼ������˳�����Ա�L�Ѵ��� */
	/* �����������cur_e��L������Ԫ�أ��Ҳ������һ��������next_e�������ĺ�� */
	/*           �������ʧ�ܣ�next_e�޶���*/
	int i = 1;
	ElemType *p = L.elem;
	while (i<L.length && *p!=cur_e) {
		i++; p++;
	}
	if (i == L.length)
		return INFEASIBLE;
	else {
		*next_e = *++p;
		return OK;
	}
}

Status ListInsert(SqList *L, int i, ElemType e) {
	/* ��ʼ������˳�����Ա�L�Ѵ��ڣ�1��i��ListLength(L)+1 */
	/* �����������L�е�i��λ��֮ǰ�����µ�����Ԫ��e��L�ĳ���+1*/
	ElemType *newbase, *q, *p;
	if (i<1 || i>(*L).length+1) /* iֵ���Ϸ� */
		return ERROR;
	if ((*L).length >= (*L).listsize) { /* ��ǰ�洢�ռ����������ӷ���ռ�*/
		newbase = (ElemType *)realloc((*L).elem,((*L).listsize+LISTINCREMENT)*sizeof(ElemType));
		if (!newbase)
			exit(OVERFLOW); /* �洢����ʧ�� */
		(*L).elem = newbase; /* �»�ַ */
		(*L).listsize+=LISTINCREMENT; /* ���Ӵ洢���� */
	} 
	q = (*L).elem+i-1; /* qΪ����λ�� */
	for (p = (*L).elem+(*L).length-1;p>=q;--p) /* ����λ�ü�֮���Ԫ������ */
		*(p+1) = *p;
	*q = e; /* ����e */
	++(*L).length; /* ��+1 */
	return OK;
}

Status ListDelete(SqList *L,int i,ElemType *e) {
	/* ��ʼ������˳�����Ա�L�Ѵ��ڣ�1��i��ListLength(L) */
	/* ���������ɾ��L�ĵ�i������Ԫ�أ�����e������ֵ,L�ĳ���-1*/
	ElemType *p, *q;
	if (i<1||i>(*L).length) /* iֵ���Ϸ� */
		return ERROR;
	p = (*L).elem+i-1; /* pΪ��ɾ��Ԫ�ص�λ�� */
	*e = *p; /* ��ɾ��Ԫ�ص�ֵ����e */
	q = (*L).elem+(*L).length-1; /* ��βԪ�ص�λ�� */
	for (++p;p <= q; ++p) /* ��Ԫ��֮���Ԫ������ */
		*(p-1) = *p;
	(*L).length--; /* ��-1 */
	return OK;
}

Status ListTraverse(SqList L, void(*vi)(ElemType*)) {
	/* ��ʼ������˳�����Ա�L�Ѵ��� */
	/* ������������ζ�L��ÿ������Ԫ�ص��ú���vi()��һ��vi()ʧ�ܣ������ʧ�� */
	/*           vi()���βμӡ�&����������ͨ������vi()�ı�Ԫ�ص�ֵ*/
	ElemType *p;
	int i;
	p = L.elem;
	for (i = 1;i <= L.length; i++)
		vi(p++);
	printf("\n");
	return OK;
}

Status equal(ElemType c1,ElemType c2) {
	/* �ж��Ƿ���ȵĺ�����Unio()�õ� */
	if ( c1 == c2) return TRUE;
	else return FALSE;
}

void Union(SqList *La,SqList Lb) {
	/* �����������Ա�Lb�е�����La�е�����Ԫ�ز��뵽La�� */
	ElemType e;
	int La_len, Lb_len;
	int i;
	La_len = ListLength(*La); /* �����Ա�ĳ��� */
	Lb_len = ListLength(Lb);
	for (i = 1;i <= Lb_len; i++) {
		GetElem(Lb,i,&e); /*ȡLb�е�i������Ԫ�ظ���e */
		if (!LocateElem(*La,e,equal)) /* La�в����ں�e��ͬ��Ԫ�أ������ */
			ListInsert(La,++La_len,e);
	}
}

void print(ElemType *c) {
	printf("%d ", *c);
}

Status comp(ElemType c1,ElemType c2) {
	/* ����Ԫ���ж�����(ƽ����ϵ) */
	if (c1==c2*c2) return TRUE;
	else return FALSE;
}

void visit(ElemType *c) {
	/* ListTraverse()���õĺ��� */
	printf("%d ", *c);
}

void dbl(ElemType *c) {
	/* ListTraverse()���õ���һ���� */
	*c *= 2;
}
