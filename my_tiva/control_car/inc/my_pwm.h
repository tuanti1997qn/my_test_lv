#ifndef __MY_PWM__
#define __MY_PWM__

typedef enum
{
    toi = 0,
    lui = 1
} huong;

typedef enum
{
    right_motor = 0,
    left_motor = 1
} select_motor;

void init_PWM(void);
void mypwm_setpwm(select_motor channel, float duty, huong dir); // channel 1: trai, 0: phai| dir 0: thuan , 1 nghich

#endif
