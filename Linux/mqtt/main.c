
/*************************************************************************
	> File Name: main.c
	> Author: LFJ
	> Mail: 
	> Created Time: 2018年09月05日 星期三 13时48分17秒
 ************************************************************************/

#include <stdio.h>
#include "pthread.h"
#include "mqtt.h"
#include "unistd.h"
#include "string.h"


void *thread_fun(void *arg);
void *thread_database(void *arg);
void *thread_camera(void *arg);
void *thread_flash(void *arg);


int main(int argc, char *argv[])
{
    pthread_t thread_ID;		//定义线程id
    pthread_t tid,th_id2,th_id3,th_id4;

    my_mqtt.beep=0;
    strcpy(my_mqtt.hum,"0");
    my_mqtt.key='0';
    my_mqtt.flash=0;
    strcpy(my_mqtt.led,"0000");
    strcpy(my_mqtt.temp,"0");
    
    init_senser(&my_mqtt);
    
    pthread_create(&thread_ID, NULL, &cloud_mqtt_thread, NULL);	//创建一个线程执行mqtt客户端
    pthread_detach(thread_ID);	//设置线程结束收尸

    

    printf("My pid is %d, my tid is %lu.\n", getpid(), pthread_self()); // pthread_self()用于获取当前线程的线程id

    if ((pthread_create(&tid, NULL, thread_fun, (void *)(&tid)) != 0)) {
        perror("key pthread_create error");
        return -1;
        //exit(1);
    }
    if (pthread_create(&th_id2, NULL, thread_database, (void *)(&th_id2)) != 0) {//database
        perror("database pthread_create error");
        return -1;
    }
    if (pthread_create(&th_id3, NULL, thread_camera, (void *)(&th_id3)) != 0) {//cam
        perror("cam pthread_create error");
        return -1;
    }
    if (pthread_create(&th_id4, NULL, thread_flash, (void *)(&th_id4)) != 0) {//cam
        perror("cam pthread_create error");
        return -1;
    }
    
    
    
    set_funcation(&my_mqtt);
    while (1) {
        //mqtt_data_write("my yes", 6, 0);//循环发布"my yes"
        char send_data[100]={0};
        get_senser_data(&my_mqtt);
        sprintf(send_data,"led1%c|led2%c|hum%s|beep%d|temp%s|key%d|flash%d",my_mqtt.led[0],my_mqtt.led[1],my_mqtt.hum,my_mqtt.beep,my_mqtt.temp,my_mqtt.key,my_mqtt.flash);
        printf("pushdata%s\n",send_data);
        mqtt_data_write(send_data, strlen(send_data), 0);
        sleep(2);						//睡眠3s
        
    }

    return 0;
}
void *thread_fun(void *arg) {
	while(1){
		//  printf("My pid is %d, my tid is %lu.\n", getpid(), *((unsigned long int *)arg));
		//  sleep(2);
        	get_key(&my_mqtt);
	}
   

    return NULL;
}
void *thread_database(void *arg) {
	printf("database\n");
	while(1){
	//sleep(3);
		 database_start();
	}
   

    return NULL;
}
void *thread_flash(void *arg) {
	printf("flash\n");
	while(1){
	
	
	
	
		 usleep(50000);
		 
		 flash(&my_mqtt);
		 usleep(50000);
		 
	
	}
   

    return NULL;
}

void *thread_camera(void *arg){
printf("cam\n");//sleep(3);
	while(1){
	//printf("database\n");sleep(3);
		 if(cam_start()<0){
		 	sleep(3);
		 	printf("cam_err\n");
		 }
	}
   

    return NULL;
}
