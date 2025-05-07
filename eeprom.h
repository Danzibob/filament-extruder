#pragma once

#include "pid.h"
#include "sensor.h"
#include <EEPROM.h>

namespace eeprom {

int addr_setpoint = 0;
int addr_offset = 4;

void load();
void update();

}
