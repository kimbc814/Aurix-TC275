#include "IfxPort.h"

#define PORT1     &MODULE_P33,11
#define PORT2     &MODULE_P33,12
#define PORT3     &MODULE_P33,13
#define PORT4     &MODULE_P32,0
#define PORT5     &MODULE_P23,1
#define PORT6     &MODULE_P23,0
#define PORT7     &MODULE_P23,3
#define PORT8     &MODULE_P23,2

#define LED_PORT1 &MODULE_P00,0
#define LED_PORT2 &MODULE_P00,1
#define LED_PORT3 &MODULE_P00,2
#define LED_PORT4 &MODULE_P00,3
#define LED_PORT5 &MODULE_P00,8
#define LED_PORT6 &MODULE_P00,7
#define LED_PORT7 &MODULE_P00,10
#define LED_PORT8 &MODULE_P00,9

uint8 cch_input[8]={0,};
void init_Port(void)
{
    IfxPort_setPinMode(PORT1, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(PORT2, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(PORT3, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(PORT4, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(PORT5, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(PORT6, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(PORT7, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(PORT8, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinState(PORT1, IfxPort_State_high);
    IfxPort_setPinState(PORT2, IfxPort_State_high);
    IfxPort_setPinState(PORT3, IfxPort_State_high);
    IfxPort_setPinState(PORT4, IfxPort_State_high);
    IfxPort_setPinState(PORT5, IfxPort_State_high);
    IfxPort_setPinState(PORT6, IfxPort_State_high);
    IfxPort_setPinState(PORT7, IfxPort_State_high);
    IfxPort_setPinState(PORT8, IfxPort_State_high);
    IfxPort_setPinMode(LED_PORT1, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(LED_PORT2, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(LED_PORT3, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(LED_PORT4, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(LED_PORT5, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(LED_PORT6, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(LED_PORT7, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(LED_PORT8, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinState(LED_PORT1, IfxPort_State_low);
    IfxPort_setPinState(LED_PORT2, IfxPort_State_low);
    IfxPort_setPinState(LED_PORT3, IfxPort_State_low);
    IfxPort_setPinState(LED_PORT4, IfxPort_State_low);
    IfxPort_setPinState(LED_PORT5, IfxPort_State_low);
    IfxPort_setPinState(LED_PORT6, IfxPort_State_low);
    IfxPort_setPinState(LED_PORT7, IfxPort_State_low);
    IfxPort_setPinState(LED_PORT8, IfxPort_State_low);
}

struct filter1{
    int arr[9];
    int cnt1;

};
struct filter1 LP1,LP2,LP3,LP4,LP5,LP6,LP7,LP8;
int ffill(struct filter1 LP){
    int count0=0,count1=5;
    for(int i=0;i<sizeof(LP.arr)/sizeof(int);i++){
        count0+=LP.arr[i];
    }
    if(count0<count1){return 0;}else{return 1;}
}

void change_rdata_type1(uint8 iindata){
    int ttempdata=iindata;
    if(ttempdata>=128){
        ttempdata-=128;
        cch_input[7]=1;
        LP8.arr[LP8.cnt1]=1;
        if(LP8.cnt1>5){LP8.cnt1=0;}
        else{LP8.cnt1++;}
    }
    else{
        cch_input[7]=0;
        LP8.arr[LP8.cnt1]=0;
        if(LP8.cnt1>5){LP8.cnt1=0;}
        else{LP8.cnt1++;}
    }
    if(ttempdata>=64){ttempdata-=64;cch_input[6]=1;LP7.arr[LP7.cnt1]=1;if(LP7.cnt1>4){LP7.cnt1=0;}else{LP7.cnt1++;}}else{cch_input[6]=0;LP7.arr[LP7.cnt1]=0;if(LP7.cnt1>4){LP7.cnt1=0;}else{LP7.cnt1++;}}
    if(ttempdata>=32){ttempdata-=32;cch_input[5]=1;LP6.arr[LP6.cnt1]=1;if(LP6.cnt1>4){LP6.cnt1=0;}else{LP6.cnt1++;}}else{cch_input[5]=0;LP6.arr[LP6.cnt1]=0;if(LP6.cnt1>4){LP6.cnt1=0;}else{LP6.cnt1++;}}
    if(ttempdata>=16){ttempdata-=16;cch_input[4]=1;LP5.arr[LP5.cnt1]=1;if(LP5.cnt1>4){LP5.cnt1=0;}else{LP5.cnt1++;}}else{cch_input[4]=0;LP5.arr[LP5.cnt1]=0;if(LP5.cnt1>4){LP5.cnt1=0;}else{LP5.cnt1++;}}
    if(ttempdata>=8){ttempdata-=8;cch_input[3]=1;LP4.arr[LP4.cnt1]=1;if(LP4.cnt1>4){LP4.cnt1=0;}else{LP4.cnt1++;}}else{cch_input[3]=0;LP4.arr[LP4.cnt1]=0;if(LP4.cnt1>4){LP4.cnt1=0;}else{LP4.cnt1++;}}
    if(ttempdata>=4){ttempdata-=4;cch_input[2]=1;LP3.arr[LP3.cnt1]=1;if(LP3.cnt1>4){LP3.cnt1=0;}else{LP3.cnt1++;}}else{cch_input[2]=0;LP3.arr[LP3.cnt1]=0;if(LP3.cnt1>4){LP3.cnt1=0;}else{LP3.cnt1++;}}
    if(ttempdata>=2){ttempdata-=2;cch_input[1]=1;LP2.arr[LP2.cnt1]=1;if(LP2.cnt1>4){LP2.cnt1=0;}else{LP2.cnt1++;}}else{cch_input[1]=0;LP2.arr[LP2.cnt1]=0;if(LP2.cnt1>4){LP2.cnt1=0;}else{LP2.cnt1++;}}
    if(ttempdata>=1){ttempdata-=1;cch_input[0]=1;LP1.arr[LP1.cnt1]=1;if(LP1.cnt1>4){LP1.cnt1=0;}else{LP1.cnt1++;}}else{cch_input[0]=0;LP1.arr[LP1.cnt1]=0;if(LP1.cnt1>4){LP1.cnt1=0;}else{LP1.cnt1++;}}
}
void LED_ONOFF(){
    if(ffill(LP1)==1){IfxPort_setPinState(LED_PORT1, IfxPort_State_high);}else{IfxPort_setPinState(LED_PORT1, IfxPort_State_low);}
    if(ffill(LP2)==1){IfxPort_setPinState(LED_PORT2, IfxPort_State_high);}else{IfxPort_setPinState(LED_PORT2, IfxPort_State_low);}
    if(ffill(LP3)==1){IfxPort_setPinState(LED_PORT3, IfxPort_State_high);}else{IfxPort_setPinState(LED_PORT3, IfxPort_State_low);}
    if(ffill(LP4)==1){IfxPort_setPinState(LED_PORT4, IfxPort_State_high);}else{IfxPort_setPinState(LED_PORT4, IfxPort_State_low);}
    if(ffill(LP5)==1){IfxPort_setPinState(LED_PORT5, IfxPort_State_high);}else{IfxPort_setPinState(LED_PORT5, IfxPort_State_low);}
    if(ffill(LP6)==1){IfxPort_setPinState(LED_PORT6, IfxPort_State_high);}else{IfxPort_setPinState(LED_PORT6, IfxPort_State_low);}
    if(ffill(LP7)==1){IfxPort_setPinState(LED_PORT7, IfxPort_State_high);}else{IfxPort_setPinState(LED_PORT7, IfxPort_State_low);}
    if(ffill(LP8)==1){IfxPort_setPinState(LED_PORT8, IfxPort_State_high);}else{IfxPort_setPinState(LED_PORT8, IfxPort_State_low);}
}

/*void change_Port(void)
{
    if(cch_input[0]==1){IfxPort_setPinState(LED_PORT1, IfxPort_State_high);}else{IfxPort_setPinState(LED_PORT1, IfxPort_State_low);}
    if(cch_input[1]==1){IfxPort_setPinState(LED_PORT2, IfxPort_State_high);}else{IfxPort_setPinState(LED_PORT2, IfxPort_State_low);}
    if(cch_input[2]==1){IfxPort_setPinState(LED_PORT3, IfxPort_State_high);}else{IfxPort_setPinState(LED_PORT3, IfxPort_State_low);}
    if(cch_input[3]==1){IfxPort_setPinState(LED_PORT4, IfxPort_State_high);}else{IfxPort_setPinState(LED_PORT4, IfxPort_State_low);}
    if(cch_input[4]==1){IfxPort_setPinState(LED_PORT5, IfxPort_State_high);}else{IfxPort_setPinState(LED_PORT5, IfxPort_State_low);}
    if(cch_input[5]==1){IfxPort_setPinState(LED_PORT6, IfxPort_State_high);}else{IfxPort_setPinState(LED_PORT6, IfxPort_State_low);}
    if(cch_input[6]==1){IfxPort_setPinState(LED_PORT7, IfxPort_State_high);}else{IfxPort_setPinState(LED_PORT7, IfxPort_State_low);}
    if(cch_input[7]==1){IfxPort_setPinState(LED_PORT8, IfxPort_State_high);}else{IfxPort_setPinState(LED_PORT8, IfxPort_State_low);}
}*/
