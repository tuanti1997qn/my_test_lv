#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
extern "C"
// TivaC specific includes
{
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include "inc/main.h"
#include "inc/my_pwm.h"
#include "inc/my_PID.h"
}
// ROS includes
#include <ros.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Twist.h>
#include <ros/time.h>
#include <tf/transform_broadcaster.h>

void my_sub_vel_callback(const geometry_msgs::Twist &msg);

std_msgs::String str_msg;

// ROS nodehandle subscrible vel
ros::NodeHandle my_nh_sub;
ros::Subscriber<geometry_msgs::Twist> my_sub_vel("cmd_vel", &my_sub_vel_callback);

// ROS nodehandle
ros::NodeHandle nh;
ros::Publisher chatter("chatter", &str_msg);
char hello[13] = "Hello world!";
char my_debug[50];
double my_ex_debug;

// tf
geometry_msgs::TransformStamped t;
tf::TransformBroadcaster broadcaster;

char base_link[] = "/base_footprint";
char odom[] = "/odom";
my_pos my_car_pos;
Quaterniond my_car_quaternion;

huong dir = toi;

int main(void)
{
    // init_PWM();
    // TivaC application specific code
    MAP_FPUEnable();
    MAP_FPULazyStackingEnable();

    // TivaC system clock configuration. Set to 80MHz.
    MAP_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    // ROS nodehandle initialization and topic registration
    nh.initNode();
    // nh.advertise(chatter);
    nh.subscribe(my_sub_vel);

    /************  broadcast tf  *************/
    broadcaster.init(nh);
    /*******************************/

    main_c();

    // mypwm_setpwm(right_motor, 80, dir);
    // mypwm_setpwm(left_motor, 80, dir);

    while (1)
    {
        // // Publish message to be transmitted.
        // sprintf(my_debug, "toc do banh trai: %f", my_PID_get_vel_left_sp());
        // str_msg.data = my_debug;
        // chatter.publish(&str_msg);

        // sprintf(my_debug, "toc do banh phai: %f", my_PID_get_vel_right_sp());
        // str_msg.data = my_debug;
        // chatter.publish(&str_msg);

        // // sprintf(my_debug, "toc do set banh trai: %f", my_PID_get_vel_left_sp());
        // // str_msg.data = my_debug;
        // // chatter.publish(&str_msg);

        // // sprintf(my_debug, "toc do set banh phai: %f", my_PID_get_vel_right_sp());
        // // str_msg.data = my_debug;
        // // chatter.publish(&str_msg);

        // sprintf(my_debug, "my debug: %f", my_debug_fnc());
        // str_msg.data = my_debug;
        // chatter.publish(&str_msg);

        // sprintf(my_debug, "debug value: %f", my_debug_fnc());
        // str_msg.data = my_debug;
        // chatter.publish(&str_msg);

        /**************************** tf publish ***************************************************/
        // Publish message to be transmitted.

        my_car_pos = my_pos_get_pos();
        my_car_quaternion = my_pos_get_Quaternion();

        t.header.frame_id = odom;
        t.child_frame_id = base_link;
        t.transform.translation.x = my_car_pos.x;
        t.transform.translation.y = my_car_pos.y;
        t.transform.translation.z = 0;
        t.transform.rotation.x = my_car_quaternion.x;
        t.transform.rotation.y = my_car_quaternion.y;
        t.transform.rotation.z = my_car_quaternion.z;
        t.transform.rotation.w = my_car_quaternion.w;
        t.header.stamp = nh.now();
        broadcaster.sendTransform(t);
        /******************************************************************************************/

        // sprintf(my_debug, "x value: %f", t.transform.translation.x);
        // str_msg.data = my_debug;
        // chatter.publish(&str_msg);
        // sprintf(my_debug, "y value: %f", t.transform.translation.y);
        // str_msg.data = my_debug;
        // chatter.publish(&str_msg);


        // Handle all communications and callbacks.
        nh.spinOnce();

        // Delay for a bit.
        nh.getHardware()->delay(100);
    }
}

/*********************************************************************************************/
// callback function
void my_sub_vel_callback(const geometry_msgs::Twist &msg)
{
    float my_linear_var, my_angular_var;
    my_linear_var = msg.linear.x;
    my_angular_var = msg.angular.z;
    my_PID_set_vel(my_linear_var, my_angular_var);
    sprintf(my_debug, "toc do di thang get duoc: %f", my_linear_var);
    // my_PID_set_vel_left_sp(temp);
    // my_PID_set_vel_right_sp(temp);

    // str_msg.data = my_debug;
    // chatter.publish(&str_msg);
}