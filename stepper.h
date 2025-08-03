#pragma once

#define clamp(lo, val, hi) min(hi, max(lo, val))

namespace stepper {

// Pins
const int PIN_STEPPER_ENABLE = 8;
const int PIN_PULLER_DIR = 2;
const int PIN_PULLER_STEP = 5;
const int PIN_DISTRIB_DIR = 3;
const int PIN_DISTRIB_STEP = 6;
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

const unsigned long MAX_POS = (unsigned long)(1900) * (unsigned long)(32);

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
