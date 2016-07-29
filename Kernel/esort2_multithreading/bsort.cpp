#include "stdafx.h"
#include "bsort.h" 
typedef double   *PDouble;
typedef struct _SortParam
{
	KeyType *buff;
    int begin;   //��ʼλ��
	int count;
	int lapNum;
};
typedef struct _SortParam  * SortParam;
const int w=256; 
/************************************************************************/
/* Ͱ����                                                                */
/************************************************************************/

void BucketSort(KeyType *a,int n)
{
	//preprocess(a,n);
	const int bt=sizeof(double);
	PDouble pd;
	double * btmp, *temp;
	btmp=new double[n];
	int c[w],i,j,index,ct;
	for (i=0; i<bt; i++){    //���˸��ֽڣ��ӵ͵��߷�Ϊ�˴�ͳ��
	   temp=a, a=btmp, btmp=temp;  //a,b���齻��ָ��
	   for (j=0; j<w; j++){ //����������
		  c[j]=0;
	   }
	   for (j=0; j<n; j++){ //ͳ�Ƽ���
		  pd=btmp+j;
		  index= *((LPBYTE)pd+i);
		  c[index]++;
	   }
	   c[w-1] = n - c[w-1];     //c[0]+c[1]+c[2]+...+c[255]=n
	   for (j=w-2; j>=0; j--){ //���������ʼλ��
		  c[j]=c[j+1] - c[j];
	   }
	   for (j=0; j<n; j++){  //���ƻ�����a
		  pd=btmp+j;
		  index= *((LPBYTE)pd+i);
		  ct = c[index]++;
	      a[ct] = btmp[j]; 
	   } 
	}
	for (i=0; i<n && a[i]>=0.0; i++); //ͳ�ƷǸ������� ����ʱiΪ�Ǹ����ĸ���O
	for (j=0; j<i / 2; j++){ //��תǰ��ķǸ�������O,ʹ����Ӵ�С 
		double dt = a[j];
		a[j]=a[i-1-j];
		a[i-1-j]=dt; 
	}
	for(i=0,j=n-1;i<j;i++,j--){  //���巭ת��ʹ֮��С���� 
		double dt = a[i];
		a[i]=a[j];
		a[j]=dt;                       
	}
	delete[] btmp;
}


int cc[w];    //�ٽ�����Դ����������Դ
CRITICAL_SECTION g_cs;
/*** һ��Ͱ������ռ���������Ԫ�أ�Ԫ�ظ��������� ***/
/**********************����������������Сʱ������Ȼ��Ȼ���ֶ���߳�ͬʱ����btmp���鲻�������⣬�ɶ���߳�ͬʱдcc����һ���������**************************/
unsigned __stdcall bSort(LPVOID param)
{
    SortParam stParam=(SortParam)param;
	KeyType * btmp=stParam->buff;
	int begin=stParam->begin;
	int n=stParam->count;
	int i=stParam->lapNum;    //��i��	
    PDouble pd;	
	int index;	
	EnterCriticalSection(&g_cs);
	for (int j=begin; j<(begin+n); j++){ //ͳ�Ƽ���
		  pd=btmp+j;
		  index= *((LPBYTE)pd+i);
		  cc[index]++;
	}
	LeaveCriticalSection(&g_cs);
	return 0;
}

/*** ���ؾ��ⷢ���̲߳��л������� ***/
void  MultiThreadBsort(KeyType * a,int n)
{
	
	int uCount=n/threadNum;     //ÿ�δ�С
	const int bt=sizeof(KeyType);    //�ܵ�����
	PDouble pd;
	int index,ct;
	int i,j;
	KeyType * btmp, *temp;
	btmp=new KeyType[n];
	HANDLE * hth=new HANDLE[threadNum];
	InitializeCriticalSection(&g_cs);   //��ʼ���ٽ���
	for(i=0;i<bt;i++){		
		temp=a, a=btmp, btmp=temp;  //a,b���齻��ָ��
		for ( j=0; j<w; j++){ //����������
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
	    for ( j=w-2; j>=0; j-- ){ //���������ʼλ��
			cc[j]=cc[j+1] - cc[j];		   
	    }
	    for ( j=0; j<n; j++){  //���ƻ�����a
		   pd=btmp+j;
		   index= *((LPBYTE)pd+i);
		   ct = cc[index]++;
	       a[ct] = btmp[j]; 
	    } 
	}	
	for (i=0; i<n && a[i]>=0.0; i++); //ͳ�ƷǸ������� ����ʱiΪ�Ǹ����ĸ���O
	for (j=0; j<i / 2; j++){ //��תǰ��ķǸ�������O,ʹ����Ӵ�С 
		KeyType dt = a[j];
		a[j]=a[i-1-j];
		a[i-1-j]=dt; 
	}
	for(i=0,j=n-1;i<j;i++,j--){  //���巭ת��ʹ֮��С���� 
		KeyType dt = a[i];
		a[i]=a[j];
		a[j]=dt;                       
	}
 	//_ASSERTE( _CrtCheckMemory( ) );
	delete[] btmp;	
	delete[] hth;
	
}
