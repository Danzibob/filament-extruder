#include "encoder.h"

void timerIsr() {
  encoder->service();
}
void readRotaryEncoder() {
  value += encoder->getValue();
  if (value > last) {
    last = value ;
    down = true;
  } else   if (value < last) {
    last = value;
    up = true;
  }
  lastStepperPosition = StepperPosition;
  mm = StepperPosition / StepsToTake;
}
