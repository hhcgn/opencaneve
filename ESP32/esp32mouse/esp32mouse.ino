#include <BleMouse.h>
#include <WiFi.h>

const int led=2;
#define LED_ON LOW
#define LED_OFF HIGH
char flag=0,type_s=0;;
BleMouse bleMouse("ESP32蓝牙鼠标","Espressif",100);//其中“ESP32蓝牙键盘”为键盘名称；"Espressif"为制造商

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  pinMode(led,OUTPUT);
  pinMode(34,INPUT_PULLUP);
  pinMode(35,INPUT_PULLUP);
  pinMode(32,INPUT_PULLUP);
  pinMode(33,INPUT_PULLUP);
  pinMode(25,INPUT_PULLUP);
  pinMode(26,INPUT_PULLUP);
  bleMouse.begin();
}

void loop() {
  if(bleMouse.isConnected()) {
    digitalWrite(led,LED_ON);
  }else{
    digitalWrite(led,LED_OFF);
    
    }
    type_s=0;
    if(digitalRead(35)==LOW){
    Serial.println("右");//r
    type_s='r';
    }
    else if(digitalRead(34)==LOW){
    Serial.println("左");//l
    type_s='l';
    }
    else if(digitalRead(32)==LOW){
    Serial.println("切换");//切换
    flag=flag==0? 1:0;
    delay(150);
    if(digitalRead(32)==LOW){
     flag=flag==0? 1:0;
    bleMouse.click(MOUSE_BACK);
    delay(500);
    }
    }
    else if(digitalRead(33)==LOW){
    Serial.println("点击");//click
    type_s='c';
    }
    else if(digitalRead(25)==LOW){
    Serial.println("上");//up
    type_s='u';
    }
    else if(digitalRead(26)==LOW){
    Serial.println("下");//down
    type_s='d';
    }
    
    if(flag==0&&type_s!=0){
      move(type_s);
      }
     else if(flag==1&&type_s!=0){
      click(type_s);
      }
      else{
        delay(50);
        }
        delay(10);
      
      
     
}
void click(char t)
{
  unsigned long startTime;
  int c=0;
    if(t=='l'||t=='c'){
    bleMouse.click(MOUSE_LEFT);
      delay(100);
    }
    else if(t=='r'){
    bleMouse.click(MOUSE_RIGHT);
    delay(100);
    }
//    else if(t=='u')
//    bleMouse.click(MOUSE_FORWARD);
//    else if(t=='d')
//    bleMouse.click(MOUSE_BACK);
      if(t=='u'||t=='d'){
      if(t=='u') c=1;
      else c=-1;
      startTime = millis();
    while(millis()<startTime+30) {
      bleMouse.move(0,0,c);
      delay(20);
    }
      }
    
}
void move(char t)
{
   int a=0,b=0,c=0;
   unsigned long startTime;
   if(t=='c'){
    bleMouse.click(MOUSE_LEFT);
    delay(100);
   }
   if(t=='u') {a=0;b=-1;}
   else if(t=='d'){a=0;b=1;}
   else if(t=='l'){a=-1;b=0;}
   else if(t=='r'){a=1;b=0;}
   startTime = millis();
    while(millis()<startTime+60) {
      bleMouse.move(a,b);
      delay(3);
    }
  
    
}
