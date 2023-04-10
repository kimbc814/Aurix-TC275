#include <Wire.h>
#include <MsTimer2.h>
#include <avr/wdt.h>
// 자신의 슬레이브 주소를 설정해 줍니다.(슬레이브 주소에 맞게 수정해야 합니다.)
#define SLAVE 4  
#define inturrupt_time 45
//LED1~8번 PIN을 PORT NUMBER에 맞게 설정.
#define LED1 53  
#define LED2 52
#define LED3 51
#define LED4 50
#define LED5 49
#define LED6 48
#define LED7 47
#define LED8 46
//BOTTON1~8번 PIN을 PORT NUMBER에 맞게 설정.
#define BUTTON1 43
#define BUTTON2 41
#define BUTTON3 39
#define BUTTON4 37
#define BUTTON5 35
#define BUTTON6 33
#define BUTTON7 31
#define BUTTON8 29
#define PIN_WATCHDOG_OUT 25
#define PIN_WATCHDOG_IN 23
#define RING_BUFFER_SIZE 7
#define TEMP_BUFFER_SIZE 7

static char ring_buff[RING_BUFFER_SIZE]={0,};
static int R_data =0;

typedef unsigned char  BYTE_t;

union
{
  unsigned short check_sum;
  unsigned char b[2];
} crc_16_val,crc_16_val1;



// 카운터
byte count = 0; 
char temp;
char temp_data;
union{
short ADC_VALUE;  
byte adc[2];
} ADC_;

byte B_input_data;
bool B_pin_data[8];


byte SW_IN; //스위치값 비트로 나눠서 버튼이랑 비교 후 led점등.
bool SW_DATA[8];


byte LED_R; //스위치값 비트로 나눠서 버튼이랑 비교 후 led점등.
bool LED_DATA[8];

byte data_check[7];
byte S2M[9];
void setup() {
  pinMode(PIN_WATCHDOG_OUT,OUTPUT); 
  pinMode(PIN_WATCHDOG_IN,INPUT); 
  //LED 1~8번핀 OUTPUT으로 설정.
  pinMode(LED1,OUTPUT); 
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);
  pinMode(LED4,OUTPUT);
  pinMode(LED5,OUTPUT);
  pinMode(LED6,OUTPUT);
  pinMode(LED7,OUTPUT);
  pinMode(LED8,OUTPUT);
  //BUTTON 1~8번핀 INPUT으로 설정.
  pinMode(BUTTON1,INPUT);
  pinMode(BUTTON2,INPUT);
  pinMode(BUTTON3,INPUT);
  pinMode(BUTTON4,INPUT);
  pinMode(BUTTON5,INPUT);
  pinMode(BUTTON6,INPUT);
  pinMode(BUTTON7,INPUT);
  pinMode(BUTTON8,INPUT);
  // Wire 라이브러리 초기화
  // 슬레이브로 참여하기 위해서는 주소를 지정해야 한다.
  Wire.begin(SLAVE);
  Wire.onReceive(Ring_Buff);
  // 마스터의 데이터 전송 요구가 있을 때 처리할 함수 등록
  Wire.onRequest(sendToMaster);
  Serial.begin(115200);
  MsTimer2::set(2,Timer1);
  MsTimer2::set(13,TIMER_2);
  MsTimer2::start();
  Serial.println("start");
  wdt_disable();
  wdt_enable(WDTO_15MS);
  digitalWrite(PIN_WATCHDOG_OUT,HIGH);
}
void TIMER_2(){
  digitalWrite(PIN_WATCHDOG_OUT,!digitalread(PIN_WATCHDOG_OUT));
}
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

void Data_crc16() {
  for(int j=0;j<7;j++){
    data_check[j] = ring_buff[(R_data + RING_BUFFER_SIZE - 7+j) % RING_BUFFER_SIZE];
    }
  for(int i=0;i<7;i++){Serial.print(data_check[i]);Serial.print(" ");}Serial.println("");
  if(data_check[0]==174 && data_check[1]==184 && data_check[2]=='I' && data_check[3]==1){
    crc_16_val1.check_sum = CRC16_MODBUS(data_check, 5);
    if(crc_16_val1.b[0]==data_check[5] && crc_16_val1.b[1]==data_check[6]){
    }
  }
  SW_IN=data_check[4];
 }
 
