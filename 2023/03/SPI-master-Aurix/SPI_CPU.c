/**********************************************************************************************************************
 * \file SPI_CPU.c
 * \copyright Copyright (C) Infineon Technologies AG 2019
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG or its licensees. If and as long as no such terms of use
 * are agreed, use of this file is subject to following:
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization obtaining a copy of the software and
 * accompanying documentation covered by this license (the "Software") to use, reproduce, display, distribute, execute,
 * and transmit the Software, and to prepare derivative works of the Software, and to permit third-parties to whom the
 * Software is furnished to do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including the above license grant, this restriction
 * and the following disclaimer, must be included in all copies of the Software, in whole or in part, and all
 * derivative works of the Software, unless such copies or derivative works are solely in the form of
 * machine-executable object code generated by a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *********************************************************************************************************************/

/*********************************************************************************************************************/
/*-----------------------------------------------------Includes------------------------------------------------------*/
/*********************************************************************************************************************/
#include "SPI_CPU.h"
#include "IfxPort.h"

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/
/* QSPI modules */
#define QSPI1_MASTER                &MODULE_QSPI1   /* SPI Master module                                            */
#define QSPI3_SLAVE                 &MODULE_QSPI3   /* SPI Slave module                                             */

/* LED port pin */
#define LED1                        &MODULE_P00,5   /* LED1 Port, Pin definition                                    */

#define MASTER_CHANNEL_BAUDRATE     1000000         /* Master channel baud rate                                     */

/* Interrupt Service Routine priorities for Master and Slave SPI communication */
#define ISR_PRIORITY_MASTER_TX      50
#define ISR_PRIORITY_MASTER_RX      51
#define ISR_PRIORITY_MASTER_ER      52
#define ISR_PRIORITY_SLAVE_TX       53
#define ISR_PRIORITY_SLAVE_RX       54
#define ISR_PRIORITY_SLAVE_ER       55

/*********************************************************************************************************************/
/*-------------------------------------------------Global variables--------------------------------------------------*/
/*********************************************************************************************************************/
qspiComm g_qspi;

/*********************************************************************************************************************/
/*------------------------------------------------Function Prototypes------------------------------------------------*/
/*********************************************************************************************************************/
void initQSPI1Master(void);
void initQSPI1MasterChannel(void);
void initQSPI1MasterBuffers(void);
void initQSPI3Slave(void);
void initQSPI3SlaveBuffers(void);
void initQSPI(void);
void initLED(void);
void verifyData(void);

/*********************************************************************************************************************/
/*----------------------------------------------Function Implementations---------------------------------------------*/
/*********************************************************************************************************************/
IFX_INTERRUPT(masterTxISR, 0, ISR_PRIORITY_MASTER_TX);                  /* SPI Master ISR for transmit data         */
IFX_INTERRUPT(masterRxISR, 0, ISR_PRIORITY_MASTER_RX);                  /* SPI Master ISR for receive data          */
IFX_INTERRUPT(masterErISR, 0, ISR_PRIORITY_MASTER_ER);                  /* SPI Master ISR for error                 */
IFX_INTERRUPT(slaveTxISR, 0, ISR_PRIORITY_SLAVE_TX);                    /* SPI Slave ISR for transmit data          */
IFX_INTERRUPT(slaveRxISR, 0, ISR_PRIORITY_SLAVE_RX);                    /* SPI Slave ISR for receive data           */
IFX_INTERRUPT(slaveErISR, 0, ISR_PRIORITY_SLAVE_ER);                    /* SPI Slave ISR for error                  */

void masterTxISR()                                                                              //master Tx handler
{
    IfxCpu_enableInterrupts();                                                                  //enable cpu interrupt
    IfxQspi_SpiMaster_isrTransmit(&g_qspi.spiMaster);                                           //transmit
}

void masterRxISR()                                                                              //master Rx handler
{
    IfxCpu_enableInterrupts();                                                                  //enable cpu interrupt
    IfxQspi_SpiMaster_isrReceive(&g_qspi.spiMaster);
}

void masterErISR() //master error handler
{
    IfxCpu_enableInterrupts();                                                                  //enable cpu interrupt
    IfxQspi_SpiMaster_isrError(&g_qspi.spiMaster);
}

void slaveTxISR() //slave Tx handler
{
    IfxCpu_enableInterrupts();                                                                  //enable cpu interrupt
    IfxQspi_SpiSlave_isrTransmit(&g_qspi.spiSlave);
}

void slaveRxISR() //slave Rx handler
{
    IfxCpu_enableInterrupts();                                                                  //enable cpu interrupt
    IfxQspi_SpiSlave_isrReceive(&g_qspi.spiSlave);
}

