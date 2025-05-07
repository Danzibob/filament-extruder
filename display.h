#pragma once

#pragma GCC diagnostic push
#pragma GCC system_header
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <WString.h>
#pragma GCC diagnostic pop

#include "encoder.h"
#include "pid.h"
#include "sensor.h"
#include "stepper.h"

namespace display {

// Interval display is refreshed at
unsigned long interval = 100;

// Constants - Strings & 'Logos' (bitmaps)
namespace strings {

String set_up = "   Set up:";
String diam = " Diameter:";
String mode = "     Mode: ";
String pull_speed = "  PullSpd: ";
String offset = "   Offset:";
String trav_speed = "  TravSpd: ";
String spool_speed = " SpoolSpd:";
String fan_speed = "   FanSpd:";
String stats = "    Stats:";

String modes[4] = { "Soft", "Medium", "Hard", "Manual" };

}

// Shorthands for the bitmaps defined in logos
enum Logo {
    Diameter = 0,
    Speed,
    MM,
    Metr1, // TODO: ?
    Metr2, // TODO: ?
    Extruder, // ?
    Xt // ?
};

namespace logos {

byte diameter[] = { 0x1F, 0x1F, 0x13, 0x15, 0x15, 0x15, 0x13, 0x1F };
byte speed[] = { 0x1F, 0x1F, 0x11, 0x17, 0x11, 0x1D, 0x11, 0x1F };
byte mm[] = { 0x00, 0x00, 0x00, 0x00, 0x0A, 0x15, 0x15, 0x15 };
byte metr1[] = { 0x11, 0x1B, 0x15, 0x10, 0x11, 0x12, 0x04, 0x08 };
byte metr2[] = { 0x02, 0x04, 0x08, 0x10, 0x0A, 0x15, 0x15, 0x15 };
byte ex[] = { 0x1F, 0x1F, 0x11, 0x15, 0x12, 0x15, 0x11, 0x1F };
byte xt[] = { 0x1F, 0x1F, 0x10, 0x15, 0x0D, 0x15, 0x15, 0x1F };

}

void init();
void showSplash();
void loop(boolean forceUpdate);
boolean isInSetup();

}
