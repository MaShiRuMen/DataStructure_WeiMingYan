/*  ���Ա�ľ�̬������Ĵ洢�ṹ */
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

typedef int Status; /* Status�Ǻ���������,��ֵ�Ǻ������״̬���룬��OK�� */
typedef int Boolean; /* Boolean�ǲ�������,��ֵ��TRUE��FALSE */
typedef int ElemType;  /* ElemType������������� */

#define MAXSIZE 20 /* �������󳤶� */

typedef struct {
	ElemType data;
	int cur;
}component,SLinkList[MAXSIZE];

int Malloc(SLinkList space); /* �㷨2.15 */
void Free(SLinkList space,int k); /* �㷨2.16 */
void DestroyList();  /* ��̬���鲻�ܱ����� */
void visit(ElemType e);

/* һ������ֻ����һ����̬���� */
void InitList(SLinkList L);
Status ClearList(SLinkList L);
Status ListEmpty(SLinkList L);
int ListLength(SLinkList L);
Status GetElem(SLinkList L,int i,ElemType *e);
int LocateELem(SLinkList L,ElemType e); /* �㷨2.13�иĶ� */
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
	printf("��L�ı�ͷ���β���1~5��L=");
	ListTraverse(L,visit);
	i = ListEmpty(L);
	printf("L�Ƿ�Ϊ�գ�i=%d(1:�� 0:��)��L�ĳ���=%d\n", i, ListLength(L));
	i = ClearList(L);
	printf("���L��L=");
	//����ԭ��ʹ��
	// for(c1 = 0;c1 < MAXSIZE-1; c1++) {
	// 	printf("c1 = %d   cur = %d\n", c1, L[c1].cur);
	// }
	ListTraverse(L,visit);
	i = ListEmpty(L);
	printf("L�Ƿ�Ϊ�գ�i=%d(1:�� 0:��)��L�ĳ���=%d\n", i, ListLength(L));
	for(j = 1;j <= 10; j++)
		ListInsert(L,j,j);
	printf("��L�ı�β���β���1~10��L=");
	ListTraverse(L,visit);
	GetElem(L,5,&e);
	printf("��5��Ԫ�ص�ֵΪ��%d\n", e);
	for(j = 0;j <= 1; j++) {
		k = LocateELem(L,j);
		if(k)
			printf("ֵΪ%d��Ԫ���ھ�̬�����е�λ��Ϊ%d\n", j, k);
		else
			printf("û��ֵΪ%d��Ԫ��\n", j);
	}
	for(j = 1;j <= 2; j++) {  /* ����ͷ�������� */
		GetElem(L,j,&e0); /* �ѵ�j�����ݸ���e0 */
		i = PriorElem(L,e0,&e);  /* ��e0��ǰ�� */
		if(!i)
			printf("Ԫ��%d��ǰ��\n", e0);
		else
			printf("Ԫ��%d��ǰ��Ϊ��%d\n", e0, e);
	}
	for(j = ListLength(L)-1;j <= ListLength(L); j++) {  /* ����������� */
		GetElem(L,j,&e0); /* �ѵ�j�����ݸ���e0 */
		i = NextElem(L,e0,&e);  /* ��e0�ĺ�� */
		if(!i)
			printf("Ԫ��%d�޺��\n", e0);
		else
			printf("Ԫ��%d���Ϊ��%d\n", e0, e);
	}
	k = ListLength(L); /* kΪ�� */
	for(j = k+1;j >= k; j--) {
		i = ListDelete(L,j,&e); /* ɾ����j������ */
		if(i)
			printf("ɾ����Ԫ��Ϊ��%d\n", e);
		else
			printf("ɾ����%d������ʧ��\n", j);
	}
	printf("�������L��Ԫ�أ�");
	ListTraverse(L,visit);  /* ���ζ�Ԫ�ص���visit(),���Ԫ�ص�ֵ */
}

int Malloc(SLinkList space) {
	/* ����������ǿ�,�򷵻ط���Ľ���±�(��������ĵ�һ�����),���򷵻�0 */
	int i = space[0].cur;
	if(i)  /* ��������ǿ� */
		space[0].cur = space[i].cur;  /* ���������ͷ���ָ��ԭ��������ĵڶ������ */
	return i;  /* �����¿��ٽ������� */
}

void Free(SLinkList space,int k) {
	/* ���±�Ϊk�Ŀ��н����յ���������(��Ϊ��������ĵ�һ�����) */
	space[k].cur = space[0].cur;  /* ���ս���"�α�"ָ��������ĵ�һ����� */
	space[0].cur = k;  /* ���������ͷ���ָ���»��յĽ�� */
}

void visit(ElemType e) {
	printf("%d ", e);
}

/* һ������ֻ����һ����̬���� */
void InitList(SLinkList L) {
	/* ����һ���յ�����,��ͷΪL�����һ����ԪL[MAXSIZE-1],���൥Ԫ���� */
	/* һ������������ͷΪL�ĵ�һ����ԪL[0]��"0"��ʾ��ָ�� */
	int i;
	L[MAXSIZE-1].cur = 0;  /* L�����һ����ԪΪ������ı�ͷ */
	for(i = 0;i < MAXSIZE-2; i++)  /* �����൥Ԫ���ӳ���L[0]Ϊ��ͷ�ı������� */
		L[i].cur = i+1;
	L[MAXSIZE-2].cur = 0;
	for(i = 0;i < MAXSIZE; i++) {
		printf("i=%d L[i].cur=%d\n", i, L[i].cur);
	}
}

