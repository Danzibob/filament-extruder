#pragma once

#include <EEPROM.h>
#include <WString.h>
#include <LiquidCrystal_I2C.h>

#include "encoder.h"
#include "sensor.h"
#include "stepper.h"
#include "pid.h"

// LCD
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

unsigned long     LcdpreviousMillis = 0;
int               Lcdinterval = 1000;
unsigned long     LcdpreviousMillis2 = 0;
int               Lcdinterval2 = 100;


// Menu state
int menuItem = 1;
int frame = 1;
int page = 1;
int lastMenuItem = 1;

// Menu strings
String menuItem1 = "   Set up:";
String menuItem2 = " Diameter:";
String menuItem3 = "     Mode: ";
String menuItem4 = "  PullSpd: ";
String menuItem5 = "   Offset:";
String menuItem6 = "  TravSpd: ";
String menuItem7 = " SpoolSpd:";
String menuItem8 = "   FanSpd:";
String menuItem9 = "    Stats:";

String mode[4] = { "Soft", "Medium", "Hard", "Manual" };

// Constants for logo, etc
byte DiameterLogo[] = {
  0x1F,
  0x1F,
  0x13,
  0x15,
  0x15,
  0x15,
  0x13,
  0x1F
};

byte SpdLogo[] = {
  0x1F,
  0x1F,
  0x11,
  0x17,
  0x11,
  0x1D,
  0x11,
  0x1F
};
byte mmLogo[] = {
  0x00,
  0x00,
  0x00,
  0x00,
  0x0A,
  0x15,
  0x15,
  0x15
};

byte MetrLogo1[] = {
  0x11,
  0x1B,
  0x15,
  0x10,
  0x11,
  0x12,
  0x04,
  0x08
};
byte MetrLogo2[] = {
  0x02,
  0x04,
  0x08,
  0x10,
  0x0A,
  0x15,
  0x15,
  0x15
};
byte ExLogo[] = {
  0x1F,
  0x1F,
  0x11,
  0x15,
  0x12,
  0x15,
  0x11,
  0x1F
};
byte XtLogo[] = {
  0x1F,
  0x1F,
  0x10,
  0x15,
  0x0D,
  0x15,
  0x15,
  0x1F
};

void drawHome();
void drawMenu();
void displayIntMenuPage(String menuItem, int position, int value);
void displayString2MenuPage(String value);
void displayIntStringMenuPage(String item, int position, boolean selected, String value);
void displayMenuItem(String item, int position, boolean selected, int value);
