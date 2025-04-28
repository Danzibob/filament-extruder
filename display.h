#pragma once

#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <WString.h>

#include "encoder.h"
#include "pid.h"
#include "sensor.h"
#include "stepper.h"

// LCD
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

unsigned long lcd_prevMillis = 0;
unsigned long lcd_interval = 100;

// Menu state
int menu_curr_item = 1;
int menu_frame = 1;
int menu_page = 1;
int menu_last_item = 1;

// Menu strings
String str_set_up = "   Set up:";
String str_diam = " Diameter:";
String str_mode = "     Mode: ";
String str_pull_speed = "  PullSpd: ";
String str_offset = "   Offset:";
String str_trav_speed = "  TravSpd: ";
String str_spool_speed = " SpoolSpd:";
String str_fan_speed = "   FanSpd:";
String str_stats = "    Stats:";

String mode[4] = { "Soft", "Medium", "Hard", "Manual" };

// Constants for logo, etc
byte logo_diameter[] = { 0x1F, 0x1F, 0x13, 0x15, 0x15, 0x15, 0x13, 0x1F };

byte logo_speed[] = { 0x1F, 0x1F, 0x11, 0x17, 0x11, 0x1D, 0x11, 0x1F };
byte logo_mm[] = { 0x00, 0x00, 0x00, 0x00, 0x0A, 0x15, 0x15, 0x15 };

byte logo_metr1[] = { 0x11, 0x1B, 0x15, 0x10, 0x11, 0x12, 0x04, 0x08 };
byte logo_metr2[] = { 0x02, 0x04, 0x08, 0x10, 0x0A, 0x15, 0x15, 0x15 };
byte logo_ex[] = { 0x1F, 0x1F, 0x11, 0x15, 0x12, 0x15, 0x11, 0x1F };
byte logo_xt[] = { 0x1F, 0x1F, 0x10, 0x15, 0x0D, 0x15, 0x15, 0x1F };

void drawHome();
void drawMenu();
void displayIntMenuPage(String menuItem, int position, int value);
void displayString2MenuPage(String value);
void displayIntStringMenuPage(String item, int position, boolean selected, String value);
void displayMenuItem(String item, int position, boolean selected, int value);

int steps_per_click = 100; // Controls the speed of the Stepper per Rotary click
