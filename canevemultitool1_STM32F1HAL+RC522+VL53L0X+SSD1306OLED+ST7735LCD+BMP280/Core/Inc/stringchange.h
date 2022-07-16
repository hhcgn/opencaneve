#ifndef __STRINGCHANGE_H
#define __STRINGCHANGE_H 			   
//#include "stm32f10x.h"
char* itoa(int num,char* str,int radix);
int ctoi(char *str);
int detopwm(int *deg);
long hexToDec(char *source);
unsigned long long  numtoc(int n);
#endif