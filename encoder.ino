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

boolean read()
{
    boolean dirty = false;
    btn = inner_encoder->getButton();

    if ((btn == ClickEncoder::Clicked) != clicked)
        dirty = true;
    clicked = btn == ClickEncoder::Clicked;

    val += inner_encoder->getValue();
    if (val > last) {
        dirty = true;
        down = true;
    } else if (val < last) {
        dirty = true;
        up = true;
    }
    last = val;

    return dirty;
}

}
