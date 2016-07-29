#include "stdafx.h"
#include "bsort.h" 
typedef double   *PDouble;
typedef struct _SortParam
{
	KeyType *buff;
    int begin;   //开始位置
	int count;
	int lapNum;
};
typedef struct _SortParam  * SortParam;
const int w=256; 
/************************************************************************/
/* 桶排序                                                                */
/************************************************************************/

void BucketSort(KeyType *a,int n)
{
	//preprocess(a,n);
	const int bt=sizeof(double);
	PDouble pd;
	double * btmp, *temp;
	btmp=new double[n];
	int c[w],i,j,index,ct;
	for (i=0; i<bt; i++){    //共八个字节，从低到高分为八次统计
	   temp=a, a=btmp, btmp=temp;  //a,b数组交换指针
	   for (j=0; j<w; j++){ //计数器清零
		  c[j]=0;
	   }
	   for (j=0; j<n; j++){ //统计计数
		  pd=btmp+j;
		  index= *((LPBYTE)pd+i);
		  c[index]++;
	   }
	   c[w-1] = n - c[w-1];     //c[0]+c[1]+c[2]+...+c[255]=n
	   for (j=w-2; j>=0; j--){ //计算各段起始位置
		  c[j]=c[j+1] - c[j];
	   }
	   for (j=0; j<n; j++){  //复制回数组a
		  pd=btmp+j;
		  index= *((LPBYTE)pd+i);
		  ct = c[index]++;
	      a[ct] = btmp[j]; 
	   } 
	}
	for (i=0; i<n && a[i]>=0.0; i++); //统计非负数个数 结束时i为非负数的个数O
	for (j=0; j<i / 2; j++){ //翻转前面的非负数次序O,使整体从大到小 
		double dt = a[j];
		a[j]=a[i-1-j];
		a[i-1-j]=dt; 
	}
	for(i=0,j=n-1;i<j;i++,j--){  //整体翻转，使之从小到大 
		double dt = a[i];
		a[i]=a[j];
		a[j]=dt;                       
	}
	delete[] btmp;
}


int cc[w];    //临界区资源，即共享资源
CRITICAL_SECTION g_cs;
/*** 一趟桶分配和收集：待排序元素，元素个数，趟数 ***/
/**********************反复调试了五六个小时，才猛然虽然发现多个线程同时访问btmp数组不会有问题，可多个线程同时写cc数组一定会出问题**************************/
unsigned __stdcall bSort(LPVOID param)
{
    SortParam stParam=(SortParam)param;
	KeyType * btmp=stParam->buff;
	int begin=stParam->begin;
	int n=stParam->count;
	int i=stParam->lapNum;    //第i趟	
    PDouble pd;	
	int index;	
	EnterCriticalSection(&g_cs);
	for (int j=begin; j<(begin+n); j++){ //统计计数
		  pd=btmp+j;
		  index= *((LPBYTE)pd+i);
		  cc[index]++;
	}
	LeaveCriticalSection(&g_cs);
	return 0;
}

/*** 负载均衡发多线程并行基数排序 ***/
void  MultiThreadBsort(KeyType * a,int n)
{
	
	int uCount=n/threadNum;     //每段大小
	const int bt=sizeof(KeyType);    //总的趟数
	PDouble pd;
	int index,ct;
	int i,j;
	KeyType * btmp, *temp;
	btmp=new KeyType[n];
	HANDLE * hth=new HANDLE[threadNum];
	InitializeCriticalSection(&g_cs);   //初始化临界区
	for(i=0;i<bt;i++){		
		temp=a, a=btmp, btmp=temp;  //a,b数组交换指针
		for ( j=0; j<w; j++){ //计数器清零
		    cc[j]=0;
	    }
		
		for( j=0;j<threadNum;j++){
			SortParam stParam=new _SortParam();
			stParam->buff=btmp;
			stParam->begin=j*uCount;
			stParam->count=uCount;
			if( j == threadNum-1 ){
				stParam->count = n - (j*uCount);
			}
			stParam->lapNum=i;
			hth[j] = (HANDLE)_beginthreadex(NULL, 0, bSort, (void *)stParam, 0, NULL);
		}
		WaitForMultipleObjects(threadNum,hth,true,INFINITE);
		for( j=0;j<threadNum;j++){
			CloseHandle(hth[j]);
	    }
		
		cc[w-1] = n - cc[w-1];     //c[0]+c[1]+c[2]+...+c[255]=n
	    for ( j=w-2; j>=0; j-- ){ //计算各段起始位置
			cc[j]=cc[j+1] - cc[j];		   
	    }
	    for ( j=0; j<n; j++){  //复制回数组a
		   pd=btmp+j;
		   index= *((LPBYTE)pd+i);
		   ct = cc[index]++;
	       a[ct] = btmp[j]; 
	    } 
	}	
	for (i=0; i<n && a[i]>=0.0; i++); //统计非负数个数 结束时i为非负数的个数O
	for (j=0; j<i / 2; j++){ //翻转前面的非负数次序O,使整体从大到小 
		KeyType dt = a[j];
		a[j]=a[i-1-j];
		a[i-1-j]=dt; 
	}
	for(i=0,j=n-1;i<j;i++,j--){  //整体翻转，使之从小到大 
		KeyType dt = a[i];
		a[i]=a[j];
		a[j]=dt;                       
	}
 	//_ASSERTE( _CrtCheckMemory( ) );
	delete[] btmp;	
	delete[] hth;
	
}
