#include "eeprom.h"
#include "display.h"
#include "encoder.h"
#include "fans.h"
#include "pid.h"
#include "sensor.h"
#include "stepper.h"

// MAIN //
void setup()
{

    // LCD
    lcd.begin(16, 2);
    lcd.setCursor(0, 0);
    lcd.print("Felfil Spooler");
    lcd.setCursor(0, 1);
    lcd.print("Calibrating 1.4c");
    lcd.createChar(1, logo_speed);
    lcd.createChar(0, logo_diameter);
    lcd.createChar(2, logo_mm);
    lcd.createChar(3, logo_metr1);
    lcd.createChar(4, logo_metr2);
    lcd.createChar(5, logo_ex);
    lcd.createChar(6, logo_xt);
    eeprom::load();
    sensor::init();
    stepper::init();
    encoder::init();
    // LCD clear
    lcd.clear();
}


void loop()
{

    // Pullinterval = 90;
    //  LCD //
    unsigned long currentMillis = millis();
    if (currentMillis - lcd_prevMillis >= lcd_interval) {
        // save the last time you blinked the LED
        lcd_prevMillis = currentMillis;
        drawMenu();
    }

    // ENCODER //
    encoder::read();
    switch (encoder::btn) {
    case ClickEncoder::Clicked:
        lcd.clear();
        break;
    case ClickEncoder::Held:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Resetting...");
        stepper::distrib::reset();
        stepper::distrib::pos = 0;
        menu_curr_item = 1;
        menu_page = 2;
        lcd.clear();
        break;
    case ClickEncoder::DoubleClicked:
        if (pid::mode < 3) {
            if (menu_page == 2 && menu_curr_item == 8) {
                stepper::pull::num_revs = 0;
            }
        } else if (pid::mode == 3) {
            if (menu_page == 2 && menu_curr_item == 9) {
                stepper::pull::num_revs = 0;
            }
        }
        break;
    default:
        break;
    }

    Var();
    fans::set();

    if (sensor::width() - sensor::offset() <= 0.10 && menu_curr_item != 1) {
        // myPID.SetMode(MANUAL);
        stepper::enabled = 0;
    } else {
        stepper::enabled = 1;
        pid::Brain();
    } //    myPID.SetMode(AUTOMATIC);

    if (stepper::enabled == 0) {
        digitalWrite(stepper::PIN_STEPPER_ENABLE, HIGH);
    } else
        digitalWrite(stepper::PIN_STEPPER_ENABLE, LOW);
}
// MAIN //

// VARIABILIES //
void Var()
{
    pid::setpoint_float = pid::setpoint_int * 0.01;

    stepper::Var();
}
// VARIABILIES //
