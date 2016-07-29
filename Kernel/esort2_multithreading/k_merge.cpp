#include "stdafx.h"
#include "k_merge.h"
#include "dtostr.h"


/****************************************************************************/
/* �ļ��Ѵ򿪵�����£��ӵ�i���ļ�(��i���鲢��)����öε�ǰ��1����¼�Ĺؼ��ֵ�����   */               
/****************************************************************************/  
int input( int i,  KeyType *a){ 
	if( pbuf[i]>=segCount[i] ){	
		if(feof(fp[i+1])){   //�ѵ����ļ�β
           return 0;
		}
        segCount[i] = fread(segbuf[i],sizeof(segbuf[0][0]),segSize,fp[i+1]);   //��ȡsegSize����
		//printf("segSize=%d  segCount[%d]=%d  feof(fp)=%d\n",segSize,i,segCount[i],feof(fp[i+1]));
		if( segCount[i]!=segSize && (!feof(fp[i+1])) ){  //û�дﵽ�ļ�β			    
				fputs ("Reading error",stderr);  
				exit (3); 
		}
		pbuf[i]=0;
	}
	* a = segbuf[i][pbuf[i]];
	pbuf[i]++;
	return 1;
} 

/************************************************************************/
/* ����i���ļ�(��i���鲢��)�е�ǰ�ļ�¼д��������������鲢�Σ�                  */
/************************************************************************/  
void output(int i){ 
	char ss[50];
	int dlen = FloatToStr(ss,b[i]);
	
	if( length + dlen <= BufferSize ){
		strcpy(buffer+length,ss);
		length += dlen;
	}else{
	    fwrite(buffer,1,length,fp[0]); 
		strcpy(buffer,ss);
		length=dlen;
	}
} 

/************************************************************************/
/* �ش�Ҷ�ӽ��b[s]�������ls[0]��·��������������                            */
/************************************************************************/ 
void Adjust(LoserTree &ls, int s,int k)
{
	int i, t; 
	/* ls[t]��b[s]��˫�׽�� */ 
	t = (s + k) / 2;  
	while(t > 0){ 
		/* sָʾ�µ�ʤ�� */ 
		if(b[s] > b[ls[t]]){ 
			i = s; 
			s = ls[t];  
			ls[t] = i; 
		} 
		t = t / 2; 
	} 
	ls[0] = s; 
}

/************************************************************************/
/* ��֪b[0]��b[k-1]Ϊ��ȫ������ls��Ҷ�ӽ�㣬����k���ؼ��֣��ش�Ҷ��            */
/* ������k��·����ls������Ϊ��������                                         */
/************************************************************************/
void CreateLoserTree(LoserTree &ls,int k)
{
	int i; 
	b[k] = MINKEY;    //�Ը�������Сֵ�������ڱ�
	/* ����ls�С����ߡ��ĳ�ֵ */ 
	for( i = 0; i < k; ++i ){ 
		ls[i] = k;  
	} 
	/* ���δ�b[k-1]��b[k-2]������b[0]������������ */ 
	for( i = k - 1; i >= 0; --i ){ 
		Adjust(ls, i,k); 
	} 
}

/************************************************************************/
/* ���ð�����ls����Ŵ�0��k-1��k������鲢���еļ�¼�鲢������鲢�Ρ�           */
/* b[0]��b[k-1]Ϊ�������ϵ�k��Ҷ�ӽ�㣬�ֱ���k������鲢���е�ǰ��¼�Ĺؼ��֡� */
/************************************************************************/
void K_Merge(LoserTree &ls, External &b,int k)
{
	
	int i, q;
	long long cnter=0; 
	/* �ֱ��k������鲢�ζ���öε�ǰ��һ����¼�Ĺؼ��ֵ����� */ 
	for(i = 0; i < k; ++i) { 
		input(i, &b[i]); 
	} 
	/* ��������ls��ѡ����С�ؼ���Ϊb[ls[0]].key */ 
	CreateLoserTree(ls,k);  

	while( 1 ){     //b[ls[0]] != MAXKEY
		/* qָʾ��ǰ��С�ؼ������ڹ鲢�� */ 
		q = ls[0];  
		/* �����Ϊq�Ĺ鲢���е�ǰ���ؼ���Ϊb[q].key���ļ�¼д������鲢�� */ 
		output(q); 
		cnter++;
		if( cnter==TotalCount ) break;
		/* �ӱ��Ϊq������鲢���ж�����һ����¼�Ĺؼ��� */ 
		if(input(q, &b[q]) > 0){ 
			/* ������������ѡ���µ���С�ؼ��� */ 
			Adjust(ls,q,k); 
		}  
	} 
	fwrite(buffer,1,length,fp[0]); 

	//printf("TotalCount=%lld  cnter=%lld\n",TotalCount,cnter);

}