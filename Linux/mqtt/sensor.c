/*===============================================
*   文件名称：sensor.c
*   创 建 者：     
*   创建日期：2023年05月15日
*   描    述：
================================================*/
#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <asm/ioctl.h>
#include <string.h>
#include <stdlib.h>

#include "mqtt.h"

//led
#define LEDON _IO('L',0)
#define LEDOFF _IO('L',2)
#define ledon(fd) ioctl(fd, LEDON)
#define ledoff(fd) ioctl(fd, LEDOFF)

//pwm
#define PWMON  _IO('P', 0)
#define PWMOFF _IO('P', 1)
#define PWMSET _IO('P', 2)

//adc
#define SETMUX _IO('A', 0)
#define SETBIT _IO('A', 1)
#define BIT10  _IO('A', 2)
#define BIT12  _IO('A', 3)
//ds18b20
#define TYPE 'c'
#define temp_9  _IO(TYPE,0)
#define temp_10 _IO(TYPE,1)
#define temp_11 _IO(TYPE,2)
#define temp_12 _IO(TYPE,3)
#define PATH "/dev/ds18b20"


//key

int adc_data;
char adc_buf[10];
int fd_dsb;
int fd_key;
int fd_pwm;
int fd_led0;
int fd_led1;
int fd_led2;
int fd_led3;
int fd_adc;
short temp;
char zheng,fen;
float temputer,resolution;



int init_senser(MQTT_DATA *senser){
     fd_dsb= open(PATH,O_RDWR);

     fd_key= open("/dev/key3", O_RDWR);
    
     fd_pwm= open("/dev/pwm0", O_RDWR);
    
     fd_led0= open("/dev/led0", O_RDWR);
     fd_led1= open("/dev/led1", O_RDWR);
     fd_led2= open("/dev/led2", O_RDWR);
     fd_led3= open("/dev/led3", O_RDWR);
    
     fd_adc= open("/dev/adc", O_RDWR);
    ioctl(fd_adc, SETMUX, 3);//mux 3
    ioctl(fd_adc, SETBIT, BIT10);//10 bit
    	
    
    if(fd_dsb < 0){
		perror("open");
		return -EINVAL;
	}
	if(ioctl(fd_dsb,temp_12,&resolution))
	{
		perror("ioctl \n");
		return -EINVAL;
	}

    }
int get_senser_data(MQTT_DATA *senser){
	


    read(fd_adc, &adc_data, sizeof adc_data);
    // adc_buf[10]=0;       	
    sprintf(senser->hum,"%d",adc_data);//湿度
    printf("hum%s\n",senser->hum);


    

    if(!read(fd_dsb,&temp,sizeof(short))){
    perror("read");
    }
    zheng = temp>>4;
    fen = temp & 0xf;
    if(zheng & (1<<8)){
    temputer = (temp - 65535) * resolution;
    }else{
    temputer = zheng + fen * resolution;
    }
    sleep(1);
    if((temputer >= (-55)) && (temputer <= 125))
    printf("temp :%d \t resolution :%0.3f ,tempter:%0.3f\n",temp ,resolution,temputer);
    //char temp2[100]={0};
    
    sprintf(senser->temp,"%d",(int)temputer);//温度



    return 0;
} 
void get_key(MQTT_DATA *senser){
  	//char c;
    read(fd_key, &senser->key, 1);//按键
    //sprintf(senser->key,"%c",c);
}
void fd_close(){
    close(fd_pwm);
    close(fd_led0);
    close(fd_led1);
    close(fd_led2);
    close(fd_led3);
    close(fd_adc);
    close(fd_dsb);
    close(fd_key);
}

void set_funcation(MQTT_DATA *senser){
	/*switch(senser->key)
	{
		case '1'://led
         
            // ledoff(fd_led0);
            // ledoff(fd_led1);
            // ledoff(fd_led2);
            // ledoff(fd_led3);
		break;
		case '2'://beep
            // ioctl(fd_pwm, PWMOFF);
		break;
		case '3':
		break;
	}*/
	//if(senser->led[0]=='1') ledon(fd_led0);
	//else ledoff(fd_led0);
	//if(senser->led[1]=='1') ledon(fd_led1);
	//else ledoff(fd_led1);
	
	if(1!=senser->flash){
	if(senser->led[0]=='1') ledon(fd_led2);
	else ledoff(fd_led2);
	if(senser->led[1]=='1') ledon(fd_led3);
	else ledoff(fd_led3);
	}
	
	if(senser->beep==1) {if(senser->temp[0]>='4'&&senser->temp[0]<'9'||(senser->temp[1]<='9'&&senser->temp[0]=='3'&&senser->temp[1]>='5'))ioctl(fd_pwm, PWMON);}
	else ioctl(fd_pwm, PWMOFF);


}
void flash(MQTT_DATA *s){
	int static flash_flag=0;
	//printf("flash\n");
	if(s->flash){
	if(0==flash_flag){
	flash_flag=1;
	ledon(fd_led2);
	ledon(fd_led3);
	}else{
	flash_flag=0;
	ledoff(fd_led2);
	ledoff(fd_led3);
	}}
}


