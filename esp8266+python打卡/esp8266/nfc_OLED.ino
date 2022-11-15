//////////////////////////////////////////////////////////////////////////////////   
//  文 件 名   : oled.c
//  作    者   : CanEve
//  生成日期   : 2022-11-6
//              D0   GPIO16（SCL）
//              D1   GPIO5 （SDA）  
//              ----------------------------------------------------------------
//******************************************************************************/
#include "oledfont.h"
#include <ESP8266WiFi.h>

#include <WiFiUDP.h>

#include <EEPROM.h>

#include <Ticker.h>




//I2C管脚的定义
#define SCL 5  //D0
#define SDA 4   //D1

//输出电平的宏定义
#define I2C_SCL_L digitalWrite(SCL,LOW)//SCL
#define I2C_SCL_H digitalWrite(SCL,HIGH)

#define I2C_SDA_L digitalWrite(SDA,LOW)//SDA
#define I2C_SDA_H digitalWrite(SDA,HIGH)

//管脚模式的宏定义
#define SCL_IN   pinMode(SCL, INPUT)   
#define SCL_OUT  pinMode(SCL, OUTPUT) //SCL
#define SDA_IN   pinMode(SDA, INPUT) 
#define SDA_OUT  pinMode(SDA, OUTPUT) //SDA

#define OLED_CMD  0  //写命令
#define OLED_DATA 1 //写数据


#ifdef DEBUG_PRINT
	#define DMSG(args...)       Serial.println(args)
	#define DMSG_STR(str)       Serial.println(str)
	#define DMSG_HEX(num)       Serial.println(num, HEX)
	#define DMSG_INT(num)       Serial.println(num)
#else
	#define DMSG(args...)       
	#define DMSG_STR(str)       
	#define DMSG_HEX(num)       
	#define DMSG_INT(num)       
#endif
#define PN532_PREAMBLE                (0x00)
#define PN532_STARTCODE1              (0x00)
#define PN532_STARTCODE2              (0xFF)
#define PN532_POSTAMBLE               (0x00)
#define PN532_HOSTTOPN532             (0xD4)
#define PN532_PN532TOHOST             (0xD5)
#define PN532_ACK_WAIT_TIME           (10)  
#define PN532_INVALID_ACK             (-1)
#define PN532_TIMEOUT                 (-2)
#define PN532_INVALID_FRAME           (-3)
#define PN532_NO_SPACE                (-4)
#define SerialNFC Serial
#define uint8_t unsigned char
#define RECV_BUFF_LEN 255

#define EEPROM_write(address,p) { int i = 0;byte *pp = (byte*)&(p);for(;i<sizeof(p);i++) EEPROM.write(address+i,pp[i]);EEPROM.end();}
#define EEPROM_read(address,p) { int i = 0;byte *pp = (byte*)&(p);for(;i<sizeof(p);i++) pp[i]=EEPROM.read(address+i);}		
uint8_t read_data[16]={0};

uint8_t command;
WiFiUDP Udp;
int hand[5]={0};
Ticker runontime ;
Ticker runontime1 ;
char *phone_date_str;
bool isstartf=false;
bool iswifi=true;
char number[10]={'0','1','2','3','4','5','6','7','8','9'};
bool islighting=false;
char packetBuffer[255];
char count_down=0;

int clickt,firsts,i11,i22,beepst=0,err_tim=0;
unsigned char card_id[10]={0},last_card_id[10]={0};
char read_state[15]="",is_signup[5]="no";
char wifi_state[15]="";
char recive_data[15]="0123";
char page_state=0,page[10][8]={"0","1","2","3","4","555","666","777","888","999"};
int num_i=0;
char dischange_i=0;


uint8_t nfc_wakeUp();//唤醒操作

uint8_t nfc_scaning(uint8_t []);//寻卡操作

uint8_t nfc_vertify(int block, uint8_t key[], uint8_t uuid[],uint8_t type);//验证