void slaveErISR() //slave error handler
{
    IfxCpu_enableInterrupts();                                                                  //enable cpu interrupt
    IfxQspi_SpiSlave_isrError(&g_qspi.spiSlave);
}

/* QSPI Master initialization
 * This function initializes the QSPI1 module in Master mode.
 */
void initQSPI1Master(void)
{
    IfxQspi_SpiMaster_Config spiMasterConfig;                           /* Define a Master configuration            */

    IfxQspi_SpiMaster_initModuleConfig(&spiMasterConfig, QSPI1_MASTER); /* Initialize it with default values        */

    spiMasterConfig.base.mode = SpiIf_Mode_master;                      /* Configure the mode                       */

    /* Select the port pins for communication */
    const IfxQspi_SpiMaster_Pins qspi1MasterPins = {
        &IfxQspi1_SCLK_P10_2_OUT, IfxPort_OutputMode_pushPull,          /* SCLK Pin                       (CLK)     */
        &IfxQspi1_MTSR_P10_3_OUT, IfxPort_OutputMode_pushPull,          /* MasterTransmitSlaveReceive pin (MOSI)    */
        &IfxQspi1_MRSTA_P10_1_IN, IfxPort_InputMode_pullDown,           /* MasterReceiveSlaveTransmit pin (MISO)    */
        IfxPort_PadDriver_cmosAutomotiveSpeed3                          /* Pad driver mode                          */
    };
    spiMasterConfig.pins = &qspi1MasterPins;                            /* Assign the Master's port pins            */

    /* Set the ISR priorities and the service provider */
    spiMasterConfig.base.txPriority = ISR_PRIORITY_MASTER_TX;                                    //set Master Tx priority
    spiMasterConfig.base.rxPriority = ISR_PRIORITY_MASTER_RX;                                    //set Master Rx priority
    spiMasterConfig.base.erPriority = ISR_PRIORITY_MASTER_ER;                                    //set Master error priority
    spiMasterConfig.base.isrProvider = IfxSrc_Tos_cpu0;                                          //set Master SPI at base cpu0

    /* Initialize the QSPI Master module */
    IfxQspi_SpiMaster_initModule(&g_qspi.spiMaster, &spiMasterConfig);                           //initialize SPI module
}

/* QSPI Master channel initialization
 * This function initializes the QSPI1 Master channel.
 */
void initQSPI1MasterChannel(void)
{
    IfxQspi_SpiMaster_ChannelConfig spiMasterChannelConfig;             /* Define a Master Channel configuration    */

    /* Initialize the configuration with default values */
    IfxQspi_SpiMaster_initChannelConfig(&spiMasterChannelConfig, &g_qspi.spiMaster);

    spiMasterChannelConfig.base.baudrate = MASTER_CHANNEL_BAUDRATE;     /* Set SCLK frequency to 1 MHz              */

    /* Select the port pin for the Chip Select signal */
    const IfxQspi_SpiMaster_Output qspi1SlaveSelect = {                 /* QSPI1 Master selects the QSPI3 Slave     */
        &IfxQspi1_SLSO10_P10_0_OUT, IfxPort_OutputMode_pushPull,         /* Slave Select port pin (CS)               */
        IfxPort_PadDriver_cmosAutomotiveSpeed1                          /* Pad driver mode                          */
    };
    spiMasterChannelConfig.sls.output = qspi1SlaveSelect;

    /* Initialize the QSPI Master channel */
    IfxQspi_SpiMaster_initChannel(&g_qspi.spiMasterChannel, &spiMasterChannelConfig);
}

/* QSPI Master SW buffer initialization
 * This function initializes SW buffers the Master uses.
 */
void initQSPI1MasterBuffers(void)
{
    /* Fill TX Master Buffer with pattern       */
    g_qspi.spiBuffers.spiMasterTxBuffer[0] = (uint8)85;                         //set 1btye data to SPI master TxBuffer index 0  dec : 85 hex : 0x55
    for (uint8 i = 0; i < SPI_BUFFER_SIZE; i++)
    {

        g_qspi.spiBuffers.spiMasterRxBuffer[i] = 0;                     /* Clear RX Buffer                          */
    }
}

/* QSPI Slave initialization
 * This function initializes the QSPI3 module in Slave mode.
 */
