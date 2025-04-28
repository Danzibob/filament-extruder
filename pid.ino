#include "./pid.h"

namespace {

double curr_setpoint, input, output;
float last_output = 0;

pid::PIDMode curr_mode;

struct PIDDetails {
    double kp;
    double ki;
    double kd;
};

namespace modes {

PIDDetails soft = {6.9, 0.23, 5.175};
PIDDetails medium = {10.8, 0.45, 6.48};
PIDDetails hard = {15.48, 0.62, 9.675};
PIDDetails hard2 = {0.1548, 0.0062, 0.9675};

}

PID inner_pid(&input, &output, &curr_setpoint, modes::soft.kp, modes::soft.kp, modes::soft.kd, DIRECT);

}

namespace pid {

void Brain()
{
    input = sensor::width();

    if (menu_curr_item == 1) // paused
        return;

    if (curr_mode == Manual) { // manual mode
        inner_pid.SetMode(MANUAL);
        stepper::pull::tick();
        stepper::distrib::tick();
        stepper::spool::tick();
        return;
    }

    // set output limits
    inner_pid.SetOutputLimits(6, 120);
    if (setpoint() >= 240) {
        inner_pid.SetOutputLimits(12, 100);
    }

    // get tuning parameters to use
    PIDDetails *mode_tuning;
    switch (curr_mode) {
    case Soft:
        mode_tuning = &modes::soft;
        break;

    case Medium:
        mode_tuning = &modes::medium;
        break;

    case Hard:
        double gap = abs(curr_setpoint - input); // distance away from setpoint
        if (gap >= 0.06)
            mode_tuning = &modes::hard;
        else
            mode_tuning = &modes::hard2;
        break;

    default:
        setMode(Soft);
        mode_tuning = &modes::soft;
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

int setpoint() { return int(curr_setpoint * 100.0); }
void setSetpoint(int setpoint) {
    curr_setpoint = setpoint * 0.01;
}
double setpoint_mm() { return curr_setpoint; }

PIDMode mode() { return curr_mode; };
void setMode(PIDMode mode) {
    curr_mode = mode;
    // TODO: set tunings here?
};
}
