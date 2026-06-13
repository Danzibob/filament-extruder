#pragma once

#define clamp(lo, val, hi) min(hi, max(lo, val))

namespace stepper {

// Microstepping level per driver (set by MS1/MS2/MS3 jumpers on each A4988).
// Valid values: 1, 2, 4, 8, 16.
const int MICROSTEPS_PULL    = 1;  // no jumpers: full step
const int MICROSTEPS_DISTRIB = 16; // all 3 jumpers fitted
const int MICROSTEPS_SPOOL   = 16; // all 3 jumpers fitted

// Pins
const int PIN_STEPPER_ENABLE = 8;
const int PIN_PULLER_DIR = 2;
const int PIN_PULLER_STEP = 10; // OC1B
const int PIN_DISTRIB_DIR = 3;
const int PIN_DISTRIB_STEP = 9; // OC1A
const int PIN_SPOOL_DIR = 4;
const int PIN_SPOOL_STEP = 7;

// Enable
boolean isEnabled();
void enable();
void disable();

namespace pull {

unsigned long interval();
void setInterval(unsigned long interval);

float speed();
void setSpeed(float speed);

void tick();

float total();
void resetCounter();

}

namespace spool {

void tick();

unsigned long interval();
void setInterval(unsigned long interval);

float rpm();
void setRpm(float rpm);

}

namespace distrib {

// Physical range in hardware step pulses — set empirically by watching the
// serial position output and noting where the carriage reaches each end stop.
// With 1/16 microstepping on the distributor, this is 16× the full-step count.
const unsigned long MAX_POS = 16000;

void tick();
void reset();

unsigned long interval();
void setInterval(unsigned long interval);

void goToStart();
void goToEnd();
unsigned long pos();

unsigned long startPos();
void setStartPos(unsigned long startPos);

unsigned long endPos();
void setEndPos(unsigned long endPos);

}

void init();
}
