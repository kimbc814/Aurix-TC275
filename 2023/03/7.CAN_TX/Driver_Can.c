/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include "Driver_Can.h"


/*********************************************************************************************************************/
/*-------------------------------------------------Global variables--------------------------------------------------*/
/*********************************************************************************************************************/
AppLedType        g_led;                                                                                                                    /* Global LED configuration and control structure                       */


/***********************************************************************/
/*Define*/ 
/***********************************************************************/


/***********************************************************************/
/*Typedef*/ 
/***********************************************************************/


/***********************************************************************/
/*Static Function Prototype*/ 
/***********************************************************************/




/* Macro to define Interrupt Service Routine.*/

IFX_INTERRUPT(canIsrTxHandler, 0, ISR_PRIORITY_CAN_TX);                                                                                     /* Execute the canIsrTxHandler when an interrupt occurs. Priority is 2  */

/***********************************************************************/
/*Variable*/ 
/***********************************************************************/
App_MulticanBasic g_MulticanBasic;                                                                                                          /**< \brief Demo information */
volatile CanRxMsg rec;                                                                                                                      /* Assigning a non-optimizing 'CanRxMsg' structure named 'rec'          */
int a[8]={0,0x1,0x2,0x3,0x4,0x5,0x6,0x7};                                                                                                   /* Assigning data array 'a'                                             */
 
/***********************************************************************/
/*Function*/ 
/***********************************************************************/

void Driver_Can_Init(void)                                                                                                                  /*Initialize Driver_Can                                                 */
{
    /* create module config */
    IfxMultican_Can_Config canConfig;                                                                                                       /* Configuration structure CAN named canConfig                          */
    IfxMultican_Can_initModuleConfig(&canConfig, &MODULE_CAN);                                                                              /* Configuration Initialize CAN module                                  */

    /* Interrupt configuration*/

    canConfig.nodePointer[TX_INTERRUPT_SRC_ID].priority = ISR_PRIORITY_CAN_TX;                                                              /* Assigning CAN nodePointer id 0 priority is 2                         */

    /* initialize module */
    IfxMultican_Can_initModule(&g_MulticanBasic.drivers.can, &canConfig);                                                                   /* Initialize CAN module*/

    /* create CAN node config */
    IfxMultican_Can_NodeConfig canNodeConfig;                                                                                               /* Configuration structure CAN named canConfig                          */
    IfxMultican_Can_Node_initConfig(&canNodeConfig, &g_MulticanBasic.drivers.can);                                                          /* Configuration Initialize CAN module                                  */

    canNodeConfig.baudrate = 500000UL;                                                                                                      // CAN �ӵ� ���� 500kbps
    {
        canNodeConfig.nodeId    = IfxMultican_NodeId_0;//(IfxMultican_NodeId)((int)IfxMultican_NodeId_0);                                   // CAN�� Node ID ���� 0������ ������
        canNodeConfig.rxPin     = &IfxMultican_RXD0B_P20_7_IN;                                                                              // �Է��� ����
        canNodeConfig.rxPinMode = IfxPort_InputMode_pullUp;
        canNodeConfig.txPin     = &IfxMultican_TXD0_P20_8_OUT;                                                                              // ����� ����
        canNodeConfig.txPinMode = IfxPort_OutputMode_pushPull;

        IfxMultican_Can_Node_init(&g_MulticanBasic.drivers.canSrcNode, &canNodeConfig);                                                     // CAN node �ʱ�ȭ
    }

    /* Create message object config */
    IfxMultican_Can_MsgObjConfig canMsgObjConfig;                                                                                           // CAN message object configuration
    IfxMultican_Can_MsgObj_initConfig(&canMsgObjConfig, &g_MulticanBasic.drivers.canSrcNode);

    canMsgObjConfig.msgObjId              = 0;                                                                                              //256���� message object�� ����
    canMsgObjConfig.messageId             = 0x100;
    canMsgObjConfig.acceptanceMask        = 0x7FFFFFFFUL;
    canMsgObjConfig.frame                 = IfxMultican_Frame_transmit;                                                                     // CAN TX�� ����
    canMsgObjConfig.control.messageLen    = IfxMultican_DataLengthCode_8;                                                                   // Data ���̴� 8
    canMsgObjConfig.control.extendedFrame = FALSE;                                                                                          // Extended ID ��� ����
    canMsgObjConfig.control.matchingId    = TRUE;

    canMsgObjConfig.txInterrupt.enabled = TRUE;                                                                                             /* CAN Interrupt enable                                                 */
    canMsgObjConfig.txInterrupt.srcId = TX_INTERRUPT_SRC_ID;                                                                                /* CAN Interrupt source                                                 */

    /* initialize message object */
    IfxMultican_Can_MsgObj_init(&g_MulticanBasic.drivers.canSrcMsgObj, &canMsgObjConfig);                                                   /* Initialize CAN message object                                        */

    /* IO Port */
    IfxPort_setPinModeOutput(STB, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);                                                  /* Set pinmode STB output                                               */
    /* Set STB Pin of CAN chip (low-level active) */
    IfxPort_setPinLow(STB);                                                                                                                 /* Set pin STB LOW                                                      */
}


