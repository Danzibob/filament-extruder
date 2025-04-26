#pragma once

#include "stepper.h"
#include <ClickEncoder.h>
#include <TimerOne.h> // trigger timer to read encoder

namespace encoder {

int16_t last, val;

ClickEncoder::Button btn;
boolean up, down, middle = false;

void init();
void read();

}
