#include "stdafx.h"
#include "bsort.h" 
typedef double   *PDouble;
/* 将浮点数看成无符号整数，将符号为1->0,0->1 */
void preprocess(double *a,int n)
{
    PDouble pd;
	for(int i=0;i<n;i++){
       pd=a+i;
	   /* 符号位取反:1与任何数异或，得反；0与任何数异或，得本身 */
	   *((LPBYTE)pd+7) ^= 0x80;  
	}

}

void postprocess(double *a,int n)
{
	PDouble pd;
	for(int i=0;i<n;i++){
		pd=a+i;
		/* 符号位取反:1与任何数异或，得反；0与任何数异或，得本身 */
		*((LPBYTE)pd+7) ^= 0x80;  
	}

}

/************************************************************************/
/* 桶排序                                                                */
/************************************************************************/

void BucketSort(double *a,int n)
{
	//preprocess(a,n);
	const int w=256;
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