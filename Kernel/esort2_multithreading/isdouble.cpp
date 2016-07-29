#include "stdafx.h"
#include "isdouble.h"

/* 将字符串转化为浮点数 */
double Strtod(const char *s, char **endptr)
{
	register const char *p     = s;
	register long double  value = 0.L;
	int sign = 0;
	long double factor;
	unsigned int expo;   //指数部分
	while( isspace(*p) ){ //跳过前面的空格
		p++;
	}
	if(*p == '-' || *p == '+'){
		sign = *p++;   //把符号赋给字符sign，指针后移。
	}
	/* 处理小数点前的部分 */
	while((unsigned int)(*p - '0') < 10u ){//转换整数部分
		value = value*10 + (*p++ - '0');
	}
	/* 如果是正常的表示方式（如：1234.5678） */
	if( *p == '.' )
	{
		factor = 1.;
		p++;
		while( (unsigned int)(*p - '0') < 10u )
		{
			factor *= 0.1;
			value += (*p++ - '0') * factor;
		}
	}
	/* 如果是IEEE754标准的格式（如：1.23456E+3）*/
	if( *p == 'e' || *p == 'E')
	{
		expo = 0;
		factor = 10.L;
		switch (*++p)
		{
		case '-':   factor = 0.1;
		case '+':   p++; break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':  break;
		default :  value = 0.L;
			p = s;
			goto done;
		}
		while ( (unsigned int)(*p - '0') < 10u ){
			expo = 10 * expo + (*p++ - '0');
		}
		while( 1 ){
			if ( expo & 1 ){
				value *= factor;
			}
			if ( (expo >>= 1) == 0 ){
				break;
			}
			factor *= factor;
		}
	}
done:
	if ( endptr != 0 )
		*endptr = (char*)p;
	return (sign == '-' ? -value : value);

}

double Atof(char *str)
{
	return Strtod(str,0);
}


/*** 判断一个字符串是不是浮点数：
是的话，返回浮点数值；否则，改变标记位flag，并返回0.0 **/
double isDouble( int &flag,char *str )
{
	int i=0,len;
	len=strlen( str );
	int sign=1;  //符号，默认为正
	int exponet=0;     //指数部分
	int ispoint=0 , isexponent=0;
	double result=0.0  , base;
	while( str[i]==' '||str[i]=='\t'||str[i]=='\n' ){
		i++;
	}
	if( i>=len ){
		flag=-1;
		return (double)0.0;
	}
	/* 判断底数符号位 */
	if( str[i]=='+' ){
		i++;
	}else if( str[i]=='-' ){
		sign=-1;
		i++;
	}else if( str[i]<'0' || str[i]>'9' ){//非法字符
		flag=-1;
		return (double)0.0;
	}
	/* 查找小数点前的一部分 */
	for( ;i<len;i++ ){
		if( str[i]=='.' ){ //小数点
			i++;
			ispoint=1;
			break;
		}
		if( str[i]=='e'||str[i]=='E' ){
			i++;
			isexponent=1;
			break;
		}
		if( str[i]<'0' || str[i]>'9' ){ //非法字符
			flag=-1;
			return (double)0.0;
		}
		result=result*10.0+( str[i]-'0' );
	}	
	/* 如果有小数点，则查找小数点后、e/E之前的部分 */
	if( ispoint ){  //1.e-10算作符合
		base=1.0;
		for( ;i<len;i++ ){
			if( str[i]=='e'||str[i]=='E' ){
				i++;
				isexponent=1;
				break;
			}
			if( str[i]<'0' || str[i]>'9' ){ //非法字符
				//printf("str[%d]=%d\n",i,str[i]);
				flag=-1;
				return ( double )0.0;
			}
			base *=0.1;
			result += ( str[i]-'0' )*base;
		}
	}	
	/* 如果有指数部分，则判断指数部分符号位，并查找指数部分 */
	if( isexponent ){
		base = 10.0;
		if( str[i]=='+' ){
			i++;
		}else if( str[i]=='-' ){
			base=0.1; 
			i++;
		}
		for( ;i<len;i++ ){
			if( str[i]<'0' || str[i]>'9' ){ //非法字符
				//printf("str[%d]=%d\n",i,str[i]);
				flag=-1;
				return ( double )0.0;
			}
			exponet=exponet*10+( str[i]-'0' );
		}
		/* 求值，将指数部分算进去 */
		while( 1 ){
			if( exponet & 1 ){  //指数为奇数
				result *= base;
			}
			exponet >>= 1;
			if( exponet == 0 ){
				break;
			}
			base *= base;
		}
	}	
	result = sign == 1 ? result : -result;
	if(result > MAXKEY || result < MINKEY ||(result < DBL_MIN && result > -DBL_MIN)){
		flag=-1;
		return (double)0.0;
	}
	return  result;
}

/*** 判断一个字符串是不是浮点数：是的话，返回1；否则，返回0 ***/