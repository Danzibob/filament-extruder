#include "./stepper.h"

namespace {

bool enabled = false;

boolean stepperTick(const int pin, int* step, unsigned long* previous_millis, unsigned int interval, int* step_in_rev = nullptr)
{
    unsigned long current_millis = millis();
    if (current_millis - *previous_millis >= interval) {
        *previous_millis = current_millis;

        if (*step == LOW) {
            *step = HIGH;
            if (step_in_rev)
                (*step_in_rev)++;
        } else
            *step = LOW;
        // set the LED with the ledState of the variable:
        digitalWrite(pin, *step);
        return true;
    }
    return false;
}

}

namespace stepper {

namespace pull {

unsigned long last_millis = 0;

const int STEPS_PER_REV = 400;
const int MM_PER_REV = 94; // diam = 30mm

int step_in_rev = 0;
int num_revs = 0;
unsigned long curr_interval = 0;

void tick()
{
    // hardware pwm: no need to do anything so just update step count
    if (last_millis == 0 || curr_interval == 0) {
        last_millis = millis();
        return;
    }
    step_in_rev += (millis() - last_millis) / interval();
    while (step_in_rev >= STEPS_PER_REV) {
        step_in_rev = step_in_rev - STEPS_PER_REV;
        num_revs++;
    }
}

unsigned long interval() { return curr_interval; }
void setInterval(unsigned long interval) {
    digitalWrite(PIN_PULLER_DIR, HIGH);
    curr_interval = interval;
    OCR0A = clamp(2, interval * 2, 255);
    OCR0B = clamp(1, interval, 128);
}

// mm / sec
float speed()
{
    float revs_per_sec = (1000.0 / float(interval())) / float(STEPS_PER_REV);
    float speed = float(MM_PER_REV) * revs_per_sec;
    if (!isfinite(speed)) {
        setSpeed(1.0);
        return 1.0;
    }
    return speed;
}
void setSpeed(float speed)
{
    float revs_per_sec = speed / float(MM_PER_REV);
    float steps_per_sec = revs_per_sec * float(STEPS_PER_REV);
    float new_interval = 1000.0 / steps_per_sec;
    if (isfinite(new_interval))
        setInterval(new_interval);
}

float total() { return num_revs * MM_PER_REV; }
void resetCounter()
{
    num_revs = 0;
    step_in_rev = 0;
}

}

namespace spool {

const int STEPS_PER_REV = 300;

int step = HIGH;

unsigned long previous_millis = 0;
unsigned long curr_interval = 0;

void tick()
{
    digitalWrite(PIN_SPOOL_DIR, HIGH);
    stepperTick(PIN_SPOOL_STEP, &step, &previous_millis, curr_interval);
}

unsigned long interval() { return curr_interval; }
void setInterval(unsigned long interval) { curr_interval = interval; }

float rpm()
{
    return (interval() * STEPS_PER_REV) / (60.0 * 1000.0);
}
void setRpm(float rpm)
{
    setInterval((60.0 * 1000.0) / (rpm * STEPS_PER_REV));
}

}

namespace distrib {

int dir = LOW;

int curr_pos = 0; // in steps in HIGH direction
int pos_start = 0;
int pos_end = MAX_POS;

unsigned long last_millis = 0;

void tick()
{
    // the alt tick of the hardware pwm: no need to do anything so just update step count
    unsigned long new_millis = millis();
    if (last_millis != 0)
        millis_since_last_step += new_millis - last_millis;
    last_millis = new_millis;

    unsigned long step_interval = max(1, (pull::interval()));
    while (millis_since_last_step >= step_interval) {
        curr_pos = clamp(0, curr_pos + (dir == HIGH ? 1 : -1), MAX_POS);
        millis_since_last_step -= step_interval;
    }

    if (curr_pos >= pos_end) {
        dir = LOW;
    } else if (curr_pos <= pos_start) {
        dir = HIGH;
    }
    digitalWrite(PIN_DISTRIB_DIR, dir);
}

void reset()
{
    dir = LOW;
    digitalWrite(PIN_DISTRIB_DIR, dir);
    digitalWrite(PIN_STEPPER_ENABLE, LOW);
    for (int x = 0; x < MAX_POS; x++) {
        digitalWrite(PIN_DISTRIB_STEP, HIGH);
        delay(1);
        digitalWrite(PIN_DISTRIB_STEP, LOW);
        delay(1);
    }

    // it would be nice to try detect stalls here or something, but not sure if its possible
    curr_pos = 0;
}

unsigned long interval() { return pull::interval(); }

int pos() { return curr_pos; }

int startPos() { return pos_start; }
void setStartPos(int startPos)
{
    pos_start = startPos;
}

int endPos() { return pos_end; }
void setEndPos(int endPos)
{
    pos_end = endPos;
}

void goToPos(int new_pos)
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
        delay(1);
        curr_pos += dir ? 1 : -1;
    }
    if (!wasEnabled)
        disable();
}

void goToStart() { goToPos(pos_start); }
void goToEnd() { goToPos(pos_end); }

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

    // TODO: find better default values probably
    pull::setInterval(100);
    spool::setInterval(100);
    distrib::reset();

    disable();

    // PWM frequency of 976.56 Hz, counting to OCR0A and turning off at OCR0B for puller_step
    TCCR0A = _BV(COM2A0) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
    TCCR0B = _BV(WGM22) | B00000011;
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
}

}
