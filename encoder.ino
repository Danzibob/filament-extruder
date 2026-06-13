#include "encoder.h"

namespace {

RotaryEncoder enc(A0, A1, RotaryEncoder::LatchMode::FOUR3);

bool lastBtnRaw = HIGH;
bool btnStable = HIGH;
unsigned long lastBtnChange = 0;
unsigned long btnPressTime = 0;
bool holdFired = false;
bool pendingClick = false;
bool pendingHeld = false;
long lastPos = 0;

constexpr unsigned long HOLD_THRESHOLD_MS = 800;

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
        if (btnRaw == LOW && btnStable == HIGH) {
            btnPressTime = now;
            holdFired = false;
        }
        if (btnRaw == HIGH && btnStable == LOW && !holdFired)
            pendingClick = true;
        btnStable = btnRaw;
    }
    if (btnStable == LOW && !holdFired && now - btnPressTime >= HOLD_THRESHOLD_MS) {
        pendingHeld = true;
        holdFired = true;
    }

    clicked = pendingClick;
    if (pendingClick) dirty = true;
    pendingClick = false;

    held = pendingHeld;
    if (pendingHeld) dirty = true;
    pendingHeld = false;

    return dirty;
}

}
