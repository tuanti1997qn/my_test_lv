#if !defined(__MY_ENCODER__)
#define __MY_ENCODER__

#include <stdint.h>
/**************************************************
    QEI1, PWM chanel 1: ben trai
    QEI0, PWM chanel 0: ben phai

**************************************************/
void my_encoder_init(void);
int32_t my_encoder_get_left_var(void);
int32_t my_encoder_get_right_var(void);


#endif // __MY_ENCODER__
