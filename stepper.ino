#include "./stepper.h"

namespace {
void stepperTick(const int pin, int *step, unsigned long *previous_millis, int interval, int *step_in_rev=nullptr) {
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
    }
}

}

namespace stepper {

// Puller
namespace pull {

int step = HIGH;
unsigned long previous_millis = 0;
unsigned long curr_interval = 9000;

const int STEPS_PER_REV = 400;

int step_in_rev = 0;
int num_revs = 0;

void tick()
{
    digitalWrite(PIN_PULLER_DIR, HIGH);
    stepperTick(PIN_PULLER_STEP, &step, &previous_millis, curr_interval, &step_in_rev);
    if (step_in_rev == STEPS_PER_REV) {
        step_in_rev = 0;
        num_revs++;
    }
}

unsigned long interval() { return curr_interval; }
void setInterval(unsigned long interval) { curr_interval = interval; }

// TODO: not convinced either of these are right
float speed() { return 60 / ((interval() * 400) / 1000) * 0.062; }
void setSpeed(float speed) { setInterval(1000 * (60 / (speed / 0.062)) / 400); }

float total() { return num_revs * 0.194; }
void resetCounter() { num_revs = 0; step_in_rev = 0; }

}


namespace spool {

int step = HIGH;

unsigned long previous_millis = 0;
float curr_interval = 0;

void tick()
{
    digitalWrite(PIN_SPOOL_DIR, LOW);
    stepperTick(PIN_SPOOL_STEP, &step, &previous_millis, curr_interval);
}

float interval() { return curr_interval; }
void setInterval(float interval) { curr_interval = interval; }

float rpm() { return 300/interval(); }
void setRpm(float rpm) { setInterval(300 / rpm); }

}

namespace distrib {

const int MAX_POS = 7900/4;

int step = HIGH;
int dir = LOW;

unsigned long time;
unsigned long previous_millis = 0;
float curr_interval = 0;

int curr_pos = 0; // in steps in HIGH direction
int pos_start = 0;
int pos_end = MAX_POS;

void tick() {
    stepperTick(PIN_DISTRIB_STEP, &step, &previous_millis, curr_interval);

    if (dir)
        curr_pos++;
    else
        curr_pos = max(pos - 1, 0);

    int steps_per_dir_change = (pos_end - pos_start) / 2;
    if (curr_pos >= pos_end || curr_pos <= pos_start) {
        dir = !dir;
        digitalWrite(PIN_DISTRIB_DIR, dir);
    }
}

void reset()
{
    digitalWrite(PIN_DISTRIB_DIR, LOW);
    for (int x = 0; x < MAX_POS; x++) {
        digitalWrite(PIN_STEPPER_ENABLE, LOW);
        digitalWrite(PIN_DISTRIB_STEP, HIGH);
        delay(1);
        digitalWrite(PIN_DISTRIB_STEP, LOW);
        delay(1);
    }

    // it would be nice to try detect stalls here or something, but not sure if its possible

    digitalWrite(PIN_DISTRIB_DIR, HIGH);
    curr_pos = 0;
}

float interval() { return curr_interval; }
void setInterval(float interval) { curr_interval = interval; }

int pos() { return curr_pos; }

int startPos() { return pos_start; }
void setStartPos(int startPos) {
    pos_start = startPos;
}

int endPos() { return pos_end; }
void setEndPos(int endPos) {
    pos_end = endPos;
}

void goToPos(int new_pos) {
    if (curr_pos == new_pos)
        return;

    dir = curr_pos < new_pos ? HIGH : LOW;
    digitalWrite(PIN_DISTRIB_DIR, dir);
    digitalWrite(PIN_STEPPER_ENABLE, LOW);
    while (curr_pos != new_pos) {
        digitalWrite(PIN_DISTRIB_STEP, HIGH);
        delay(1);
        digitalWrite(PIN_DISTRIB_STEP, LOW);
        delay(1);
        curr_pos += dir ? 1 : -1;
    }

    dir = !dir;
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

    distrib::reset();
}

}
