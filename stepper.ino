#include "./stepper.h"

// Puller
void Pull() {
  unsigned long currentMillis = millis();
  //Stepper 1 - Puller
  digitalWrite(PIN_PULLER_DIR, HIGH);
  if (currentMillis - puller_previous_millis >= puller_interval) {
    // save the last time you blinked the LED
    puller_previous_millis = currentMillis;
    // if the LED is off turn it on and vice-versa:
    if (puller_step == LOW)
      puller_step = HIGH , puller_step_in_rev++;
    else
      puller_step = LOW;
    // set the LED with the ledState of the variable:
    digitalWrite(PIN_PULLER_STEP, puller_step);
  }
  if (puller_step_in_rev == 400) {
    puller_step_in_rev = 0;
    puller_num_revs++;
  }

}

void ManualPull(){
  Pull(); // this is the same lol
}

// Spooler
void Spool() {
  unsigned long currentMillis = millis();
  //Stepper 3 - Spool
  digitalWrite(PIN_SPOOL_DIR, LOW);
  if (currentMillis - spool_previous_millis >= spool_interval) {
    // save the last time you blinked the LED
    spool_previous_millis = currentMillis;
    // if the LED is off turn it on and vice-versa:
    if (spool_step == LOW)
      spool_step = HIGH;
    else
      spool_step = LOW;
    // set the LED with the ledState of the variable:
    digitalWrite(PIN_SPOOL_STEP, spool_step);
  }
}

//Distribution
void Distr() {
  unsigned long currentMillis = millis();
  //Stepper 2 - Distribution
  if (currentMillis - distrib_previous_millis >= distrib_interval) {
    // save the last time you blinked the LED
    distrib_previous_millis = currentMillis;
    // if the LED is off turn it on and vice-versa:
    if (distrib_step == LOW)
      distrib_step = HIGH, distrib_step_since_dir_change++;
    else
      distrib_step = LOW;
    // set the LED with the ledState of the variable:
    digitalWrite(PIN_DISTRIB_STEP, distrib_step);
  }

  //Stepper 2 - Distribution Direction
  if ( distrib_step_since_dir_change >= 0 && distrib_step_since_dir_change <= distrib_steps / 2) {
    distrib_dir = LOW;
  } else if (  distrib_step_since_dir_change >= distrib_steps / 2 && distrib_step_since_dir_change <= distrib_steps - 1) {
    distrib_dir = HIGH;
  }
  if (distrib_step_since_dir_change >= distrib_steps ) {
    distrib_step_since_dir_change = 0;
  }

  digitalWrite(PIN_DISTRIB_DIR, distrib_dir);
}
void resetDistr () {
  digitalWrite(PIN_DISTRIB_DIR, LOW);

  for (int x = 0; x < distrib_num_steps; x++) {
    digitalWrite (PIN_STEPPER_ENABLE, LOW);
    digitalWrite(PIN_DISTRIB_STEP, HIGH);
    delay(1);
    digitalWrite(PIN_DISTRIB_STEP, LOW);
    delay(1);
  }
  digitalWrite(PIN_DISTRIB_DIR, HIGH);

}
