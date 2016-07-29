// esort1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "readFile.h"
#include "isdouble.h"
#include "bsort.h"
#include "dtostr.h"
#include "k_merge.h"


int len[M];   //ÿ�����ļ��ļ�¼��
External b;     //External�͵�ȫ�ֱ���

char fname[M][20];    //�ļ���
FILE *fp0, *fp[M + 1];           //�ļ�ָ��

char buffer[BufferSize];    //IO������
KeyType buf[ArraySize];
int length;     //�������������ֽ���
KeyType segbuf[K][segSize];    //�鲢�λ�����
int segCount[K];
int pbuf[K];       //�鲢������
ull TotalCount=0;   //�ܼ�¼��
int m=0;  //m����ʼ�鲢�Σ���m�����ļ�
int threadNum=0;   //�̸߳���

//HANDLE readHandle;
//HANDLE writeHandle;

void run(char *infile, char *outfile)
{
	
	int ndcnt=0;  //�Ƿ��ַ�����
	ull lSize;

	/* ��Ҫһ��byte��©�ض��������ļ���ֻ�ܲ��ö����Ʒ�ʽ�� rb */   
	fp0 = fopen (infile, "rb" );    //D:\\OneMillion.txt  FiftyMillion.txt TwoHundredAndFiftyMillion
	if (fp0==NULL)  {  
		fputs ("Can't open file !",stderr);  
		exit (1);  
	}  

	fseek(fp0, 0, SEEK_END);
	lSize=ftell(fp0);     //��ȡ�ļ���С,����ֵΪlong,�ļ�̫��Ļ�ȡ���Ϊ0
	rewind(fp0);

	if(lSize && lSize<BufferSize){  //ֱ������
		
	   printf("Total bytes of the file: %d\n",lSize); 
       size_t result = fread (buffer,1,lSize,fp0);  
	   if (result != lSize){  
		   fputs ("Reading error",stderr);  
		   exit (3);  
	   }
	   fclose(fp0);
	   int cnt=0;
	   int i=0,j=0,flag=1;
	   char tmp[strSize];

	   while( buffer[i]==' '||buffer[i]=='\t'||buffer[i]=='\r'||buffer[i]=='\n' ){
		   i++;
	   }
	   for(  ;i<lSize;i++){
		   if( buffer[i]==' '||buffer[i]=='\t'||buffer[i]=='\r'||buffer[i]=='\n' ){
			   tmp[j]='\0';
			   buf[cnt]=isDouble( flag,tmp );
			   if( flag == -1 )  ndcnt++;
			   else  cnt++;
			   j=0;
			   flag=1;
			   if( buffer[i]=='\r' ) i++;       //�س��������������ַ�
			   continue;
		   }
		   tmp[j++]=buffer[i];	  
	   } 
	   printf("number of illegal strings: %d\n",ndcnt);

	   MultiThreadBsort(buf,cnt);
	   //BucketSort(buf,cnt);

	   fp0=fopen(outfile,"wb");
	   if(fp0==NULL){
		   printf("cannot open file!");
		   exit(1);             
	   }
	   int len,length=0;
	   for(int i=0;i<cnt;i++){
		   len = FloatToStr(buffer+length,buf[i]);
		   length += len;
	   }
	   fwrite(buffer,1,length,fp0);
	   fclose(fp0);
	}else{
       m=readFile(fp0,BufferSize,ndcnt);
       fclose (fp0);  
	  
	   //end=clock();
	   //printf("Total Time Reading: %d\n",(end-start)); 
	   printf("number of illegal strings: %d\n",ndcnt);
	   printf("Number of subfiles: %d\n",m);

	   /************************************************************************/
	   /* K·�鲢                                                              */
	   /************************************************************************/
	   int k = m;
    
	   /* ���δ򿪸������ļ�,�Զ����Ʒ�ʽ�� */
	   for(int i=0; i<k; i++ ){
          fp[i+1]=fopen(fname[i],"rb+");
	   }

	    /* ��д�ķ�ʽ�򿪴��ļ�fout */
        fp[0]=fopen(outfile,"w");
	    LoserTree ls;
	    /* ������ʼ�� */
	    memset(pbuf,0,sizeof(pbuf));
	    memset(segCount,0,sizeof(segCount));
	
	    length=0;
	    /* ���ð�����ls��k������鲢���еļ�¼�鲢������鲢�Σ������ļ�fout */ 
	    K_Merge(ls,b,k);
	    for(int i = 0; i <= k; i++){ 
		    fclose(fp[i]);  
	    }
	} 

}

int _tmain(int argc, _TCHAR* argv[])
{
	clock_t start,end;
	

	char infile[pathLength],outfile[pathLength];     //��������ļ�·��
	char file[2*pathLength];
	
	start=clock();
	/* ��ȡ�����ļ� */
	FILE *fpc=fopen("D:\\config.txt","r");   
	if(fpc==NULL){
		printf("can't open file!");
		exit(1);
	}
	fseek(fpc,0L,SEEK_END);
	int pathlen=ftell(fpc);
	rewind(fpc);
	fread(file,1,pathlen,fpc);      //��ȡ�����ļ�
	int i,j=0;
	for(i=0;file[i]!='#';i++){
		infile[j++]=file[i];
	}
	infile[j]='\0';
	i++;
	j=0;
	for( ;file[i]!='#';i++){
       outfile[j++]=file[i];
	}
	outfile[j]='\0';
	i++;	
	for( ;i<pathlen;i++){
	   threadNum=threadNum*10+(file[i]-'0');
	}

	time_t t = time(NULL); //��ȡĿǰ��ʱ��;  
	tm* local; //����ʱ�� 
	char timebuf[128]= {0};
	local = localtime(&t); //תΪ����ʱ��
	strftime(timebuf, 64, "%H-%M-%S", local);
	strcat(timebuf,"out.txt");
	strcat(outfile,"\\");
	strcat(outfile,timebuf);
	printf("in: %s\nout: %s\nNumber of threads: %d\n",infile,outfile,threadNum); 

	run(infile,outfile);
	
	end=clock();
	printf("Total Time: %.3f s\n",(float)(end-start)*1.0/CLOCKS_PER_SEC); 

	system("pause");
	return 0;
}