void Ring_Buff(){
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
     Data_crc16();
}

void Timer1() //ADC 값을 읽어와서, 이진수로 변환한 후 8개의 LED로 출력하는 함수
{
    if(digitalRead(PIN_WATCHDOG_IN) == HIGH){wdt_reset();}
    ADC_.ADC_VALUE=analogRead(A0);
    //Serial.println(ADC_.ADC_VALUE);
    for(int i=0;i<8;i++){
    SW_DATA[i]=(SW_IN >> i)& 0x01;
  }
  Turn_ON_LED();
    LED_R=0;
    for(int i=0;i<8;i++){
      LED_R=LED_R|(LED_DATA[i]<<i);
    }    
    B_pin_data[1]=digitalRead(BUTTON1);
    B_pin_data[2]=digitalRead(BUTTON2);
    B_pin_data[3]=digitalRead(BUTTON3);
    B_pin_data[4]=digitalRead(BUTTON4);
    B_pin_data[5]=digitalRead(BUTTON5);
    B_pin_data[6]=digitalRead(BUTTON6);
    B_pin_data[7]=digitalRead(BUTTON7);
    B_pin_data[0]=digitalRead(BUTTON8);
  }
void loop () {
}
void sendToMaster(){
  S2M[0]=164;
  S2M[1]=174;
  S2M[2]='O';
  S2M[3]=3;
  S2M[4]=LED_R;
  S2M[5]=ADC_.adc[0];
  S2M[6]=ADC_.adc[1];
  crc_16_val.check_sum = CRC16_MODBUS(S2M, 7);
  S2M[7]=crc_16_val.b[0];
  S2M[8]=crc_16_val.b[1];
  for(int i=0; i<9; i++){
    Wire.write(S2M[i]);
    }
  }

void Turn_ON_LED() //LED 구동 함수
{
  if(SW_DATA[0]== 1){digitalWrite(LED1, HIGH);} else{digitalWrite(LED1, LOW);}
  if(SW_DATA[1]== 1){digitalWrite(LED2, HIGH);} else{digitalWrite(LED2, LOW);}
  if(SW_DATA[2]== 1){digitalWrite(LED3, HIGH);} else{digitalWrite(LED3, LOW);}
  if(SW_DATA[3]== 1){digitalWrite(LED4, HIGH);} else{digitalWrite(LED4, LOW);}
  if(SW_DATA[4]== 1){digitalWrite(LED5, HIGH);} else{digitalWrite(LED5, LOW);}
  if(SW_DATA[5]== 1){digitalWrite(LED6, HIGH);} else{digitalWrite(LED6, LOW);}
  if(SW_DATA[6]== 1){digitalWrite(LED7, HIGH);} else{digitalWrite(LED7, LOW);}
  if(SW_DATA[7]== 1){digitalWrite(LED8, HIGH);} else{digitalWrite(LED8, LOW);}
  if(B_pin_data[0]==0){LED_DATA[0]=1;} else{LED_DATA[0]=0;}
  if(B_pin_data[1]==0){LED_DATA[1]=1;} else{LED_DATA[1]=0;}
  if(B_pin_data[2]==0){LED_DATA[2]=1;} else{LED_DATA[2]=0;}
  if(B_pin_data[3]==0){LED_DATA[3]=1;} else{LED_DATA[3]=0;}
  if(B_pin_data[4]==0){LED_DATA[4]=1;} else{LED_DATA[4]=0;}
  if(B_pin_data[5]==0){LED_DATA[5]=1;} else{LED_DATA[5]=0;}
  if(B_pin_data[6]==0){LED_DATA[6]=1;} else{LED_DATA[6]=0;}
  if(B_pin_data[7]==0){LED_DATA[7]=1;} else{LED_DATA[7]=0;}
  
}
