#pragma once

namespace sensor {

const int PIN_SENSOR_IN = A7;

const int lut_len = 16; // length of lookup table
static float lut[lut_len][2] = {
    // ADC input, diameter output
    { -10, 0 }, { 0, 0 }, { 5, 0 }, { 30, 0.5 }, { 96, 1.17 }, { 115, 1.46 }, { 127, 1.60 }, { 140, 1.75 },
    { 160, 1.99 }, { 200, 2.48 }, { 240, 3.00 }, { 248, 3.10 }, { 256, 3.20 }, { 280, 3.50 }, { 345, 4.00 }, { 360, 4.15 }
};

float inp = lut[0][0]; // ADC input
float inp_raw = 0; // before smoothing
float inp_min = 0; // zero point observed at startup

int offset = 0; // width offset set by user, in 1/100ths of a mm
float offset_float = 0; // offset * 0.01

// storage

float width = 0; // current width reading in mm

void init();
void update();

}
