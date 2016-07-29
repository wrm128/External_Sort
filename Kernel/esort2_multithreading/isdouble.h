#ifndef ISDOUBLE_H
#define ISDOUBLE_H

/* 将字符串转化为浮点数 */
double Strtod(const char *s, char **endptr);

/*** 判断一个字符串是不是浮点数：
是的话，返回浮点数值；否则，改变标记位flag，并返回0.0 **/
double isDouble( int &flag,char *str );

#endif