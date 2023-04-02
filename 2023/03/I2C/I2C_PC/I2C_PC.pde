
import processing.serial.*;
Serial port;

float CIR_SIZE = 60;
int[] input_cnt = {0,0,0,0,0,0,0,0};
int[] data_output = new int[8];

int send_data1=0;
int tempdata=0,adc_data1=0,adc_data2=0;
void setup(){
  size(1100,850);
  background(150,100,105);
  smooth();
  String portName ;
  println(Serial.list()); //Shows your connected serial ports
  portName = Serial.list()[0];
  port = new Serial(this,"COM25",115200); 
  mk_button();
 
  //Up there you should select port which arduino connected and same baud rate.
  port.bufferUntil('\n');
  
}

void draw(){
  fill(100,100,100);
  rect(100,45,220,70);
  textSize(50);
  fill(255);
  text((256*adc_data1)+adc_data2,120,95);
  P_button();
}
int send_data(){
   send_data1=0;                      //이전 송신 데이터 초기화.
   for(char i=0;i<8;i++){       //입력 배열 갯수만큼 반복.
     if(input_cnt[i]==1){      //입력된 값이 있는 상태 판별.
       send_data1 = send_data1 | (1<<i);     //1 입력 후 i 값만큼 비트 이동 후 이전 값과 OR 연산.
     }
   }
   port.write(send_data1);            //데이터 PC->Aurix 송신
   //println(send_data1);
   return send_data1;
  }
void serialEvent(Serial port){
 byte[] in_buff = port.readBytesUntil('\n'); 
 int byte_length = 0; int[] input_io_data = new int[10]; int i;
 if(in_buff != null){
    byte_length = in_buff.length;
    print("Data length :  "); println(byte_length);
    print("in_buff : ");
      for(i=0;i<byte_length;i++){
          print( (in_buff[i]+256)%256); print("   ");
      }println();
       if( ( (in_buff[0]+ 256) == 174) && ( (in_buff[1]+ 256) == 184)  &&  ( (in_buff[2] == 'I') ) ){
           println("anaylze");print("tempdata : ");  println( (in_buff[4]+256)%256);
           tempdata=(in_buff[4]+256)%256;
           adc_data1=((in_buff[6]+256)%256);
           adc_data2=(in_buff[5]+256)%256;
        print("input_io_data : ");
           for(i=0;i<byte_length;i++){
              input_io_data[i] = (int)((in_buff[4] >>i) & 0x01);  // input ioport data가 저장 됨
              print(input_io_data[i]); print("  ");
           }println();}
    print("data_output : ");
          for(int k =0;k<8;k++){
            data_output[k]=input_io_data[k];
            print(data_output[k]);
              print("  ");
          }println();
          print(" adc 7 :"); println(adc_data1);
          print(" adc 8 :");println(adc_data2); println((int)send_data1);
}}


void P_button(){
  for(int i=0; i<8; i++){
   fill(0);
   ellipse(170+100*i,390,40,40);
   ellipse(190+100*i,390,40,40);
   rect(170+100*i,370,20,40);
   
   if(input_cnt[i]==0){
      fill(255);
      ellipse(170+100*i,390,40,40);
    } else{
      fill(255);
      ellipse(190+100*i,390,40,40);
    }
   if(mousePressed){
     if(input_cnt[i]==0){
        if(pow((mouseX-170-100*i),2)+pow((mouseY-390),2)<=400){
            input_cnt[i]=1;       
        }
      } 
     else{
       if(pow((mouseX-190-100*i),2)+pow((mouseY-390),2)<=400){
         input_cnt[i]=0;          
       }
     }
    }
    send_data();
      if(data_output[i]==0){              //수신받은 데이터가 1일 때
          fill(255);
        } else{
          fill(255,0,0);
        }
        ellipse(180+100*i,180,40,40);
}
delay(50);
}
void mk_button(){
  
  background(100,170,100);
  fill(255);
  textSize(30);
  text("ADC_VALUE",105,35);
  textSize(50);
  text("OUTPUT",800,100);
  text("INPUT",847,310);
  fill(100,120,150);
  rect(100,330,900,120);
  fill(100,100,200);
  rect(100,120,900,120);
}
