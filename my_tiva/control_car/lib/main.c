#include "main.h"
#include "my_pwm.h"
#include "my_PID.h"
#include "my_encoder.h"
#include "my_timer.h"

int32_t qeiPosition, x;
float a,b;
// int test_qei(void);
// int test_PWM(void);

int main_c(void)
{
	// Set the clocking to run directly from the crystal.
	SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
	// test_qei();
	// config();
	// my_imer_init();
	init_PWM();
	my_encoder_init();
	my_timer_init();

	my_PID_set_vel_left_sp(10);
	my_PID_set_vel_right_sp(10);

	while (1)
	{
		a= my_PID_get_vel_left_PV();
		b = my_PID_get_vel_right_PV();
		// mypwm_setpwm(1,50,1);
		// qeiPosition = my_encoder_get_left_var();
		// x = my_encoder_get_right_var();
		// qeiPosition = QEIPositionGegt(QEI1_BASE);
		// SysCtlDelay(1000);
		//qeiPosition = my_encoder_getvar();
		// mypwm_setpwm(1, 25, 0);
		// SysCtlDelay(SysCtlClockGet()/3);
		// mypwm_setpwm(0, 60, 0);
	}
	return 0;
	
}
