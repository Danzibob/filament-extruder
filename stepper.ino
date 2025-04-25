#include "./stepper.h"

// Puller
void Pull() {
  unsigned long currentMillis = millis();
  //Stepper 1 - Puller
  digitalWrite(PullpinDir, HIGH);
  if (currentMillis - PullpreviousMillis >= Pullinterval) {
    // save the last time you blinked the LED
    PullpreviousMillis = currentMillis;
    // if the LED is off turn it on and vice-versa:
    if (PullpinStepState == LOW)
      PullpinStepState = HIGH , r++;
    else
      PullpinStepState = LOW;
    // set the LED with the ledState of the variable:
    digitalWrite(PullpinStep, PullpinStepState);
  }
  if (r == 400) {
    r = 0;
    R++;
  }

}
void ManualPull(){

  unsigned long currentMillis = micros();
  //Stepper 1 - Puller
  digitalWrite(PullpinDir, HIGH);
  if (currentMillis - PullpreviousMillis >= Pullinterval) {
    // save the last time you blinked the LED
    PullpreviousMillis = currentMillis;
    // if the LED is off turn it on and vice-versa:
    if (PullpinStepState == LOW)
      PullpinStepState = HIGH , r++;
    else
      PullpinStepState = LOW;
    // set the LED with the ledState of the variable:
    digitalWrite(PullpinStep, PullpinStepState);
  }
  if (r == 400) {
    r = 0;
    R++;
  }

}
// Spooler
void Spool() {
  unsigned long currentMillis = millis();
  //Stepper 3 - Spool
  digitalWrite(SpoolpinDir, LOW);
  if (currentMillis - SpoolpreviousMillis >= Spoolinterval) {
    // save the last time you blinked the LED
    SpoolpreviousMillis = currentMillis;
    // if the LED is off turn it on and vice-versa:
    if (SpoolpinStepState == LOW)
      SpoolpinStepState = HIGH;
    else
      SpoolpinStepState = LOW;
    // set the LED with the ledState of the variable:
    digitalWrite(SpoolpinStep, SpoolpinStepState);
  }
}
//Distribution
void Distr() {
  unsigned long currentMillis = millis();
  //Stepper 2 - Distribution
  if (currentMillis - DistrpreviousMillis >= Distrinterval) {
    // save the last time you blinked the LED
    DistrpreviousMillis = currentMillis;
    // if the LED is off turn it on and vice-versa:
    if (DistrpinStepState == LOW) {
      DistrpinStepState = HIGH;
      y++;
    }  else
      DistrpinStepState = LOW;
    // set the LED with the ledState of the variable:
    digitalWrite(DistrpinStep, DistrpinStepState);
  }
  //Stepper 2 - Distribution Direction
  if ( y >= 0 && y <= DistributionSteps / 2) {
    DistrpinDirState = LOW;
  } else if (  y >= DistributionSteps / 2 && y <= DistributionSteps - 1) {
    DistrpinDirState = HIGH;
  }
  if (y >= DistributionSteps ) {
    y = 0;
  }

  digitalWrite(DistrpinDir, DistrpinDirState);
}
void resetDistr () {
  //definiamo la direzione del motore

  digitalWrite(DistrpinDir, LOW);

  //esegue un giro completo in un senso
  for (int x = 0; x < numStepMotore; x++) {
    digitalWrite (enablePin, LOW);
    digitalWrite(DistrpinStep, HIGH);
    delay(1);
    digitalWrite(DistrpinStep, LOW);
    delay(1);
  }
  digitalWrite(DistrpinDir, HIGH);

}
