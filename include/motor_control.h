#ifndef __MOTOR_CONTORL_H__
#define __MOTOR_CONTORL_H__

#include "type_info.h"

#define  MS32006_ADD_0  0X20  //PA3:ADA1=0
#define  MS32006_ADD_1  0X30  //PA3:ADA1=1

#define  MOT1  0X01
#define  MOT2  0X02 
#define  BOTH  0x08

#define  IRCUT_CW   0X04
#define  IRCUT_CCW  0X08
#define  IRCUT_CLOSE 0X00

#define  FULL  0X01         // Whole step drive mode
#define  HALF  0x02         // Half step drive mode

#define  MOT42 0X01         // 4-wire 2-phase motor
#define  MOT54 0X02         // 5 wire 4 phase motor

#define  OUT_INT 0X01       // Interrupt output
#define  OUT_PWM 0X02       // PWM output

#define  CW    0X01         // Forward rotation
#define  CCW   0X02         // reversing rotation

#define  TRUE   1
#define  FALSE  0

#define MOTOR_MAX_STEP  4095

#define MOTOR_MAX_Y_STEP    

/**
 * @brief   Read temperature status
 * 
 * @param[in]   fd
 * @param[in]   mot
 * @return  unsigned char 
 * 
 */
unsigned char ms32006_read_temp_state(unsigned int fd,  unsigned int mot);

/**
 * @brief   Read motor running state
 * 
 * @param[in]   fd
 * @param[in]   mot     
 * @return  unsigned char 
 * 
 */
unsigned char ms32006_read_run_state(unsigned int fd,  unsigned int mot);

/**
 * @brief   Read whether the buffer contains data
 * 
 * @param[in]   fd
 * @param[in]   mot
 * @return  unsigned char 
 * 
 */
unsigned char ms32006_read_buffer_state(unsigned int fd,  unsigned int mot);

/**
 * @brief   Clear motor movement steps
 * 
 * @param[in]   fd
 * @param[in]   mot     motorA\motorB
 * 
 */
void ms32006_clear_step(unsigned int fd,  unsigned int mot);

/**
 * @brief   Read motor movement steps
 * 
 * @param[in]   fd
 * @param[in]   mot         motorA\motorB
 * @return  unsigned int 
 * 
 */
unsigned int ms32006_readstep(unsigned int fd,  unsigned int mot);

/**
 * @brief   PWM output
 * 
 * @param[in]   fd
 * @param[in]   pwm_frequ       frequency 
 * @param[in]   pwm_duty        period 
 * 
 */
void ms32006_pwm_out(unsigned int fd, unsigned int pwm_frequ, unsigned int pwm_duty);

/**
 * @brief   Control the IRCUT switch 
 * 
 * @param[in]   fd
 * @param[in]   irct_state      state|on|off
 * 
 */
void ms32006_ircut(unsigned int fd, unsigned int ircut_state);

/**
 * @brief   close motor
 * 
 * @param[in]   fd          
 * @param[in]   set_mot     Zero stops
 * 
 */
void ms32006_close(unsigned int fd, unsigned int set_mot);

/**
 * @brief   Control motor stop
 * 
 * @param[in]   fd          
 * @param[in]   set_mot     If it's not zero, it stops
 * 
 */
void ms32006_pause(unsigned int fd, unsigned int set_mot);

/**
 * @brief   Control motor movement
 * 
 * @param[in]   fd              
 * @param[in]   mot1_dir        mot1 dir| CW OR CCW
 * @param[in]   mot1_pps        pps
 * @param[in]   mot1_step       Number of steps per exercise
 * @param[in]   mot2_dir        mot2 dir| CW OR CCW
 * @param[in]   mot2_pps        pps
 * @param[in]   mot2_step       Number of steps per exercise
 * 
 */
void ms32006_move(unsigned int fd, unsigned int mot1_dir ,unsigned int mot1_pps,unsigned int mot1_step , unsigned int mot2_dir ,unsigned int mot2_pps,unsigned int mot2_step);

/**
 * @brief   init
 * 
 * @param[in]   fd      
 * 
 */
void ms32006_init(int fd);

#endif
