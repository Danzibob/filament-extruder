#include "encoder.h"

namespace {
ClickEncoder* inner_encoder;

void interrupt()
{
    inner_encoder->service();
}

}

namespace encoder {

void init()
{
    // last = -1;
    inner_encoder = new ClickEncoder(A2, A1, A3);
    Timer1.initialize(1000);
    Timer1.attachInterrupt(interrupt);
}

void read()
{
    btn = inner_encoder->getButton();
    // TODO: ideally this wouldnt be ored, but something weird is happening which causes
    // this to be discarded rn
    middle |= btn == ClickEncoder::Clicked;

    val += inner_encoder->getValue();
    if (val > last) {
        down = true;
    } else if (val < last) {
        up = true;
    }
    last = val;

    // TODO: this should probably be somewhere else
    distrib_last_stepper_pos = distrib_stepper_pos;
    mm = distrib_stepper_pos / steps_per_click;
}

}
