#include <Wire.h>
#include <MsTimer2.h>
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


char r_tempdata[10];
void setup() {
  pinMode(inturrupt_time,OUTPUT);
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
  Wire.onReceive(receiveFromMaster);
  // 마스터의 데이터 전송 요구가 있을 때 처리할 함수 등록
  Wire.onRequest(sendToMaster);
  Serial.begin(115200);
  MsTimer2::set(10,MK_ADC_BT);
  MsTimer2::start();
}

void MK_ADC_BT() //ADC 값을 읽어와서, 이진수로 변환한 후 8개의 LED로 출력하는 함수
{
  digitalWrite(inturrupt_time, HIGH);
  Turn_ON_LED();
    ADC_.ADC_VALUE=analogRead(A0);
    //Serial.println(ADC_.ADC_VALUE);
    LED_R=0;
    for(int i=0;i<8;i++)

    {
      LED_R=LED_R|(LED_DATA[i]<<i);
    }    
/*  SW_DATA[0]=1;
  SW_DATA[1]=1;
  SW_DATA[2]=1;
  SW_DATA[3]=1;
  SW_DATA[4]=1;
  SW_DATA[5]=1;
  SW_DATA[6]=1;
  SW_DATA[7]=1;*/  
    /*for(int i=0;i<8;i++){
      LED_DATA[i]=1;
      LED_R=LED_R|(LED_DATA[i]<<i);
  Serial.print(LED_DATA[i]);
  }
  Serial.println();
     Serial.println(LED_R);
    Serial.println(ADC_.ADC_VALUE);
    Serial.println(ADC_.adc[1]);
    Serial.println(ADC_.adc[0]);
    
  Serial.println(ADC_.ADC_VALUE);*/
    /*Serial.print(digitalRead(BUTTON1));
    Serial.print(digitalRead(BUTTON2));
    Serial.print(digitalRead(BUTTON3));
    Serial.print(digitalRead(BUTTON4));
    Serial.print(digitalRead(BUTTON5));
    Serial.print(digitalRead(BUTTON6));
    Serial.print(digitalRead(BUTTON7));
    Serial.print(digitalRead(BUTTON8));*/
    B_pin_data[1]=digitalRead(BUTTON1);
    B_pin_data[2]=digitalRead(BUTTON2);
    B_pin_data[3]=digitalRead(BUTTON3);
    B_pin_data[4]=digitalRead(BUTTON4);
    B_pin_data[5]=digitalRead(BUTTON5);
    B_pin_data[6]=digitalRead(BUTTON6);
    B_pin_data[7]=digitalRead(BUTTON7);
    B_pin_data[0]=digitalRead(BUTTON8);
    /*
    if(digitalRead(BUTTON1==HIGH)){B_pin_data[0]=1;}else{B_pin_data[0]=0;}
    if(digitalRead(BUTTON2==HIGH)){B_pin_data[1]=1;}else{B_pin_data[1]=0;}
    if(digitalRead(BUTTON3==HIGH)){B_pin_data[2]=1;}else{B_pin_data[2]=0;}
    if(digitalRead(BUTTON4==HIGH)){B_pin_data[3]=1;}else{B_pin_data[3]=0;}
    if(digitalRead(BUTTON5==HIGH)){B_pin_data[4]=1;}else{B_pin_data[4]=0;}
    if(digitalRead(BUTTON6==HIGH)){B_pin_data[5]=1;}else{B_pin_data[5]=0;}
    if(digitalRead(BUTTON7==HIGH)){B_pin_data[6]=1;}else{B_pin_data[6]=0;}
    if(digitalRead(BUTTON8==HIGH)){B_pin_data[7]=1;}else{B_pin_data[7]=0;}*/
    /*Serial.println();
  for(int i=0;i<8;i++){
  Serial.print(B_pin_data[i]);
  }
  Serial.println();*/
  digitalWrite(inturrupt_time, LOW);
  }

void SWITCH_BUTTON(){
       Wire.beginTransmission(1);
      // 슬레이브 주소를 시리얼 창에 입력시 해당 시리얼 주소로 'O'문자와 데이터 요구를 보냅니다.
      Wire.write('S');
      Wire.write('\n');
      Wire.endTransmission(1);
      Wire.requestFrom(1,1);
      SW_IN=Wire.read();
      for(int i=0;i<8;i++){
        SW_DATA[i]=(SW_IN >> i)& 0x01;
        }
      delay(10);
}

void loop () {
  SWITCH_BUTTON();
  // 요청이 들어오면 13번 LED를 0.5초간 켜줍니다.
  if (temp == 'A' || temp == 'L' ) { 
    play();
  }
  
}

void receiveFromMaster(int bytes) {
  char ch[2];
  for (int i = 0 ; i < bytes ; i++) {
    // 수신 버퍼 읽기
    ch[i] = Wire.read(); 
  }
  temp = ch[0];
}

void play() { 
  digitalWrite(13, HIGH);
  delay(300);
  digitalWrite(13, LOW);
  
}

void sendToMaster() {
  // 자신의 슬레이브 주소를 담은 메세지를 마스터에게 보냅니다. 슬레이브 주소에 맞게 수정해야 합니다.
   //Wire.write(ADC_.ADC_VALUE);
  if(temp == 'A'){
    //ADC_.ADC_VALUE=analogRead(A0);
    //Wire.write(ADC_.ADC_VALUE);
    
    Wire.write(ADC_.adc[0]);
    Wire.write(ADC_.adc[1]);
    
    }
  else if(temp == 'L'){
    /*LED_R=0;
    for(int i=0;i<8;i++){
      LED_R=LED_R|(LED_DATA[i]<<i);*/
  }
    Wire.write(LED_R);
    }
    //temp = 0;
   

void Turn_ON_LED() //LED 구동 함수
{
  if(SW_DATA[0]== 1&&B_pin_data[0]==1){LED_DATA[0]=1;digitalWrite(LED1, HIGH);} else{LED_DATA[0]=0;digitalWrite(LED1, LOW);}
  if(SW_DATA[1]== 1&&B_pin_data[1]==1){LED_DATA[1]=1;digitalWrite(LED2, HIGH);} else{LED_DATA[1]=0;digitalWrite(LED2, LOW);}
  if(SW_DATA[2]== 1&&B_pin_data[2]==1){LED_DATA[2]=1;digitalWrite(LED3, HIGH);} else{LED_DATA[2]=0;digitalWrite(LED3, LOW);}
  if(SW_DATA[3]== 1&&B_pin_data[3]==1){LED_DATA[3]=1;digitalWrite(LED4, HIGH);} else{LED_DATA[3]=0;digitalWrite(LED4, LOW);}
  if(SW_DATA[4]== 1&&B_pin_data[4]==1){LED_DATA[4]=1;digitalWrite(LED5, HIGH);} else{LED_DATA[4]=0;digitalWrite(LED5, LOW);}
  if(SW_DATA[5]== 1&&B_pin_data[5]==1){LED_DATA[5]=1;digitalWrite(LED6, HIGH);} else{LED_DATA[5]=0;digitalWrite(LED6, LOW);}
  if(SW_DATA[6]== 1&&B_pin_data[6]==1){LED_DATA[6]=1;digitalWrite(LED7, HIGH);} else{LED_DATA[6]=0;digitalWrite(LED7, LOW);}
  if(SW_DATA[7]== 1&&B_pin_data[7]==1){LED_DATA[7]=1;digitalWrite(LED8, HIGH);} else{LED_DATA[7]=0;digitalWrite(LED8, LOW);}
}
