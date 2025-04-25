#include "./fans.h"

namespace fans {

void set()
{
    if (speed <= 0) {
        speed = 0;
    }
    if (speed >= 255) {
        speed = 255;
    }
    analogWrite(PIN_FAN_PWM, speed);
}

}
