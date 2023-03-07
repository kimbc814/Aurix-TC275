#include "IfxPort.h"
#include "Bsp.h"

#define PORT3     &MODULE_P00,2   
#define PORT2     &MODULE_P00,6   
#define PORT1     &MODULE_P00,8   

#define WAIT_TIME 500

void init_Port(void)
{
    IfxPort_setPinMode(PORT1, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(PORT2, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(PORT3, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinState(PORT1, IfxPort_State_low);
    IfxPort_setPinState(PORT2, IfxPort_State_low);
    IfxPort_setPinState(PORT3, IfxPort_State_low);
    int tm=0;
}

void change_Port(void)
{
    tm++;
    waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, 1));
    if(tm%5 == 0){IfxPort_togglePin(PORT1);}
    if(tm%10 == 0){IfxPort_togglePin(PORT2);}
    if(tm%20 == 0){IfxPort_togglePin(PORT3);}
    /*
    IfxPort_togglePin(PORT1);
    IfxPort_togglePin(PORT2);
    IfxPort_togglePin(PORT3);
    waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, 5));
    IfxPort_togglePin(PORT1);
    waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, 5));
    IfxPort_togglePin(PORT2);
    IfxPort_togglePin(PORT1);
    waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, 5));
    IfxPort_togglePin(PORT1);
    waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, 5));
    */
    
}
