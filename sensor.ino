#include "./sensor.h"

void Sensor() {
  sensor_inp_raw = analogRead(PIN_SENSOR_IN);
  sensor_inp += (sensor_inp_raw - sensor_inp) - sensor_inp_min; //smoothing
  width_curr = abs (lookup(sensor_inp, lut3)) + width_offset_float;
}
float lookup(float inval, float lut[][2]) {
  float out;
  byte i;
  for (i = 1; i < lut_len; i++)
  {
    if (lut[i][0] > inval)
    {
      return lut[i - 1][1] + (inval - lut[i - 1][0]) * (lut[i][1] - lut[i - 1][1]) / (lut[i][0] - lut[i - 1][0]);
    }
  }
}
