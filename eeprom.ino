#include "eeprom.h"

namespace eeprom {
void load() {
    sensor::setOffset(EEPROM.read(addr_offset));

    // EEPROM
    pid::setpoint_int = ((EEPROM.read(addr_setpoint) * 256) + EEPROM.read(addr_setpoint + 1));
    if (pid::setpoint_int < 1) {
        pid::setpoint_int = 175;
        EEPROM.update(addr_setpoint, highByte(pid::setpoint_int));
        EEPROM.update(addr_setpoint + 1, lowByte(pid::setpoint_int));
    }
}

void update() {
    EEPROM.update(addr_setpoint, highByte(pid::setpoint_int));
    EEPROM.update(addr_setpoint + 1, lowByte(pid::setpoint_int));
    EEPROM.update(addr_offset, sensor::offset);
}

}
