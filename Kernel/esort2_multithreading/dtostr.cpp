#include "stdafx.h"
#include "dtostr.h"

/************************************************************************/
/*  ����˫���ȸ�����Ϊʮ�����ַ����������븡������¼��                          /
/   ������������ָ�룬���ȣ�С��λ����������¼                                */
/************************************************************************/
void FloatResolve(PEXTENDED pvalue, int precision, int decimals, FloatRec *rec)
{
	int power;
	double val;
	// 63λ��˫���ȸ���������λ���ڵ�8���ֽڣ�С�˴洢���߸ߵ͵� �� 
	rec->sign = *((LPBYTE)pvalue + 7) >> 7;      //1Ϊ����0Ϊ��
	// 52-62λ��˫���ȸ���������(���� - 0x3ff = ������ָ��)
	rec->exponent = (*(PUINT64)pvalue >> 52) & 0x7ff;
	if (!rec->exponent){  // E=0  |  *pvalue = 0
		rec->sign = 0;
		strcpy(rec->digits,"0000000000");
		return;
	}
	/****��������ں��ڻ���������֣�ǰ�ڶԷǷ�������������*****/
	if (rec->exponent == 0x7ff){// E=2047  |  *pvalue = nan or inf
		if ((*(PUINT64)pvalue & 0x000fffffffffffff) == 0){
			strcpy(rec->digits, "INF");   //�����sign=1,�����sign=0,������ 
		} else{
			rec->exponent ++;
			rec->sign = 0;
			strcpy(rec->digits, "NAN");
		}
		return;
	}
	//printf("sign=%d\n",rec->sign);
	// ����ת��Ϊʮ����ָ��
	rec->exponent = DECIMAL_EXP(rec->exponent);

	//printf("exponent=%d\n",rec->exponent);
	// 0-51λ��˫���ȸ�����β��ת����F_MAXPRECISIONλʮ���Ƹ���������ʽ
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
		val *= *(PEXTENDED)&_tab0[power & 31];   //powerС��31�Ĳ��� 

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
	// 16λʮ���Ƹ���������������
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
/* ������ת��Ϊ�ַ������������ַ�����������                                   */
/************************************************************************/
int FloatToStr(char * str, double value)
{
	FloatRec rec;  
	char * pd = rec.digits;  
	char * ps = str;  

	// ������������������Ϣ������rec  
	FloatResolve(&value, 18, 17, &rec);  
	// ��ӡ��������  
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

	// ת��Ϊָ����ʽ   
	int e, exp = rec.exponent;  
	// ת��Ϊָ����ʽ   
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