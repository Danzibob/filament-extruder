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
int enabled = 1;

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

int step = HIGH;

unsigned long time;
unsigned long previous_millis = 0;
float interval = 0;

int speed = 12;
float rpm = 0;

void tick();
}


namespace distrib {
int new_position = 0;
int new_position_end = 0;

int step = HIGH;
int dir = LOW;
int steps = 0;

unsigned long time;
unsigned long previous_millis = 0;
float interval = 0;

// time before we change direction
unsigned long dir_time;
unsigned long dir_previous_millis = 0;
float dir_interval = 50000 / 4; // Moves 4x correct amount

float radius = 0;

int num_steps = 7800 / 4;
int step_since_dir_change = 0;

int pos = 0;
int last_pos = 0;

int travel_begin = 0;
int travel_end = 0;
float travel_step = 0;
float travel_speed = 4;

void tick();
void reset();

}

void init();
void Var(); // TODO: AAAAAA
}
