#include "stdafx.h"
#include "qsort.h"
#include "isdouble.h"
#include "bsort.h"
#include "dtostr.h"


/************************************************************************/
/* ���ļ���ȡ                                                            */
/* ���������ļ���ȡָ��fp0��
		ÿ�ζ����Ĵ�СlSize		
		�Ƿ��ַ����� &ndcnt
   ����ֵ�����ļ�����m                                                      */
/************************************************************************/

int readFile(FILE * fp0, ull lSize, int &ndcnt)
{   

	int cnt=0;
	char tmp[strSize];
	char s[10];
	size_t result;
	int m=0;
	/* ���ļ���ȡ��ÿ�ζ�ȡlSize-1�ֽ�;������Ԥ����Ȼ��д�����ļ� */
	int fend=0;
	while(1){
		
		/* ���ļ�������buffer�� */  
		result = fread (buffer,1,lSize-1,fp0);  
		//printf("%d %d %d\n",m,result,buffer[result-1]);
		if (result != lSize-1)  {  
			if(feof(fp0)){    //�ﵽ�ļ�β
               fend=1;
			}else{
				fputs ("Reading error!",stderr);  
				exit (3); 
			}
		}
		if(!fend){
		   char curchar = fgetc(fp0);

		   /* ����պö���һ��doubleβ����û�ж����������Ҫ����һ������� */
		   if( curchar==' '||curchar=='\t'||curchar=='\r'||curchar=='\n' ){

			  buffer[result++]='\n';
		   }

		   /* ����պö���һ��double���������������һ��double�м䣬��double���ضϣ� */
		   else{
			  _fseeki64( fp0, -2L, SEEK_CUR );  //�������ַ�
              curchar = fgetc(fp0);
			  if( curchar==' '||curchar=='\t'||curchar=='\r'||curchar=='\n' ){
                  curchar = fgetc(fp0);    //ָ������һλ
			  }else{   //double�м�
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
		/* ���������Ż����� */
		/*buf = (KeyType *) malloc (sizeof(KeyType)*ArraySize);  
		if (buf == NULL)  {  
			fputs ("Memory error!",stderr);   
			exit (2);  
		}  */

		/*  �ַ�������->������  */
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
				if( buffer[i]=='\r' ) i++;       //�س��������������ַ�
				//endptr="\0";
				continue;
			}
			tmp[j++]=buffer[i];	  
		} 
		
		TotalCount += cnt;
		//printf("cnt=%d  total=%d\n",cnt,TotalCount);
		if(cnt){
		   /* �ڲ����� */
		   //start=clock();
		   //QuickSort(buf,0,cnt-1);
		   //BucketSort(buf,cnt);
		   MultiThreadBsort(buf,cnt);
		   //end=clock();
		   //printf("The time of quicksort��%d ms\n",(end-start));
        
		   /* д�����ļ� */
		   len[m++]=cnt;
		   buf[cnt++]=MAXKEY;      //д�����ֵ�����ڱ�
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
		if(fend)  break;   //�ѵ����ļ�β
	}
	return m;
}