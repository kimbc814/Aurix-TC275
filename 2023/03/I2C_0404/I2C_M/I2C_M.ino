#include <Wire.h>
#include <MsTimer2.h>
#include <avr/wdt.h>
////////////////////////////////////////////////////////////////modbus16crc
#define SIZE_MAX_SEND_BUFFER  32
#define PID_PC   184
#define PID_IPD  174
typedef unsigned char  BYTE_t;


union
{
  unsigned short check_sum;
  unsigned char b[2];
} crc_16_val;



BYTE_t protocal_test[SIZE_MAX_SEND_BUFFER] = { 0, };

int CRC16_MODBUS(const uint8_t *nData, uint16_t wLength)
{

  static const uint16_t wCRCTable[] = { 0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241, 0XC601, 0X06C0, 0X0780,
    0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440, 0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1,
    0XCE81, 0X0E40, 0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841, 0XD801,
    0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40, 0X1E00, 0XDEC1, 0XDF81, 0X1F40,
    0XDD01, 0X1DC0, 0X1C80, 0XDC41, 0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680,
    0XD641, 0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040, 0XF001, 0X30C0,
    0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240, 0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501,
    0X35C0, 0X3480, 0XF441, 0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
    0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840, 0X2800, 0XE8C1, 0XE981,
    0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41, 0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1,
    0XEC81, 0X2C40, 0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640, 0X2200,
    0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041, 0XA001, 0X60C0, 0X6180, 0XA141,
    0X6300, 0XA3C1, 0XA281, 0X6240, 0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480,
    0XA441, 0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41, 0XAA01, 0X6AC0,
    0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840, 0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01,
    0X7BC0, 0X7A80, 0XBA41, 0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
    0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640, 0X7200, 0XB2C1, 0XB381,
    0X7340, 0XB101, 0X71C0, 0X7080, 0XB041, 0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0,
    0X5280, 0X9241, 0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440, 0X9C01,
    0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40, 0X5A00, 0X9AC1, 0X9B81, 0X5B40,
    0X9901, 0X59C0, 0X5880, 0X9841, 0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81,
    0X4A40, 0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41, 0X4400, 0X84C1,
    0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641, 0X8201, 0X42C0, 0X4380, 0X8341, 0X4100,
    0X81C1, 0X8081, 0X4040 };

  uint8_t nTemp;
  uint16_t wCRCWord = 0xFFFF;

  while (wLength--)
  {
    nTemp = *nData++ ^ wCRCWord;
    wCRCWord >>= 8;
    wCRCWord ^= wCRCTable[nTemp];
  }

  return wCRCWord;
}
////////////////////////////////////////////////////////////////MsTimer2.h

////////////////////////////////////////////////////////////////
// 슬레이브 주소

//ring_buff 
#define RING_BUFFER_SIZE 9
#define TEMP_BUFFER_SIZE 9
static char ring_buff[RING_BUFFER_SIZE]={0,};
static int R_data =0;
//i2c send arr
byte sendi2c_data[7]={0,};
//check crc16
byte data_check[8]={0,};
//
byte adc11,adc22,LED_OUT,button1;
byte SW_IN ;
int flag;

#define PIN_WATCHDOG_OUT 25
#define PIN_WATCHDOG_IN 23
void setup() {
  pinMode(PIN_WATCHDOG_OUT,OUTPUT); 
  pinMode(29,OUTPUT);
  pinMode(27,OUTPUT);
  pinMode(PIN_WATCHDOG_IN,INPUT); 
  //mstimer시작
  MsTimer2::set(2,TIMER_1);
  MsTimer2::start();
  // Wire 라이브러리 초기화
  Wire.begin(); 
  // 직렬 통신 초기화
  Serial1.begin(115200);
  Serial.begin(115200); 
  Serial.println("start");
  wdt_disable();
  wdt_enable(WDTO_15MS);
  digitalWrite(PIN_WATCHDOG_OUT,HIGH);
}

////////////////////////////////////////////////////////////////
void send2pc_data(void)
{
  int i = 0;
  protocal_test[0] = 174;                               // IPD_controller
  protocal_test[1] = 184;                               // PC
  protocal_test[2] = 'I';                               // PID
  protocal_test[3] = 1;                                 // data number
  protocal_test[4] = LED_OUT;
  protocal_test[5] = adc11;
  protocal_test[6] = adc22;
////////////////////////////////////////////////////////////////
  crc_16_val.check_sum = CRC16_MODBUS(protocal_test, 7);
  // Modbus crc16
  protocal_test[7] = crc_16_val.b[1];
  protocal_test[8] = crc_16_val.b[0];

  for (i = 0; i < 9; i++)  
  {
     Serial1.write(protocal_test[i]);    
  }
     Serial1.write('\n');
}
////////////////////////////////////////////////////////////////
int watchdog_temp;
void TIMER_1(){
  if(digitalRead(PIN_WATCHDOG_IN) == HIGH){wdt_reset();}
  if (Serial1.available()) {
    SW_IN = Serial1.read();
  }
  send2pc_data();
  if(flag==2){flag=3;}else{flag=1;}
}
void send_data_I2C(){//I2C 데이터를 서버로 보내는 함수
  sendi2c_data[0]=174;
  sendi2c_data[1]=184;
  sendi2c_data[2]='I';
  sendi2c_data[3]=1;
  sendi2c_data[4]=SW_IN;
  crc_16_val.check_sum = CRC16_MODBUS(sendi2c_data, 5);
  sendi2c_data[5]=crc_16_val.b[0];
  sendi2c_data[6]=crc_16_val.b[1];
  Wire.beginTransmission(4);
  for(int i=0; i<7; i++){
    Wire.write(sendi2c_data[i]);
  }
  Wire.endTransmission(4);
  }
void Ring_Buff(){
  Wire.requestFrom(4,9);
  char read_temp[TEMP_BUFFER_SIZE];
    byte leng = Wire.readBytes(read_temp, TEMP_BUFFER_SIZE);
    if(leng <=RING_BUFFER_SIZE){
      for(int i=0; i<RING_BUFFER_SIZE; i++){
      ring_buff[i]=0;
      }
      for(int i=0; i<leng; i++){
        ring_buff[R_data]=read_temp[i];
        R_data++;
      if(R_data >= RING_BUFFER_SIZE){ R_data= 0;}
      }
    }
    else{
       for(int i=0; i<RING_BUFFER_SIZE; i++){
          ring_buff[i]=0;
       }
       R_data=0;
     }
}
void Data_crc16(){
  for(int j=0;j<9;j++){
    data_check[j] = ring_buff[(R_data + RING_BUFFER_SIZE - 9+j) % RING_BUFFER_SIZE];
  }
  if(data_check[0]==164 && data_check[1]==174 && data_check[2]=='O' && data_check[3]==3){
    crc_16_val.check_sum = CRC16_MODBUS(data_check, 7);
    if(crc_16_val.b[0]==data_check[7] && crc_16_val.b[1]==data_check[8]){
     LED_OUT=data_check[4];
     adc11=data_check[5];
     adc22=data_check[6];
    }
  }
}

void loop() {
  Serial.println(SW_IN);
  if(flag==1){
    digitalWrite(29, HIGH);
    flag=2;
    send_data_I2C();
    digitalWrite(29, LOW);
    }
  else if(flag==3){
    digitalWrite(27, HIGH);
    flag=0;
    Ring_Buff();
    Data_crc16();
    digitalWrite(27, LOW);
    }
  }
