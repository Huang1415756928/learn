#include "Speaker.h"
#include "music_8bit.h"

SPEAKER Speaker;

void setup() {
  // put your setup code here, to run once:
   Speaker.begin();
   Speaker.setVolume(1);//音量1-10

}

void loop() {
  // put your main code here, to run repeatedly:
    Speaker.playMusic(data, 64000);  //后一位表示采样率
   Speaker.update();
   delay(1000);
}
