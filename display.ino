#include "./display.h"

void drawHome()
{
    // MenuHome
    if (stepper::enabled == 0) {
        lcd.setCursor(0, 0);
        lcd.print(" Spooler ready! ");
    } else {
        lcd.setCursor(0, 0);
        lcd.print(char(0));
        lcd.setCursor(5, 0);
        lcd.print(char(2));
        lcd.setCursor(6, 0);
        lcd.print(char(2));
        //
        lcd.setCursor(7, 0);
        lcd.print(" ");
        lcd.setCursor(8, 0);
        lcd.print(char(1));
        lcd.setCursor(13, 0);
        lcd.print(" ");
        lcd.setCursor(14, 0);
        lcd.print(char(3));
        lcd.setCursor(15, 0);
        lcd.print(char(4));

        unsigned long currentMillis = millis();
        if (currentMillis - lcd_prevMillis >= lcd_interval) {
            lcd.setCursor(1, 0);
            lcd.print(abs(sensor::width), 2);
            lcd.setCursor(9, 0);
            lcd.print(stepper::pull::interval, 2); // TODO: make this a speed, not an interval
            lcd_prevMillis = currentMillis;
        }
    }
}

void drawMenu()
{
    // MENU UI Start
    if (menu_curr_item == 2 && menu_page == 1) {
        encoder::up = false;
    }
    if (encoder::up && menu_page == 1) {

        encoder::up = false;
        menu_last_item = menu_curr_item;
        menu_curr_item--;
        if (menu_curr_item == 0) {
            menu_curr_item = 1;
        }
    }
    if (encoder::down && menu_page == 1) // We have turned the Rotary Encoder Clockwise
    {
        encoder::down = false;
        menu_last_item = menu_curr_item;
        menu_curr_item++;
        if (menu_curr_item == 10 && pid::mode == 3) {
            menu_curr_item--;
        } else if (menu_curr_item == 9 && pid::mode != 3) {
            menu_curr_item--;
        }
    }
    // SETUP MENU
    // Begin____________________________________________________________________
    if (menu_page == 1 && menu_curr_item == 1) {
        menu_page = 2;
    }
    if (encoder::middle && menu_curr_item == 1) // Middle Button is Pressed - SETUP MENU
    {
        encoder::middle = false;

        if (menu_page == 1) {

            menu_page = 2;
        } else if (menu_page == 2) {
            menu_page = 3;
        } else if (menu_page == 3) {
            menu_page = 4;
        } else if (menu_page == 4) {
            using namespace stepper::distrib;

            travel_begin = pos;

            lcd.setCursor(0, 0);
            lcd.print("Wait while I'm");
            lcd.setCursor(0, 1);
            lcd.print("moving stepper");
            digitalWrite(stepper::PIN_DISTRIB_DIR, HIGH);
            for (int x = 0; x < new_position_end; x++) {
                digitalWrite(stepper::PIN_DISTRIB_STEP, HIGH);
                delay(1);
                digitalWrite(stepper::PIN_DISTRIB_STEP, LOW);
                delay(1);
            }

            pos = new_position_end;
            lcd.clear();
            menu_page = 5;
        } else if (menu_page == 5) {
            using namespace stepper::distrib;

            travel_end = pos;
            travel_step = travel_end;
            if (pid::mode <= 2) {
                menu_curr_item = 2;
                menu_page = 1;
            } else
                (menu_page = 6);
        } else if (menu_page == 6) {
            menu_curr_item = 2;
            menu_page = 1;
        }
    }
    // SETUP MENU
    // End_______________________________________________________________________

    if (encoder::middle && menu_curr_item >= 2) // Middle Button is Pressed - USING MENU
    {
        encoder::middle = false;
        if (menu_page == 1) {
            menu_page = 2;
        } else if (menu_page == 2) {
            menu_page = 1;
        }
    }
    // MENU UI END

    // Menu using structure begin
    if (menu_page == 1 && menu_curr_item >= 2 && pid::mode == 3) {
        if (menu_curr_item == 2) {
            drawHome();
            displayMenuItem(str_diam, 1, true, pid::setpoint_float);
        } else if (menu_curr_item == 3) {
            drawHome();
            displayIntStringMenuPage(str_mode, 1, true, mode[pid::mode]);
        } else if (menu_curr_item == 4) {

            drawHome();

            lcd.setCursor(0, 1);
            lcd.print(">  PullSpd: ??"); // TODO
        } else if (menu_curr_item == 5) {
            drawHome();
            displayMenuItem(str_offset, 1, true, sensor::offset_float);
        } else if (menu_curr_item == 6) {
            using namespace stepper::distrib;

            drawHome();
            lcd.setCursor(0, 1);
            lcd.print(">  TravSpd: ");
            if (travel_speed > 0 && travel_speed < 10) {
                lcd.setCursor(12, 1);
                lcd.print("   ");
                lcd.setCursor(15, 1);
                lcd.print(travel_speed);
            } else if (travel_speed > 9 && travel_speed < 100) {
                lcd.setCursor(12, 1);
                lcd.print("  ");
                lcd.setCursor(14, 1);
                lcd.print(travel_speed);
            } else if (travel_speed > 99 && travel_speed < 999) {
                lcd.setCursor(12, 1);
                lcd.print(" ");
                lcd.setCursor(13, 1);
                lcd.print(travel_speed);
            } else if (travel_speed == 0) {
                lcd.setCursor(12, 1);
                lcd.print("Auto");
            }
        } else if (menu_curr_item == 7) {
            drawHome();
            displayMenuItem(str_spool_speed, 1, true, stepper::spool::rpm);
        } else if (menu_curr_item == 8) {
            drawHome();
            displayMenuItem(str_fan_speed, 1, true, fans::speed);
        } else if (menu_curr_item == 9) {
            drawHome();
            displayMenuItem(str_stats, 1, true, stepper::pull::total);
        }
    } else if (menu_page == 1 && menu_curr_item >= 2 && pid::mode != 3) {
        if (menu_curr_item == 2) {
            drawHome();
            displayMenuItem(str_diam, 1, true, pid::setpoint_float);
        } else if (menu_curr_item == 3) {
            drawHome();
            displayIntStringMenuPage(str_mode, 1, true, mode[pid::mode]);
        } else if (menu_curr_item == 4) {

            drawHome();
            displayMenuItem(str_offset, 1, true, sensor::offset_float);
        } else if (menu_curr_item == 5) {
            using namespace stepper::distrib;
            drawHome();
            lcd.setCursor(0, 1);
            lcd.print(">  TravSpd: ");
            if (travel_speed > 0 && travel_speed < 10) {
                lcd.setCursor(12, 1);
                lcd.print("   ");
                lcd.setCursor(15, 1);
                lcd.print(travel_speed);
            } else if (travel_speed > 9 && travel_speed < 100) {
                lcd.setCursor(12, 1);
                lcd.print("  ");
                lcd.setCursor(14, 1);
                lcd.print(travel_speed);
            } else if (travel_speed > 99 && travel_speed < 999) {
                lcd.setCursor(12, 1);
                lcd.print(" ");
                lcd.setCursor(13, 1);
                lcd.print(travel_speed);
            } else if (travel_speed == 0) {
                lcd.setCursor(12, 1);
                lcd.print("Auto");
            }
        } else if (menu_curr_item == 6) {
            drawHome();
            displayMenuItem(str_spool_speed, 1, true, stepper::spool::rpm);
        } else if (menu_curr_item == 7) {
            drawHome();
            displayMenuItem(str_fan_speed, 1, true, fans::speed);
        } else if (menu_curr_item == 8) {
            drawHome();
            displayMenuItem(str_stats, 1, true, stepper::pull::total);
        }
    }
    // Menu using structure end

    if (pid::mode == 3) {

        // Manual Mode Setting
        // Begin___________________________________________________________
        if (menu_page == 2 && menu_curr_item == 1) {
            // digitalWrite (enablePin, HIGH);
            displayStringMenuPage(mode[pid::mode]);
            if (encoder::up) {
                encoder::up = false;
                pid::mode--;

                if (pid::mode <= 0) {
                    pid::mode = 0;
                }
            } else if (encoder::down) {
                encoder::down = false;
                pid::mode++;

                if (pid::mode >= 3) {
                    pid::mode = 3;
                }
            }
        }
        if (menu_page == 3 && menu_curr_item == 1) {
            // digitalWrite (enablePin, HIGH);
            lcd.setCursor(0, 0);
            lcd.print("Set Diameter:");
            lcd.setCursor(0, 1);
            lcd.print(pid::setpoint_float, 2);

            if (encoder::up) {
                encoder::up = false;
                pid::setpoint_int--;
            } else if (encoder::down) {
                encoder::down = false;
                pid::setpoint_int++;
            }
            eeprom::update();
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (menu_page == 4 && menu_curr_item == 1) {
            using namespace stepper::distrib;
            lcd.setCursor(0, 0);
            lcd.print("Set Spool Begin:");
            if (entering_mm <= 9) {
                lcd.setCursor(0, 1);
                lcd.print(entering_mm);
                lcd.setCursor(1, 1);
                lcd.print(" ");
            } else if (entering_mm >= 10) {
                lcd.setCursor(0, 1);
                lcd.print(entering_mm);
            }
            if (encoder::up) {
                encoder::up = false;
                digitalWrite(stepper::PIN_DISTRIB_DIR, LOW);
                if (pos > 0) {
                    pos = pos - steps_per_click;
                } else
                    pos = 0;
                if (pos != last_pos) {
                    for (int x = 0; x < steps_per_click; x++) {
                        digitalWrite(stepper::PIN_DISTRIB_STEP, HIGH);
                        delay(1);
                        digitalWrite(stepper::PIN_DISTRIB_STEP, LOW);
                        delay(1);
                        // steppini--;
                    }
                }
                new_position = pos;
            } else if (encoder::down) {
                encoder::down = false;
                digitalWrite(stepper::PIN_DISTRIB_DIR, HIGH);
                if (pos < 7900 / 4) {
                    pos = pos + steps_per_click;
                } else
                    pos = 7900 / 4;
                if (pos != last_pos) {

                    for (int x = 0; x < steps_per_click; x++) {

                        digitalWrite(stepper::PIN_DISTRIB_STEP, HIGH);
                        delay(1);
                        digitalWrite(stepper::PIN_DISTRIB_STEP, LOW);
                        delay(1);
                        // steppini--;
                    }
                }
                new_position = pos;
            }
        }
        //////////////////////////////////////////////////////////
        if (menu_page == 5 && menu_curr_item == 1) {
            using namespace stepper::distrib;

            lcd.setCursor(0, 0);
            lcd.print("Set Spool End:");
            if (entering_mm <= 9) {
                lcd.setCursor(0, 1);
                lcd.print(entering_mm);
                lcd.setCursor(1, 1);
                lcd.print(" ");
            } else if (entering_mm >= 10) {
                lcd.setCursor(0, 1);
                lcd.print(entering_mm);
            }

            using namespace stepper::distrib;
            if (encoder::up) {
                encoder::up = false;
                digitalWrite(stepper::PIN_DISTRIB_DIR, LOW);
                if (pos > 0) {
                    pos = pos - steps_per_click;
                } else
                    pos = 0;
                if (pos != last_pos) {
                    for (int x = 0; x < steps_per_click; x++) {
                        digitalWrite(stepper::PIN_DISTRIB_STEP, HIGH);
                        delay(1);
                        digitalWrite(stepper::PIN_DISTRIB_STEP, LOW);
                        delay(1);
                        // steppini--;
                    }
                }
            } else if (encoder::down) {
                encoder::down = false;
                digitalWrite(stepper::PIN_DISTRIB_DIR, HIGH);
                if (pos < new_position_end) {
                    pos = pos + steps_per_click;
                } else
                    pos = new_position_end;
                if (pos != last_pos) {

                    for (int x = 0; x < steps_per_click; x++) {

                        digitalWrite(stepper::PIN_DISTRIB_STEP, HIGH);
                        delay(1);
                        digitalWrite(stepper::PIN_DISTRIB_STEP, LOW);
                        delay(1);
                        // steppini--;
                    }
                }
            }
        }
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (menu_page == 6 && menu_curr_item == 1) {
            using namespace stepper::pull;

            lcd.setCursor(0, 0);
            lcd.print("Set PullIntvl:");
            lcd.setCursor(0, 1);
            lcd.print(interval, 2);
            if (interval == 0) {
                interval = 9000;
            }

            if (encoder::up) {
                encoder::up = false;
                if (interval < 90000) {
                    interval = interval + 100;
                } else
                    interval = interval;
            } else if (encoder::down) {
                encoder::down = false;
                if (interval > 1000) {
                    interval = interval - 100;
                } else
                    interval = interval;
            }
        }
        // Manual Mode Setting
        // End_______________________________________________________________

        if (menu_page == 2 && menu_curr_item == 2) {
            drawHome();
            displayIntMenuPage(str_diam, 1, pid::setpoint_float);
            if (encoder::up) {
                encoder::up = false;
                pid::setpoint_int--;
            } else if (encoder::down) {
                encoder::down = false;
                pid::setpoint_int++;
            }
            eeprom::update();
        } else if (menu_page == 2 && menu_curr_item == 3) {
            drawHome();
            displayString2MenuPage(mode[pid::mode]);
            if (encoder::up) {
                encoder::up = false;
                pid::mode--;

                if (pid::mode <= 0) {
                    pid::mode = 0;
                }
            } else if (encoder::down) {
                encoder::down = false;
                pid::mode++;

                if (pid::mode >= 3) {
                    pid::mode = 3;
                }
            }
        } else if (menu_page == 2 && menu_curr_item == 4) {
            using namespace stepper::pull;
            drawHome();
            lcd.setCursor(0, 1);
            lcd.print("Set");
            lcd.setCursor(3, 1);
            lcd.print("Pullint");
            lcd.setCursor(11, 1);
            lcd.print("  ");
            lcd.setCursor(12, 1);
            lcd.print(interval, 2);
            if (interval == 0) {
                interval = 9000;
            }

            if (encoder::up) {
                encoder::up = false;
                if (interval < 90000) {
                    interval = interval + 100;
                } else
                    interval = interval;
            } else if (encoder::down) {
                encoder::down = false;
                if (interval > 1000) {
                    interval = interval - 100;
                } else
                    interval = interval;
            }
        } else if (menu_page == 2 && menu_curr_item == 5) {
            drawHome();
            if (sensor::offset_float >= 0) {
                lcd.setCursor(0, 1);
                lcd.print("Set");
                lcd.setCursor(3, 1);
                lcd.print("Offset");
                lcd.setCursor(11, 1);
                lcd.print("  ");
                lcd.setCursor(12, 1);
                lcd.print(sensor::offset_float, 2);
            } else if (sensor::offset_float < 0) {
                lcd.setCursor(0, 1);
                lcd.print("Set");
                lcd.setCursor(3, 1);
                lcd.print("Offset");
                lcd.setCursor(11, 1);
                lcd.print(sensor::offset_float, 2);
            }
            if (encoder::up) {
                encoder::up = false;
                sensor::offset--;
            } else if (encoder::down) {
                encoder::down = false;
                sensor::offset++;
            }
            eeprom::update();
        } else if (menu_page == 2 && menu_curr_item == 6) {
            using namespace stepper::distrib;

            drawHome();
            lcd.setCursor(0, 1);
            lcd.print("Set TravSpd ");
            if (travel_speed > 0 && travel_speed < 10) {
                lcd.setCursor(12, 1);
                lcd.print("   ");
                lcd.setCursor(15, 1);
                lcd.print(travel_speed);
            } else if (travel_speed > 9 && travel_speed < 100) {
                lcd.setCursor(12, 1);
                lcd.print("  ");
                lcd.setCursor(14, 1);
                lcd.print(travel_speed);
            } else if (travel_speed > 99 && travel_speed < 999) {
                lcd.setCursor(12, 1);
                lcd.print(" ");
                lcd.setCursor(13, 1);
                lcd.print(travel_speed);
            } else if (travel_speed == 0) {
                lcd.setCursor(12, 1);
                lcd.print("Auto");
            }
            if (encoder::up) {
                encoder::up = false;
                travel_speed--;
            } else if (encoder::down) {
                encoder::down = false;
                travel_speed++;
            }
        } else if (menu_page == 2 && menu_curr_item == 7) {
            drawHome();
            displayIntMenuPage(str_spool_speed, 1, stepper::spool::rpm);
            if (encoder::up) {
                encoder::up = false;
                stepper::spool::speed++;
            } else if (encoder::down) {
                encoder::down = false;
                stepper::spool::speed--;
            }
        } else if (menu_page == 2 && menu_curr_item == 8) {
            drawHome();
            displayIntMenuPage(str_fan_speed, 1, fans::speed);
            if (encoder::up) {
                encoder::up = false;
                fans::speed--;
            } else if (encoder::down) {
                encoder::down = false;
                fans::speed++;
            }
        } else if (menu_page == 2 && menu_curr_item == 9) {
            drawHome();
            lcd.setCursor(0, 1);
            lcd.print("Total meter:");
            lcd.setCursor(13, 1);
            lcd.print(stepper::pull::total);
        }
    }

    else if (pid::mode != 3) {
        // Preset Mode Setting
        // Begin_____________________________________________________________
        if (menu_page == 2 && menu_curr_item == 1) {
            displayStringMenuPage(mode[pid::mode]);
            if (encoder::up) {
                encoder::up = false;
                pid::mode--;

                if (pid::mode <= 0) {
                    pid::mode = 0;
                }
            } else if (encoder::down) {
                encoder::down = false;
                pid::mode++;

                if (pid::mode >= 3) {
                    pid::mode = 3;
                }
            }
        }
        if (menu_page == 3 && menu_curr_item == 1) {
            lcd.setCursor(0, 0);
            lcd.print("Set Diameter:");
            lcd.setCursor(0, 1);
            lcd.print(pid::setpoint_float, 2);
            if (encoder::up) {
                encoder::up = false;
                pid::setpoint_int--;
            } else if (encoder::down) {
                encoder::down = false;
                pid::setpoint_int++;
            }
            eeprom::update();
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (menu_page == 4 && menu_curr_item == 1) {
            using namespace stepper::distrib;
            lcd.setCursor(0, 0);
            lcd.print("Set Spool Begin:");
            if (entering_mm <= 9) {
                lcd.setCursor(0, 1);
                lcd.print(entering_mm);
                lcd.setCursor(1, 1);
                lcd.print(" ");
            } else if (entering_mm >= 10) {
                lcd.setCursor(0, 1);
                lcd.print(entering_mm);
            }
            if (encoder::up) {
                encoder::up = false;
                digitalWrite(stepper::PIN_DISTRIB_DIR, LOW);
                if (pos > 0) {
                    pos = pos - steps_per_click;
                } else
                    pos = 0;
                if (pos != last_pos) {

                    for (int x = 0; x < steps_per_click; x++) {

                        digitalWrite(stepper::PIN_DISTRIB_STEP, HIGH);
                        delay(1);
                        digitalWrite(stepper::PIN_DISTRIB_STEP, LOW);
                        delay(1);
                        // steppini--;
                    }
                }
                new_position = pos;
            } else if (encoder::down) {
                encoder::down = false;
                digitalWrite(stepper::PIN_DISTRIB_DIR, HIGH);
                if (pos < 7900 / 4) {
                    pos = pos + steps_per_click;
                } else
                    pos = 7900 / 4;
                if (pos != last_pos) {

                    for (int x = 0; x < steps_per_click; x++) {

                        digitalWrite(stepper::PIN_DISTRIB_STEP, HIGH);
                        delay(1);
                        digitalWrite(stepper::PIN_DISTRIB_STEP, LOW);
                        delay(1);
                        // steppini--;
                    }
                }
                new_position = pos;
            }
        }
        //////////////////////////////////////////////////////////////////////////////////////
        if (menu_page == 5 && menu_curr_item == 1) {
            using namespace stepper::distrib;
            lcd.setCursor(0, 0);
            lcd.print("Set Spool End:");
            if (entering_mm <= 9) {
                lcd.setCursor(0, 1);
                lcd.print(entering_mm);
                lcd.setCursor(1, 1);
                lcd.print(" ");
            } else if (entering_mm >= 10) {
                lcd.setCursor(0, 1);
                lcd.print(entering_mm);
            }

            if (encoder::up) {
                encoder::up = false;
                digitalWrite(stepper::PIN_DISTRIB_DIR, LOW);
                if (pos > 0) {
                    pos = pos - steps_per_click;
                } else
                    pos = 0;
                if (pos != last_pos) {

                    for (int x = 0; x < steps_per_click; x++) {

                        digitalWrite(stepper::PIN_DISTRIB_STEP, HIGH);
                        delay(1);
                        digitalWrite(stepper::PIN_DISTRIB_STEP, LOW);
                        delay(1);
                        // steppini--;
                    }
                }
            } else if (encoder::down) {
                encoder::down = false;
                digitalWrite(stepper::PIN_DISTRIB_DIR, HIGH);
                if (pos < new_position_end) {
                    pos = pos + steps_per_click;
                } else
                    pos = new_position_end;
                if (pos != last_pos) {

                    for (int x = 0; x < steps_per_click; x++) {

                        digitalWrite(stepper::PIN_DISTRIB_STEP, HIGH);
                        delay(1);
                        digitalWrite(stepper::PIN_DISTRIB_STEP, LOW);
                        delay(1);
                        // steppini--;
                    }
                }
            }
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (menu_page == 6 && menu_curr_item == 1) {
            lcd.setCursor(0, 0);
            lcd.print("Set PullSpeed:");
            lcd.setCursor(0, 1);
            lcd.print(stepper::pull::speed);
        }
        // Preset Mode Setting
        // End________________________________________________________________

        if (menu_page == 2 && menu_curr_item == 2) {
            drawHome();
            lcd.setCursor(0, 1);
            lcd.print("Set");
            lcd.setCursor(3, 1);
            lcd.print("Diameter");
            lcd.setCursor(11, 1);
            lcd.print("  ");
            lcd.setCursor(12, 1);
            lcd.print(pid::setpoint_float, 2);
            if (encoder::up) {
                encoder::up = false;
                pid::setpoint_int--;
            } else if (encoder::down) {
                encoder::down = false;
                pid::setpoint_int++;
            }
            eeprom::update();
        } else if (menu_page == 2 && menu_curr_item == 3) {
            drawHome();
            displayString2MenuPage(mode[pid::mode]);
            if (encoder::up) {
                encoder::up = false;
                pid::mode--;

                if (pid::mode <= 0) {
                    pid::mode = 0;
                }
            } else if (encoder::down) {
                encoder::down = false;
                pid::mode++;
                if (pid::mode >= 3) {
                    pid::mode = 3;
                }
            }
        } else if (menu_page == 2 && menu_curr_item == 4) {
            drawHome();
            if (sensor::offset_float >= 0) {
                lcd.setCursor(0, 1);
                lcd.print("Set");
                lcd.setCursor(3, 1);
                lcd.print("Offset");
                lcd.setCursor(11, 1);
                lcd.print("  ");
                lcd.setCursor(12, 1);
                lcd.print(sensor::offset_float, 2);
            } else if (sensor::offset_float < 0) {
                lcd.setCursor(0, 1);
                lcd.print("Set");
                lcd.setCursor(3, 1);
                lcd.print("Offset");
                lcd.setCursor(11, 1);
                lcd.print(sensor::offset_float, 2);
            }

            if (encoder::up) {
                encoder::up = false;
                sensor::offset--;
            } else if (encoder::down) {
                encoder::down = false;
                sensor::offset++;
            }
            eeprom::update();
        } else if (menu_page == 2 && menu_curr_item == 5) {
            using namespace stepper::distrib;
            drawHome();
            lcd.setCursor(0, 1);
            lcd.print("Set TravSpd ");
            if (travel_speed >= 0 && travel_speed < 10) {
                lcd.setCursor(12, 1);
                lcd.print("   ");
                lcd.setCursor(15, 1);
                lcd.print(travel_speed);
            } else if (travel_speed > 9 && travel_speed < 100) {
                lcd.setCursor(12, 1);
                lcd.print("  ");
                lcd.setCursor(14, 1);
                lcd.print(travel_speed);
            } else if (travel_speed > 99 && travel_speed < 999) {
                lcd.setCursor(12, 1);
                lcd.print(" ");
                lcd.setCursor(13, 1);
                lcd.print(travel_speed);
            }
            if (encoder::up) {
                encoder::up = false;
                travel_speed--;
            } else if (encoder::down) {
                encoder::down = false;
                travel_speed++;
            }
        } else if (menu_page == 2 && menu_curr_item == 6) {
            drawHome();
            displayIntMenuPage(str_spool_speed, 1, stepper::spool::rpm);
            if (encoder::up) {
                encoder::up = false;
                stepper::spool::speed++;
            } else if (encoder::down) {
                encoder::down = false;
                stepper::spool::speed--;
            }
        } else if (menu_page == 2 && menu_curr_item == 7) {
            drawHome();
            displayIntMenuPage(str_fan_speed, 1, fans::speed);
            if (encoder::up) {
                encoder::up = false;
                fans::speed--;
            } else if (encoder::down) {
                encoder::down = false;
                fans::speed++;
            }
        } else if (menu_page == 2 && menu_curr_item == 8) {
            drawHome();
            // STATS
            lcd.setCursor(0, 1);
            lcd.print("Total meter:");
            lcd.setCursor(13, 1);
            lcd.print(stepper::pull::total);
        }
    }

    if (menu_curr_item > 1) {
        sensor::update();
    }
}

void displayIntMenuPage(String menuItem, int position, int value)
{

    lcd.setCursor(0, position);
    lcd.print("Set");
    lcd.setCursor(3, position);
    lcd.print(menuItem);

    if (value == 0 && menuItem != 2) {
        lcd.setCursor(12, position);
        lcd.print("   ");
        lcd.setCursor(15, position);
    } else if (value > 0 && value < 10 && menuItem != 2) {
        lcd.setCursor(12, position);
        lcd.print("   ");
        lcd.setCursor(15, position);
    } else if (value > 9 && value < 100 && menuItem != 2) {
        lcd.setCursor(12, position);
        lcd.print("  ");
        lcd.setCursor(14, position);
    } else if (value > 99 && value < 999 && menuItem != 2) {
        lcd.setCursor(12, position);
        lcd.print(" ");
        lcd.setCursor(13, position);
    } else if (value > 999 && value < 9999 && menuItem != 2) {
        lcd.setCursor(12, position);
    }
    lcd.print(value);
}
void displayStringMenuPage(String value)
{
    lcd.setCursor(0, 0);
    lcd.print("Set Mode:");
    lcd.setCursor(0, 1);
    lcd.print(">");
    if (pid::mode == 0) {
        lcd.setCursor(1, 1);
        lcd.print("    ");
        lcd.setCursor(5, 1);
    } else if (pid::mode == 1) {
        lcd.setCursor(1, 1);
        lcd.print(" ");
        lcd.setCursor(3, 1);
    } else if (pid::mode == 2) {
        lcd.setCursor(1, 1);
        lcd.print("    ");
        lcd.setCursor(5, 1);
    } else if (pid::mode == 3) {
        lcd.setCursor(1, 1);
        lcd.print(" ");
        lcd.setCursor(3, 1);
    }

    lcd.print(value);
}
void displayString2MenuPage(String value)
{
    lcd.setCursor(0, 1);
    lcd.print("Set");
    lcd.setCursor(3, 1);
    lcd.print(" Mode");

    if (pid::mode == 0) {
        lcd.setCursor(8, 1);
        lcd.print("    ");
        lcd.setCursor(12, 1);
    } else if (pid::mode == 1) {
        lcd.setCursor(8, 1);
        lcd.print(" ");
        lcd.setCursor(10, 1);
    } else if (pid::mode == 2) {
        lcd.setCursor(8, 1);
        lcd.print("    ");
        lcd.setCursor(12, 1);
    } else if (pid::mode == 3) {
        lcd.setCursor(8, 1);
        lcd.print(" ");
        lcd.setCursor(10, 1);
    }
    lcd.print(value);
}
void displayIntStringMenuPage(String item, int position, boolean selected, String value)
{
    lcd.setCursor(0, position);
    lcd.print(">");
    lcd.setCursor(1, position);
    lcd.print(item);
    lcd.setCursor(12, position);
    lcd.print(value);
}
void displayMenuItem(String item, int position, boolean selected, int value)
{
    lcd.setCursor(0, position);
    lcd.print(">");
    lcd.setCursor(1, position);
    lcd.print(item);
    if (value == 0 && menu_curr_item != 2 && menu_curr_item != 4) {
        lcd.setCursor(11, position);
        lcd.print("     ");
        lcd.setCursor(15, position);
    } else if (value > 0 && value < 10 && menu_curr_item != 2 && menu_curr_item != 4) {
        lcd.setCursor(11, position);
        lcd.print("     ");
        lcd.setCursor(15, position);
    } else if (value > 9 && value < 100 && menu_curr_item != 2 && menu_curr_item != 4) {
        lcd.setCursor(11, position);
        lcd.print("    ");
        lcd.setCursor(14, position);
    } else if (value > 99 && value < 999 && menu_curr_item != 2 && menu_curr_item != 4) {
        lcd.setCursor(11, position);
        lcd.print("   ");
        lcd.setCursor(13, position);
    } else if (value > 999 && value < 9999 && menu_curr_item != 2 && menu_curr_item != 4) {
        lcd.setCursor(11, position);
        lcd.print("  ");
        lcd.setCursor(12, position);
    }
    lcd.print(value);

    if (menu_curr_item == 2) {
        lcd.setCursor(11, position);
        lcd.print("  ");
        lcd.setCursor(12, position);
        lcd.print(pid::setpoint_float, 2);
    }

    if (menu_curr_item == 4) {
        if (sensor::offset_float >= 0) {
            lcd.setCursor(11, position);
            lcd.print("+");
            lcd.setCursor(12, position);
            lcd.print(sensor::offset_float, 2);
        } else if (sensor::offset_float < 0) {
            lcd.setCursor(11, position);
            lcd.print(sensor::offset_float, 2);
        }
    }
}
