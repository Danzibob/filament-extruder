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

constexpr int STEPS_PER_CLICK = 100;
constexpr int MM_PER_CLICK = 10; // in 1/100ths of mm

// Constants - Strings & 'Logos' (bitmaps)
namespace strings {

// clang-format off
String setup_mode        = "Set mode:       ";
String setup_diameter    = "Set diameter:   ";
String setup_distrib_start = "Set spool start:";
String setup_distrib_end   = "Set spool end:";
String spooler_ready     = " Spooler ready! ";
String diam              = " Diameter:";
String set_diam          = ">Diameter:";
String mode              = " Mode:";
String set_mode          = ">Mode:";
String pull_speed        = " PullInt:";
String set_pull_speed    = ">PullInt:";
String distrib_speed     = " DstrbInt:";
String set_distrib_speed = ">DstrbInt:";
String spool_speed       = " SplInt:";
String set_spool_speed   = ">SplInt:";
String width_offset      = " Offset:";
String set_width_offset  = ">Offset:";
String stats             = "Pulled: ";
// clang-format on

// Must line up with PIDMode
String modes[4] = { "Soft", "Medium", "Hard", "Manual" };

}

// Shorthands for the bitmaps defined in logos
enum class Logo : char {
    // Inverted D
    Diameter = 0,
    // Inverted S
    Speed,

    // mm (stacked)
    MM,

    // Two part graphic, showing m / m
    Metr1,
    Metr2,

    // Two part graphic, inverted EXT
    Extruder1,
    Extruder2
};

namespace logos {

// consider https://maxpromer.github.io/LCD-Character-Creator/
byte diameter[] = { 0x1F, 0x1F, 0x13, 0x15, 0x15, 0x15, 0x13, 0x1F };
byte speed[] = { 0x1F, 0x1F, 0x11, 0x17, 0x11, 0x1D, 0x11, 0x1F };
byte mm[] = { 0x0A, 0x15, 0x15, 0x00, 0x0A, 0x15, 0x15, 0x00 };
byte metr1[] = { 0x11, 0x1B, 0x15, 0x10, 0x11, 0x12, 0x04, 0x08 };
byte metr2[] = { 0x02, 0x04, 0x08, 0x10, 0x0A, 0x15, 0x15, 0x15 };
byte extruder1[] = { 0x1F, 0x1F, 0x12, 0x16, 0x13, 0x16, 0x12, 0x1F };
byte extruder2[] = { 0x1F, 0x1F, 0x10, 0x15, 0x0D, 0x15, 0x15, 0x1F };
}

void init();
void showSplash();
void clear();
void loop(boolean receivedInput);
boolean isInSetup();

}
