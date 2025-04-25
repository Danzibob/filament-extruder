#pragma once

//Enable
const int enablePin = 8;
int enableState = 1;
int preenableState = 0;

float travel = 0;
float travelspd = 4;
float pullspd = 12;
int spoolspd = 12;

float spoolRPM = 0 ;
int kilograms = 0;
float extspd = 0;
float extspd2 = 0;

int newpositionEnd = 0;
int newposition = 0;
int TravelBegin = 0;
int TravelEnd = 0;

//Stepper 1 - Puller - Pin Definition
const int         PullpinDir = 2;
const int         PullpinStep = 5;
int               PullpinStepState = HIGH;
//Stepper 1 - Puller - Time
unsigned long     Pulltime;
unsigned long     PullpreviousMillis = 0;
float             Pullinterval = 0 ;
//Stepper 1 - round counter
int               r = 0;
int               R = 0;
float             Total = 0;
// start speed
float             stsp = 0;
float             stspMin ;
float             stspMax ;

//Stepper 2 - Distribution - Pin Definitio
const int         DistrpinDir = 3;
const int         DistrpinStep = 6;
int               DistrpinStepState = HIGH;
int               DistrpinDirState = LOW;
int               DistributionSteps = 0;
//Stepper 2 - Distribution - Speed Time
unsigned long     Distrtime;
unsigned long     DistrpreviousMillis = 0;
float             Distrinterval = 0;
//Stepper 2 - Distribution - Direction Time
unsigned long     DirDistrtime;
unsigned long     DirDistrpreviousMillis = 0;
float             DirDistrinterval = 50000/4; // Moves 4x correct amount
// Stepper 2 - Distribution - Radious counter
float             rad = 0;
// Stepper 2 reset
int               numStepMotore = 7800 / 4;
int               steppini = 0;

int               mm = 0 ;
int               StepperPosition = 0;  // To store Stepper Motor Position
int               lastStepperPosition = 0;  // To store Stepper Motor Position

int               StepsToTake = 100;    // Controls the speed of the Stepper per Rotary click


//const int numStepMotore = 200;
//Stepper 3 - Spool - Pin Definition
const int         SpoolpinDir = 4;
const int         SpoolpinStep = 7;
int               SpoolpinStepState = HIGH;
//Stepper 3 - Spool - Time
unsigned long     Spooltime;
unsigned long     SpoolpreviousMillis = 0;
float             Spoolinterval = 0;

void Pull();
void ManualPull();
void Spool();
void Distr();
void resetDistr();
