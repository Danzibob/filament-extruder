#pragma once

#include <PID_v1.h>

double Setpoint, Input, Output;
//Define the aggressive and conservative Tuning Parameters
double SoftKp = 6.9 ,   SoftKi = 0.23,   SoftKd = 5.175;
double MediumKp = 10.8 , MediumKi = 0.45, MediumKd = 6.48;
double HardKp = 15.48,      HardKi = 0.62,   HardKd = 9.675 ;
double Hard2Kp = 0.1548,      Hard2Ki = 0.0062,   Hard2Kd = 0.9675 ;

int selectedMode = 0;

int diameter = 175;
float intdiameter = 0;
float lastOutput = 0;


PID myPID(&Input, &Output, &Setpoint, SoftKp, SoftKi, SoftKd, DIRECT);

void Brain();
