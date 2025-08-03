#include "./pid.h"

namespace {

int curr_setpoint_int = 175;
double curr_setpoint = 1.75, input, output;
float last_output = 0;

pid::PIDMode curr_mode;

struct PIDDetails {
    double kp;
    double ki;
    double kd;
};

namespace modes {

PIDDetails hard = { 15.48, 0.62, 9.675 };
PIDDetails hard2 = { 0.1548, 0.0062, 0.9675 };

}

PID inner_pid(&input, &output, &curr_setpoint, modes::hard.kp, modes::hard.ki, modes::hard.kd, DIRECT);

}

namespace pid {

void loop()
{
    input = sensor::width();

    if (curr_mode == Manual) { // manual mode
        inner_pid.SetMode(MANUAL);
        stepper::pull::tick();
        stepper::distrib::tick();
        stepper::spool::tick();
        return;
    }

    // set output limits
    inner_pid.SetOutputLimits(12, 128);

    // get tuning parameters to use
    PIDDetails* mode_tuning;
    switch (curr_mode) {
    case Hard: {
        double gap = abs(curr_setpoint - input); // distance away from setpoint
        if (gap >= 0.06)
            mode_tuning = &modes::hard;
        else
            mode_tuning = &modes::hard2;
        break;
    }

    case Manual: // unreachable
    default:
        setMode(Hard);
        mode_tuning = &modes::hard;
    };

    // get pid output
    inner_pid.SetTunings(mode_tuning->kp, mode_tuning->ki, mode_tuning->kd);
    inner_pid.SetMode(AUTOMATIC);
    inner_pid.Compute();
    last_output = output;
    stepper::pull::setInterval(output);

    // do the thing
    stepper::pull::tick();
    stepper::distrib::tick();
    stepper::spool::tick();
}

int setpoint() { return curr_setpoint_int; }
void setSetpoint(int setpoint)
{
    curr_setpoint_int = setpoint;
    curr_setpoint = setpoint * 0.01;
}
double setpoint_mm() { return curr_setpoint; }

PIDMode mode() { return curr_mode; };
void setMode(PIDMode mode)
{
    curr_mode = mode;
};

}
