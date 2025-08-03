#include "display.h"
#include "eeprom.h"
#include "encoder.h"
#include "pid.h"
#include "sensor.h"
#include "stepper.h"

void setup()
{
    Serial.begin(9600);
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

    // Update things
    display::loop(mustUpdate);
    if (stepper::isEnabled()) {
        pid::loop();

        if (sensor::width() - sensor::offset() <= 0.10) {
            stepper::disable();
        }
    } else if (!display::isInSetup() && sensor::width() - sensor::offset() >= 0.10) {
        stepper::enable();
    }
}
