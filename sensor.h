#pragma once

//TTL - Width Sensor
#define NUMTEMPS 16  //length of lookup table
#define smooth 10  //exponential smoothing factor, higher is smoother
#define sensIn A7

// ADC input, diameter output
static float lut3[NUMTEMPS][2] = {
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
float sensbuf = lut3[0][0]; //smoothed raw value

int offset = 0;
float intOffset = 0;

//EEPROM where offset is stored
int adressDiam = 0;
int adressOffset = 4;

float sensorValue = 0;
float measure = 0 ;
float targettino = 0;
float sensorMin = 0;
char Result [0];

void Sensor();
float lookup(float inval, float lut[][2]);
