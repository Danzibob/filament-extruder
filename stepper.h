#pragma once

// Enable
const int PIN_STEPPER_ENABLE = 8;
int stepper_enabled = 1;
int stepper_preenable = 0;

float travel_step = 0;

float travel_speed = 4;
float pull_speed = 12;
int spool_speed = 12;

float spool_rpm = 0;

float extrude_speed = 0;
float extrude_speed2 = 0;

int distrib_new_position = 0;
int distrib_new_position_end = 0;

int travel_begin = 0;
int travel_end = 0;

// Stepper 1 - Puller - Pin Definition
const int PIN_PULLER_DIR = 2;
const int PIN_PULLER_STEP = 5;

int puller_step = HIGH;
// Stepper 1 - Puller - Time
unsigned long puller_time;
unsigned long puller_previous_millis = 0;
float puller_interval = 0;
// Stepper 1 - round counter
int puller_step_in_rev = 0;
int puller_num_revs = 0;
float puller_total = 0;
// start speed
float puller_start_speed = 0;
float puller_start_speed_min;
float puller_start_speed_max;

// Stepper 2 - Distribution - Pin Definitio
const int PIN_DISTRIB_DIR = 3;
const int PIN_DISTRIB_STEP = 6;
int distrib_step = HIGH;
int distrib_dir = LOW;
int distrib_steps = 0;
// Stepper 2 - Distribution - Speed Time
unsigned long distrib_time;
unsigned long distrib_previous_millis = 0;
float distrib_interval = 0;
// Stepper 2 - Distribution - Direction Time
unsigned long distrib_dir_time;
unsigned long distrib_dir_previous_millis = 0;
float distrib_dir_interval = 50000 / 4; // Moves 4x correct amount
// Stepper 2 - Distribution - Radious counter
float distrib_radius = 0;
// Stepper 2 reset
int distrib_num_steps = 7800 / 4;

int distrib_step_since_dir_change = 0;

int mm = 0;
int distrib_stepper_pos = 0; // To store Stepper Motor Position
int distrib_last_stepper_pos = 0; // To store Stepper Motor Position

int steps_per_click = 100; // Controls the speed of the Stepper per Rotary click

// Stepper 3 - Spool - Pin Definition
const int PIN_SPOOL_DIR = 4;
const int PIN_SPOOL_STEP = 7;
int spool_step = HIGH;
// Stepper 3 - Spool - Time
unsigned long spool_time;
unsigned long spool_previous_millis = 0;
float spool_interval = 0;

void Pull();
void ManualPull();
void Spool();
void Distr();
void resetDistr();
