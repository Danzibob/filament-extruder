#pragma once

#include "stepper.h"
#include <ClickEncoder.h>

ClickEncoder* encoder;
int16_t encoder_last, encoder_val;

boolean encoder_up = false;
boolean encoder_down = false;
boolean encoder_middle = false;

void timerIsr();
void readRotaryEncoder();
