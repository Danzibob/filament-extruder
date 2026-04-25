#include "encoder.h"

namespace {

RotaryEncoder enc(A1, A0, RotaryEncoder::LatchMode::FOUR3);

bool lastBtnRaw = HIGH;
bool btnStable = HIGH;
unsigned long lastBtnChange = 0;
bool pendingClick = false;
long lastPos = 0;

}

namespace encoder {

void init()
{
    pinMode(A3, OUTPUT);
    digitalWrite(A3, HIGH);
    pinMode(A2, INPUT_PULLUP);
}

boolean read()
{
    boolean dirty = false;

    enc.tick();
    long newPos = enc.getPosition();
    long delta = newPos - lastPos;
    lastPos = newPos;

    up = delta > 0;
    down = delta < 0;
    if (delta != 0)
        dirty = true;

    // Button debounce: emit one click on stable release
    bool btnRaw = digitalRead(A2);
    unsigned long now = millis();
    if (btnRaw != lastBtnRaw) {
        lastBtnRaw = btnRaw;
        lastBtnChange = now;
    }
    if (now - lastBtnChange >= 50 && btnRaw != btnStable) {
        if (btnRaw == HIGH && btnStable == LOW)
            pendingClick = true;
        btnStable = btnRaw;
    }

    clicked = pendingClick;
    if (pendingClick)
        dirty = true;
    pendingClick = false;

    return dirty;
}

}
