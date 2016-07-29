#include "stdafx.h"
#include "dtostr.h"

/************************************************************************/
/*  解析双精度浮点数为十进制字符串，并存入浮点数记录中                          /
/   参数：浮点数指针，精度，小数位，浮点数记录                                */
/************************************************************************/
void FloatResolve(PEXTENDED pvalue, int precision, int decimals, FloatRec *rec)
{
	int power;
	double val;
	// 63位：双精度浮点数符号位，在第8个字节（小端存储，高高低低 ） 
	rec->sign = *((LPBYTE)pvalue + 7) >> 7;      //1为负，0为正
	// 52-62位：双精度浮点数阶码(阶码 - 0x3ff = 二进制指数)
	rec->exponent = (*(PUINT64)pvalue >> 52) & 0x7ff;
	if (!rec->exponent){  // E=0  |  *pvalue = 0
		rec->sign = 0;
		strcpy(rec->digits,"0000000000");
		return;
	}
	/****这种情况在后期基本不会出现，前期对非法数据已做处理*****/
	if (rec->exponent == 0x7ff){// E=2047  |  *pvalue = nan or inf
		if ((*(PUINT64)pvalue & 0x000fffffffffffff) == 0){
			strcpy(rec->digits, "INF");   //无穷大，sign=1,负无穷；sign=0,正无穷 
		} else{
			rec->exponent ++;
			rec->sign = 0;
			strcpy(rec->digits, "NAN");
		}
		return;
	}
	//printf("sign=%d\n",rec->sign);
	// 阶码转换为十进制指数
	rec->exponent = DECIMAL_EXP(rec->exponent);

	//printf("exponent=%d\n",rec->exponent);
	// 0-51位：双精度浮点数尾数转换成F_MAXPRECISION位十进制浮点整数格式
	val = *pvalue;	
	*((LPBYTE)&val + 7) &= 0x7f;// val = fabs(*pvalue)

	int tmpexp=rec->exponent;
	double tmpval=val;
	if( tmpexp > 0){
		tmpval /= *(PEXTENDED)&_tab0[tmpexp & 31];
		tmpexp >>= 5;
		if(tmpexp)
			tmpval /=*(PEXTENDED)&_tab1[tmpexp - 1];
		if(tmpval < 1.0) rec->exponent --;
		else if(tmpval > 10.0) rec->exponent ++;
	}else if(tmpexp <0){
		tmpexp = -tmpexp;
		tmpval *= *(PEXTENDED)&_tab0[tmpexp & 31];
		tmpexp >>= 5;
		if(tmpexp)
			tmpval *=*(PEXTENDED)&_tab1[tmpexp - 1];
		if(tmpval < 1.0) rec->exponent --;
		else if(tmpval > 10.0) rec->exponent ++;
	}else {
		if(tmpval < 1.0) rec->exponent --;
		else if(tmpval > 10.0) rec->exponent ++;
	}

	power = F_MAXPRECISION - rec->exponent;
	if (power > 0){// if (power > 0) val *= (10**power)
		val *= *(PEXTENDED)&_tab0[power & 31];   //power小于31的部分 

		power >>= 5;            // power /= 32;
		if (power)
			val *= *(PEXTENDED)&_tab1[power - 1];
	}else if (power < 0){// if (power < 0) val /= (10**power)
		power = -power;
		val /= *(PEXTENDED)&_tab0[power & 31];
		power >>= 5;            // power /= 32;
		if (power)
			val /= *(PEXTENDED)&_tab1[power - 1];
	}
	//printf("val=%lf\n",val);
	// 16位十进制浮点整数四舍五入
	val += 0.5;
	if (val >= _conPrec){
		val /= 10;
		rec->exponent ++;
	}
	UINT64 value=(UINT64) val; //((*(PUINT64)&val & 0x000fffffffffffff) | 0x0010000000000000) <<  -(52 - ((*(PUINT64)&val >> 52) - 0x3ff));
	//printf("value=%lld\n",value);
	int i;
	for (i = 0; i<F_MAXPRECISION+1; i++){
		rec->digits[i] = (char)((value / _cvttab[i]) | 0x30);
		value %= _cvttab[i];
	}
	rec->digits[i]=0;
}

/************************************************************************/
/* 浮点数转换为字符串。参数：字符串，浮点数                                   */
/************************************************************************/
int FloatToStr(char * str, double value)
{
	FloatRec rec;  
	char * pd = rec.digits;  
	char * ps = str;  

	// 解析浮点数，并将信息保存在rec  
	FloatResolve(&value, 18, 17, &rec);  
	// 打印负数符号  
	if (rec.sign) *ps ++ = '-';  
	// NAN or INF  
	if (*pd > '9')                 
	{  
		memcpy(ps, pd, 4);  
		return strlen(str);  
	}  
	*ps ++ = *pd ++;  
	if (*pd)  
		for (*ps ++ = '.'; *pd; *ps ++ = *pd ++);    

	// 转换为指数形式   
	int e, exp = rec.exponent;  
	// 转换为指数形式   
	*ps ++ = 'E';  
	if (exp < 0)  {  
		exp = -exp;  
		*ps ++ = '-';  
	}  
	else *ps ++ = '+';  
	if ((e = (exp / 1000)) != 0)  {  
		*ps ++ = e | 0x30;  
		exp %= 1000;  
	}  
	*ps ++ = (exp / 100) | 0x30;  
	exp %= 100;  
	*(unsigned short *)ps  = (((exp % 10) << 8) | (exp / 10)) | 0x3030; 
	ps +=2;
	* ps++= '\n';
	* ps = 0; 
	return strlen(str);  
}