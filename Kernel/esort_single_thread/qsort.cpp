#include "stdafx.h"
//����
int Partition(double *a,int lt,int rt)
{
	int i=lt,j=rt;
	double key=a[lt];   //һ��ȡ��һ����Ϊ�ؼ���
	while( i<j ){
		/* ����������С��key����
		/* ����һ�����ϸ������еȵĻ���������������ܽ����ͻ���ѭ����*/
		while( i<j && (a[j]>key) )  j--;     
		if( i<j ){
			a[i]=a[j];
			i++;
		}
		/* ���������Ҵ���key���� */
		while( i<j && (key>a[i]) )   i++;
		if( i<j ){
			a[j]=a[i];
			j--;
		}
	}
	a[i]=key;
	return i;
}

/* ��������λ�õ���������Կ��Ž����Ż� */
int Partition1(double *a,int lt,int rt)
{
	srand((unsigned)time(NULL));
	int e=rand()%(rt-lt+1)+lt;
	int tmp;
	tmp=a[lt];
	a[lt]=a[e];
	a[e]=tmp;
	int i=lt,j=rt;
	double key=a[lt];   //һ��ȡ��һ����Ϊ�ؼ���
	while( i<j ){
		/* ����������С��key����
		/* ����һ�����ϸ������еȵĻ���������������ܽ����ͻ���ѭ����*/
		while( i<j && (a[j]-key)>accuracy )  j--;     
		a[i]=a[j];
		if( i<j ) i++;
		/* ���������Ҵ���key���� */
		while( i<j && (key-a[i])>accuracy )   i++;
		a[j]=a[i];
		if( i<j ) j--;
	}
	a[i]=key;
	return i;
}


void QuickSort(double *a,int lt,int rt)
{
	if( lt<rt ){
		int i=Partition(a,lt,rt);
		QuickSort(a,lt,i-1);  //�ݹ����
		QuickSort(a,i+1,rt);
	}
}

void QuickSort1(double *a,int lt,int rt)
{
	if( lt<rt ){
		int i=Partition1(a,lt,rt);
		QuickSort1(a,lt,i-1);  //�ݹ����
		QuickSort1(a,i+1,rt);
	}
}