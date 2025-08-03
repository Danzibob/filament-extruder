#pragma once

#include "./stepper.h"
#include <PID_v1.h>

namespace pid {

enum PIDMode {
    Hard,
    Manual
};

const int MAX_MODE = Manual;

void loop();

int setpoint();
void setSetpoint(int setpoint);
double setpoint_mm();

PIDMode mode();
void setMode(PIDMode mode);

}