uint8_t nfc_readBlock(int block, uint8_t *data); //读取某一个块 

uint8_t nfc_writeBlock(int block, uint8_t *data);//写入数据到某个块



uint8_t recv_buff[RECV_BUFF_LEN] = {0};//NFC串口返回的数据

char* itoa(int num,char* str,int radix);

void waitforset(char type);

void turn_on_off(char type);

void report_state();

void light(int code00);

void StrToHex(unsigned char *pbDest, unsigned char *pbSrc, int nLen);

void hex2str(unsigned char/*int*/ data,unsigned char* s, int len);

uint8_t writeCommand(const uint8_t *header, uint8_t hlen, const uint8_t *body, uint8_t blen)

{
	/** dump serial buffer */
	if(SerialNFC.available()){
		DMSG("Dump serial buffer: ");
	}
	while(SerialNFC.available()){
		uint8_t ret = SerialNFC.read();
		DMSG_HEX(ret);
	}
	command = header[0];
	SerialNFC.write(PN532_PREAMBLE);//00
	SerialNFC.write(PN532_STARTCODE1);//00
	SerialNFC.write(PN532_STARTCODE2);//ff
	uint8_t length = hlen + blen + 1;//3+11+1=f   // length of data field: TFI + DATA
	SerialNFC.write(length);
	SerialNFC.write(~length + 1);//f1         // checksum of length
	SerialNFC.write(PN532_HOSTTOPN532);//d4
	uint8_t sum = PN532_HOSTTOPN532;  //d4  // sum of TFI + DATA
	DMSG("\nWrite: ");
	SerialNFC.write(header, hlen);
	for (uint8_t i = 0; i < hlen; i++) {
		sum += header[i];
		DMSG_HEX(header[i]);
	}
	SerialNFC.write(body, blen);
	for (uint8_t i = 0; i < blen; i++) {
		sum += body[i];
		DMSG_HEX(body[i]);
	}
	uint8_t checksum = ~sum + 1;            // checksum of TFI + DATA
	SerialNFC.write(checksum);
	SerialNFC.write(PN532_POSTAMBLE);//00
	return readAckFrame();
}
uint16_t readResponse(uint8_t buf[], uint8_t len, uint16_t timeout)
{
	uint8_t tmp[3];
	delay(100);
	DMSG("\nRead:  ");
	/** Frame Preamble and Start Code */
	if(receive(tmp, 3, timeout)<=0){
		return PN532_TIMEOUT;
	}
	if(0 != tmp[0] || 0!= tmp[1] || 0xFF != tmp[2]){
		DMSG("Preamble error");
		return PN532_INVALID_FRAME;
	}
	/** receive length and check */
	uint8_t length[2];
	if(receive(length, 2, timeout) <= 0){
		return PN532_TIMEOUT;
	}
	if( 0 != (uint8_t)(length[0] + length[1]) ){
		DMSG("Length error");
		return PN532_INVALID_FRAME;
	}
	length[0] -= 2;
//	if( length[0] > len){
//		return PN532_NO_SPACE;
//	}
	/** receive command byte */
	uint8_t cmd = command + 1;               // response command
	if(receive(tmp, 2, timeout) <= 0){
		return PN532_TIMEOUT;
	}
	if( PN532_PN532TOHOST != tmp[0] || cmd != tmp[1]){
		DMSG("Command error");
		return PN532_INVALID_FRAME;
	}
	if(receive(buf, length[0], timeout) != length[0]){
		return PN532_TIMEOUT;
	}
	uint8_t sum = PN532_PN532TOHOST + cmd;
	for(uint8_t i=0; i<length[0]; i++){
		sum += buf[i];
	}
	/** checksum and postamble */
	if(receive(tmp, 2, timeout) <= 0){
		return PN532_TIMEOUT;
	}
	if( 0 != (uint8_t)(sum + tmp[0]) || 0 != tmp[1] ){
		DMSG("Checksum error");
		return PN532_INVALID_FRAME;
	}
	return length[0];
}
uint8_t readAckFrame()
{
	const uint8_t PN532_ACK[] = {0, 0, 0xFF, 0, 0xFF, 0};
	uint8_t ackBuf[sizeof(PN532_ACK)];
	delay(100);
	DMSG("\nAck: ");
	if( receive(ackBuf, sizeof(PN532_ACK), PN532_ACK_WAIT_TIME) <= 0 ){
		DMSG("Timeout\n");
		return PN532_TIMEOUT;
	}
	if( memcmp(ackBuf, PN532_ACK, sizeof(PN532_ACK)) ){
		DMSG("Invalid\n");
		return PN532_INVALID_ACK;
	}
	DMSG("OK\n");
	return 0;
}
/**
	@brief receive data .
	@param buf --> return value buffer.
			len --> length expect to receive.
			timeout --> time of reveiving
	@retval number of received bytes, 0 means no data received.
*/
uint8_t receive(uint8_t *buf, int len, uint16_t timeout)
{
	int read_bytes = 0;
	int ret;
	unsigned long start_millis;
	while (read_bytes < len) {
	start_millis = millis();
	do {
		ret = SerialNFC.read();
		if (ret >= 0) {
		break;
		}
		delay(10);
	} while((timeout == 0) || ((millis()- start_millis ) < timeout));
	if (ret < 0) {
		if(read_bytes){
			return read_bytes;
		}else{
			return PN532_TIMEOUT;
		}
	}
	buf[read_bytes] = (uint8_t)ret;
	read_bytes++;
	}
	return read_bytes;
}
uint8_t nfc_wakeUp()
{
	const uint8_t cmd_wake_up[] = {
	0x55, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x03, 0xfd, 0xd4, 
	0x14, 0x01, 0x17, 0x00};
	const uint8_t wake_up_resp[] = {0x00 ,0x00 ,0xFF ,0x00 ,0xFF ,
		0x00 ,0x00 ,0x00 ,0xFF ,0x02 ,0xFE ,0xD5 ,0x15 ,0x16 ,0x00};
	for(int i=0; i<sizeof(cmd_wake_up); i++)
	{
		SerialNFC.write(cmd_wake_up[i]);
	}
	memset(recv_buff,0,255);///
	int getLen = receive(recv_buff,15,200);
	for(int i=0; i<5; i++)
	{
		if(wake_up_resp[i]!=recv_buff[i]){
			return -1;
		}
	}
	if(recv_buff[2]=='\0') return -1;
	return 0;
}
uint8_t nfc_scaning(uint8_t uuid[])
{
	uint8_t cmd_head[1] = {0x4a};
	uint8_t cmd_body[2] = {0x02,0x00};
	if(writeCommand(cmd_head, 1, cmd_body, 2) == 0)
	{
//      Serial.println("wr cmd ok");
	}
	memset(recv_buff,0,RECV_BUFF_LEN);
	int8_t ret = 0;
	if( (ret = readResponse(recv_buff, 13, 200)) >=0)
	{
//      PrintHex(recv_buff,13);
		for(int i=0; i<4; i++)
			uuid[i] = recv_buff[i+6];
		return 0;
	}else{

		return -1;
	}
}
uint8_t nfc_vertify(int block, uint8_t key[], uint8_t uuid[], uint8_t type)
{
	//40 01 60 07 FF FF FF FF FF FF 90 1F 2A A4
//00 00 ff 0f f1 d4 40 01 60 03 ff ff ff ff ff ff b1 57 68 70 ae 00
	uint8_t cmd_head[3] = {0x40,0x01,0x60};
	if(type==1) cmd_head[2]=0x61;
	uint8_t cmd_body[1+6+4] = {0};
	cmd_body[0] = block;
	for(int i=0; i<6; i++)
	{
		cmd_body[i+1] = key[i]; 
	}
	for(int i=0; i<4; i++)
	{
		cmd_body[i+7] = uuid[i];
	}
	if(writeCommand(cmd_head, 3, cmd_body, 11) == 0)
	{
//      Serial.println("wr cmd ok");
	}
	memset(recv_buff,0,RECV_BUFF_LEN);
	int8_t ret = readResponse(recv_buff, 1, 1000);
	if(ret >= 0){
		if(recv_buff[0] == 0x00)
			return 0;
	}
	return -1;
}



