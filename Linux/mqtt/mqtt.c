/*************************************************************************
	> File Name: main.c
	> Author: LFJ
	> Mail: 
	> Created Time: 2018年09月05日 星期三 13时48分17秒
 ************************************************************************/

#include <stdio.h>

#include "MQTTClient.h"
#include "mqtt.h"
#include "pthread.h"
#include "string.h"
#include "unistd.h"
#include "sys/stat.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "fcntl.h"


#define MQTT_TOPIC_SIZE     (128)		//订阅和发布主题长度
#define MQTT_BUF_SIZE       (8 * 1024) 	//接收后发送缓冲区大小

#define MQTT_HOST "81.70.2.209"		//ip地址
#define MQTT_PORT 1883					//端口号
#define MQTT_USER ""				//用户名
#define MQTT_PASS ""			//密码
#define MQTT_CLIENT_ID "caneveclient"		//客户端标识

typedef struct {
    Network Network;
    Client Client;
    char sub_topic[MQTT_TOPIC_SIZE];		//存放订阅主题
    char pub_topic[MQTT_TOPIC_SIZE];		//存放发布主题
    char mqtt_buffer[MQTT_BUF_SIZE];		//发送缓冲区
    char mqtt_read_buffer[MQTT_BUF_SIZE];	//接收缓冲区

    unsigned char willFlag;					
    MQTTPacket_willOptions will;
    char will_topic[MQTT_TOPIC_SIZE];		//存放遗嘱主题

    pMessageArrived_Fun DataArrived_Cb;
}Cloud_MQTT_t;

typedef struct{
    enum iot_ctrl_status_t iotstatus;
    char model[5];
    char company[32];
} iot_device_info_t;//主题结构体


struct opts_struct {
    char    *clientid;
    int     nodelimiter;
    char    *delimiter;
    enum    QoS qos;
    char    *username;
    char    *password;
    char    *host;
    int     port;
    int     showtopics;
} opts = {
    (char *)"iot-dev", 0, (char *)"\n", QOS0, "admin", "password", (char *)"localhost", 1883, 0
};//初始化结构体

Cloud_MQTT_t Iot_mqtt;

iot_device_info_t gateway = {
    .iotstatus = IOT_STATUS_LOGIN,
    .model = {"can"},
    .company = {""}
};//初始化主题


void iot_mqtt_init(Cloud_MQTT_t *piot_mqtt) 
{
    //printf("3init?\n");//3
    memset(piot_mqtt, '\0', sizeof(Cloud_MQTT_t));

    sprintf(piot_mqtt->sub_topic, "%s%s1", gateway.model, gateway.company);	//将初始化好的订阅主题填到数组中
    printf("3subscribe:%s\n", piot_mqtt->sub_topic);

    sprintf(piot_mqtt->pub_topic, "%s%s0", gateway.model, gateway.company);	//将初始化好的发布主题填到数组中
    printf("pub:%s\n", piot_mqtt->pub_topic);

    piot_mqtt->DataArrived_Cb = mqtt_data_rx_cb;		//设置接收到数据回调函数

}
void MQTTMessageArrived_Cb(MessageData* md)
{
    MQTTMessage *message = md->message; 

    Cloud_MQTT_t *piot_mqtt = &Iot_mqtt;

    if (NULL != piot_mqtt->DataArrived_Cb) {
        piot_mqtt->DataArrived_Cb((void *)message->payload, message->payloadlen);//异步消息体
    }
}

int mqtt_device_connect(Cloud_MQTT_t *piot_mqtt)
{//5
    int rc = 0, ret = 0;

    NewNetwork(&piot_mqtt->Network);

    printf("5topic = %s\n", piot_mqtt->sub_topic);

    rc = ConnectNetwork(&piot_mqtt->Network, MQTT_HOST, (int)MQTT_PORT);	
    if (rc != 0) {
        printf("mqtt connect network fail \n");
        ret = -101;
        goto __END;
    }
    MQTTClient(&piot_mqtt->Client, &piot_mqtt->Network, 1000, piot_mqtt->mqtt_buffer, MQTT_BUF_SIZE, piot_mqtt->mqtt_read_buffer, MQTT_BUF_SIZE);
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;

    if (piot_mqtt->willFlag) {
        data.willFlag = 1;
        memcpy(&data.will, &piot_mqtt->will, sizeof(MQTTPacket_willOptions));
    } else {
        data.willFlag = 0;
    }
    data.MQTTVersion = 3;
    data.clientID.cstring = MQTT_CLIENT_ID;
    data.username.cstring = MQTT_USER;
    data.password.cstring = MQTT_PASS;
    data.keepAliveInterval = 30;
    data.cleansession = 1;
    
    rc = MQTTConnect(&piot_mqtt->Client, &data);
    if (rc) {
        printf("mqtt connect broker fail \n");
        printf("rc = %d\n", rc);
        ret = -102;
        goto __END;
    }
    rc = MQTTSubscribe(&piot_mqtt->Client, piot_mqtt->sub_topic, opts.qos, MQTTMessageArrived_Cb);
    if (rc) {
        printf("mqtt subscribe fail \n");
        ret = -105;
        goto __END;
    }
    gateway.iotstatus = IOT_STATUS_CONNECT;
    printf("Subscribed %d\n", rc);

__END:
    return ret;
}

