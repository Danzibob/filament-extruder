#include "./eeprom.h"
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

float inp = 0;
float inp_raw = 0;
float inp_baseline = 0; // no-filament ADC ceiling; sensor is inverted (higher ADC = no filament)

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

void init()
{
    pinMode(PIN_SENSOR_IN, INPUT);

    // Sample for 500 ms with no filament present to establish the baseline.
    // The sensor is inverted: output is highest with no filament and drops as
    // diameter increases. We capture the maximum (ceiling) so that subtracting
    // inp_raw from it gives ~0 when empty, preventing false non-zero readings.
    // Uses relative time so it works regardless of how long earlier init steps took.
    unsigned long end = millis() + 500;
    while (millis() < end) {
        inp_raw = analogRead(PIN_SENSOR_IN);
        if (inp_raw > inp_baseline) {
            inp_baseline = inp_raw;
        }
    }
}

void loop()
{
    inp_raw = analogRead(PIN_SENSOR_IN);
    // Shift the raw reading down by the calibrated zero-point baseline so that
    // inp represents signal above the empty-sensor floor. (Not a smoothing filter
    // — alpha=1 means inp tracks inp_raw directly, offset by inp_min.)
    // Sensor output decreases as filament diameter increases, so invert relative
    // to the no-filament baseline to get a signal that rises with diameter.
    inp = inp_baseline - inp_raw;
    curr_width = lookup(inp) + offset_mm();
}

float width() { return curr_width; }

int offset() { return curr_offset; }
float offset_mm() { return curr_offset * 0.01; }
void setOffset(int newOffset)
{
    if (newOffset <= -25) {
        newOffset = -25;
    } else if (newOffset >= 25) {
        newOffset = 25;
    }

    curr_offset = newOffset;
    eeprom::update();
}

}
