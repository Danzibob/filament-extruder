#include "./pid.h"

void Brain () {
  //initialize the variables we're linked to
  Setpoint = diameter * 0.01;
  //turn the PID on

  //Strarting speed pot
  stsp = pullspd;

  //  if (pullspd == 0 && selectedMode == 3) {
  //
  //    enableState = 0;
  //    preenableState = enableState;
  //
  //  } else enableState = 0;
  //  preenableState = enableState;


  //PID
  Input = measure ;
  //  myPID.SetSampleTime (1000);
  double gap = abs(Setpoint - Input); //distance away from setpoint

  if (selectedMode == 0 && menuItem != 1) {
    myPID.SetOutputLimits(6, 120);
    if ( Setpoint >= 2.4) {
      myPID.SetOutputLimits(12, 100);
    }
    myPID.SetTunings(SoftKp, SoftKi, SoftKd);
    myPID.SetMode(AUTOMATIC);
    myPID.Compute();
    Pullinterval = Output;
    Pull();
    Distr();
    Spool();
  } else if (selectedMode == 1 && menuItem != 1) {
    myPID.SetOutputLimits(6, 120);
    if ( Setpoint >= 2.4) {
      myPID.SetOutputLimits(12, 100);
    }
    myPID.SetTunings(MediumKp, MediumKi, MediumKd);
    myPID.Compute();
    myPID.SetMode(AUTOMATIC);
    Pullinterval = Output;
    Pull();
    Distr();
    Spool();
  } else if (selectedMode == 2 && menuItem != 1 ) {
    myPID.SetOutputLimits(6, 120);
    if ( Setpoint >= 2.4) {
      myPID.SetOutputLimits(12, 100);
    }
    myPID.SetTunings(HardKp, HardKi, HardKd);
    if ( gap < 0.06) {
      myPID.SetTunings(Hard2Kp, Hard2Ki, Hard2Kd);
    }

    myPID.Compute();
    myPID.SetMode(AUTOMATIC);
    lastOutput = Output;
    Pullinterval = Output;
    Pull();
    Distr();
    Spool();
  } else if (selectedMode == 3 && menuItem != 1) {
    myPID.SetMode(MANUAL);
    //    Pullinterval = extspd;
    ManualPull();
    Distr();
    Spool();
  }
}
