#include "./pid.h"

void Brain () {
  //initialize the variables we're linked to
  pid_setpoint = pid_setpoint_int * 0.01;
  //turn the PID on

  //Strarting speed pot
  puller_start_speed = pull_speed;

  //  if (pullspd == 0 && selectedMode == 3) {
  //
  //    enableState = 0;
  //    preenableState = enableState;
  //
  //  } else enableState = 0;
  //  preenableState = enableState;


  //PID
  pid_input = width_curr ;
  //  myPID.SetSampleTime (1000);
  double gap = abs(pid_setpoint - pid_input); //distance away from setpoint

  if (pid_mode == 0 && menu_curr_item != 1) {
    pid.SetOutputLimits(6, 120);
    if ( pid_setpoint >= 2.4) {
      pid.SetOutputLimits(12, 100);
    }
    pid.SetTunings(kp_soft, ki_soft, kd_soft);
    pid.SetMode(AUTOMATIC);
    pid.Compute();
    puller_interval = pid_output;
    Pull();
    Distr();
    Spool();
  } else if (pid_mode == 1 && menu_curr_item != 1) {
    pid.SetOutputLimits(6, 120);
    if ( pid_setpoint >= 2.4) {
      pid.SetOutputLimits(12, 100);
    }
    pid.SetTunings(kp_medium, ki_medium, kd_medium);
    pid.Compute();
    pid.SetMode(AUTOMATIC);
    puller_interval = pid_output;
    Pull();
    Distr();
    Spool();
  } else if (pid_mode == 2 && menu_curr_item != 1 ) {
    pid.SetOutputLimits(6, 120);
    if ( pid_setpoint >= 2.4) {
      pid.SetOutputLimits(12, 100);
    }
    pid.SetTunings(kp_hard, ki_hard, kd_hard);
    if ( gap < 0.06) {
      pid.SetTunings(kp_hard2, ki_hard2, kd_hard2);
    }

    pid.Compute();
    pid.SetMode(AUTOMATIC);
    pid_last_output = pid_output;
    puller_interval = pid_output;
    Pull();
    Distr();
    Spool();
  } else if (pid_mode == 3 && menu_curr_item != 1) {
    pid.SetMode(MANUAL);
    //    Pullinterval = extspd;
    ManualPull();
    Distr();
    Spool();
  }
}
