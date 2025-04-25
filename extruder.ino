#include <TimerOne.h> // trigger timer to read encoder

#include <EEPROM.h>

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
    // Encoder
    encoder = new ClickEncoder(A2, A1, A3);
    Timer1.initialize(1000);
    Timer1.attachInterrupt(timerIsr);
    // last = -1;
    // Sensor
    pinMode(PIN_SENSOR_IN, INPUT);
    // calibrate during the first second
    while (millis() < 1000) {
        sensor_inp_raw = analogRead(PIN_SENSOR_IN);
        // record the minimum sensor value
        if (sensor_inp_raw > sensor_inp_min) {
            sensor_inp_min = sensor_inp_raw;
        }
    }
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
    resetDistr();
    // EEPROM
    pid_setpoint_int = ((EEPROM.read(width_eeprom_diam) * 256) + EEPROM.read(width_eeprom_diam + 1));
    if (pid_setpoint_int < 1) {
        pid_setpoint_int = 175;
        EEPROM.update(width_eeprom_diam, highByte(pid_setpoint_int));
        EEPROM.update(width_eeprom_diam + 1, lowByte(pid_setpoint_int));
    }
    width_offset = EEPROM.read(width_eeprom_offset);
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
    readRotaryEncoder();
    ClickEncoder::Button b = encoder->getButton();
    if (b != ClickEncoder::Open) {
        switch (b) {
        case ClickEncoder::Clicked:
            encoder_middle = true;
            lcd.clear();
            break;
        }
    }
    if (b != ClickEncoder::Open) {
        switch (b) {
        case ClickEncoder::Held:
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Resetting...");
            resetDistr();
            distrib_stepper_pos = 0;
            menu_curr_item = 1;
            menu_page = 2;
            lcd.clear();
            break;
        }
    }

    if (b != ClickEncoder::Open) {
        switch (b) {
        case ClickEncoder::DoubleClicked:
            if (pid_mode < 3) {
                if (menu_page == 2 && menu_curr_item == 8) {
                    puller_num_revs = 0;
                }
            } else if (pid_mode == 3) {
                if (menu_page == 2 && menu_curr_item == 9) {
                    puller_num_revs = 0;
                }
            }
            break;
        }
    }

    Var();
    fans::set();

    if (width_curr - width_offset_float <= 0.10 && menu_curr_item != 1) {
        // myPID.SetMode(MANUAL);
        stepper_enabled = 0;
        stepper_preenable = stepper_enabled;
    } else {
        stepper_enabled = 1;
        stepper_preenable = stepper_enabled;
        Brain();
    } //    myPID.SetMode(AUTOMATIC);

    if (stepper_enabled == 0) {
        digitalWrite(PIN_STEPPER_ENABLE, HIGH);
    } else
        digitalWrite(PIN_STEPPER_ENABLE, LOW);
}
// MAIN //

// VARIABILIES //
void Var()
{

    pid_setpoint_float = pid_setpoint_int * 0.01;
    if (spool_speed <= 2) {
        spool_speed = 2;
    } else if (spool_speed >= 30) {
        spool_speed = 30;
    }
    spool_interval = spool_speed;
    spool_rpm = 300 / spool_speed;

    distrib_interval = 160 / travel_speed;

    if (travel_speed <= 0) {
        travel_speed = 0;
    }
    if (travel_speed >= 160) {
        travel_speed = 160;
    }

    width_offset_float = width_offset * 0.01;
    if (width_offset <= -25) {
        width_offset = -25;
    } else if (width_offset >= 25) {
        width_offset = 25;
    }

    distrib_new_position_end = (7900 / 4) - distrib_new_position;
    distrib_steps = 2 * travel_step;
    puller_total = puller_num_revs * 0.194;
}
// VARIABILIES //
