#pragma once

#include <PID_v1.h>
#include "./stepper.h"

namespace pid {

enum PIDMode {
    Soft = 0, Medium, Hard, Manual
};

const int MAX_MODE = Manual;

void Brain();

int setpoint();
void setSetpoint(int setpoint);
double setpoint_mm();

PIDMode mode();
void setMode(PIDMode mode);

}
