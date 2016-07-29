#include "stdafx.h"
#include "bsort.h" 
typedef double   *PDouble;
/* �������������޷���������������Ϊ1->0,0->1 */
void preprocess(double *a,int n)
{
    PDouble pd;
	for(int i=0;i<n;i++){
       pd=a+i;
	   /* ����λȡ��:1���κ�����򣬵÷���0���κ�����򣬵ñ��� */
	   *((LPBYTE)pd+7) ^= 0x80;  
	}

}

void postprocess(double *a,int n)
{
	PDouble pd;
	for(int i=0;i<n;i++){
		pd=a+i;
		/* ����λȡ��:1���κ�����򣬵÷���0���κ�����򣬵ñ��� */
		*((LPBYTE)pd+7) ^= 0x80;  
	}

}

/************************************************************************/
/* Ͱ����                                                                */
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