char loop_c=0;

  // 复位或上电后运行一次:


void setup()
{
  OLED_Init();
  OLED_ColorTurn(0);//0正常显示 1反色显示
  OLED_DisplayTurn(0);//0正常显示 1翻转180度显示
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(13, OUTPUT_OPEN_DRAIN);
  pinMode(12, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);
  digitalWrite(LED_BUILTIN,HIGH);
  digitalWrite(13,HIGH);
  WiFi.begin("RoboMaster", "123456789"); 
  Udp.begin(8266);
  runontime.attach(0.15,display);
  
//  runontime1.attach(0.2,getback);
}

void loop()
{
	

	delay(300);
	digitalWrite(D7,HIGH);
//   OLED_DrawBMP(0,0,128,64,BMP1); //显示图片
   //延时1秒
	card_read();
}
void report_state(){
	IPAddress localIp(192,168,1,155);//发送的目标IP地址
	Udp.beginPacket(localIp/*Udp.remoteIP()*/,/*Udp.remotePort()*/45555);//目标端口(也就是app端)	
	//if(ticker_sec%2==0){
	phone_date_str = (char*)malloc(6*strlen("\",\"S1\"\:\"")+strlen("[{\"S2\"\:\"")+strlen(is_signup)+strlen((char *)card_id)+strlen("code_data1")+strlen("code_read1")+10+strlen("\"}]")+strlen(read_state));
	//else isstartf =true;
	sprintf(phone_date_str, "%s%s%s%s%s%s%s%s%s%s%s%s%s", "{\"S\"\:\"",is_signup,"\",\"I\"\:\"",card_id,"\",\"P1\"\:\"","code_scr1","\",\"W\"\:\"","code_data1","\",\"ER\"\:\"",read_state,"\",\"R\"\:\"","code_read1","\"}");//以上三行是构建连接请求
	Udp.write(phone_date_str);
	free(phone_date_str);
	Udp.endPacket();
	
}
void display(){
	dischange_i++;
	if(count_down>0)count_down--;
	
	if(dischange_i>32){
		for(char iis1=0;iis1<8;iis1++){
			  last_card_id[iis1]=0;
			  
		}
		strcpy(read_state,"Place Card");
		
		dischange_i=0;
	} if(dischange_i>26) strcpy(recive_data,"bbbb");
	if (WiFi.status() == WL_CONNECTED) {
	strcpy(wifi_state,"connect   ");
	iswifi==true;
    }else{
		strcpy(wifi_state,"disconnect");
		iswifi==false;
	}
	if(dischange_i%4==0)light(0);	
	

//  card_read();
//  OLED_Clear();//清除界面
   OLED_ShowString(0,0,read_state,16);
//  OLED_ShowString(0,2,recive_data,16);
	OLED_ShowChinese(0,2,chartonum(recive_data[0]),16);
	OLED_ShowChinese(16,2,chartonum(recive_data[1]),16);
	OLED_ShowChinese(32,2,chartonum(recive_data[2]),16);
	OLED_ShowChinese(48,2,chartonum(recive_data[3]),16);
//   OLED_ShowString(20,4,"2021/1/18",16);
//   OLED_ShowString(0,4,page[num_i],8);  
   OLED_ShowString(0,5,page[page_state],8);
   OLED_ShowString(0,6,wifi_state,8);
//   OLED_ShowString(63,6,"CODE:",16);
//   delay(1000); //延时1秒 delay函数的单位为ms
//   OLED_Clear();
	num_i++;
	if(num_i==9) num_i=0;
	if(digitalRead(12)==LOW){
		page_state=1;
	}else if(digitalRead(14)==LOW){
		page_state=2;
	}else if(digitalRead(16)==LOW){
		page_state=3;
	}
	
	int packetSize = Udp.parsePacket();
	if (packetSize) {
	IPAddress remoteIp = Udp.remoteIP();
	int len = Udp.read(packetBuffer, 255);
	if (len > 0) {packetBuffer[len] = 0;}
	String sg=packetBuffer;
	if(sg=="ok"){
		recive_data[0]='0';
		recive_data[1]='1';
		recive_data[2]='2';
		recive_data[3]='3';
	}else if(sg=="end"){
		recive_data[0]='4';
		recive_data[1]='5';
		recive_data[2]='6';
		recive_data[3]='7';
		}else {
		recive_data[0]='8';
		recive_data[1]='9';
		recive_data[2]='a';
		recive_data[3]='b';
	}}
}
void getback(){
	int packetSize = Udp.parsePacket();
	if (packetSize) {
	IPAddress remoteIp = Udp.remoteIP();
	int len = Udp.read(packetBuffer, 255);
	if (len > 0) {packetBuffer[len] = 0;}
	strcpy(recive_data,packetBuffer);
	
	}
}
void light(int code00){
	if(code00==1){
		digitalWrite(LED_BUILTIN,HIGH);
	}else{
	if(!islighting){
	digitalWrite(LED_BUILTIN,LOW);
		islighting=true;
	}else{
	digitalWrite(LED_BUILTIN,HIGH);
	islighting=false;
	}}
	if(iswifi==false) digitalWrite(LED_BUILTIN,LOW);
}

