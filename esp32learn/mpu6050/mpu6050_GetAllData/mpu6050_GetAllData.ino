
#include <MPU6050_tockn.h>
#include <Wire.h>
#include <SSD1306Wire.h>
#include "Arduino.h" 

SSD1306Wire display(0x3c, 4, 5);
MPU6050 mpu6050(Wire);


#define dustPin  35//定义 adc 采集引脚
#define ledPower 2 //定义 led 发射引脚

int counter = 1;
long timer = 0;



void setup() {
  Serial.begin(115200);
  pinMode(ledPower, OUTPUT);
  adcAttachPin(dustPin);
  adcStart(dustPin);
  Wire.begin();
  Wire.begin(4, 5, 1000);//SDA SDL for esp32
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_16);
  display.clear();
  display.drawString(12, 1, "Do not move");
  display.drawString(3, 16, "the MPU6050!!! ");
  display.display();
  
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  display.clear();
  display.drawString(15, 1, "All done ^_^");
  display.drawString(16, 16, "welcome ...");
  display.display();
  delay(1000);
  display.setFont(ArialMT_Plain_10);
}

void loop() {
  digitalWrite(ledPower, 1);//指示灯亮表示正在工作
  mpu6050.update();

  if(millis() - timer > 10){
    
    //Serial.println("=======================================================");
   // Serial.print("temp : ");Serial.println(mpu6050.getTemp());
   // Serial.print("accX : ");Serial.print(mpu6050.getAccX());
   // Serial.print("\taccY : ");Serial.print(mpu6050.getAccY());
   // Serial.print("\taccZ : ");Serial.println(mpu6050.getAccZ());
  
   // Serial.print("gyroX : ");Serial.print(mpu6050.getGyroX());
   // Serial.print("\tgyroY : ");Serial.print(mpu6050.getGyroY());
   // Serial.print("\tgyroZ : ");Serial.println(mpu6050.getGyroZ());
  
   // Serial.print("accAngleX : ");Serial.print(mpu6050.getAccAngleX());
   // Serial.print("\taccAngleY : ");Serial.println(mpu6050.getAccAngleY());
  
   // Serial.print("gyroAngleX : ");Serial.print(mpu6050.getGyroAngleX());
    //Serial.print("\tgyroAngleY : ");Serial.print(mpu6050.getGyroAngleY());
    //Serial.print("\tgyroAngleZ : ");Serial.println(mpu6050.getGyroAngleZ());
    
    Serial.print("angleX : ");Serial.print(mpu6050.getAngleX());
    Serial.print("\tangleY : ");Serial.print(mpu6050.getAngleY());
    Serial.print("\tangleZ : ");Serial.println(mpu6050.getAngleZ());
    //Serial.println("=======================================================\n");

    display.clear();
    /*
    display.drawString(0, 1, "Ang:");
    display.drawString(25, 1, String(mpu6050.getAngleX()));
    display.drawString(55, 1, String(mpu6050.getAngleY()));
    display.drawString(85, 1, String(mpu6050.getAngleZ()));

    display.drawString(0, 11, "Acc:");
    display.drawString(25, 11, String(mpu6050.getAccX()));
    display.drawString(55, 11, String(mpu6050.getAccY()));
    display.drawString(85, 11, String(mpu6050.getAccZ()));

    display.drawString(0, 21, "Gra:");
    display.drawString(25, 21, String(mpu6050.getGyroAngleX()));
    display.drawString(55, 21, String(mpu6050.getGyroAngleY()));
    display.drawString(85, 21, String(mpu6050.getGyroAngleZ()));

    */
    display.setFont(ArialMT_Plain_16);
    display.drawString(25, 0, "battery:");
    float v;
    float battery;
    v = (analogRead(dustPin) / 10 * 10)/561.0;
    battery = ((v * v) * (-350.52) +v * (2958.77) - (6143.14));//拟合的电量与电压关系曲线
    if(battery>=100)
    display.drawString(30, 15, String(int(battery)/5*5)+"%");//电量百分比显示
    else if(battery>10&&battery<100)
    display.drawString(30, 15, String(int(battery) /5 * 5) + "%");//电量百分比显示
    else if (battery <= 10)
    {
        //display.setFont(ArialMT_Plain_10);
        display.drawString(30, 15, String(int(battery) / 5 * 5) + "%");//电量百分比显示
        //display.drawString(1, 15, "Low power. please charge！！！");//电量百分比显示
    }
    display.display();
    timer = millis();
    
  }

}


