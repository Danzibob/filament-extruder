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
void setSpeed(float speed) { curr_interval = 1000 * (60 / (speed / 0.062)) / 400; }

float total() { return num_revs * 0.194; }
void resetCounter() { num_revs = 0; step_in_rev = 0; }

}


namespace spool {

void tick()
{
    unsigned long currentMillis = millis();
    // Stepper 3 - Spool
    digitalWrite(PIN_SPOOL_DIR, LOW);
    stepperTick(PIN_SPOOL_STEP, &step, &previous_millis, interval);
}

}

namespace distrib {
void tick() {
    stepperTick(PIN_DISTRIB_STEP, &step, &previous_millis, interval, &step_since_dir_change);

    // Stepper 2 - Distribution Direction
    if (step_since_dir_change >= 0 && step_since_dir_change <= steps / 2) {
        dir = LOW;
    } else if (step_since_dir_change >= steps / 2 && step_since_dir_change <= steps - 1) {
        dir = HIGH;
    }

    if (step_since_dir_change >= steps) {
        step_since_dir_change = 0;
    }

    digitalWrite(PIN_DISTRIB_DIR, dir);

}

void reset()
{
    digitalWrite(PIN_DISTRIB_DIR, LOW);
    for (int x = 0; x < num_steps; x++) {
        digitalWrite(PIN_STEPPER_ENABLE, LOW);
        digitalWrite(PIN_DISTRIB_STEP, HIGH);
        delay(1);
        digitalWrite(PIN_DISTRIB_STEP, LOW);
        delay(1);
    }
    digitalWrite(PIN_DISTRIB_DIR, HIGH);
}

}

void init()
{
    // Stepper 1 - Puller
    pinMode(PIN_PULLER_STEP, OUTPUT);
    pinMode(PIN_PULLER_DIR, OUTPUT);
    // Stepper 2 - Distribution
    pinMode(PIN_DISTRIB_STEP, OUTPUT);
    pinMode(PIN_DISTRIB_DIR, OUTPUT);
    // Stepper 3 - Spool
    pinMode(PIN_SPOOL_STEP, OUTPUT);
    pinMode(PIN_SPOOL_DIR, OUTPUT);
    // Stepper enable
    pinMode(PIN_STEPPER_ENABLE, OUTPUT);

    // ResetDistr
    distrib::reset();
}

void Var() {
    if (spool::speed <= 2) {
        spool::speed = 2;
    } else if (spool::speed >= 30) {
        spool::speed = 30;
    }
    spool::interval = spool::speed;
    spool::rpm = 300 / spool::speed;
    distrib::interval = 160 / distrib::travel_speed;

    if (distrib::travel_speed <= 0) {
        distrib::travel_speed = 0;
    }
    if (distrib::travel_speed >= 160) {
        distrib::travel_speed = 160;
    }

    distrib::new_position_end = (7900 / 4) - distrib::new_position;
    distrib::steps = 2 * distrib::travel_step;
}

}
