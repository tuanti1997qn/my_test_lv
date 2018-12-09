#if !defined(__MY_PID__)
#define __MY_PID__

#include "my_pwm.h"

#define T 0.01
#define xmv 374 // xung moi vong
#define cvb 5 // chu vi banh 5 cm

typedef struct
{
    float Kp;
    float Kd;
    float Ki;
} PID_para;

void my_PID_set_vel_left_sp(float value);
void my_PID_set_vel_right_sp(float value);
float my_PID_get_vel_left_sp(void);
float my_PID_get_vel_right_sp(void);
void my_PID_set_PID_params(select_motor motor, PID_para *para);
void my_PID_get_PID_params(select_motor motor, PID_para *para);
float my_PID_get_vel_left_PV(void);
float my_PID_get_vel_right_PV(void);

#endif // __MY_PID__
