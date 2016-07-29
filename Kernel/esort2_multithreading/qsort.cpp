#include "stdafx.h"
//快排
int Partition(double *a,int lt,int rt)
{
	int i=lt,j=rt;
	double key=a[lt];   //一般取第一个数为关键字
	while( i<j ){
		/* 从右向左找小于key的数
		/* 这里一旦加上浮点数判等的话，如果两个数相差很近，就会死循环。*/
		while( i<j && (a[j]>key) )  j--;     
		if( i<j ){
			a[i]=a[j];
			i++;
		}
		/* 从左向右找大于key的数 */
		while( i<j && (key>a[i]) )   i++;
		if( i<j ){
			a[j]=a[i];
			j--;
		}
	}
	a[i]=key;
	return i;
}

/* 加上枢轴位置的随机化来对快排进行优化 */
int Partition1(double *a,int lt,int rt)
{
	srand((unsigned)time(NULL));
	int e=rand()%(rt-lt+1)+lt;
	int tmp;
	tmp=a[lt];
	a[lt]=a[e];
	a[e]=tmp;
	int i=lt,j=rt;
	double key=a[lt];   //一般取第一个数为关键字
	while( i<j ){
		/* 从右向左找小于key的数
		/* 这里一旦加上浮点数判等的话，如果两个数相差很近，就会死循环。*/
		while( i<j && (a[j]-key)>accuracy )  j--;     
		a[i]=a[j];
		if( i<j ) i++;
		/* 从左向右找大于key的数 */
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
		QuickSort(a,lt,i-1);  //递归调用
		QuickSort(a,i+1,rt);
	}
}

void QuickSort1(double *a,int lt,int rt)
{
	if( lt<rt ){
		int i=Partition1(a,lt,rt);
		QuickSort1(a,lt,i-1);  //递归调用
		QuickSort1(a,i+1,rt);
	}
}