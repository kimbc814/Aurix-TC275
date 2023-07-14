/*
 * MotorControl.h
 *
 *  Created on: 2023. 7. 10.
 *      Author: pc_2
 */
/*
#ifndef MotorControl_H
#define MotorControl_H




#define PWM_A_PIN             &MODULE_P02,3     //PWM6
#define DIR_A_PIN             &MODULE_P10,4     //DIGITAL

void initGtmATomPwm(void);
void setPWM(int dutyCycle);
//void initMotorDriver(void);
#endif
*/
#ifndef GPT12_PWM_CAPTURE_H_
#define GPT12_PWM_CAPTURE_H_

#define PWM_A_PIN             &MODULE_P02,3     //PWM6
#define DIR_A_PIN             &MODULE_P10,4     //DIGITAL
#define m_2_pulse     1135.282             // 1m 당 pulse 수  확인 해야 함
#define pulse_2_m     1./m_2_pulse         // pulse 당 m  확인 해야 함
#define vel_2_pulse   m_2_pulse/500.  // 500Hz 제어 주기에서 속도와 Δpulse 변환 값

//#define PWM_A_PIN             &MODULE_P00,5     //PWM6
//#define DIR_A_PIN             &MODULE_P00,9     //DIGITAL
//#define DIR_A_PIN1             &MODULE_P00,9     //DIGITAL


/*********************************************************************************************************************/
/*------------------------------------------------Function Prototypes------------------------------------------------*/
/*********************************************************************************************************************/
extern float target_speed;

void init_GPT12_module(void);
int  timeNow(void);
void runGpt12PWM(void);
#endif /* GPT12_PWM_CAPTURE_H_ */
