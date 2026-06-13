#include "./stepper.h"

namespace {

bool enabled = false;

boolean stepperTick(const int pin, int* step, unsigned long* previous_micros, unsigned long interval, int* step_in_rev = nullptr)
{
    unsigned long current_micros = micros();
    if (current_micros - *previous_micros >= interval) {
        *previous_micros = current_micros;

        if (*step == LOW) {
            *step = HIGH;
            if (step_in_rev)
                (*step_in_rev)++;
        } else
            *step = LOW;
        digitalWrite(pin, *step);
        return true;
    }
    return false;
}

}

namespace stepper {

namespace pull {

unsigned long last_micros = 0;
unsigned long micros_since_last_step = 0;

const int STEPS_PER_REV = 400 * MICROSTEPS_PULL;
const int MM_PER_REV = 94; // diam = 30mm

int step_in_rev = 0;
int num_revs = 0;
unsigned long curr_interval = 0;

void tick()
{
    // The puller STEP pin is driven by Timer1 OC1B hardware PWM, so there is
    // nothing to toggle here. We only track elapsed time to keep the odometer
    // (step_in_rev / num_revs) in sync with the actual hardware output.
    if (last_micros == 0 || curr_interval == 0) {
        last_micros = micros();
        return;
    }
    unsigned long now = micros();
    micros_since_last_step += now - last_micros;
    last_micros = now;

    // Exact hardware step period: 1024*(OCR1A+1)/F_CPU µs, OCR1A = clamp(2, 2*interval, 255).
    unsigned long ocr1a = min(255UL, max(2UL, curr_interval * 2UL));
    unsigned long step_period_us = 1024UL * (ocr1a + 1UL) / 16UL;
    while (micros_since_last_step >= step_period_us) {
        micros_since_last_step -= step_period_us;
        if (++step_in_rev >= STEPS_PER_REV) {
            step_in_rev = 0;
            num_revs++;
        }
    }
}

unsigned long interval() { return curr_interval; }
void setInterval(unsigned long interval)
{
    digitalWrite(PIN_PULLER_DIR, HIGH);
    curr_interval = interval;
    // In Fast PWM mode 15 (TOP=OCR1A), OC1A toggles at TOP and OC1B clears at
    // OCR1B. Setting OCR1A = 2*interval and OCR1B = interval gives ~50% duty
    // on both outputs at the same frequency.
    OCR1A = clamp(2, interval * 2, 255);
    OCR1B = clamp(1, interval, 128);
}

// mm / sec — derived from actual hardware step period: 1024*(OCR1A+1)/F_CPU µs
float speed()
{
    if (curr_interval == 0) return 0;
    unsigned long ocr1a = min(255UL, max(2UL, curr_interval * 2UL));
    float step_period_us = 1024.0f * float(ocr1a + 1UL) / 16.0f;
    float steps_per_sec = 1000000.0f / step_period_us;
    float spd = float(MM_PER_REV) * steps_per_sec / float(STEPS_PER_REV);
    if (!isfinite(spd)) { setSpeed(1.0); return 1.0; }
    return spd;
}
void setSpeed(float spd)
{
    if (spd <= 0) return;
    float step_period_us = float(MM_PER_REV) * 1000000.0f / (spd * float(STEPS_PER_REV));
    // OCR1A = step_period_us * F_CPU_MHz / 1024 - 1 = step_period_us / 64 - 1
    unsigned long ocr1a = max(2UL, (unsigned long)(step_period_us / 64.0f) - 1UL);
    setInterval(clamp(12UL, ocr1a / 2UL, 128UL));
}

float total() { return num_revs * MM_PER_REV; }
void resetCounter()
{
    num_revs = 0;
    step_in_rev = 0;
    micros_since_last_step = 0;
}

}

namespace spool {

const int STEPS_PER_REV = 200 * MICROSTEPS_SPOOL;

int step = HIGH;

unsigned long previous_micros = 0;
unsigned long curr_interval = 0; // in microseconds

void tick()
{
    digitalWrite(PIN_SPOOL_DIR, LOW);
    stepperTick(PIN_SPOOL_STEP, &step, &previous_micros, curr_interval);
}

unsigned long interval() { return curr_interval; }
void setInterval(unsigned long interval) { curr_interval = interval; }

float rpm()
{
    if (curr_interval == 0) return 0;
    return 60000000.0 / (float(curr_interval) * STEPS_PER_REV);
}
void setRpm(float rpm)
{
    if (rpm > 0)
        setInterval((unsigned long)(60000000.0 / (rpm * STEPS_PER_REV)));
}

}

namespace distrib {

int dir = LOW;

unsigned long curr_pos = 0; // in steps in HIGH direction
unsigned long pos_start = 0;
unsigned long pos_end = MAX_POS;

unsigned long last_millis = 0;
unsigned long millis_since_last_step = 0;

void tick()
{
    // The distributor STEP pin is driven by Timer1 OC1A hardware PWM, so we
    // cannot toggle it manually. Instead, track elapsed time to estimate how
    // far the carriage has travelled and update curr_pos accordingly.
    unsigned long new_millis = millis();
    if (last_millis != 0)
        millis_since_last_step += new_millis - last_millis;
    last_millis = new_millis;

    // Distributor step period = 2 × 1024 × (OCR1A+1) / F_CPU
    // OCR1A ≈ 2 × pull_interval, so period ≈ pull_interval × 256 / 1000 ms.
    unsigned long step_interval = max(1UL, pull::interval() * 256UL / 1000UL);
    while (millis_since_last_step >= step_interval) {
        millis_since_last_step -= step_interval;
        // Check boundary before moving to avoid unsigned underflow at pos_start=0.
        // Direction reverses inside the loop so subsequent steps use the new direction.
        if (dir == HIGH) {
            if (curr_pos < pos_end) curr_pos++;
            else dir = LOW;
        } else {
            if (curr_pos > pos_start) curr_pos--;
            else dir = HIGH;
        }
    }
    digitalWrite(PIN_DISTRIB_DIR, dir);
}

void reset()
{
    // Blind home: drive toward the LOW end at ~2604 Hz (clk/1024, OCR1A=2).
    // MAX_POS=16000 microsteps / 2604 Hz = 6.1 s worst case; 8 s gives 30% margin.
    dir = LOW;
    digitalWrite(PIN_DISTRIB_DIR, dir);
    OCR1A = 2;
    OCR1B = 1;

    digitalWrite(PIN_STEPPER_ENABLE, LOW);
    delay(8000);
    digitalWrite(PIN_STEPPER_ENABLE, HIGH);

    curr_pos = 0;
    dir = HIGH;
    digitalWrite(PIN_DISTRIB_DIR, dir);
}

unsigned long interval() { return pull::interval(); }

unsigned long pos() { return curr_pos; }

unsigned long startPos() { return pos_start; }
void setStartPos(unsigned long startPos)
{
    pos_start = startPos;
}

unsigned long endPos() { return pos_end; }
void setEndPos(unsigned long endPos)
{
    pos_end = endPos;
}

void goToPos(unsigned long new_pos)
{
    if (curr_pos == new_pos)
        return;

    bool wasEnabled = isEnabled();
    dir = new_pos > curr_pos ? HIGH : LOW;
    digitalWrite(PIN_DISTRIB_DIR, dir);
    digitalWrite(PIN_STEPPER_ENABLE, LOW);
    while (curr_pos != new_pos) {
        digitalWrite(PIN_DISTRIB_STEP, HIGH);
        delay(1);
        digitalWrite(PIN_DISTRIB_STEP, LOW);
        curr_pos += dir ? 1 : -1;
    }
    if (!wasEnabled)
        disable();
}

// TODO: make these do things again
void goToStart()
{
    // goToPos(pos_start);
}
void goToEnd()
{
    // goToPos(pos_end);
}

}

void init()
{
    pinMode(PIN_PULLER_STEP, OUTPUT);
    pinMode(PIN_PULLER_DIR, OUTPUT);

    pinMode(PIN_DISTRIB_STEP, OUTPUT);
    pinMode(PIN_DISTRIB_DIR, OUTPUT);

    pinMode(PIN_SPOOL_STEP, OUTPUT);
    pinMode(PIN_SPOOL_DIR, OUTPUT);

    pinMode(PIN_STEPPER_ENABLE, OUTPUT);

    // Configure Timer1 for Fast PWM mode 15 (TOP=OCR1A) with clk/1024 prescaler.
    // OC1A (D9, distrib STEP) toggles at TOP; OC1B (D10, puller STEP) clears at OCR1B.
    // Timer0 is left untouched so millis()/delay()/micros() remain accurate.
    // analogWrite() on D9/D10 will not work while this config is active.
    TCCR1A = _BV(COM1A0) | _BV(COM1B1) | _BV(WGM11) | _BV(WGM10);
    TCCR1B = _BV(WGM13) | _BV(WGM12) | B00000101;

    distrib::reset();
    pull::setInterval(100);
    spool::setRpm(5.0);

    disable();
}

boolean isEnabled()
{
    return enabled;
}

void enable()
{
    enabled = true;
    digitalWrite(PIN_STEPPER_ENABLE, LOW);
}

void disable()
{
    enabled = false;
    digitalWrite(PIN_STEPPER_ENABLE, HIGH);
    // Reset time references so idle periods don't count as travel when re-enabled.
    distrib::last_millis = 0;
    pull::last_micros = 0;
    pull::micros_since_last_step = 0;
}

}