int mqtt_device_disconnect(Cloud_MQTT_t *piot_mqtt)//断开mqtt连接
{
    int ret = 0;

    ret = MQTTDisconnect(&piot_mqtt->Client);
    printf("disconnectNetwork ret = %d\n", ret);

    return ret;
}

void iot_yield(Cloud_MQTT_t *piot_mqtt)
{
    //printf("iot_yield?\n");
    int ret;

    switch (gateway.iotstatus) {
        case IOT_STATUS_LOGIN:
        ret = mqtt_device_connect(piot_mqtt);
        if (ret < 0) {
            printf("iot connect error code %d\n", ret);
            sleep(1);
        }
        break;
        case IOT_STATUS_CONNECT:
        ret = MQTTYield(&piot_mqtt->Client, 100);
        if (ret != SUCCESS) {
            gateway.iotstatus = IOT_STATUS_DROP;
        }
        break;
        case IOT_STATUS_DROP:
        printf("IOT_STATUS_DROP\n");
        mqtt_device_disconnect(piot_mqtt);
        gateway.iotstatus = IOT_STATUS_LOGIN;
        usleep(1000);
        break;
        default:
        break;
    }
}

int mqtt_will_msg_set(Cloud_MQTT_t *piot_mqtt, char *pbuf, int len)//设置遗嘱函数
{
    printf("4msg_set?\n");//5
    memset(piot_mqtt->will_topic, '\0', MQTT_TOPIC_SIZE);
    MQTTPacket_willOptions mqtt_will = MQTTPacket_willOptions_initializer;

    strcpy(piot_mqtt->will_topic, Iot_mqtt.pub_topic);
    memcpy(&Iot_mqtt.will, &mqtt_will, sizeof(MQTTPacket_willOptions));

    Iot_mqtt.willFlag = 1;
    Iot_mqtt.will.retained = 1;
    Iot_mqtt.will.topicName.cstring = (char *)piot_mqtt->will_topic;
    Iot_mqtt.will.message.cstring = (char *)pbuf;
    Iot_mqtt.will.qos = QOS2;

}

void mqtt_data_rx_cb(void *pbuf, int len) 
{

    printf("data = %s\n", (unsigned char *)pbuf);	//打印接收到的数据
    if(strncmp(pbuf,"led1",4)==0){
        if('0'==my_mqtt.led[0])
    	my_mqtt.led[0]='1';
    	else
    	my_mqtt.led[0]='0';
    	
        //my_mqtt.led[2]=led_buf[5];
        //my_mqtt.led[3]=led_buf[6];
    } if(strncmp(pbuf,"led2",4)==0){
    	if('0'==my_mqtt.led[1])
    	my_mqtt.led[1]='1';
    	else
    	my_mqtt.led[1]='0';
    }
     if(strncmp(pbuf,"flash",5)==0){
    	if(0==my_mqtt.flash)
    	my_mqtt.flash=1;
    	else
    	my_mqtt.flash=0;
    }
     if(0==strncmp(pbuf,"beep",4)){
    if(0==my_mqtt.beep)
    my_mqtt.beep=1;
    else
    my_mqtt.beep=0;
    
    }
    set_funcation(&my_mqtt);
    
    char send_data[100]={0};
    sprintf(send_data,"led1%c|led2%c|hum%s|beep%d|temp%s|key%d|flash%d",my_mqtt.led[0],my_mqtt.led[1],my_mqtt.hum,my_mqtt.beep,my_mqtt.temp,my_mqtt.key,my_mqtt.flash);
        printf("pushdata%s\n",send_data);
        mqtt_data_write(send_data, strlen(send_data), 0);


}

int mqtt_data_write(char *pbuf, int len, char retain)
{//start
    //printf("start\n");//1
    Cloud_MQTT_t *piot_mqtt = &Iot_mqtt; 
    int ret = 0;
    MQTTMessage message;
    char my_topic[128] = {0};

    strcpy(my_topic, piot_mqtt->pub_topic);

    printf("1publish topic is :%s\n%s\r\n", my_topic,pbuf);
    printf("mqtt tx len = %d\r\n", len);

    message.payload = (void *)pbuf;
    message.payloadlen = len;
    message.dup = 0;
    message.qos = QOS2;
    if (retain) {
        message.retained = 1;
    } else {
        message.retained = 0;
    }

    ret = MQTTPublish(&piot_mqtt->Client, my_topic, &message);	//发布一个主题

    return ret;
}

void *cloud_mqtt_thread(void *arg)
{ //2
    int ret, len; 
    char will_msg[256] = {"hello world"};						//初始化遗嘱数据
    printf("2start?\n");
    
    iot_mqtt_init(&Iot_mqtt);									//初始化主题
    mqtt_will_msg_set(&Iot_mqtt, will_msg, strlen(will_msg));	//设置遗嘱

    ret = mqtt_device_connect(&Iot_mqtt);						//初始化并连接mqtt服务器
    
    while (ret < 0) {//?
        printf("ret = %d\r\n", ret);
        sleep(3);
        ret = mqtt_device_connect(&Iot_mqtt);
    }

    while (1){//loop?
    //printf("while2\n");
    
        iot_yield(&Iot_mqtt);									//维持服务器稳定，断开重连
    }
}
