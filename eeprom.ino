#include "eeprom.h"

namespace eeprom {
void load() {
    sensor::offset = EEPROM.read(addr_offset);

    // EEPROM
    pid_setpoint_int = ((EEPROM.read(addr_setpoint) * 256) + EEPROM.read(addr_setpoint + 1));
    if (pid_setpoint_int < 1) {
        pid_setpoint_int = 175;
        EEPROM.update(addr_setpoint, highByte(pid_setpoint_int));
        EEPROM.update(addr_setpoint + 1, lowByte(pid_setpoint_int));
    }
}

void update() {
    EEPROM.update(addr_setpoint, highByte(pid_setpoint_int));
    EEPROM.update(addr_setpoint + 1, lowByte(pid_setpoint_int));
    EEPROM.update(addr_offset, sensor::offset);
}

}
