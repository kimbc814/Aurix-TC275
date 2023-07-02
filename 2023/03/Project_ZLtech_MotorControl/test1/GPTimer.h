/*
 * GPTimer.h
 *
 *  Created on: 2023. 7. 1.
 *      Author: pc_2
 */
#ifndef GPTimer_H
#define GPTimer_H


/*********************************************************************************************************************/
/*-----------------------------------------------Function Prototypes-------------------------------------------------*/
/*********************************************************************************************************************/
void initGpt12Timer(void);                          /* Function to initialize the GPT12 Module and start the timer  */
void interruptGPTimer(void);                          /* Interrupt Service Routine of the GPT12                       */

#endif /* GPT12_TIMER_INTERRUPT_H_ */




