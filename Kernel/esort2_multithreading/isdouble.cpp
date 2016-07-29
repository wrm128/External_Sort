#include "stdafx.h"
#include "isdouble.h"

/* ���ַ���ת��Ϊ������ */
double Strtod(const char *s, char **endptr)
{
	register const char *p     = s;
	register long double  value = 0.L;
	int sign = 0;
	long double factor;
	unsigned int expo;   //ָ������
	while( isspace(*p) ){ //����ǰ��Ŀո�
		p++;
	}
	if(*p == '-' || *p == '+'){
		sign = *p++;   //�ѷ��Ÿ����ַ�sign��ָ����ơ�
	}
	/* ����С����ǰ�Ĳ��� */
	while((unsigned int)(*p - '0') < 10u ){//ת����������
		value = value*10 + (*p++ - '0');
	}
	/* ����������ı�ʾ��ʽ���磺1234.5678�� */
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
	/* �����IEEE754��׼�ĸ�ʽ���磺1.23456E+3��*/
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


/*** �ж�һ���ַ����ǲ��Ǹ�������
�ǵĻ������ظ�����ֵ�����򣬸ı���λflag��������0.0 **/
double isDouble( int &flag,char *str )
{
	int i=0,len;
	len=strlen( str );
	int sign=1;  //���ţ�Ĭ��Ϊ��
	int exponet=0;     //ָ������
	int ispoint=0 , isexponent=0;
	double result=0.0  , base;
	while( str[i]==' '||str[i]=='\t'||str[i]=='\n' ){
		i++;
	}
	if( i>=len ){
		flag=-1;
		return (double)0.0;
	}
	/* �жϵ�������λ */
	if( str[i]=='+' ){
		i++;
	}else if( str[i]=='-' ){
		sign=-1;
		i++;
	}else if( str[i]<'0' || str[i]>'9' ){//�Ƿ��ַ�
		flag=-1;
		return (double)0.0;
	}
	/* ����С����ǰ��һ���� */
	for( ;i<len;i++ ){
		if( str[i]=='.' ){ //С����
			i++;
			ispoint=1;
			break;
		}
		if( str[i]=='e'||str[i]=='E' ){
			i++;
			isexponent=1;
			break;
		}
		if( str[i]<'0' || str[i]>'9' ){ //�Ƿ��ַ�
			flag=-1;
			return (double)0.0;
		}
		result=result*10.0+( str[i]-'0' );
	}	
	/* �����С���㣬�����С�����e/E֮ǰ�Ĳ��� */
	if( ispoint ){  //1.e-10��������
		base=1.0;
		for( ;i<len;i++ ){
			if( str[i]=='e'||str[i]=='E' ){
				i++;
				isexponent=1;
				break;
			}
			if( str[i]<'0' || str[i]>'9' ){ //�Ƿ��ַ�
				//printf("str[%d]=%d\n",i,str[i]);
				flag=-1;
				return ( double )0.0;
			}
			base *=0.1;
			result += ( str[i]-'0' )*base;
		}
	}	
	/* �����ָ�����֣����ж�ָ�����ַ���λ��������ָ������ */
	if( isexponent ){
		base = 10.0;
		if( str[i]=='+' ){
			i++;
		}else if( str[i]=='-' ){
			base=0.1; 
			i++;
		}
		for( ;i<len;i++ ){
			if( str[i]<'0' || str[i]>'9' ){ //�Ƿ��ַ�
				//printf("str[%d]=%d\n",i,str[i]);
				flag=-1;
				return ( double )0.0;
			}
			exponet=exponet*10+( str[i]-'0' );
		}
		/* ��ֵ����ָ���������ȥ */
		while( 1 ){
			if( exponet & 1 ){  //ָ��Ϊ����
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

/*** �ж�һ���ַ����ǲ��Ǹ��������ǵĻ�������1�����򣬷���0 ***/