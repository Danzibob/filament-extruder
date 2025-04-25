#pragma once

#include <EEPROM.h>
#include "sensor.h"
#include "pid.h"

namespace eeprom {

int addr_setpoint = 0;
int addr_offset = 4;

void load();
void update();

}
