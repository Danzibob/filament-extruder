#include "./fans.h"

void fans() {
  if (fanspd <= 0) {
    fanspd = 0;
  }
  if (fanspd >= 255) {
    fanspd = 255;
  }
  analogWrite ( pwmPin, fanspd);
}
