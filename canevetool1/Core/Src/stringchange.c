#include "stringchange.h"
#include "string.h"

int detopwm(int *deg){//��������pwm�����ݽ�����Ӧת��
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
	char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";//������
	unsigned unum;//���Ҫת���������ľ���ֵ,ת�������������Ǹ���
	int ic=0,j,k;//i����ָʾ�����ַ�����Ӧλ��ת��֮��i��ʵ�����ַ����ĳ��ȣ�ת����˳��������ģ��������������k����ָʾ����˳��Ŀ�ʼλ��;j����ָʾ����˳��ʱ�Ľ�����
 char temp;//��ʱ��������������ֵʱ�õ�
	//��ȡҪת���������ľ���ֵ
	if(radix==10&&num<0)//Ҫת����ʮ�����������Ǹ���
	{
		unum=(unsigned)-num;//��num�ľ���ֵ����unum
		str[ic++]='-';//���ַ�����ǰ������Ϊ'-'�ţ�����������1
	}
	else unum=(unsigned)num;//����numΪ����ֱ�Ӹ�ֵ��unum
 
	//ת�����֣�ע��ת�����������
	do
	{
		str[ic++]=index[unum%(unsigned)radix];//ȡunum�����һλ��������Ϊstr��Ӧλ��ָʾ������1
		unum/=radix;//unumȥ�����һλ
 
	}while(unum);//ֱ��unumΪ0�˳�ѭ��
 
	str[ic]='\0';//���ַ���������'\0'�ַ���c�����ַ�����'\0'������
 
	//��˳���������
	if(str[0]=='-') k=1;//����Ǹ��������Ų��õ������ӷ��ź��濪ʼ����
	else k=0;//���Ǹ�����ȫ����Ҫ����
 
	
	for(j=k;j<=(ic-1)/2;j++)//ͷβһһ�Գƽ�����i��ʵ�����ַ����ĳ��ȣ��������ֵ�ȳ�����1
	{
		temp=str[j];//ͷ����ֵ����ʱ����
		str[j]=str[ic-1+k-j];//β����ֵ��ͷ��
		str[ic-1+k-j]=temp;//����ʱ������ֵ(��ʵ����֮ǰ��ͷ��ֵ)����β��
	}
 
	return str;//����ת������ַ���
}

 
