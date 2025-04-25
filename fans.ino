#include "./fans.h"

void fans()
{
    if (fan_speed <= 0) {
        fan_speed = 0;
    }
    if (fan_speed >= 255) {
        fan_speed = 255;
    }
    analogWrite(PIN_FAN_PWM, fan_speed);
}
