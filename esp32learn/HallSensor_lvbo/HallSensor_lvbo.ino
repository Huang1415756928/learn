//Simple sketch to access the internal hall effect detector on the esp32.
//values can be quite low. 
//Brian Degger / @sctv  
int val = 0;
int hall1[5];
int hall[10];
int i;//比较的次数
int j;//每轮比较的次数
int k;
int buf;//
void setup() {
  Serial.begin(115200);
    }

void loop() {
  // put your main code here, to run repeatedly:
for(i=0;i<10;i++)//获取5次霍尔传感器的值
hall[i] = hallRead();//将5个值依次放到数组中
for(k=0;k<5;k++)
{
      for(i=0;i<9;i++)
      {
          for(j=0;j<9-i;j++)
            {
              if(hall[j]<hall[j+1])
                {
                 buf=hall[j];
                 hall[j]=hall[j+1];
                 hall[j+1]=buf;
                }            
            }
      }
 hall1[k]=(hall[3]+hall[4]+hall[5]+hall[6])/4;  
}
//for(i=0;i<10;i++)
//Serial.println(hall[i]);
//delay(100);
val=(hall1[0]+hall1[1]+hall1[2]+hall1[3]+hall1[4])/5; 
//val=(hall[1]+hall[2]+hall[3]+hall[4]+hall[5]+hall[6]+hall[7]+hall[8]+hall[9]+hall[10]+hall[11]+hall[12]+hall[13]+hall[14]+hall[15]+hall[16]+hall[17]+hall[18])/18;       

Serial.println(val);//to graph 
//delay(1);
}
