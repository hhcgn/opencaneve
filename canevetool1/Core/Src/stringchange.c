#include "stringchange.h"
#include "string.h"

int detopwm(int *deg){//将度数与pwm波数据进行相应转化
	 int pwmvalue=115**deg/180+20;
	if(*deg==0) pwmvalue=20;
	else if(*deg>180) {
		pwmvalue=135;
	if(*deg>180) *deg=180;
	}
	return pwmvalue;
}
unsigned long long  numtoc(int n){
    switch(n){
    case 0:
        return '0';
        break;
    case 1:
            return '1';
            break;
    case 2:
            return '2';
            break;
    case 3:
            return '3';
            break;

    case 4:
            return '4';
            break;
    case 5:
            return '5';
            break;
    case 6:
            return '6';
            break;
    case 7:
            return '7';
            break;
    case 8:
                return '8';
                break;
    case 9:
                return '9';
                break;
    default:
                return '\0';
                break;
    }
}
int ctoi(char *str)
{
	int len=0,num=0,count=0,i1=0;
	unsigned char ass=0;
	char index[]="0123456789";
	if(str[0]=='-') ass=1;
	len=strlen(str);
//	while(str[i1]!='\0'){
//		len++;
//		i1++;
//	}
	for( i1=0+ass;i1<len;i1++)
	{
		for(int i2=0;i2<10;i2++){
			if(str[i1]==index[i2]){
				num*=10;
				num+=i2;
				
			}
		}
	}
	return (num)*(ass==0? 1:-1);
}
char* itoa(int num,char* str,int radix)
{
	char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";//索引表
	unsigned unum;//存放要转换的整数的绝对值,转换的整数可能是负数
	int ic=0,j,k;//i用来指示设置字符串相应位，转换之后i其实就是字符串的长度；转换后顺序是逆序的，有正负的情况，k用来指示调整顺序的开始位置;j用来指示调整顺序时的交换。
 char temp;//临时变量，交换两个值时用到
	//获取要转换的整数的绝对值
	if(radix==10&&num<0)//要转换成十进制数并且是负数
	{
		unum=(unsigned)-num;//将num的绝对值赋给unum
		str[ic++]='-';//在字符串最前面设置为'-'号，并且索引加1
	}
	else unum=(unsigned)num;//若是num为正，直接赋值给unum
 
	//转换部分，注意转换后是逆序的
	do
	{
		str[ic++]=index[unum%(unsigned)radix];//取unum的最后一位，并设置为str对应位，指示索引加1
		unum/=radix;//unum去掉最后一位
 
	}while(unum);//直至unum为0退出循环
 
	str[ic]='\0';//在字符串最后添加'\0'字符，c语言字符串以'\0'结束。
 
	//将顺序调整过来
	if(str[0]=='-') k=1;//如果是负数，符号不用调整，从符号后面开始调整
	else k=0;//不是负数，全部都要调整
 
	
	for(j=k;j<=(ic-1)/2;j++)//头尾一一对称交换，i其实就是字符串的长度，索引最大值比长度少1
	{
		temp=str[j];//头部赋值给临时变量
		str[j]=str[ic-1+k-j];//尾部赋值给头部
		str[ic-1+k-j]=temp;//将临时变量的值(其实就是之前的头部值)赋给尾部
	}
 
	return str;//返回转换后的字符串
}

 
