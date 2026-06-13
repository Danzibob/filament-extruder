#include "display.h"
#include "eeprom.h"
#include "encoder.h"
#include "pid.h"
#include "sensor.h"
#include "stepper.h"

void setup()
{
    Serial.begin(115200);
    Serial.println("Hello.");

    display::init();
    display::showSplash();

    eeprom::load();
    sensor::init();
    stepper::init();
    encoder::init();

    display::clear();
    display::loop(true);
    Serial.println("Init done");
}

void loop()
{
    // Read inputs
    sensor::loop();
    boolean mustUpdate = encoder::read();

    static unsigned long lastDebug = 0;
    if (millis() - lastDebug >= 250) {
        lastDebug = millis();
        Serial.print(stepper::distrib::pos());
        Serial.print(" pulled=");
        Serial.println(stepper::pull::total());
    }

    // Update things
    display::loop(mustUpdate);
    if (stepper::isEnabled()) {
        pid::loop();

        if (sensor::width() - sensor::offset_mm() <= 0.10) {
            stepper::disable();
        }
    } else if (!display::isInSetup() && sensor::width() - sensor::offset_mm() >= 0.10) {
        stepper::enable();
    }
}
