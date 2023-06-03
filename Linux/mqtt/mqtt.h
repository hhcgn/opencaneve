/*
 * @Author: CanEve 2785546297@qq.com
 * @Date: 2023-05-19 14:32:39
 * @LastEditors: CanEve 2785546297@qq.com
 * @LastEditTime: 2023-05-19 14:35:20
 * @FilePath: \mqtt\mqtt.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/*************************************************************************
	> File Name: matt.h
	> Author: 
	> Mail: 
	> Created Time: 2018年09月05日 星期三 16时40分31秒
 ************************************************************************/

#ifndef NET_PROC_H
#define NET_PROC_H

#ifdef __cplusplus
extern "C" {
#endif

enum  iot_ctrl_status_t
{
	IOT_STATUS_LOGIN,
	IOT_STATUS_CONNECT,
	IOT_STATUS_DROP,
};
typedef struct mqtt
{
	char led[4],temp[5],hum[4],key;
	int beep,flash;
	
	
	
}MQTT_DATA;
MQTT_DATA my_mqtt;

typedef void  (*pMessageArrived_Fun)(void*,int len);

void mqtt_module_init(void);
int mqtt_data_write(char *pbuf, int len, char retain);

void mqtt_data_rx_cb(void *pbuf, int len);
void *cloud_mqtt_thread(void *arg);

void set_funcation(MQTT_DATA *senser);
void fd_close();
int get_senser_data(MQTT_DATA *s);
void get_key(MQTT_DATA *s);
int init_senser(MQTT_DATA *s);
int database_start();

int cam_start();
void flash(MQTT_DATA *s);

#define mDEBUG(fmt, ...)  printf("%s[%s](%d):" fmt,__FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif
