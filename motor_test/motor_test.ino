const int PIN_STEPPER_ENABLE = 8;
const int PIN_PULLER_DIR = 2;
const int PIN_PULLER_STEP = 5;
const int PIN_DISTRIB_DIR = 3;
const int PIN_DISTRIB_STEP = 6;
const int PIN_SPOOL_DIR = 4;
const int PIN_SPOOL_STEP = 7;
const int interval = 1;

void setup() {
  pinMode(PIN_PULLER_STEP, OUTPUT);
  pinMode(PIN_PULLER_DIR, OUTPUT);
  pinMode(PIN_DISTRIB_STEP, OUTPUT);
  pinMode(PIN_DISTRIB_DIR, OUTPUT);
  pinMode(PIN_SPOOL_STEP, OUTPUT);
  pinMode(PIN_SPOOL_DIR, OUTPUT);
  pinMode(PIN_STEPPER_ENABLE, OUTPUT);

  digitalWrite(PIN_STEPPER_ENABLE, LOW);
  digitalWrite(PIN_PULLER_DIR, HIGH);
  digitalWrite(PIN_SPOOL_DIR, HIGH);
  digitalWrite(PIN_DISTRIB_DIR, HIGH);
  Serial.begin(9600);
  Serial.println("Begin");

  TCCR0A = _BV(COM2A0) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR0B = _BV(WGM22) | B00000011;

  OCR0A = 48;
  OCR0B = 24;
  
  // TCCR0B = TCCR0B & B11111000 | B00000010;    // set timer 0 divisor to     8 for PWM frequency of  7812.50 Hz
  // TCCR0A = _BV(COM2A0) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  // TCCR0B = _BV(WGM22) | B00000011;    // set timer 0 divisor to    64 for PWM frequency of   976.56 Hz (The DEFAULT)
  // TCCR0B = TCCR0B & B11111000 | B00000100;    // set timer 0 divisor to   256 for PWM frequency of   244.14 Hz
  // TCCR0B = TCCR0B & B11111000 | B00000101;    // set timer 0 divisor to  1024 for PWM frequency of    61.04 Hz
  // TCCR1A = _BV(COM2A0) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  // TCCR1B = _BV(WGM22) | _BV(CS22);

  // analogWrite(PIN_PULLER_STEP, 128);
}

void loop() {
  // put your main code here, to run 
  // Serial.println("upstep");
  // digitalWrite(PIN_PULLER_STEP, HIGH);
  // digitalWrite(PIN_SPOOL_STEP, HIGH);
  // digitalWrite(PIN_DISTRIB_STEP, HIGH);
  // delay(interval);
  // digitalWrite(PIN_PULLER_STEP, LOW);
  // digitalWrite(PIN_SPOOL_STEP, LOW);
  // digitalWrite(PIN_DISTRIB_STEP, LOW);
  // delay(interval);
}
