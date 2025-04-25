#include "./sensor.h"

namespace {
using namespace sensor;

float lookup(float inval)
{
    float out;
    byte i;
    for (i = 1; i < lut_len; i++) {
        if (lut[i][0] > inval) {
            return lut[i - 1][1] + (inval - lut[i - 1][0]) * (lut[i][1] - lut[i - 1][1]) / (lut[i][0] - lut[i - 1][0]);
        }
    }
}
}

namespace sensor {

void init() {
    pinMode(PIN_SENSOR_IN, INPUT);

    // take a second to calibrate
    // this assumes the sensor is empty on boot
    while (millis() < 1000) {
        inp_raw = analogRead(PIN_SENSOR_IN);
        if (inp_raw > inp_min) {
            inp_min = inp_raw;
        }
    }
}

void update()
{
    inp_raw = analogRead(PIN_SENSOR_IN);
    inp += (inp_raw - inp) - inp_min; // smoothing
    width = abs(lookup(inp)) + offset_float;
}

}
