#include <Wire.h>
#include <MsTimer2.h>

#define SLAVE 2
#define start_chack   4

int data_1 = 0;
int data_1_slave = 4;
int data_2 = 0;
int data_2_slave = 8;
int data_3 = 0;
int data_3_slave = 16;
int data_4 = 0;
int data_4_slave = 32;

int flag = 0;

void onTimer()
{
   digitalWrite(27, HIGH);
  flag = 1;
   digitalWrite(27, LOW);
 }

void setup()
{
  
  pinMode(29,OUTPUT);
  pinMode(27,OUTPUT);
  pinMode(start_chack, OUTPUT);
  Serial.begin(115200);
  Wire.begin();
  MsTimer2::set(2, onTimer);   //1.25 // 이걸 1
  MsTimer2::start();
}

void loop()
{
  if(flag == 1)
  {
   digitalWrite(27, HIGH);
   Wire.requestFrom(SLAVE,4); 
  data_1 = Wire.read();
  data_2 = Wire.read();
  data_3 = Wire.read();
  data_4 = Wire.read();
  digitalWrite(27, LOW);
  digitalWrite(29, HIGH);
   Wire.beginTransmission(SLAVE);
  Wire.write(data_1_slave);
  Wire.write(data_2_slave);
 Wire.write(data_3_slave);
  Wire.write(data_4_slave);
  Wire.endTransmission();
  digitalWrite(29, LOW);
  flag = 0;
  }
}
