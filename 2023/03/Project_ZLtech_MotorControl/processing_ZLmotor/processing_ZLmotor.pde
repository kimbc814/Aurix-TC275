import processing.serial.*;
Serial port; 




void setup() 
{
  size(700,350);
  
  smooth();
  String portName ;
  println(Serial.list());
  port = new Serial(this,"COM12",115200); 
  port.bufferUntil('\n');
}
char[] Data_pkg={184,174,'I',0,0,0,0,0};
int Mod=0;
int Velocity=0;
int Velocity_mouseX=52;
int Velocity_front_reverse=1;
int Position =0;
int keyPressed_error=0;
String Position_str="";


void keyPressed(){
  if(Mod==2){
    if(key==BACKSPACE){Position_str="";keyPressed_error=0;}
    else if(key==ENTER){
      try{Position=Integer.parseInt(Position_str);}
      catch(NumberFormatException e){keyPressed_error=1;}
    }
    else Position_str+=key;
    
  }
}
void motor_stop(){
  Data_pkg[3]=5;
  Data_pkg[4]=0;
  Data_pkg[5]=0;
  Data_pkg[6]=0;
  Data_pkg[7]=0;

  for(int i=0;i<8;i++){
    port.write(Data_pkg[i]);
  }
  delay(10);
}
void send_Velocity(){
  if(Velocity_front_reverse==1){
    Data_pkg[3]=1;
  }
  else{
    Data_pkg[3]=2;
  }
  Data_pkg[4]=(char)(Velocity>>8);
  Data_pkg[5]=(char)(Velocity%256);
  Data_pkg[6]=0;
  Data_pkg[7]=0;
  for(int i=0;i<8;i++){
    port.write(Data_pkg[i]);
  }
  delay(10);
}
void send_Position(){
  if(Position>=0){
    Data_pkg[3]=3;
    Data_pkg[4]=(char)(Position>>8);
    Data_pkg[5]=(char)(Position%256);
  }
  else{
    Data_pkg[3]=4;
    Data_pkg[4]=(char)((Position*-1)>>8);
    Data_pkg[5]=(char)((Position*-1)%256);
  }
  Data_pkg[6]=0;
  Data_pkg[7]=0;
  for(int i=0;i<8;i++){
    port.write(Data_pkg[i]);
  }
  delay(10);
}