void card_read(){ 

	uint8_t uuid[4] = {0};
      if(nfc_wakeUp() == 0)
      {
		  err_tim=0;
		  
//		  strcpy(read_state,"pn532_ok");
		  if(recive_data[0]!='b')
		  for(char iis1=0;iis1<8;iis1++){
			  read_state[iis1]=card_id[iis1];
		}
      }else{
		  strcpy(read_state,"system_er");
		  err_tim++;
			// if(err_tim>10) ESP.reset();
		  delay(100);
		  memset(recv_buff,0,RECV_BUFF_LEN);
		  
		
//		  goto end;
      }
	delay(100);
	  if(read_state[1]==' ')
	  strcpy(read_state,"Place Card");
      nfc_scaning(uuid);
		  card_id[8]='\0';
	  if(uuid[0]!='\0'&&uuid[1]!='\0'&&uuid[2]!='\0'&&uuid[3]!='\0'&&(uuid[1]!=0&&uuid[2]!=0&&uuid[3]!=0)){
		  
			
		  hex2str(uuid[0],&card_id[0],2);
		  hex2str(uuid[1],&card_id[2],2);
		  hex2str(uuid[2],&card_id[4],2);
		  hex2str(uuid[3],&card_id[6],2);

		  
		for(char iis1=0;iis1<8;iis1++){
			if(card_id[iis1]!=last_card_id[iis1])
			{
				dischange_i=0;
				
			digitalWrite(13,LOW);
			report_state();
			
				break;
				}
		}
//		if(recive_data[0]!='b'){
		for(char iis1=0;iis1<8;iis1++){
			  read_state[iis1]=card_id[iis1];
			  last_card_id[iis1]=card_id[iis1];
		}
//		}else strcpy(read_state,"Place Card");
		
		}
		read_state[8]=' ';
		read_state[9]=' ';
		read_state[10]='\0';
		 
		
		

	  end:;
}
int chartonum(char c){
	switch(c){
		case '0':
			return 0;
		break;
		case '1':
			return 1;
		break;
		case '2':
			return 2;
		break;
		case '3':
			return 3;
		break;
		case '4':
			return 4;
		break;
		case '5':
			return 5;
		break;
		case '6':
			return 6;
		break;
		case '7':
			return 7;
		break;
		case '8':
			return 8;
		break;
		case '9':
			return 9;
		break;
		case 'a':
			return 10;
		break;
		case 'b':
			return 11;
		break;
		default:
			return 11;
	}
}
void hex2str(unsigned char/*int*/ data,unsigned char* s, int len)
{//16转字符串
    int i;
 
    s[len] = 0;
    for (i = len - 1; i >= 0; i--, data >>= 4)
    {
        if ((data & 0xf) <= 9)
            s[i] = (data & 0xf) + '0';
        else
            s[i] = (data & 0xf) + 'A' - 0x0a;
    }
    //return 1;
}
//I2C初始化
void I2C_Init()
{
  SDA_OUT;
  SCL_OUT;
  I2C_SDA_H;
  I2C_SCL_H;
}

