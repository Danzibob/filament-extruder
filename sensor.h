#pragma once

namespace sensor {

void init();
void loop();

// current filament width read by sensor
float width();

// width offset set by user
int offset(); // in 1/100ths of a mm
float offset_mm(); // in mm

void setOffset(int newOffset);

}