Status ClearList(SLinkList L) {
	/* ��ʼ���������Ա�L�Ѵ��ڡ������������L����Ϊ�ձ� */
	int i, j, k;
	i = L[MAXSIZE-1].cur;   /* �����һ���ڵ��λ�� */
	L[MAXSIZE-1].cur = 0;   /* ����� */
	k = L[0].cur;  /* ���������һ������λ�� */
	L[0].cur = i;  /* ������Ľ��������������ı�ͷ */
	while(i) {  /* û������β */
		j = i;
		i = L[i].cur;  /* ָ����һ��Ԫ�� */
	}
	L[j].cur = k; /* ��������ĵ�һ�����ӵ������β�� */
	return OK;
}

Status ListEmpty(SLinkList L) {
	/* ��L�ǿձ�����TRUE�����򷵻�FALSE */
	if(L[MAXSIZE-1].cur == 0)  /* �ձ� */
		return TRUE;
	else
		return FALSE;
}

int ListLength(SLinkList L) {
	/* ����L������Ԫ�ظ��� */
	int j = 0,i = L[MAXSIZE-1].cur; /* iָ���һ��Ԫ�� */
	while(i) {  /* û����̬����β */
		i = L[i].cur;  /* ָ����һ��Ԫ�� */
		j++;
	}
	return j;
}

Status GetElem(SLinkList L,int i,ElemType *e) {
	/* ��e����L�е�i��Ԫ�ص�ֵ */
	int l, k = MAXSIZE-1; /* kָ���ͷ��� */
	if(i<1 || i>ListLength(L))
		return ERROR;
	for(l = 1;l <= i; l++)  /* �ƶ�����i��Ԫ�ش� */
		k = L[k].cur;
	*e = L[k].data;
	return OK;
}

int LocateELem(SLinkList L,ElemType e) {
	/* �ھ�̬�������Ա�L�в��ҵ�1��ֵΪe��Ԫ�ء����ҵ����򷵻�����L�е� */
	/* λ�򣬷��򷵻�0��(������LocateElem()�Ķ��岻ͬ) */
	int i = L[MAXSIZE-1].cur;  /* iָʾ���е�һ����� */
	while(i && L[i].data != e)  /* �ڱ���˳������(e�������ַ���) */
		i = L[i].cur;
	return i;
}

Status PriorElem(SLinkList L,ElemType cur_e,ElemType *pre_e) {
	/* ��ʼ���������Ա�L�Ѵ��� */
	/* �����������cur_e��L������Ԫ��,�Ҳ��ǵ�һ��������pre_e��������ǰ���� */
	/* 			 �������ʧ�ܣ�pre_e�޶��� */
	int j, i = L[MAXSIZE-1].cur;  /* iָʾ�����һ������λ�� */
	do {  /* ����ƶ���� */
		j = i;
		i = L[i].cur;
	}while(i && cur_e!=L[i].data);
	if(i) {/* �ҵ���Ԫ�� */
		*pre_e = L[j].data;
		return OK;
	}
	return ERROR;
}

Status NextElem(SLinkList L,ElemType cur_e,ElemType *next_e) {
	/* ��ʼ���������Ա�L�Ѵ��� */
    /* �����������cur_e��L������Ԫ�أ��Ҳ������һ��������next_e�������ĺ�̣� */
    /*           �������ʧ�ܣ�next_e�޶��� */
    int j, i = LocateELem(L,cur_e);  /* ��L�в��ҵ�һ��ֵΪcur_e��Ԫ�ص�λ�� */
    if(i) {/* L�д���Ԫ��cur_e */
		j = L[i].cur;  /* cur_e�ĺ�̵�λ�� */
		if(j) {/* cur_e�к�� */
			*next_e = L[j].data;
			return OK;  /* cur_e�ĺ�̴��� */
		}
	}
	return ERROR;  /* L������cur_eԪ��,cur_eԪ���޺�� */
}

Status ListInsert(SLinkList L,int i,ElemType e) {
	/* ��L�е�i��Ԫ��֮ǰ�����µ�����Ԫ��e */
	int l, j, k = MAXSIZE-1;  /* kָ���ͷ */
	if(i<1 || i>ListLength(L)+1)
		return ERROR;
	j = Malloc(L);  /* �����µ�Ԫ */
	// printf("j = %d k = %d\n", j, k);
	if(j) {/* ����ɹ� */
		L[j].data = e; /* ��ֵ���µ�Ԫ */
		for(l=1;l<i;l++) { /* �ƶ�i-1��Ԫ�� */
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
	/* ɾ����L�е�i������Ԫ��e����������ֵ */
	int j, k = MAXSIZE-1;  /* kָ���ͷ */
	if(i<1 || i>ListLength(L))
		return ERROR;
	for(j = 1;j < i; j++)  /* �ƶ�i-1��Ԫ�� */
		k = L[k].cur;
	j = L[k].cur;
	L[k].cur = L[j].cur;
	*e = L[j].data;
	Free(L,j);
	return OK;
}

Status ListTraverse(SLinkList L,void(*vi)(ElemType)) {
	/* ��ʼ����:���Ա�L�Ѵ��� */
    /* �������:���ζ�L��ÿ������Ԫ�ص��ú���vi()��һ��vi()ʧ��,�����ʧ�� */
    int i=L[MAXSIZE-1].cur;  /* ָ���һ��Ԫ�� */
    while(i) {  /* û����̬����β */
		// printf("%d: %d ", i, L[i].cur);
		vi(L[i].data);  /* ����vi() */
		// printf("\n");
		i = L[i].cur; /* ָ����һ��Ԫ�� */
    }
    printf("\n");
    return OK;
}