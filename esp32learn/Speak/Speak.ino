#include "Speaker.h"
#include "music_8bit.h"

SPEAKER Speaker;

void setup() {
  // put your setup code here, to run once:
   Speaker.begin();
   Speaker.setVolume(1);//����1-10

}

void loop() {
  // put your main code here, to run repeatedly:
    Speaker.playMusic(data, 64000);  //��һλ��ʾ������
   Speaker.update();
   delay(1000);
}