//I2C开始
void I2C_Start()
{
  I2C_SCL_H;
  I2C_SDA_H;
  I2C_SDA_L;
  I2C_SCL_L;
}

//I2C结束
void I2C_Stop()
{
  I2C_SCL_H;
  I2C_SDA_L;
  I2C_SDA_H;
}

//检查应答信号
void I2C_Wait_Ack()
{
    I2C_SDA_H;
    I2C_SCL_H;
    I2C_SCL_L;
}

//I2C写入一个字节
void Write_I2C_Byte(unsigned char dat)
{
  unsigned char i;
  
  for(i=0;i<8;i++)    
  {
    I2C_SCL_L;
    if( dat & 0x80 )
    {
      I2C_SDA_H;  
    }
    else
    {
      I2C_SDA_L;
    }
    dat <<= 1;
    I2C_SCL_H;
   }
   I2C_SCL_L;
}

//发送一个字节
//向SSD1306写入一个字节。
//mode:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(u8 dat,u8 mode)
{
  I2C_Start();
  Write_I2C_Byte(0x78);
  I2C_Wait_Ack();
  if(mode){
    Write_I2C_Byte(0x40);
  }
  else{
    Write_I2C_Byte(0x00);
  }
  I2C_Wait_Ack();
  Write_I2C_Byte(dat);
  I2C_Wait_Ack();
  I2C_Stop();
}

