#include "Arduino.h" 
#define dustPin  35//定义 adc 采集引脚
#define ledPower 2 //定义 led 发射引脚
int value;
float dustVal=0;
int delayTime=280;
int delayTime2=40;
float offTime=9680;
void setup() {
// put your setup code here, to run once:
pinMode(ledPower,OUTPUT);
adcAttachPin(dustPin);
adcStart(dustPin);
Serial.begin(115200);
}
void loop() {
// put your main code here, to run repeatedly:
digitalWrite(ledPower,LOW);
delayMicroseconds(delayTime);
dustVal=analogRead(dustPin);
// Serial.write(value>>8); Serial.write(value);
delayMicroseconds(delayTime2);
digitalWrite(ledPower,HIGH);
delayMicroseconds(offTime);
delay(100);
Serial.println(analogRead(dustPin));

}
