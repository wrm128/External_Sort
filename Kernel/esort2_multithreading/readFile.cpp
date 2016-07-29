#include "stdafx.h"
#include "qsort.h"
#include "isdouble.h"
#include "bsort.h"
#include "dtostr.h"


/************************************************************************/
/* 大文件读取                                                            */
/* 参数：大文件读取指针fp0，
		每次读入块的大小lSize		
		非法字符个数 &ndcnt
   返回值：子文件个数m                                                      */
/************************************************************************/

int readFile(FILE * fp0, ull lSize, int &ndcnt)
{   

	int cnt=0;
	char tmp[strSize];
	char s[10];
	size_t result;
	int m=0;
	/* 大文件读取，每次读取lSize-1字节;并进行预处理，然后写入子文件 */
	int fend=0;
	while(1){
		
		/* 将文件拷贝到buffer中 */  
		result = fread (buffer,1,lSize-1,fp0);  
		//printf("%d %d %d\n",m,result,buffer[result-1]);
		if (result != lSize-1)  {  
			if(feof(fp0)){    //达到文件尾
               fend=1;
			}else{
				fputs ("Reading error!",stderr);  
				exit (3); 
			}
		}
		if(!fend){
		   char curchar = fgetc(fp0);

		   /* 如果刚好读到一个double尾，但没有读间隔符；则要补加一个间隔符 */
		   if( curchar==' '||curchar=='\t'||curchar=='\r'||curchar=='\n' ){

			  buffer[result++]='\n';
		   }

		   /* 如果刚好读完一个double，包括间隔符；或到一个double中间，即double被截断； */
		   else{
			  _fseeki64( fp0, -2L, SEEK_CUR );  //退两个字符
              curchar = fgetc(fp0);
			  if( curchar==' '||curchar=='\t'||curchar=='\r'||curchar=='\n' ){
                  curchar = fgetc(fp0);    //指针下移一位
			  }else{   //double中间
				  int back=0;
		
		     	  while ( curchar!=EOF && curchar!=' ' && curchar!='\t' && curchar!='\r' && curchar!='\n' )
	   			  {
					  back++;
					  _fseeki64( fp0, -2L, SEEK_CUR );
				      curchar=fgetc(fp0);
				  }
				  result -= back;
			  }
		   }
		}
		/* 浮点数内排缓冲区 */
		/*buf = (KeyType *) malloc (sizeof(KeyType)*ArraySize);  
		if (buf == NULL)  {  
			fputs ("Memory error!",stderr);   
			exit (2);  
		}  */

		/*  字符串解析->浮点数  */
		int i=0,j=0,flag=1;
		cnt=0;
		//char * endptr;
		while( buffer[i]==' '||buffer[i]=='\t'||buffer[i]=='\r'||buffer[i]=='\n' ){
			i++;
		}
		for(  ;i<result;i++){
			if( buffer[i]==' '||buffer[i]=='\t'||buffer[i]=='\r'||buffer[i]=='\n' ){
				tmp[j]='\0';
				//buf[cnt]=Strtod(tmp,&endptr);
				//if( strlen(endptr) ) ndcnt++;
				buf[cnt]=isDouble( flag,tmp );
				if( flag == -1 )  ndcnt++;
				else  cnt++;
				j=0;
				flag=1;
				if( buffer[i]=='\r' ) i++;       //回车、换行是两个字符
				//endptr="\0";
				continue;
			}
			tmp[j++]=buffer[i];	  
		} 
		
		TotalCount += cnt;
		//printf("cnt=%d  total=%d\n",cnt,TotalCount);
		if(cnt){
		   /* 内部排序 */
		   //start=clock();
		   //QuickSort(buf,0,cnt-1);
		   //BucketSort(buf,cnt);
		   MultiThreadBsort(buf,cnt);
		   //end=clock();
		   //printf("The time of quicksort：%d ms\n",(end-start));
        
		   /* 写入子文件 */
		   len[m++]=cnt;
		   buf[cnt++]=MAXKEY;      //写入最大值，做哨兵
		   //printf("cnt%d=%d\n",m,cnt);
		   itoa(m, s, 10);  
		   strcpy(fname[m-1], "f"); 
		   strcat(fname[m-1], s); 
		   strcat(fname[m-1],".in");

		   fp[m] = fopen(fname[m-1], "wb+"); 
		   if( fp[m] == NULL ){
		      printf("can't open file!");
		      exit(1); 
		   }		   
		   fwrite(buf,sizeof(buf[0]),cnt,fp[m]);
		   fclose(fp[m]);
		   
		}
		//free(buf);
		//buf=NULL;
		if(fend)  break;   //已到达文件尾
	}
	return m;
}