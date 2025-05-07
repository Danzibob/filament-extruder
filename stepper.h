#pragma once

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

float interval();
void setInterval(float interval);

float rpm();
void setRpm(float rpm);

}

namespace distrib {

void tick();
void reset();

float interval();
void setInterval(float interval);

int pos();

int startPos();
void setStartPos(int startPos);

int endPos();
void setEndPos(int endPos);

void goToStart();
void goToEnd();

}

void init();
}