void draw(){
  // mod
  background(200);
  fill(255);
  rect(40,80,600,200);
  switch(Mod){
    case 0:
      fill(255);
      rect(40,40,70,40);//mod
      rect(42,42,66,36);//now
      rect(110,40,85,40);//Velocity
      rect(195,40,85,40);//Position
      rect(280,40,60,40);//Stop
      textSize(20);
      fill(0);
      text("MODE",50,67);
      text("Velocity",120,67);
      text("Position",205,67);
      text("Stop",290,67);
      if((mouseX)>=40 && (mouseX)<=40+70 && (mouseY)>=40 && (mouseY)<=40+40 && (mousePressed)){Mod=0;print("Mod : ");println(Mod);delay(50);}
      else if((mouseX)>=110 && (mouseX)<=110+85 && (mouseY)>=40 && (mouseY)<=40+40 && (mousePressed)){Mod=1;Velocity_front_reverse=1;Velocity_mouseX=52;print("Mod : ");println(Mod);delay(50);}
      else if((mouseX)>=195 && (mouseX)<=195+85 && (mouseY)>=40 && (mouseY)<=40+40 && (mousePressed)){Mod=2;Position_str="";Position=0;print("Mod : ");println(Mod);delay(50);}
      else if((mouseX)>=280 && (mouseX)<=280+60 && (mouseY)>=40 && (mouseY)<=40+40 && (mousePressed)){motor_stop();Mod=0;print("Mod : ");println(Mod);delay(50);}
      textSize(35);
      text("Press button Velocity or Position.",50,157);
      text("If you want stop, press stop button.",50,217);
      break;
    case 1://Velocity
      
      fill(255);
      rect(40,40,70,40);//mod
      rect(110,40,85,40);//Velocity
      rect(112,42,81,36);//now
      rect(195,40,85,40);//Position
      rect(280,40,60,40);//Stop
      rect(160,90,60,40);//front
      rect(220,90,80,40);//reverse
      if(Velocity_front_reverse==1){rect(162,92,56,36);}//front
      else{rect(222,92,76,36);}
      
      textSize(20);
      fill(0);
      text("MODE",50,67);
      text("Velocity",120,67);
      text("Position",205,67);
      text("Stop",290,67);
      text("Front",167,117);
      text("Reverse",227,117);
      if((mouseX)>=40 && (mouseX)<=40+70 && (mouseY)>=40 && (mouseY)<=40+40 && (mousePressed)){Mod=0;print("Mod : ");println(Mod);delay(50);}
      else if((mouseX)>=110 && (mouseX)<=110+85 && (mouseY)>=40 && (mouseY)<=40+40 && (mousePressed)){Mod=1;Velocity_front_reverse=1;Velocity_mouseX=52;print("Mod : ");println(Mod);delay(50);}
      else if((mouseX)>=195 && (mouseX)<=195+85 && (mouseY)>=40 && (mouseY)<=40+40 && (mousePressed)){Mod=2;Position_str="";Position=0;print("Mod : ");println(Mod);delay(50);}
      else if((mouseX)>=280 && (mouseX)<=280+60 && (mouseY)>=40 && (mouseY)<=40+40 && (mousePressed)){motor_stop();Mod=0;print("Mod : ");println(Mod);delay(50);}
      
      if((mouseX)>=160 && (mouseX)<=220 && (mouseY)>=90 && (mouseY)<=90+40 && (mousePressed)){Velocity_front_reverse=1;delay(50);}
      else if((mouseX)>=220 && (mouseX)<=310 && (mouseY)>=90 && (mouseY)<=90+40 && (mousePressed)){Velocity_front_reverse=-1;delay(50);}
      
      textSize(30);
      text("Velocity",50,117);
      fill(255);
      
      rect(50,140,524,30);
      fill(100);
      if((mouseY)>=140&& (mouseY)<=170 && (mousePressed)){
        if((mouseX)>=552){Velocity_mouseX=552;}
        else if((mouseX)<=52){Velocity_mouseX=52;}
        else {Velocity_mouseX=mouseX;}
      } 
      rect(Velocity_mouseX,142,20,26);
      Velocity=(Velocity_mouseX-52);
      fill(0);
      text(Velocity_front_reverse*Velocity,310,117);
      text("RPM",365,117);
      send_Velocity();
      break;
      
    case 2://Position
      fill(255);
      rect(40,40,70,40);//mod
      rect(110,40,85,40);//Velocity
      rect(195,40,85,40);//Position
      rect(197,42,81,36);//now
      rect(280,40,60,40);//Stop
      rect(200,90,60,40);//send
      
      textSize(20);
      fill(0);
      text("MODE",50,67);
      text("Velocity",120,67);
      text("Position",205,67);
      text("Stop",290,67);
      text("Send",207,117);
      if((mouseX)>=40 && (mouseX)<=40+70 && (mouseY)>=40 && (mouseY)<=40+40 && (mousePressed)){Mod=0;print("Mod : ");println(Mod);delay(50);}
      else if((mouseX)>=110 && (mouseX)<=110+85 && (mouseY)>=40 && (mouseY)<=40+40 && (mousePressed)){Mod=1;Velocity_front_reverse=1;Velocity_mouseX=52;print("Mod : ");println(Mod);delay(50);}
      else if((mouseX)>=195 && (mouseX)<=195+85 && (mouseY)>=40 && (mouseY)<=40+40 && (mousePressed)){Mod=2;Position_str="";Position=0;print("Mod : ");println(Mod);delay(50);}
      else if((mouseX)>=280 && (mouseX)<=280+60 && (mouseY)>=40 && (mouseY)<=40+40 && (mousePressed)){motor_stop();Mod=0;print("Mod : ");println(Mod);delay(50);}
      textSize(40);
      text("Position",50,117);
      textSize(30);
      text("input target Encoder :",60,157);
      text(Position_str,340,157);
      text(Position,60,197);
      if(keyPressed_error==1){
        fill(232 ,130 ,141);
        rect(50,170,480,40);
        fill(0);
        text("ERRER :: Please enter only numbers.",60,200);
      }
      if((mouseX)>=200 && (mouseX)<=200+60 && (mouseY)>=90 && (mouseY)<=90+40 && (mousePressed)){  //send
        fill(255);
        rect(202,92,56,36);
        fill(0);
        textSize(20);
        text("Send",207,117);
        send_Position();
      }
      break;
  }
}
