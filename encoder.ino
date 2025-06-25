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
    last = 0;
    pinMode(A3, OUTPUT);
    digitalWrite(A3, HIGH);
    inner_encoder = new ClickEncoder(A1, A0, A2);

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
    down = val < last;
    up = val > last;
    if (val != last)
        dirty = true;
    last = val;

    return dirty;
}

}
