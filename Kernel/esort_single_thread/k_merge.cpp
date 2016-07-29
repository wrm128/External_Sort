#include "stdafx.h"
#include "k_merge.h"
#include "dtostr.h"


/****************************************************************************/
/* 文件已打开的情况下：从第i个文件(第i个归并段)读入该段当前第1个记录的关键字到外结点   */               
/****************************************************************************/  
int input( int i,  KeyType *a){ 
	if( pbuf[i]>=segCount[i] ){	
		if(feof(fp[i+1])){   //已到达文件尾
           return 0;
		}
        segCount[i] = fread(segbuf[i],sizeof(segbuf[0][0]),segSize,fp[i+1]);   //读取segSize个数
		//printf("segSize=%d  segCount[%d]=%d  feof(fp)=%d\n",segSize,i,segCount[i],feof(fp[i+1]));
		if( segCount[i]!=segSize && (!feof(fp[i+1])) ){  //没有达到文件尾			    
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
/* 将第i个文件(第i个归并段)中当前的记录写至输出缓冲区（归并段）                  */
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
/* 沿从叶子结点b[s]到根结点ls[0]的路径调整败者树。                            */
/************************************************************************/ 
void Adjust(LoserTree &ls, int s,int k)
{
	int i, t; 
	/* ls[t]是b[s]的双亲结点 */ 
	t = (s + k) / 2;  
	while(t > 0){ 
		/* s指示新的胜者 */ 
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
/* 已知b[0]到b[k-1]为完全二叉树ls的叶子结点，存有k个关键字，沿从叶子            */
/* 到根的k条路径将ls调整成为败者树。                                         */
/************************************************************************/
void CreateLoserTree(LoserTree &ls,int k)
{
	int i; 
	b[k] = MINKEY;    //以浮点数最小值，设置哨兵
	/* 设置ls中“败者”的初值 */ 
	for( i = 0; i < k; ++i ){ 
		ls[i] = k;  
	} 
	/* 依次从b[k-1]，b[k-2]，…，b[0]出发调整败者 */ 
	for( i = k - 1; i >= 0; --i ){ 
		Adjust(ls, i,k); 
	} 
}

/************************************************************************/
/* 利用败者树ls将编号从0到k-1的k个输入归并段中的记录归并到输出归并段。           */
/* b[0]至b[k-1]为败者树上的k个叶子结点，分别存放k个输入归并段中当前记录的关键字。 */
/************************************************************************/
void K_Merge(LoserTree &ls, External &b,int k)
{
	
	int i, q;
	long long cnter=0; 
	/* 分别从k个输入归并段读入该段当前第一个记录的关键字到外结点 */ 
	for(i = 0; i < k; ++i) { 
		input(i, &b[i]); 
	} 
	/* 建败者树ls，选得最小关键字为b[ls[0]].key */ 
	CreateLoserTree(ls,k);  

	while( 1 ){     //b[ls[0]] != MAXKEY
		/* q指示当前最小关键字所在归并段 */ 
		q = ls[0];  
		/* 将编号为q的归并段中当前（关键字为b[q].key）的记录写至输出归并段 */ 
		output(q); 
		cnter++;
		if( cnter==TotalCount ) break;
		/* 从编号为q的输入归并段中读人下一个记录的关键字 */ 
		if(input(q, &b[q]) > 0){ 
			/* 调整败者树，选择新的最小关键字 */ 
			Adjust(ls,q,k); 
		}  
	} 
	fwrite(buffer,1,length,fp[0]); 

	//printf("TotalCount=%lld  cnter=%lld\n",TotalCount,cnter);

}