void Driver_Can_TxTest(void)                                                                                                                /* Test transmit text using CAN communication                           */
{
    const uint32 dataLow  = 0x12340000;                                                                                                     /* Assign data HEX form                                                 */
    const uint32 dataHigh = 0x9abc0000;                                                                                                     /* Assign data HEX form                                                 */

    /* Transmit Data */
    {
        IfxMultican_Message msg;                                                                                                            /* Create structure 'IfxMultican_Message' named 'msg'                   */
        IfxMultican_Message_init(&msg, 0x100, dataLow, dataHigh, IfxMultican_DataLengthCode_8);                                             /* packaging id 0x100, data dataLow,dataHigh, dataLength 8              */

        while (IfxMultican_Can_MsgObj_sendMessage(&g_MulticanBasic.drivers.canSrcMsgObj, &msg) == IfxMultican_Status_notSentBusy)           /* If CAN status is Busy wait transmit                                  */
        {}
    }
}


void CAN_send(CanRxMsg *message)                                                                                                            /* transmit text using CAN communication                                */
{


    IfxMultican_Message msg;                                                                                                                /* Create structure 'IfxMultican_Message' named 'msg'                   */

    const unsigned dataLow = message->Data[0]|(message->Data[1]<<8)|(message->Data[2]<<16)|(message->Data[3]<<24);                          /* Create checksum data using data array 0,1,2,3                        */
    const unsigned dataHigh = message->Data[4]|(message->Data[5]<<8)|(message->Data[6]<<16)|(message->Data[7]<<24);                         /* Create checksum data using data array 4,5,6,7                        */


    IfxMultican_Message_init(&msg,message->ID,dataLow,dataHigh,message->DLC);                                                               /* packaging id, dataLow, dataHigh, dataLength                          */

    while (IfxMultican_Can_MsgObj_sendMessage(&g_MulticanBasic.drivers.canSrcMsgObj, &msg) == IfxMultican_Status_notSentBusy)               /* If CAN status is Busy wait transmit                                  */
    {

    }
}


void CAN_TEST(void)                                                                                                                         /* Set CAN id,frame type, dataLength and send struct 'MES'              */
{
    CanRxMsg MES;                                                                                                                           /* Create union 'CanRxMsg' named 'MES'.                                 */
    int i=0;
    MES.ID=0x890;                                                                                                                           /* ID of device.                                                        */
    MES.IDE=0;                                                                                                                              /* Set frame standard.                                                  */
    MES.DLC=8;                                                                                                                              /* Set Data length.                                                     */
    for(i=0; i<8; i++)
    {
        MES.Data[i]=a[i];                                                                                                                   /* Assigning array 'a'(Driver_Can.c line39) to the 'MES.Data' array.    */
    }
    CAN_send(&MES);                                                                                                                         /* Send CAN message 'MES' using function CAN_send().                    */
}


