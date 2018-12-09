#include "my_PID.h"
#include "my_timer.h"
#include "my_pwm.h"
#include "my_encoder.h"

PID_para
    left =
        {
            .Kp = 1,
            .Ki = 0.01,
            .Kd = 0.001},
    right =
        {
            .Kp = 1,
            .Ki = 0.01,
            .Kd = 0.001};

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

float my_PID_get_vel_left_PV( void )
{
    return vel_left;
}

float my_PID_get_vel_right_PV( void )
{
    return vel_right;
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
        return P_part + I_part + D_part;
    }
}

extern void my_custom_timer_ISR(void)
{
    static float error_left, pre_error_left, error_right, pre_error_right;
    huong dir;

    //tinh van toc xe
    vel_left = ((float)my_encoder_get_left_var() * cvb / xmv) / T;
	// tinh sai so va set van toc
    error_left = vel_left_sp - vel_left;
    if (vel_left_sp == 0)
    {
        mypwm_setpwm(left_motor, 0, dir);
        pre_error_left = 0;
    }
    else
        mypwm_setpwm(left_motor, my_PID_process(&error_left, &pre_error_left, &left, &dir), dir);

    //tinh van toc xe
    vel_right = ((float)my_encoder_get_right_var() * cvb / xmv) / T;
    error_right = vel_right_sp - vel_right;
    if (vel_right_sp == 0)
    {
        mypwm_setpwm(right_motor, 0, dir);
        pre_error_right = 0;
    }
    else
        mypwm_setpwm(right_motor, my_PID_process(&error_right, &pre_error_right, &right, &dir), dir);
}

//float PID_controler(  )
