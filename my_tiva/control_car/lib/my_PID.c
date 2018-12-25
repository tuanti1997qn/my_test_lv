#include "my_PID.h"
#include "my_timer.h"
#include "my_pwm.h"
#include "my_encoder.h"

/*********************************************/
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
/*****************************************************/

float debug;
float my_debug_fnc(void)
{
    return debug;
}

PID_para
    left =
        {
            .Kp = 100,
            .Ki = 40,
            .Kd = 0},
    right =
        {
            .Kp = 100,
            .Ki = 40,
            .Kd = 0};

float vel_left, vel_right, vel_left_sp, vel_right_sp;

static float my_PID_process(float *error, float *pre_error, PID_para *para, huong *dir);

void my_PID_set_vel_left_sp(float value)
{
    vel_left_sp = value;
}

void my_PID_set_vel_right_sp(float value)
{
    vel_right_sp = value;
}

float my_PID_get_vel_left_sp(void)
{
    return vel_left_sp;
}

float my_PID_get_vel_right_sp(void)
{
    return vel_right_sp;
}

void my_PID_set_PID_params(select_motor motor, PID_para *para)
{
    if (motor == left_motor)
    {
        left.Kp = para->Kp;
        left.Ki = para->Ki;
        left.Kd = para->Kd;
    }
    else
    {
        right.Kp = para->Kp;
        right.Ki = para->Ki;
        right.Kd = para->Kd;
    }
}

void my_PID_get_PID_params(select_motor motor, PID_para *para)
{
    if (motor == left_motor)
    {
        para->Kp = left.Kp;
        para->Ki = left.Ki;
        para->Kd = left.Kd;
    }
    else
    {
        para->Kp = right.Kp;
        para->Ki = right.Ki;
        para->Kd = right.Kd;
    }
}

float my_PID_get_vel_left_PV(void)
{
    return vel_left;
}

float my_PID_get_vel_right_PV(void)
{
    return vel_right;
}

void my_PID_set_vel(float linear, float angular) // linear : m/s | angular: rad/s
{
    // cong thuc van toc dai = duong kinh * van toc goc / 2 * pi
    my_PID_set_vel_left_sp(linear - D2W * angular);
    my_PID_set_vel_right_sp(linear + D2W * angular);
}

/************************ static function ****************************************/
static float my_PID_process(float *error, float *pre_error, PID_para *para, huong *dir)
{
    float P_part, I_part, D_part;
    P_part = para->Kp * *error;
    I_part = para->Ki * T * (*error + *pre_error) / 2;
    D_part = para->Kd * (*error - *pre_error) * T;
    *pre_error = *error;
    if (P_part + I_part + D_part > 0)
    {
        *dir = toi;
        return P_part + I_part + D_part;
    }
    else
    {
        *dir = lui;
        return -(P_part + I_part + D_part);
    }
}

extern void my_custom_timer_ISR(void)
{
    static float error_left, pre_error_left, error_right, pre_error_right, duty_temp, error_left_smooth, error_right_smooth;

    huong dir;

    // //tinh van toc xe
    vel_left = ((float)my_encoder_get_left_var() * CVB / XMV) / T;
    // tinh sai so va set van toc

    error_left = vel_left_sp - vel_left;
    error_left_smooth = error_left_smooth + 0.25*(error_left - error_left_smooth); // cong thuc a= a + alpha*(b - a) de lam muot lai error
    if (vel_left_sp == 0)
    {
        mypwm_setpwm(left_motor, 0, dir);
        pre_error_left = vel_left_sp;
    }
    else
    {
        duty_temp = my_PID_process(&error_left_smooth, &pre_error_left, &left, &dir);
        mypwm_setpwm(left_motor, duty_temp, dir);
            debug = duty_temp;
        // 1.2 la con so than thanh ma minh chem vao xem thu no co chay tot ko
        // mypwm_setpwm(left_motor, 99, toi);
        // debug = my_PID_process(&error_left, &pre_error_left, &left, &dir);
        // mypwm_setpwm(left_motor, 70, toi);
    }
    //tinh van toc xe
    vel_right = ((float)my_encoder_get_right_var() * CVB / XMV) / T;
    error_right = vel_right_sp - vel_right;
    error_right_smooth = error_right_smooth + 0.25*(error_right - error_right_smooth);
    if (vel_right_sp == 0)
    {
        mypwm_setpwm(right_motor, 0, dir);
        pre_error_right = vel_left_sp;
    }
    else
    {
        mypwm_setpwm(right_motor, my_PID_process(&error_right_smooth, &pre_error_right, &right, &dir), dir); // mypwm_setpwm(right_motor, 0, toi);
        // 1.2 la con so than thanh ma minh chem vao xem thu no co chay tot ko
    }
}

//float PID_controler(  )
