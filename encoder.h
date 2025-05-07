#pragma once

#include "stepper.h"
#include <ClickEncoder.h>
#include <TimerOne.h> // trigger timer to read encoder

namespace encoder {

int16_t last, val;

ClickEncoder::Button btn;
boolean up, down, clicked = false;

void init();

// Read the encoder input, and update the variables above.
// Returns true if any input has happened
boolean read();

}
