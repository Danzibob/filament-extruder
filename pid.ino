#include "./pid.h"

namespace {

PID inner_pid(&pid::input, &pid::output, &pid::setpoint, pid::modes::soft.kp, pid::modes::soft.kp, pid::modes::soft.kd, DIRECT);

}

namespace pid {

void Brain()
{
    // initialize the variables we're linked to
    setpoint = setpoint_int * 0.01;
    // Starting speed
    stepper::pull::start_speed = stepper::pull::speed;

    input = sensor::width();

    if (menu_curr_item == 1) // paused
        return;

    if (pid::mode == 3) { // manual mode
        inner_pid.SetMode(MANUAL);
        stepper::pull::tick();
        stepper::distrib::tick();
        stepper::spool::tick();
        return;
    }

    // set output limits
    inner_pid.SetOutputLimits(6, 120);
    if (setpoint >= 2.4) {
        inner_pid.SetOutputLimits(12, 100);
    }

    // get tuning parameters to use
    PIDMode *mode_tuning;
    switch (mode) {
    case 0:
        mode_tuning = &pid::modes::soft;
        break;

    case 1:
        mode_tuning = &pid::modes::medium;
        break;

    case 2:
        double gap = abs(setpoint - input); // distance away from setpoint
        if (gap >= 0.06)
            mode_tuning = &pid::modes::hard;
        else
            mode_tuning = &pid::modes::hard2;
        break;

    default:
        mode = 0;
        mode_tuning = &pid::modes::soft;
    };


    // get pid output
    inner_pid.SetTunings(mode_tuning->kp, mode_tuning->ki, mode_tuning->kd);
    inner_pid.SetMode(AUTOMATIC);
    inner_pid.Compute();
    last_output = output;
    stepper::pull::interval = output;

    // do the thing
    stepper::pull::tick();
    stepper::distrib::tick();
    stepper::spool::tick();
}
}
