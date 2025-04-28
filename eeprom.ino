#include "eeprom.h"

namespace eeprom {
void load() {
    sensor::setOffset(EEPROM.read(addr_offset));

    // EEPROM
    int setpoint_int = (EEPROM.read(addr_setpoint) * 256) + EEPROM.read(addr_setpoint + 1);
    if (setpoint_int < 1) {
        setpoint_int = 175;
        EEPROM.update(addr_setpoint, highByte(setpoint_int));
        EEPROM.update(addr_setpoint + 1, lowByte(setpoint_int));
    }
    pid::setSetpoint(setpoint_int);
}

void update() {
    EEPROM.update(addr_setpoint, highByte(pid::setpoint()));
    EEPROM.update(addr_setpoint + 1, lowByte(pid::setpoint()));
    EEPROM.update(addr_offset, sensor::offset());
}

}
