/*
 * MotorControl.h
 *
 *  Created on: 2023. 7. 10.
 *      Author: pc_2
 */

#ifndef GPT12_PWM_CAPTURE_H_
#define GPT12_PWM_CAPTURE_H_

//#define PWM_A_PIN             &MODULE_P23,0     //PWM6
//#define DIR_A_PIN             &MODULE_P00,6     //DIGITAL
//#define DIR_B_PIN             &MODULE_P00,9     //DIGITAL

#define PWM_A_PIN             &MODULE_P02,3     //PWM6
#define DIR_A_PIN             &MODULE_P10,4     //DIGITAL


/*********************************************************************************************************************/
/*------------------------------------------------Function Prototypes------------------------------------------------*/
/*********************************************************************************************************************/
extern float pwm_output;
void init_GPT12_module(void);
int  timeNow(void);
void runGpt12PWM(void);
#endif /* GPT12_PWM_CAPTURE_H_ */
