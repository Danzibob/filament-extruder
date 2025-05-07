#include "./sensor.h"

namespace {
const int PIN_SENSOR_IN = A7;

using namespace sensor;

const int lut_len = 16; // length of lookup table
static float lut[lut_len][2] = {
    // ADC input, diameter output
    { -10, 0 }, { 0, 0 }, { 5, 0 }, { 30, 0.5 }, { 96, 1.17 }, { 115, 1.46 }, { 127, 1.60 }, { 140, 1.75 },
    { 160, 1.99 }, { 200, 2.48 }, { 240, 3.00 }, { 248, 3.10 }, { 256, 3.20 }, { 280, 3.50 }, { 345, 4.00 }, { 360, 4.15 }
};

float inp = lut[0][0]; // ADC input
float inp_raw = 0; // before smoothing
float inp_min = 0; // zero point observed at startup

float lookup(float inval)
{
    byte i;
    for (i = 1; i < lut_len; i++) {
        if (lut[i][0] > inval) {
            return lut[i - 1][1] + (inval - lut[i - 1][0]) * (lut[i][1] - lut[i - 1][1]) / (lut[i][0] - lut[i - 1][0]);
        }
    }
    return 0.0; // >.>
}

float curr_width = 0; // current width reading in mm
int curr_offset = 0;

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

void loop()
{
    inp_raw = analogRead(PIN_SENSOR_IN);
    inp += (inp_raw - inp) - inp_min; // smoothing
    curr_width = abs(lookup(inp)) + offset_mm();
}


float width() { return curr_width; }

int offset() { return curr_offset; }
float offset_mm() { return curr_offset * 0.01; }
void setOffset(int newOffset) {
    if (newOffset <= -25) {
        newOffset = -25;
    } else if (newOffset >= 25) {
        newOffset = 25;
    }

    curr_offset = newOffset;
    eeprom::update();
}

}
