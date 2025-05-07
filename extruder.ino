#include "eeprom.h"
#include "display.h"
#include "encoder.h"
#include "fans.h"
#include "pid.h"
#include "sensor.h"
#include "stepper.h"

void setup()
{
    display::init();
    display::showSplash();

    eeprom::load();
    sensor::init();
    stepper::init();
    encoder::init();
}


void loop()
{
    // Read inputs
    sensor::loop();
    boolean mustUpdate = encoder::read();

    // Update things
    display::loop(mustUpdate);
    fans::set(); // TODO: only do on change? cant see where its changed lol
    if (stepper::isEnabled()) {
        pid::loop();
    } else if (!display::isInSetup() && sensor::width() - sensor::offset() >= 0.10) {
        stepper::enable();
    }
}
