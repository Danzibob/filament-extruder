#include "eeprom.h"

namespace eeprom {
void load()
{
    int8_t stored_offset = (int8_t)EEPROM.read(addr_offset);
    // Treat ±25 boundary values as corrupt (old code clamped 0xFF→25 on first boot).
    if (stored_offset > -25 && stored_offset < 25)
        sensor::setOffset(stored_offset);
    else
        sensor::setOffset(0);

    // EEPROM
    int setpoint_int = (EEPROM.read(addr_setpoint) * 256) + EEPROM.read(addr_setpoint + 1);
    if (setpoint_int < 1) {
        setpoint_int = 175;
        EEPROM.update(addr_setpoint, highByte(setpoint_int));
        EEPROM.update(addr_setpoint + 1, lowByte(setpoint_int));
    }
    pid::setSetpoint(setpoint_int);
}

void update()
{
    EEPROM.update(addr_setpoint, highByte(pid::setpoint()));
    EEPROM.update(addr_setpoint + 1, lowByte(pid::setpoint()));
    EEPROM.update(addr_offset, sensor::offset());
}

}
