#include <TimerOne.h> // trigger timer to read encoder

#include <EEPROM.h>

#include "display.h"
#include "encoder.h"
#include "stepper.h"
#include "pid.h"
#include "fans.h"
#include "sensor.h"

// MAIN //
void setup() {

  //LCD
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Felfil Spooler");
  lcd.setCursor(0, 1);
  lcd.print("Calibrating 1.4c");
  lcd.createChar(1, SpdLogo);
  lcd.createChar(0, DiameterLogo);
  lcd.createChar(2, mmLogo);
  lcd.createChar(3, MetrLogo1);
  lcd.createChar(4, MetrLogo2);
  lcd.createChar(5, ExLogo);
  lcd.createChar(6, XtLogo);
  //Encoder
  encoder = new ClickEncoder(A2, A1, A3);
  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr);
  //last = -1;
  //Sensor
  pinMode( sensIn, INPUT);
  // calibrate during the first second
  while (millis() < 1000) {
    sensorValue = analogRead(sensIn);
    // record the minimum sensor value
    if (sensorValue > sensorMin) {
      sensorMin = sensorValue;
    }
  }
  //Stepper 1 - Puller
  pinMode(PullpinStep, OUTPUT);
  pinMode(PullpinDir, OUTPUT);
  //Stepper 2 - Distribution
  pinMode(DistrpinStep, OUTPUT);
  pinMode(DistrpinDir, OUTPUT);
  //Stepper 3 - Spool
  pinMode(SpoolpinStep, OUTPUT);
  pinMode(SpoolpinDir, OUTPUT);
  //Stepper enable
  pinMode ( enablePin, OUTPUT);
  //ResetDistr
  resetDistr ();
  // EEPROM
  diameter = ((EEPROM.read(adressDiam) * 256) + EEPROM.read(adressDiam + 1));
  if (diameter < 1 ) {
    diameter = 175;
    EEPROM.update(adressDiam, highByte(diameter));
    EEPROM.update(adressDiam + 1, lowByte(diameter));
  }
  offset = EEPROM.read(adressOffset);
  //LCD clear
  lcd.clear();
  // Fan

}
void loop() {

  //Pullinterval = 90;
  // LCD //
  unsigned long currentMillis = millis();
  if (currentMillis - LcdpreviousMillis2 >= Lcdinterval2) {
    // save the last time you blinked the LED
    LcdpreviousMillis2 = currentMillis;
    drawMenu();
  }
  // ENCODER //
  readRotaryEncoder();
  ClickEncoder::Button b = encoder->getButton();
  if (b != ClickEncoder::Open) {
    switch (b) {
      case ClickEncoder::Clicked:
        middle = true;
        lcd.clear();
        break;
    }
  }
  if (b != ClickEncoder::Open) {
    switch (b) {
      case ClickEncoder::Held:
        lcd.clear();
        lcd.setCursor( 0, 0);
        lcd.print("Resetting...");
        resetDistr ();
        StepperPosition = 0;
        menuItem = 1;
        page = 2;
        lcd.clear();
        break;

    }
  }

  if (b != ClickEncoder::Open) {
    switch (b) {
      case ClickEncoder::DoubleClicked:
        if (selectedMode < 3) {
          if (page == 2 && menuItem == 8) {
            R = 0;
          }
        } else if (selectedMode == 3) {
          if (page == 2 && menuItem == 9) {
            R = 0;
          }
        }
        break;

    }
  }


  Var();
  // Fans
  fans();

  if (measure - intOffset <= 0.10 && menuItem != 1 )
  {
    //myPID.SetMode(MANUAL);
    enableState = 0;
    preenableState = enableState;
  }
  else
  { enableState = 1;
    preenableState = enableState;
    Brain();
  }  //    myPID.SetMode(AUTOMATIC);

  if (enableState == 0) {
    digitalWrite (enablePin, HIGH);
  }
  else digitalWrite (enablePin, LOW);

}
// MAIN //

// VARIABILIES //
void Var() {
  if ( extspd < 0) {
    extspd = 0;
  }


  intdiameter = diameter * 0.01;
  if (spoolspd <= 2) {
    spoolspd = 2;
  } else if (spoolspd >= 30) {
    spoolspd = 30;
  }
  Spoolinterval = spoolspd;
  spoolRPM = 300 / spoolspd;

  Distrinterval = 160 / travelspd;

  if (travelspd <= 0 ) {
    travelspd = 0;
  }
  if (travelspd >= 160 ) {
    travelspd = 160;
  }

  intOffset = offset * 0.01;
  if ( offset <= -25) {
    offset = -25;
  } else if ( offset >= 25) {
    offset = 25;
  }

  newpositionEnd = (7900/4) - newposition;
  extspd = 60 / ((Pullinterval * 400) / 1000) * 0.062;
  extspd2 = extspd  * 1000;
  DistributionSteps = 2 * travel ;
  Total = R * 0.194;
}
// VARIABILIES //