//反显函数
void OLED_ColorTurn(u8 i)
{
  if(!i) OLED_WR_Byte(0xA6,OLED_CMD);//正常显示
  else  OLED_WR_Byte(0xA7,OLED_CMD);//反色显示
}

//屏幕旋转180度
void OLED_DisplayTurn(u8 i)
{
  if(i==0)
  {
      OLED_WR_Byte(0xC8,OLED_CMD);//正常显示
      OLED_WR_Byte(0xA1,OLED_CMD);
  }
  else
  {
    OLED_WR_Byte(0xC0,OLED_CMD);//反转显示
    OLED_WR_Byte(0xA0,OLED_CMD);
  }
}

//坐标设置
void OLED_Set_Pos(u8 x, u8 y) 
{ 
  OLED_WR_Byte(0xb0+y,OLED_CMD);
  OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
  OLED_WR_Byte((x&0x0f),OLED_CMD);
}
       
//开启OLED显示    
void OLED_Display_On(void)
{
  OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
  OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
  OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}

//关闭OLED显示     
void OLED_Display_Off(void)
{
  OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
  OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
  OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}
            
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!   
void OLED_Clear(void)  
{  
  u8 i,n;       
  for(i=0;i<8;i++)  
  {  
    OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
    OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
    OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
    for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
  } //更新显示
}

