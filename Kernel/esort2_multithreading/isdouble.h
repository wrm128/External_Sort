#ifndef ISDOUBLE_H
#define ISDOUBLE_H

/* ���ַ���ת��Ϊ������ */
double Strtod(const char *s, char **endptr);

/*** �ж�һ���ַ����ǲ��Ǹ�������
�ǵĻ������ظ�����ֵ�����򣬸ı���λflag��������0.0 **/
double isDouble( int &flag,char *str );

#endif