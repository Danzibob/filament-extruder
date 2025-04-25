#pragma once

#include <ClickEncoder.h>
#include "stepper.h"

ClickEncoder *encoder;
int16_t last, value;

boolean up = false;
boolean down = false;
boolean middle = false;

void timerIsr();
void readRotaryEncoder();