//在指定位置显示一个字符
//x:0~127
//y:0~63         
//sizey:选择字体 6x8  8x16
void OLED_ShowChar(u8 x,u8 y,const u8 chr,u8 sizey)
{       
  u8 c=0,sizex=sizey/2,temp;
  u16 i=0,size1;
  if(sizey==8)size1=6;
  else size1=(sizey/8+((sizey%8)?1:0))*(sizey/2);
  c=chr-' ';//得到偏移后的值
  OLED_Set_Pos(x,y);
  for(i=0;i<size1;i++)
  {
    if(i%sizex==0&&sizey!=8) OLED_Set_Pos(x,y++);
    if(sizey==8)
    {
      temp=pgm_read_byte(&asc2_0806[c][i]);
      OLED_WR_Byte(temp,OLED_DATA);//6X8字号
    }
    else if(sizey==16) 
    {
      temp=pgm_read_byte(&asc2_1608[c][i]);
      OLED_WR_Byte(temp,OLED_DATA);//8x16字号
    }
    else return;
  }
}

//m^n函数
u32 oled_pow(u8 m,u8 n)
{
  u32 result=1;  
  while(n--)result*=m;    
  return result;
}
         
//显示数字
//x,y :起点坐标
//num:要显示的数字
//len :数字的位数
//sizey:字体大小      
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 sizey)
{           
  u8 t,temp,m=0;
  u8 enshow=0;
  if(sizey==8)m=2;
  for(t=0;t<len;t++)
  {
    temp=(num/oled_pow(10,len-t-1))%10;
    if(enshow==0&&t<(len-1))
    {
      if(temp==0)
      {
        OLED_ShowChar(x+(sizey/2+m)*t,y,' ',sizey);
        continue;
      }else enshow=1;
    }
    OLED_ShowChar(x+(sizey/2+m)*t,y,temp+'0',sizey);
  }
}

//显示一个字符号串
void OLED_ShowString(u8 x,u8 y,const char *chr,u8 sizey)
{
  u8 j=0;
  while (chr[j]!='\0')
  {   
    OLED_ShowChar(x,y,chr[j++],sizey);
    if(sizey==8)x+=6;
    else x+=sizey/2;
  }
}

//显示汉字
void OLED_ShowChinese(u8 x,u8 y,const u8 no,u8 sizey)
{
  u16 i,size1=(sizey/8+((sizey%8)?1:0))*sizey;
  u8 temp;
  for(i=0;i<size1;i++)
  {
    if(i%sizey==0) OLED_Set_Pos(x,y++);
    if(sizey==16) 
    {
      temp=pgm_read_byte(&Hzk[no][i]);
      OLED_WR_Byte(temp,OLED_DATA);//16x16字号
    }
    //else if(sizey==xx) OLED_WR_Byte(xxx[c][i],OLED_DATA);//用户添加字号
    else return;
  }       
}


//显示图片
//x,y显示坐标
//sizex,sizey,图片长宽
//BMP：要显示的图片
void OLED_DrawBMP(u8 x,u8 y,u8 sizex, u8 sizey,const u8 BMP[])
{   
  u16 j=0;
  u8 i,m,temp;
  sizey=sizey/8+((sizey%8)?1:0);
  for(i=0;i<sizey;i++)
  {
    OLED_Set_Pos(x,i+y);
    for(m=0;m<sizex;m++)
    {
       temp=pgm_read_byte(&BMP[j++]);
       OLED_WR_Byte(temp,OLED_DATA);
    }
  }
} 

//OLED的初始化
void OLED_Init(void)
{ 
  I2C_Init(); //初始化I2C 管脚输出高电平
  
  OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
  OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
  OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
  OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
  OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
  OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
  OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
  OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
  OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
  OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
  OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
  OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset Shift Mapping RAM Counter (0x00~0x3F)
  OLED_WR_Byte(0x00,OLED_CMD);//-not offset
  OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
  OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
  OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
  OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
  OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
  OLED_WR_Byte(0x12,OLED_CMD);
  OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
  OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
  OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
  OLED_WR_Byte(0x02,OLED_CMD);//
  OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
  OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
  OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
  OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
  OLED_Clear();
  OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/ 
}
