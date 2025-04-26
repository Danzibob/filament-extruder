#pragma once

#include <PID_v1.h>
#include "./stepper.h"

namespace pid {

double setpoint, input, output;

struct PIDMode {
    double kp;
    double ki;
    double kd;
};

namespace modes {

PIDMode soft = {6.9, 0.23, 5.175};
PIDMode medium = {10.8, 0.45, 6.48};
PIDMode hard = {15.48, 0.62, 9.675};
PIDMode hard2 = {0.1548, 0.0062, 0.9675};

}

int mode = 0;

int setpoint_int = 175;
float setpoint_float = 0;
float last_output = 0;

void Brain();

}
