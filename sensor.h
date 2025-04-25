#pragma once

const int PIN_SENSOR_IN = A7;

const int lut_len = 16; // length of lookup table
static float lut3[lut_len][2] = {
  // ADC input, diameter output
  { -10, 0},
  {0, 0},
  {5, 0},
  {30, 0.5},
  {96, 1.17},
  {115, 1.46},
  {127, 1.60},
  {140, 1.75 },
  {160, 1.99 },
  {200, 2.48},
  {240, 3.00},
  {248, 3.10},
  {256, 3.20},
  {280, 3.50},
  {345, 4.00},
  {360, 4.15}
};

float sensor_inp_raw = 0;
float sensor_inp = lut3[0][0];

int width_offset = 0;
float width_offset_float = 0;

//EEPROM where offset is stored
int width_eeprom_diam = 0;
int width_eeprom_offset = 4;

float width_curr = 0 ;
float sensor_inp_min = 0;

void Sensor();
float lookup(float inval, float lut[][2]);
