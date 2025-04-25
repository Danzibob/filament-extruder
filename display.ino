#include "./display.h"

void drawHome()
{
    // MenuHome
    if (stepper_enabled == 0) {
        lcd.setCursor(0, 0);
        lcd.print(" Spooler ready! ");
    } else {
        lcd.setCursor(0, 0);
        lcd.print(char(0));
        unsigned long currentMillis = millis();
        if (currentMillis - lcd_prevMillis >= lcd_interval) {
            // save the last time you blinked the LED
            lcd_prevMillis = currentMillis;
            lcd.setCursor(1, 0);
            lcd.print abs(width_curr);
            lcd.setCursor(9, 0);
            if (pid_mode != 3) {
                lcd.print(extrude_speed, 2);
            } else
                lcd.print(extrude_speed2, 2);
            lcd.setCursor(13, 0);
            lcd.print(" ");
        }
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
    }
}

void drawMenu()
{
    // MENU UI Start
    if (menu_curr_item == 2 && menu_page == 1) {
        encoder_up = false;
    }
    if (encoder_up && menu_page == 1) {

        encoder_up = false;
        menu_last_item = menu_curr_item;
        menu_curr_item--;
        if (menu_curr_item == 0) {
            menu_curr_item = 1;
        }
    }
    if (encoder_down && menu_page == 1) // We have turned the Rotary Encoder Clockwise
    {
        encoder_down = false;
        menu_last_item = menu_curr_item;
        menu_curr_item++;
        if (menu_curr_item == 10 && pid_mode == 3) {
            menu_curr_item--;
        } else if (menu_curr_item == 9 && pid_mode != 3) {
            menu_curr_item--;
        }
    }
    // SETUP MENU
    // Begin____________________________________________________________________
    if (menu_page == 1 && menu_curr_item == 1) {
        menu_page = 2;
    }
    if (encoder_middle && menu_curr_item == 1) // Middle Button is Pressed - SETUP MENU
    {
        encoder_middle = false;

        if (menu_page == 1) {

            menu_page = 2;
        } else if (menu_page == 2) {
            menu_page = 3;
        } else if (menu_page == 3) {
            menu_page = 4;
        } else if (menu_page == 4) {
            travel_begin = distrib_stepper_pos;
            lcd.setCursor(0, 0);
            lcd.print("Wait while I'm");
            lcd.setCursor(0, 1);
            lcd.print("moving stepper");
            digitalWrite(PIN_DISTRIB_DIR, HIGH);
            for (int x = 0; x < distrib_new_position_end; x++) {

                digitalWrite(PIN_DISTRIB_STEP, HIGH);
                delay(1);
                digitalWrite(PIN_DISTRIB_STEP, LOW);
                delay(1);
            }

            distrib_stepper_pos = distrib_new_position_end;
            lcd.clear();
            menu_page = 5;
        } else if (menu_page == 5) {
            travel_end = distrib_stepper_pos;
            travel_step = travel_end;
            if (pid_mode <= 2) {
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

    if (encoder_middle && menu_curr_item >= 2) // Middle Button is Pressed - USING MENU
    {
        encoder_middle = false;
        if (menu_page == 1) {
            menu_page = 2;
        } else if (menu_page == 2) {
            menu_page = 1;
        }
    }
    // MENU UI END

    // Menu using structure begin
    if (menu_page == 1 && menu_curr_item >= 2 && pid_mode == 3) {
        if (menu_curr_item == 2) {
            drawHome();
            displayMenuItem(str_diam, 1, true, pid_setpoint_float);
        } else if (menu_curr_item == 3) {
            drawHome();
            displayIntStringMenuPage(str_mode, 1, true, mode[pid_mode]);
        } else if (menu_curr_item == 4) {

            drawHome();

            lcd.setCursor(0, 1);
            lcd.print(">  PullSpd:");
            lcd.setCursor(12, 1);
            lcd.print(extrude_speed2, 2);
        } else if (menu_curr_item == 5) {
            drawHome();
            displayMenuItem(str_offset, 1, true, width_offset_float);
        } else if (menu_curr_item == 6) {
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
            displayMenuItem(str_spool_speed, 1, true, spool_rpm);
        } else if (menu_curr_item == 8) {
            drawHome();
            displayMenuItem(str_fan_speed, 1, true, fans::speed);
        } else if (menu_curr_item == 9) {
            drawHome();
            displayMenuItem(str_stats, 1, true, puller_total);
        }
    } else if (menu_page == 1 && menu_curr_item >= 2 && pid_mode != 3) {
        if (menu_curr_item == 2) {
            drawHome();
            displayMenuItem(str_diam, 1, true, pid_setpoint_float);
        } else if (menu_curr_item == 3) {
            drawHome();
            displayIntStringMenuPage(str_mode, 1, true, mode[pid_mode]);
        } else if (menu_curr_item == 4) {

            drawHome();
            displayMenuItem(str_offset, 1, true, width_offset_float);
        } else if (menu_curr_item == 5) {
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
            displayMenuItem(str_spool_speed, 1, true, spool_rpm);
        } else if (menu_curr_item == 7) {
            drawHome();
            displayMenuItem(str_fan_speed, 1, true, fans::speed);
        } else if (menu_curr_item == 8) {
            drawHome();
            displayMenuItem(str_stats, 1, true, puller_total);
        }
    }
    // Menu using structure end

    if (pid_mode == 3) {

        // Manual Mode Setting
        // Begin___________________________________________________________
        if (menu_page == 2 && menu_curr_item == 1) {
            // digitalWrite (enablePin, HIGH);
            displayStringMenuPage(mode[pid_mode]);
            if (encoder_up) {
                encoder_up = false;
                pid_mode--;

                if (pid_mode <= 0) {
                    pid_mode = 0;
                }
            } else if (encoder_down) {
                encoder_down = false;
                pid_mode++;

                if (pid_mode >= 3) {
                    pid_mode = 3;
                }
            }
        }
        if (menu_page == 3 && menu_curr_item == 1) {
            // digitalWrite (enablePin, HIGH);
            lcd.setCursor(0, 0);
            lcd.print("Set Diameter:");
            lcd.setCursor(0, 1);
            lcd.print(pid_setpoint_float, 2);

            if (encoder_up) {
                encoder_up = false;
                pid_setpoint_int--;
                EEPROM.update(width_eeprom_diam, highByte(pid_setpoint_int));
                EEPROM.update(width_eeprom_diam + 1, lowByte(pid_setpoint_int));
            } else if (encoder_down) {
                encoder_down = false;
                pid_setpoint_int++;
                EEPROM.update(width_eeprom_diam, highByte(pid_setpoint_int));
                EEPROM.update(width_eeprom_diam + 1, lowByte(pid_setpoint_int));
            }
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (menu_page == 4 && menu_curr_item == 1) {
            lcd.setCursor(0, 0);
            lcd.print("Set Spool Begin:");
            if (mm <= 9) {
                lcd.setCursor(0, 1);
                lcd.print(mm);
                lcd.setCursor(1, 1);
                lcd.print(" ");
            } else if (mm >= 10) {
                lcd.setCursor(0, 1);
                lcd.print(mm);
            }
            if (encoder_up) {
                encoder_up = false;
                digitalWrite(PIN_DISTRIB_DIR, LOW);
                if (distrib_stepper_pos > 0) {
                    distrib_stepper_pos = distrib_stepper_pos - steps_per_click;
                } else
                    distrib_stepper_pos = 0;
                if (distrib_stepper_pos != distrib_last_stepper_pos) {
                    for (int x = 0; x < steps_per_click; x++) {

                        digitalWrite(PIN_DISTRIB_STEP, HIGH);
                        delay(1);
                        digitalWrite(PIN_DISTRIB_STEP, LOW);
                        delay(1);
                        // steppini--;
                    }
                }
                distrib_new_position = distrib_stepper_pos;
            } else if (encoder_down) {
                encoder_down = false;
                digitalWrite(PIN_DISTRIB_DIR, HIGH);
                if (distrib_stepper_pos < 7900 / 4) {
                    distrib_stepper_pos = distrib_stepper_pos + steps_per_click;
                } else
                    distrib_stepper_pos = 7900 / 4;
                if (distrib_stepper_pos != distrib_last_stepper_pos) {

                    for (int x = 0; x < steps_per_click; x++) {

                        digitalWrite(PIN_DISTRIB_STEP, HIGH);
                        delay(1);
                        digitalWrite(PIN_DISTRIB_STEP, LOW);
                        delay(1);
                        // steppini--;
                    }
                }
                distrib_new_position = distrib_stepper_pos;
            }
        }
        //////////////////////////////////////////////////////////
        if (menu_page == 5 && menu_curr_item == 1) {
            lcd.setCursor(0, 0);
            lcd.print("Set Spool End:");
            if (mm <= 9) {
                lcd.setCursor(0, 1);
                lcd.print(mm);
                lcd.setCursor(1, 1);
                lcd.print(" ");
            } else if (mm >= 10) {
                lcd.setCursor(0, 1);
                lcd.print(mm);
            }

            if (encoder_up) {
                encoder_up = false;
                digitalWrite(PIN_DISTRIB_DIR, LOW);
                if (distrib_stepper_pos > 0) {
                    distrib_stepper_pos = distrib_stepper_pos - steps_per_click;
                } else
                    distrib_stepper_pos = 0;
                if (distrib_stepper_pos != distrib_last_stepper_pos) {

                    for (int x = 0; x < steps_per_click; x++) {

                        digitalWrite(PIN_DISTRIB_STEP, HIGH);
                        delay(1);
                        digitalWrite(PIN_DISTRIB_STEP, LOW);
                        delay(1);
                        // steppini--;
                    }
                }
            } else if (encoder_down) {
                encoder_down = false;
                digitalWrite(PIN_DISTRIB_DIR, HIGH);
                if (distrib_stepper_pos < distrib_new_position_end) {
                    distrib_stepper_pos = distrib_stepper_pos + steps_per_click;
                } else
                    distrib_stepper_pos = distrib_new_position_end;
                if (distrib_stepper_pos != distrib_last_stepper_pos) {

                    for (int x = 0; x < steps_per_click; x++) {

                        digitalWrite(PIN_DISTRIB_STEP, HIGH);
                        delay(1);
                        digitalWrite(PIN_DISTRIB_STEP, LOW);
                        delay(1);
                        // steppini--;
                    }
                }
            }
        }
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (menu_page == 6 && menu_curr_item == 1) {
            lcd.setCursor(0, 0);
            lcd.print("Set PullSpeed:");
            lcd.setCursor(0, 1);
            lcd.print(extrude_speed2, 2);
            if (puller_interval == 0) {
                puller_interval = 9000;
            }

            if (encoder_up) {
                encoder_up = false;
                if (puller_interval < 90000) {
                    puller_interval = puller_interval + 100;
                } else
                    puller_interval = puller_interval;
            } else if (encoder_down) {
                encoder_down = false;
                if (puller_interval > 1000) {
                    puller_interval = puller_interval - 100;
                } else
                    puller_interval = puller_interval;
            }
        }
        // Manual Mode Setting
        // End_______________________________________________________________

        if (menu_page == 2 && menu_curr_item == 2) {
            drawHome();
            displayIntMenuPage(str_diam, 1, pid_setpoint_float);
            if (encoder_up) {
                encoder_up = false;
                pid_setpoint_int--;
                EEPROM.update(width_eeprom_diam, highByte(pid_setpoint_int));
                EEPROM.update(width_eeprom_diam + 1, lowByte(pid_setpoint_int));
            } else if (encoder_down) {
                encoder_down = false;
                pid_setpoint_int++;
                EEPROM.update(width_eeprom_diam, highByte(pid_setpoint_int));
                EEPROM.update(width_eeprom_diam + 1, lowByte(pid_setpoint_int));
            }
        } else if (menu_page == 2 && menu_curr_item == 3) {
            drawHome();
            displayString2MenuPage(mode[pid_mode]);
            if (encoder_up) {
                encoder_up = false;
                pid_mode--;

                if (pid_mode <= 0) {
                    pid_mode = 0;
                }
            } else if (encoder_down) {
                encoder_down = false;
                pid_mode++;

                if (pid_mode >= 3) {
                    pid_mode = 3;
                }
            }
        } else if (menu_page == 2 && menu_curr_item == 4) {
            drawHome();
            lcd.setCursor(0, 1);
            lcd.print("Set");
            lcd.setCursor(3, 1);
            lcd.print("Pullspd");
            lcd.setCursor(11, 1);
            lcd.print("  ");
            lcd.setCursor(12, 1);
            // extspd2 = 60 / ((pullspd * 400) / 1000) * 0.0942;
            lcd.print(extrude_speed2, 2);
            if (puller_interval == 0) {
                puller_interval = 9000;
            }

            if (encoder_up) {
                encoder_up = false;
                if (puller_interval < 90000) {
                    puller_interval = puller_interval + 100;
                } else
                    puller_interval = puller_interval;
            } else if (encoder_down) {
                encoder_down = false;
                if (puller_interval > 1000) {
                    puller_interval = puller_interval - 100;
                } else
                    puller_interval = puller_interval;
            }
        } else if (menu_page == 2 && menu_curr_item == 5) {
            drawHome();
            if (width_offset_float >= 0) {
                lcd.setCursor(0, 1);
                lcd.print("Set");
                lcd.setCursor(3, 1);
                lcd.print("Offset");
                lcd.setCursor(11, 1);
                lcd.print("  ");
                lcd.setCursor(12, 1);
                lcd.print(width_offset_float, 2);
            } else if (width_offset_float < 0) {
                lcd.setCursor(0, 1);
                lcd.print("Set");
                lcd.setCursor(3, 1);
                lcd.print("Offset");
                lcd.setCursor(11, 1);
                lcd.print(width_offset_float, 2);
            }
            if (encoder_up) {
                encoder_up = false;
                width_offset--;
                EEPROM.update(width_eeprom_offset, width_offset);
            } else if (encoder_down) {
                encoder_down = false;
                width_offset++;
                EEPROM.update(width_eeprom_offset, width_offset);
            }
        } else if (menu_page == 2 && menu_curr_item == 6) {
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
            if (encoder_up) {
                encoder_up = false;
                travel_speed--;
            } else if (encoder_down) {
                encoder_down = false;
                travel_speed++;
            }
        } else if (menu_page == 2 && menu_curr_item == 7) {
            drawHome();
            displayIntMenuPage(str_spool_speed, 1, spool_rpm);
            if (encoder_up) {
                encoder_up = false;
                spool_speed++;
            } else if (encoder_down) {
                encoder_down = false;
                spool_speed--;
            }
        } else if (menu_page == 2 && menu_curr_item == 8) {
            drawHome();
            displayIntMenuPage(str_fan_speed, 1, fans::speed);
            if (encoder_up) {
                encoder_up = false;
                fans::speed--;
            } else if (encoder_down) {
                encoder_down = false;
                fans::speed++;
            }
        } else if (menu_page == 2 && menu_curr_item == 9) {
            drawHome();
            lcd.setCursor(0, 1);
            lcd.print("Total meter:");
            lcd.setCursor(13, 1);
            lcd.print(puller_total);
        }
    }

    else if (pid_mode != 3) {
        // Preset Mode Setting
        // Begin_____________________________________________________________
        if (menu_page == 2 && menu_curr_item == 1) {
            displayStringMenuPage(mode[pid_mode]);
            if (encoder_up) {
                encoder_up = false;
                pid_mode--;

                if (pid_mode <= 0) {
                    pid_mode = 0;
                }
            } else if (encoder_down) {
                encoder_down = false;
                pid_mode++;

                if (pid_mode >= 3) {
                    pid_mode = 3;
                }
            }
        }
        if (menu_page == 3 && menu_curr_item == 1) {
            lcd.setCursor(0, 0);
            lcd.print("Set Diameter:");
            lcd.setCursor(0, 1);
            lcd.print(pid_setpoint_float, 2);
            if (encoder_up) {
                encoder_up = false;
                pid_setpoint_int--;
                EEPROM.update(width_eeprom_diam, highByte(pid_setpoint_int));
                EEPROM.update(width_eeprom_diam + 1, lowByte(pid_setpoint_int));
            } else if (encoder_down) {
                encoder_down = false;
                pid_setpoint_int++;
                EEPROM.update(width_eeprom_diam, highByte(pid_setpoint_int));
                EEPROM.update(width_eeprom_diam + 1, lowByte(pid_setpoint_int));
            }
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (menu_page == 4 && menu_curr_item == 1) {
            lcd.setCursor(0, 0);
            lcd.print("Set Spool Begin:");
            if (mm <= 9) {
                lcd.setCursor(0, 1);
                lcd.print(mm);
                lcd.setCursor(1, 1);
                lcd.print(" ");
            } else if (mm >= 10) {
                lcd.setCursor(0, 1);
                lcd.print(mm);
            }
            if (encoder_up) {
                encoder_up = false;
                digitalWrite(PIN_DISTRIB_DIR, LOW);
                if (distrib_stepper_pos > 0) {
                    distrib_stepper_pos = distrib_stepper_pos - steps_per_click;
                } else
                    distrib_stepper_pos = 0;
                if (distrib_stepper_pos != distrib_last_stepper_pos) {

                    for (int x = 0; x < steps_per_click; x++) {

                        digitalWrite(PIN_DISTRIB_STEP, HIGH);
                        delay(1);
                        digitalWrite(PIN_DISTRIB_STEP, LOW);
                        delay(1);
                        // steppini--;
                    }
                }
                distrib_new_position = distrib_stepper_pos;
            } else if (encoder_down) {
                encoder_down = false;
                digitalWrite(PIN_DISTRIB_DIR, HIGH);
                if (distrib_stepper_pos < 7900 / 4) {
                    distrib_stepper_pos = distrib_stepper_pos + steps_per_click;
                } else
                    distrib_stepper_pos = 7900 / 4;
                if (distrib_stepper_pos != distrib_last_stepper_pos) {

                    for (int x = 0; x < steps_per_click; x++) {

                        digitalWrite(PIN_DISTRIB_STEP, HIGH);
                        delay(1);
                        digitalWrite(PIN_DISTRIB_STEP, LOW);
                        delay(1);
                        // steppini--;
                    }
                }
                distrib_new_position = distrib_stepper_pos;
            }
        }
        //////////////////////////////////////////////////////////////////////////////////////
        if (menu_page == 5 && menu_curr_item == 1) {
            lcd.setCursor(0, 0);
            lcd.print("Set Spool End:");
            if (mm <= 9) {
                lcd.setCursor(0, 1);
                lcd.print(mm);
                lcd.setCursor(1, 1);
                lcd.print(" ");
            } else if (mm >= 10) {
                lcd.setCursor(0, 1);
                lcd.print(mm);
            }

            if (encoder_up) {
                encoder_up = false;
                digitalWrite(PIN_DISTRIB_DIR, LOW);
                if (distrib_stepper_pos > 0) {
                    distrib_stepper_pos = distrib_stepper_pos - steps_per_click;
                } else
                    distrib_stepper_pos = 0;
                if (distrib_stepper_pos != distrib_last_stepper_pos) {

                    for (int x = 0; x < steps_per_click; x++) {

                        digitalWrite(PIN_DISTRIB_STEP, HIGH);
                        delay(1);
                        digitalWrite(PIN_DISTRIB_STEP, LOW);
                        delay(1);
                        // steppini--;
                    }
                }
            } else if (encoder_down) {
                encoder_down = false;
                digitalWrite(PIN_DISTRIB_DIR, HIGH);
                if (distrib_stepper_pos < distrib_new_position_end) {
                    distrib_stepper_pos = distrib_stepper_pos + steps_per_click;
                } else
                    distrib_stepper_pos = distrib_new_position_end;
                if (distrib_stepper_pos != distrib_last_stepper_pos) {

                    for (int x = 0; x < steps_per_click; x++) {

                        digitalWrite(PIN_DISTRIB_STEP, HIGH);
                        delay(1);
                        digitalWrite(PIN_DISTRIB_STEP, LOW);
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
            lcd.print(pull_speed);
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
            lcd.print(pid_setpoint_float, 2);
            if (encoder_up) {
                encoder_up = false;
                pid_setpoint_int--;
                EEPROM.update(width_eeprom_diam, highByte(pid_setpoint_int));
                EEPROM.update(width_eeprom_diam + 1, lowByte(pid_setpoint_int));
            } else if (encoder_down) {
                encoder_down = false;
                pid_setpoint_int++;
                EEPROM.update(width_eeprom_diam, highByte(pid_setpoint_int));
                EEPROM.update(width_eeprom_diam + 1, lowByte(pid_setpoint_int));
            }
        } else if (menu_page == 2 && menu_curr_item == 3) {
            drawHome();
            displayString2MenuPage(mode[pid_mode]);
            if (encoder_up) {
                encoder_up = false;
                pid_mode--;

                if (pid_mode <= 0) {
                    pid_mode = 0;
                }
            } else if (encoder_down) {
                encoder_down = false;
                pid_mode++;
                if (pid_mode >= 3) {
                    pid_mode = 3;
                }
            }
        } else if (menu_page == 2 && menu_curr_item == 4) {
            drawHome();
            if (width_offset_float >= 0) {
                lcd.setCursor(0, 1);
                lcd.print("Set");
                lcd.setCursor(3, 1);
                lcd.print("Offset");
                lcd.setCursor(11, 1);
                lcd.print("  ");
                lcd.setCursor(12, 1);
                lcd.print(width_offset_float, 2);
            } else if (width_offset_float < 0) {
                lcd.setCursor(0, 1);
                lcd.print("Set");
                lcd.setCursor(3, 1);
                lcd.print("Offset");
                lcd.setCursor(11, 1);
                lcd.print(width_offset_float, 2);
            }

            if (encoder_up) {
                encoder_up = false;
                width_offset--;
                EEPROM.update(width_eeprom_offset, width_offset);
            } else if (encoder_down) {
                encoder_down = false;
                width_offset++;
                EEPROM.update(width_eeprom_offset, width_offset);
            }
        } else if (menu_page == 2 && menu_curr_item == 5) {
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
            if (encoder_up) {
                encoder_up = false;
                travel_speed--;
            } else if (encoder_down) {
                encoder_down = false;
                travel_speed++;
            }
        } else if (menu_page == 2 && menu_curr_item == 6) {
            drawHome();
            displayIntMenuPage(str_spool_speed, 1, spool_rpm);
            if (encoder_up) {
                encoder_up = false;
                spool_speed++;
            } else if (encoder_down) {
                encoder_down = false;
                spool_speed--;
            }
        } else if (menu_page == 2 && menu_curr_item == 7) {
            drawHome();
            displayIntMenuPage(str_fan_speed, 1, fans::speed);
            if (encoder_up) {
                encoder_up = false;
                fans::speed--;
            } else if (encoder_down) {
                encoder_down = false;
                fans::speed++;
            }
        } else if (menu_page == 2 && menu_curr_item == 8) {
            drawHome();
            // STATS
            lcd.setCursor(0, 1);
            lcd.print("Total meter:");
            lcd.setCursor(13, 1);
            lcd.print(puller_total);
        }
    }

    if (menu_curr_item > 1) {
        Sensor();
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
    if (pid_mode == 0) {
        lcd.setCursor(1, 1);
        lcd.print("    ");
        lcd.setCursor(5, 1);
    } else if (pid_mode == 1) {
        lcd.setCursor(1, 1);
        lcd.print(" ");
        lcd.setCursor(3, 1);
    } else if (pid_mode == 2) {
        lcd.setCursor(1, 1);
        lcd.print("    ");
        lcd.setCursor(5, 1);
    } else if (pid_mode == 3) {
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

    if (pid_mode == 0) {
        lcd.setCursor(8, 1);
        lcd.print("    ");
        lcd.setCursor(12, 1);
    } else if (pid_mode == 1) {
        lcd.setCursor(8, 1);
        lcd.print(" ");
        lcd.setCursor(10, 1);
    } else if (pid_mode == 2) {
        lcd.setCursor(8, 1);
        lcd.print("    ");
        lcd.setCursor(12, 1);
    } else if (pid_mode == 3) {
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
        lcd.print(pid_setpoint_float, 2);
    }

    if (menu_curr_item == 4) {
        if (width_offset_float >= 0) {
            lcd.setCursor(11, position);
            lcd.print("+");
            lcd.setCursor(12, position);
            lcd.print(width_offset_float, 2);
        } else if (width_offset_float < 0) {
            lcd.setCursor(11, position);
            lcd.print(width_offset_float, 2);
        }
    }
}
