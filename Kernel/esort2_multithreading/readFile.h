#ifndef   READFILE_H
#define   READFILE_H

#include "stdafx.h"

/************************************************************************/
/* 大文件读取                                                            */
/* 参数：大文件读取指针fp0，
		每次读入块的大小lSize		
		非法字符个数 &ndcnt
   返回值：子文件个数                                                      */
/************************************************************************/
int readFile(FILE * fp0, ull lSize, int &ndcnt);


#endif