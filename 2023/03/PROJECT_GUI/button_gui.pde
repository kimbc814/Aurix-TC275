
import processing.serial.*;
Serial port;

float CIR_SIZE = 60;
int[] input_cnt = {0,0,0,0,0,0,0,0};
int[] data_output = new int[10];
int sum=0;
int tempdata=0,adc_data1=0,adc_data2=0;
void setup(){
  size(1100,850);
  background(150,100,105);
  smooth();
  String portName ;
  println(Serial.list()); //Shows your connected serial ports
  portName = Serial.list()[0];
  port = new Serial(this,"COM13",115200); 
  mk_button();
 
  //Up there you should select port which arduino connected and same baud rate.
  port.bufferUntil('\n');
  
}

void draw(){
  fill(100,100,100);
  rect(100,30,220,70);
  textSize(50);
  fill(255);
  text(adc_data1+adc_data2,120,80);
  P_button();
  
}
int send_data(){
    
        sum=0;
   for(int i=0;i<8;i++){
    if(input_cnt[i]==1){
       sum = sum + (int(pow(2,i)));
    }
   }
    port.write(sum);
    return sum;
  }
void serialEvent(Serial port)
{
 byte[] in_buff = port.readBytesUntil('\n');  // 프로세싱을 사용하려 경우 TC275의 송신 프로토콜에 마지막에 '\n'을 반드시 추가해서 넣을 것
 int byte_length = 0;
 int[] input_io_data = new int[10];
 int i;
 
 if(in_buff != null)
  {
    byte_length = in_buff.length;
    print("Data length :  ");
    println(byte_length);
    print("in_buff : ");
      for(i=0;i<byte_length;i++)
      {
          print( (in_buff[i]+256)%256);  // byte의 범위가 -127+256이므로
          print("   ");
      }
   // println(hex(in_buff[0]));
   // println(in_buff[0]+256);
   // println(in_buff[1]+256);  
     println();
     
       if( ( (in_buff[0]+ 256) == 174) && ( (in_buff[1]+ 256) == 184)  &&  ( (in_buff[2] == 'I') ) )
        {
           println("anaylze");print("tempdata : ");
           println( (in_buff[4]+256)%256);
           tempdata=(in_buff[4]+256)%256;
           adc_data1=(change_data_type((in_buff[7]+256)%256));
           adc_data2=(in_buff[8]+256)%256;
        // java modbus crc는 복잡하므로 여기서는 생략함
        print("input_io_data : ");
           for(i=0;i<byte_length;i++)
           {
              input_io_data[i] = (int)((in_buff[4] >>i) & 0x01);  // input ioport data가 저장 됨
              print(input_io_data[i]);
              print("  ");
           }
              println(); //println(); println();      
        }
print("data_output : ");
          for(int k =0;k<byte_length;k++){
            data_output[k]=input_io_data[k];
            print(data_output[k]);
              print("  ");
          }
          println();
          print(" adc 7 :");
              println(adc_data1);
              print(" adc 8 :");
              println(adc_data2);
              println(sum);
    }
}
int change_data_type(int indata){
    int redata=257*indata;
    
    return redata;
}

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
    if(tempdata==sum){
      if(data_output[i]==0){
          fill(255);
        } else{
          fill(255,0,0);
        }
        ellipse(180+100*i,180,40,40);
    }
}
delay(50);
}
void mk_button(){
  
  background(100,170,100);
  fill(255);
  textSize(50);
  text("OUTPUT",800,100);
  text("INPUT",847,310);
  fill(100,120,150);
  rect(100,330,900,120);
  fill(100,100,200);
  rect(100,120,900,120);
}
