#pragma once

#include <PID_v1.h>

double pid_setpoint, pid_input, pid_output;
//Define the aggressive and conservative Tuning Parameters
double kp_soft = 6.9 ,   ki_soft = 0.23,   kd_soft = 5.175;
double kp_medium = 10.8 , ki_medium = 0.45, kd_medium = 6.48;
double kp_hard = 15.48,      ki_hard = 0.62,   kd_hard = 9.675 ;
double kp_hard2 = 0.1548,      ki_hard2 = 0.0062,   kd_hard2 = 0.9675 ;

int pid_mode = 0;

int pid_setpoint_int = 175;
float pid_setpoint_float = 0;
float pid_last_output = 0;


PID pid(&pid_input, &pid_output, &pid_setpoint, kp_soft, ki_soft, kd_soft, DIRECT);

void Brain();
