#if !defined(__MY_PID__)
#define __MY_PID__

#include "my_pwm.h"

#define T 0.01
#define XMV 374 // xung moi vong
#define CVB 0.195 // chu vi banh 5 cm
#define D2W 0.18// distance 2 wheel
#define MY_PI 3.1415926535897 // so pi :V

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
void my_PID_set_vel(float linear, float angular);

float my_debug_fnc(void);

#endif // __MY_PID__
