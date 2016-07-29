// esort1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "readFile.h"
#include "isdouble.h"
#include "bsort.h"
#include "dtostr.h"
#include "k_merge.h"


int len[M];   //每个子文件的记录数
External b;     //External型的全局变量

char fname[M][20];    //文件名
FILE *fp0, *fp[M + 1];           //文件指针

char buffer[BufferSize];    //IO缓冲区
KeyType buf[ArraySize];
int length;     //缓冲区中数据字节数
KeyType segbuf[K][segSize];    //归并段缓冲区
int segCount[K];
int pbuf[K];       //归并段索引
ull TotalCount=0;   //总记录数
int m=0;  //m个初始归并段，即m个子文件
int threadNum=0;   //线程个数

//HANDLE readHandle;
//HANDLE writeHandle;

void run(char *infile, char *outfile)
{
	
	int ndcnt=0;  //非法字符个数
	ull lSize;

	/* 若要一个byte不漏地读入整个文件，只能采用二进制方式打开 rb */   
	fp0 = fopen (infile, "rb" );    //D:\\OneMillion.txt  FiftyMillion.txt TwoHundredAndFiftyMillion
	if (fp0==NULL)  {  
		fputs ("Can't open file !",stderr);  
		exit (1);  
	}  

	fseek(fp0, 0, SEEK_END);
	lSize=ftell(fp0);     //获取文件大小,返回值为long,文件太大的获取结果为0
	rewind(fp0);

	if(lSize && lSize<BufferSize){  //直接内排
		
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
			   if( buffer[i]=='\r' ) i++;       //回车、换行是两个字符
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
	   /* K路归并                                                              */
	   /************************************************************************/
	   int k = m;
    
	   /* 依次打开各个子文件,以二进制方式读 */
	   for(int i=0; i<k; i++ ){
          fp[i+1]=fopen(fname[i],"rb+");
	   }

	    /* 以写的方式打开大文件fout */
        fp[0]=fopen(outfile,"w");
	    LoserTree ls;
	    /* 变量初始化 */
	    memset(pbuf,0,sizeof(pbuf));
	    memset(segCount,0,sizeof(segCount));
	
	    length=0;
	    /* 利用败者树ls将k个输入归并段中的记录归并到输出归并段，即大文件fout */ 
	    K_Merge(ls,b,k);
	    for(int i = 0; i <= k; i++){ 
		    fclose(fp[i]);  
	    }
	} 

}

int _tmain(int argc, _TCHAR* argv[])
{
	clock_t start,end;
	

	char infile[pathLength],outfile[pathLength];     //输入输出文件路径
	char file[2*pathLength];
	
	start=clock();
	/* 读取配置文件 */
	FILE *fpc=fopen("D:\\config.txt","r");   
	if(fpc==NULL){
		printf("can't open file!");
		exit(1);
	}
	fseek(fpc,0L,SEEK_END);
	int pathlen=ftell(fpc);
	rewind(fpc);
	fread(file,1,pathlen,fpc);      //读取配置文件
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

	time_t t = time(NULL); //获取目前秒时间;  
	tm* local; //本地时间 
	char timebuf[128]= {0};
	local = localtime(&t); //转为本地时间
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