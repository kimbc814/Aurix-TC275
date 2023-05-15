// demo: CAN-BUS Shield, send data
#include <mcp_can.h>
#include <SPI.h>
#include <MsTimer2.h>

MCP_CAN CAN0(10);                                      // Set CS to pin 10
long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];

void setup()
{
  Serial.begin(115200);
  // init can bus, baudrate: 500k 
  if(CAN0.begin(CAN_500KBPS,MCP_8MHz) == CAN_OK) Serial.print("can init ok!!\r\n");
  else Serial.print("Can init fail!!\r\n");
  pinMode(2, INPUT);
  MsTimer2::set(1000,TIMER_1);
  MsTimer2::start();
}
unsigned char stmp[8] = {10, 11, 12, 13, 14, 15, 16, 17};
void TIMER_1(){
 //send data:  id = 0x00, standrad flame, data len = 8, stmp: data buf
  CAN0.sendMsgBuf(0x00, 0, 8, stmp);  
//  delay(1000);                       // send data per 100ms
}
  
void loop()
{
  
  if(!digitalRead(2))                         // If pin 2 is low, read receive buffer
    {
      CAN0.readMsgBuf(&len, rxBuf);              // Read data: len = data length, buf = data byte(s)
      rxId = CAN0.getCanId();                    // Get message ID
      Serial.print("ID: ");
      Serial.print(rxId, HEX);
      Serial.print("  Data: ");
      for(int i = 0; i<len; i++)                // Print each byte of the data
      {
        if(rxBuf[i] < 0x10)                     // If data byte is less than 0x10, add a leading zero
        {
          Serial.print("0");
        }
        Serial.print(rxBuf[i], HEX);
        Serial.print(" ");
      }
      Serial.println();
    }
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
