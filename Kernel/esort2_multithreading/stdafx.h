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
#define INFEASIBLE  -1      //�����е�
#define MINKEY   -DBL_MAX
#define MAXKEY   DBL_MAX
#define accuracy   1.0e-18
#define BufferSize 200000001   //180  //3e+8�ɿ��дʱÿ��Ĵ�С   (3+17+2+4+2)*1000000
#define ArraySize  20000100      //5e+7�ڲ�����������Ĵ�С
#define segSize    1000000   //20
#define pathLength 300
#define strSize 300
#define M  30       //���M����ʼ�鲢�Σ���M�����ļ�
#define K  30      //K·�鲢

typedef unsigned char   BYTE;
typedef BYTE   *LPBYTE;
typedef double  KeyType;     //�ؼ�������

/* ����������ȫ�������Ҳ���Ҷ�ӣ��ɲ���˳��洢�ṹ  
   ������K��intԪ�ص����� */ 
typedef int LoserTree[K];
typedef KeyType Exnode, External[K+1];
/* ȫ�ֱ������� */
extern External b;     //External�͵�ȫ�ֱ���

extern char fname[M][20];    //�ļ���
extern FILE *fp[M + 1];           //�ļ�ָ��
extern int len[M];   //ÿ�����ļ��ļ�¼��
extern char buffer[BufferSize];    //IO������
extern KeyType buf[ArraySize];
extern int length;     //�������������ֽ���
extern KeyType  segbuf[K][segSize];    //�鲢�λ�����
extern int segCount[K];
extern int pbuf[K];       //�鲢������
extern ull TotalCount;   //�ܼ�¼��
extern int m;  //m����ʼ�鲢�Σ���m�����ļ�
extern int threadNum;   //�̸߳���
#endif
// TODO: reference additional headers your program requires here
