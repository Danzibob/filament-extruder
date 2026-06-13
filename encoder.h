#pragma once

#include "stepper.h"
#include <RotaryEncoder.h>

namespace encoder {

boolean up, down, clicked, held = false;

void init();

// Read the encoder input, and update the variables above.
// Returns true if any input has happened
boolean read();

}
