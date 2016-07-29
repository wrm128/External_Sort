// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

//#pragma once

#ifndef  STDAFX_H
#define  STDAFX_H

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>
#include <algorithm>
#include <string.h>
#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <set>
#include <string>
#include <math.h>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <process.h>
using namespace std;
#define ull    unsigned long long 
#define TRUE   1
#define FALSE  0
#define OK     1
#define ERROR  0
#define INFEASIBLE  -1      //不可行的
#define MINKEY   -DBL_MAX
#define MAXKEY   DBL_MAX
#define accuracy   1.0e-18
#define BufferSize 200000001   //180  //3e+8成块读写时每块的大小   (3+17+2+4+2)*1000000
#define ArraySize  20000100      //5e+7内部待排序数组的大小
#define segSize    1000000   //20
#define pathLength 300
#define strSize 300
#define M  30       //最多M个初始归并段，即M个子文件
#define K  30      //K路归并

typedef unsigned char   BYTE;
typedef BYTE   *LPBYTE;
typedef double  KeyType;     //关键字类型

/* 败者树是完全二叉树且不含叶子，可采用顺序存储结构  
   定义有K个int元素的数组 */ 
typedef int LoserTree[K];
typedef KeyType Exnode, External[K+1];
/* 全局变量声明 */
extern External b;     //External型的全局变量

extern char fname[M][20];    //文件名
extern FILE *fp[M + 1];           //文件指针
extern int len[M];   //每个子文件的记录数
extern char buffer[BufferSize];    //IO缓冲区
extern KeyType buf[ArraySize];
extern int length;     //缓冲区中数据字节数
extern KeyType  segbuf[K][segSize];    //归并段缓冲区
extern int segCount[K];
extern int pbuf[K];       //归并段索引
extern ull TotalCount;   //总记录数
extern int m;  //m个初始归并段，即m个子文件
extern int threadNum;   //线程个数
#endif
// TODO: reference additional headers your program requires here
