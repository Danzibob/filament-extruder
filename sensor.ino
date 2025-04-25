#include "./sensor.h"

void Sensor() {
  sensorValue = analogRead(sensIn);
  sensbuf += (sensorValue - sensbuf) - sensorMin; //smoothing
  measure = abs (lookup(sensbuf, lut3)) + intOffset;
}
float lookup(float inval, float lut[][2]) {
  float out;
  byte i;
  for (i = 1; i < NUMTEMPS; i++)
  {
    if (lut[i][0] > inval)
    {
      return lut[i - 1][1] + (inval - lut[i - 1][0]) * (lut[i][1] - lut[i - 1][1]) / (lut[i][0] - lut[i - 1][0]);
    }
  }
}