/* Interrupt Service Routine (ISR) called once the TX interrupt has been generated.
 * Turns on the LED1 to indicate successful CAN message transmission.
 */
void canIsrTxHandler(void)                                                                                                                  /* Interrupt Handler blinkLED1                                          */
{
    /* Just to indicate that the CAN message has been transmitted by turning on LED1 */
    blinkLED1();                                                                                                                            /* Blink LED1 delay 1ms                                                 */

    //IfxPort_setPinLow(g_led.led1.port, g_led.led1.pinIndex);
}


void initLed(void)
{
    /* ======================================================================
     * Configuration of the pins connected to the LEDs:
     * ======================================================================
     *  - define the GPIO port
     *  - define the GPIO pin that is the connected to the LED
     *  - define the general GPIO pin usage (no alternate function used)
     *  - define the pad driver strength
     * ======================================================================
     */
    g_led.led1.port      = &MODULE_P00;                                                                                                     /* Set port MODULE_P00 LED1                                             */
    g_led.led1.pinIndex  = PIN5;                                                                                                            /* Set pin 5 LED1                                                       */
    g_led.led1.mode      = IfxPort_OutputIdx_general;                                                                                       /* Set pin mode output LED1                                             */
    g_led.led1.padDriver = IfxPort_PadDriver_cmosAutomotiveSpeed1;                                                                          /* Set PadDriver Speed 1  LED1                                          */

    g_led.led2.port      = &MODULE_P00;                                                                                                     /* Set port MODULE_P00 LED2                                             */
    g_led.led2.pinIndex  = PIN6;                                                                                                            /* Set pin 6 LED2                                                       */
    g_led.led2.mode      = IfxPort_OutputIdx_general;                                                                                       /* Set pin mode output LED2                                             */
    g_led.led2.padDriver = IfxPort_PadDriver_cmosAutomotiveSpeed1;                                                                          /* Set PadDriver Speed 1 LED2                                           */

    /* Initialize the pins connected to LEDs to level "HIGH"; will keep the LEDs turned off as default state */
    IfxPort_setPinHigh(g_led.led1.port, g_led.led1.pinIndex);                                                                               /* Set pin high LED1                                                    */
    IfxPort_setPinHigh(g_led.led2.port, g_led.led2.pinIndex);                                                                               /* Set pin high LED2                                                    */

    /* Set the pin input/output mode for both pins connected to the LEDs */
    IfxPort_setPinModeOutput(g_led.led1.port, g_led.led1.pinIndex, IfxPort_OutputMode_pushPull, g_led.led1.mode);                           /* Set pin mode output LED1                                             */
    IfxPort_setPinModeOutput(g_led.led2.port, g_led.led2.pinIndex, IfxPort_OutputMode_pushPull, g_led.led2.mode);                           /* Set pin mode output LED2                                             */

    /* Set the pad driver mode for both pins connected to the LEDs */
    IfxPort_setPinPadDriver(g_led.led1.port, g_led.led1.pinIndex, g_led.led1.padDriver);                                                    /* Set PadDriver mode LED1                                              */
    IfxPort_setPinPadDriver(g_led.led2.port, g_led.led2.pinIndex, g_led.led2.padDriver);                                                    /* Set PadDriver mode LED2                                              */
}


void blinkLED1(void)
{
    //IfxPort_togglePin(LED1);                                                                                                              /* Toggle the state of the LED    */

    IfxPort_setPinHigh(LED1);                                                                                                               /*Turn off LED1                   */
    waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, WAIT_TIME_1ms));                                                            /* Wait 1 milliseconds            */
    IfxPort_setPinLow(LED1);                                                                                                                /*Turn on LED1                    */
}
void blinkLED2(void)
{
    IfxPort_togglePin(LED2);                                                                                                                /* Toggle the state of the LED2   */
    waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, WAIT_TIME_10ms));                                                           /* Wait 100 milliseconds          */
}