void initQSPI3Slave(void)
{
    IfxQspi_SpiSlave_Config spiSlaveConfig;                             /* Define the Slave configuration           */

    IfxQspi_SpiSlave_initModuleConfig(&spiSlaveConfig, QSPI3_SLAVE);    /* Initialize it with default values        */

    /* Select the port pins for communication */
    const IfxQspi_SpiSlave_Pins qspi3SlavePins = {
        &IfxQspi3_SCLKA_P02_7_IN, IfxPort_InputMode_pullDown,           /* SCLK Pin                       (CLK)     */
        &IfxQspi3_MTSRA_P02_6_IN, IfxPort_InputMode_pullDown,           /* MasterTransmitSlaveReceive pin (MOSI)    */
        &IfxQspi3_MRST_P02_5_OUT, IfxPort_OutputMode_pushPull,          /* MasterReceiveSlaveTransmit pin (MISO)    */
        &IfxQspi3_SLSIA_P02_4_IN, IfxPort_InputMode_pullDown,           /* SlaveSelect Pin                (CS)      */
        IfxPort_PadDriver_cmosAutomotiveSpeed3                          /* Pad driver mode                          */
    };
    spiSlaveConfig.pins = &qspi3SlavePins;                              /* Assign Slave port pins                   */

    /* Set the ISR priorities and the service provider */
    spiSlaveConfig.base.txPriority = ISR_PRIORITY_SLAVE_TX;                                    //set Slave Tx priority
    spiSlaveConfig.base.rxPriority = ISR_PRIORITY_SLAVE_RX;                                    //set Slave Rx priority
    spiSlaveConfig.base.erPriority = ISR_PRIORITY_SLAVE_ER;                                    //set Slave error priority
    spiSlaveConfig.base.isrProvider = IfxSrc_Tos_cpu0;                                         //set Slave SPI at base cpu0

    /* Initialize QSPI Slave module */
    IfxQspi_SpiSlave_initModule(&g_qspi.spiSlave, &spiSlaveConfig);                            //initialize SPI module
}

/* QSPI Slave SW buffer initialization
 * This function initializes the software buffers that are used by the Slave.
 */
void initQSPI3SlaveBuffers(void)
{
    for (uint8 i = 0; i < SPI_BUFFER_SIZE; i++)
    {
        g_qspi.spiBuffers.spiSlaveTxBuffer[i] = 0;                      /* Clear TX Slave Buffer                    */
        g_qspi.spiBuffers.spiSlaveRxBuffer[i] = 0;                      /* Clear RX Slave Buffer                    */
    }
}

/* This function to initialize the LED */
void initLED(void)
{
    /* Set the port pin 00.5 (to which the LED1 is connected) to output push-pull mode */
    IfxPort_setPinModeOutput(LED1, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

    /* Turn off LED (LED is low-level active) */
    IfxPort_setPinHigh(LED1);
}

/* This function initialize the QSPI modules */
void initQSPI(void)
{
    /* Firstly initialize the Slave */
    //initQSPI3Slave();
    //initQSPI3SlaveBuffers();

    /* Secondly initialize the Master */
    initQSPI1Master();
    initQSPI1MasterChannel();
    initQSPI1MasterBuffers();
}

/* This function to initialize the QSPI modules and the LED */
void initPeripherals(void)
{
    initLED();
    initQSPI();
}

/* This function starts the data transfer */
void transferData(void)
{
    while(/*IfxQspi_SpiSlave_getStatus(&g_qspi.spiSlave) == SpiIf_Status_busy ||*/
            IfxQspi_SpiMaster_getStatus(&g_qspi.spiMasterChannel) == SpiIf_Status_busy)
    {   /* Wait until the previous communication has finished, if any */
    }

    /* Instruct the SPI Slave to receive a data stream of defined length */
    //IfxQspi_SpiSlave_exchange(&g_qspi.spiSlave, NULL_PTR, &g_qspi.spiBuffers.spiSlaveRxBuffer[0], SPI_BUFFER_SIZE);            //Instruct the SPI Slave to receive a data stream of defined length

    /* Send a data stream through the SPI Master */
    IfxQspi_SpiMaster_exchange(&g_qspi.spiMasterChannel, &g_qspi.spiBuffers.spiMasterTxBuffer[0], NULL_PTR, SPI_BUFFER_SIZE);  //Send a data stream through the SPI Master

    verifyData();
}

/* This function checks if the received data is correct */
void verifyData(void)
{
    uint32 i;
    uint32 error = 0;

    /* Wait until the Slave has received all the data */
    /*
    while(IfxQspi_SpiSlave_getStatus(&g_qspi.spiSlave) == SpiIf_Status_busy)
    {
    }

    // Check if the received data match the sent one
    for(i = 0; i < SPI_BUFFER_SIZE; i++)
    {
        if(g_qspi.spiBuffers.spiSlaveRxBuffer[i] != g_qspi.spiBuffers.spiMasterTxBuffer[i])
        {
            error++;
        }
    }*/

    /* In case of no errors, turn on the LED1 (LED is low-level active) */
    if(error == 0)
    {
        IfxPort_setPinLow(LED1);
    }
}
