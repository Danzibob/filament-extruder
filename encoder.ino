#include "encoder.h"

void timerIsr()
{
    encoder->service();
}
void readRotaryEncoder()
{
    encoder_val += encoder->getValue();
    if (encoder_val > encoder_last) {
        encoder_down = true;
    } else if (encoder_val < encoder_last) {
        encoder_up = true;
    }
    encoder_last = encoder_val;
    distrib_last_stepper_pos = distrib_stepper_pos;
    mm = distrib_stepper_pos / steps_per_click;
}
