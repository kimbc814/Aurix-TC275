/*
 * GPTimer.c
 *
 *  Created on: 2023. 7. 1.
 *      Author: pc_2
 */

/*********************************************************************************************************************/
#include "GPTimer.h"
#include "Ifx_Types.h"
#include "IfxGpt12.h"
#include "IfxPort.h"
#include "Bsp.h"
#include "IfxAsclin_Asc.h"
#include "IfxCpu_Irq.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"
#include "Driver_Can.h"
#include "I2C_Read_Ext_Device.h"

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/
#define ISR_PRIORITY_GPT12_TIMER_T2  10                       /* Define the GPT12 Timer interrupt priority            */
#define ISR_PROVIDER_GPT12_TIMER     IfxSrc_Tos_cpu0         /* Interrupt provider                                   */
#define RELOAD_VALUE                 48828u                  /* Reload value to have an interrupt each 500ms         */
#define RELOAD_VALUE_200ms           19531u
#define RELOAD_VALUE_50ms            4883u
#define RELOAD_VALUE_10ms            977u

#define WAIT_TIME                    1


extern struct CONTROL DataControl;
/*********************************************************************************************************************/
/*--------------------------------------------Function Implementations-----------------------------------------------*/
/*********************************************************************************************************************/

/* Macro defining the Interrupt Service Routine */
IFX_INTERRUPT(interruptGPTimer, 0, ISR_PRIORITY_GPT12_TIMER_T2);
/* Function to initialize the GPT12 and start the timer */

void initGpt12Timer(void){
    IfxGpt12_enableModule(&MODULE_GPT120);
    IfxGpt12_setGpt1BlockPrescaler(&MODULE_GPT120, IfxGpt12_Gpt1BlockPrescaler_16);

    IfxGpt12_T2_setMode(&MODULE_GPT120, IfxGpt12_Mode_timer);
    IfxGpt12_T2_setTimerValue(&MODULE_GPT120, RELOAD_VALUE_200ms);

    volatile Ifx_SRC_SRCR *src_t2 = IfxGpt12_T2_getSrc(&MODULE_GPT120);
    IfxSrc_init(src_t2, ISR_PROVIDER_GPT12_TIMER, ISR_PRIORITY_GPT12_TIMER_T2);
    IfxSrc_enable(src_t2);

    IfxGpt12_T2_run(&MODULE_GPT120, IfxGpt12_TimerRun_start);
}

void interruptGPTimer(void)
{
    read_ext_device_address();
    if(DataControl.Seccess == 1){
        motor_control();
        DataControl.Seccess=0;
    }

}

