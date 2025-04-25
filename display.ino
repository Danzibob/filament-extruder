#include "./display.h"

void drawHome() {
  //MenuHome
  if (enableState == 0) {
    lcd.setCursor (0, 0);
    lcd.print (" Spooler ready! ");
  } else {
    lcd.setCursor (0 , 0 );
    lcd.print(char(0));
    unsigned long currentMillis = millis();
    if (currentMillis - LcdpreviousMillis >= Lcdinterval) {
      // save the last time you blinked the LED
      LcdpreviousMillis = currentMillis;
      lcd.setCursor ( 1, 0);
      lcd.print abs(measure);
      lcd.setCursor (9, 0);
      if ( selectedMode != 3){
      lcd.print (extspd, 2);
      } else lcd.print (extspd2, 2);
      lcd.setCursor (13, 0);
      lcd.print (" ");
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
    lcd.setCursor (13, 0);
    lcd.print(" ");
    lcd.setCursor (14, 0);
    lcd.print(char(3));
    lcd.setCursor (15, 0);
    lcd.print(char(4));
  }
}

void drawMenu() {
  // MENU UI Start
  if (menuItem == 2 && page == 1) {
    up = false;
  }
  if (up && page == 1 ) {

    up = false;
    lastMenuItem = menuItem;
    menuItem--;
    if (menuItem == 0)
    {
      menuItem = 1;
    }
  }
  if (down && page == 1) //We have turned the Rotary Encoder Clockwise
  {
    down = false;
    lastMenuItem = menuItem;
    menuItem++;
    if (menuItem == 10 && selectedMode == 3)
    {
      menuItem--;
    }
    else if (menuItem == 9 && selectedMode != 3)
    {
      menuItem--;
    }
  }
  // SETUP MENU Begin____________________________________________________________________
  if (page == 1 && menuItem == 1) {
    page = 2;
  }
  if (middle && menuItem == 1) //Middle Button is Pressed - SETUP MENU
  {
    middle = false;

    if (page == 1 ) {

      page = 2;
    } else if (page == 2)
    {
      page = 3;
    } else if (page == 3)
    {
      page = 4;
    } else if (page == 4) {
      TravelBegin = StepperPosition;
      lcd.setCursor (0, 0);
      lcd.print("Wait while I'm");
      lcd.setCursor (0, 1);
      lcd.print("moving stepper");
      digitalWrite(DistrpinDir, HIGH);
      for (int x = 0; x < newpositionEnd ; x++) {

        digitalWrite(DistrpinStep, HIGH);
        delay(1);
        digitalWrite(DistrpinStep, LOW);
        delay(1);

      }

      StepperPosition = newpositionEnd;
      lcd.clear();
      page = 5;
    } else if ( page == 5) {
      TravelEnd = StepperPosition;
      travel = TravelEnd;
      if (selectedMode <= 2) {
        menuItem = 2;
        page = 1;
      } else (page = 6);
    } else if (page == 6) {
      menuItem = 2;
      page = 1;
    }

  }
  // SETUP MENU End_______________________________________________________________________

  if (middle && menuItem >= 2) //Middle Button is Pressed - USING MENU
  {
    middle = false;
    if (page == 1 ) {
      page = 2;

    } else if (page == 2)
    {
      page = 1;
    }
  }
  // MENU UI END

  // Menu using structure begin
  if (page == 1 && menuItem >= 2 && selectedMode == 3)
  {
    if (menuItem == 2 )
    {
      drawHome();
      displayMenuItem(menuItem2, 1, true, intdiameter);
    }
    else if (menuItem == 3)
    {
      drawHome();
      displayIntStringMenuPage(menuItem3, 1, true, mode[selectedMode]);
    }
    else if (menuItem == 4 ) {

      drawHome();

      lcd.setCursor (0, 1);
      lcd.print(">  PullSpd:");
      lcd.setCursor(12, 1);
      lcd.print(extspd2, 2);
    }
    else if (menuItem == 5 )
    {
      drawHome();
      displayMenuItem(menuItem5, 1, true, intOffset);
    } else if (menuItem == 6 )
    {
      drawHome();
      lcd.setCursor (0, 1);
      lcd.print (">  TravSpd: ");
      if (  travelspd > 0 && travelspd < 10  )
      {
        lcd.setCursor(12, 1);
        lcd.print ("   ");
        lcd.setCursor(15, 1);
        lcd.print (travelspd);
      } else if ( travelspd > 9 && travelspd < 100 )
      {
        lcd.setCursor(12, 1);
        lcd.print ("  ");
        lcd.setCursor(14, 1);
        lcd.print (travelspd);
      } else if (travelspd > 99 && travelspd < 999)
      {
        lcd.setCursor(12, 1);
        lcd.print (" ");
        lcd.setCursor(13, 1);
        lcd.print (travelspd);
      } else if (travelspd == 0)
      {
        lcd.setCursor(12, 1);
        lcd.print ("Auto");
      }
    } else if (menuItem == 7 )
    {
      drawHome();
      displayMenuItem(menuItem7, 1, true, spoolRPM);
    } else if (menuItem == 8 )
    {
      drawHome();
      displayMenuItem(menuItem8, 1, true, fanspd);
    }
    else if (menuItem == 9 )
    {
      drawHome();
      displayMenuItem(menuItem9, 1, true, Total);
    }
  }
  else if (page == 1 && menuItem >= 2 && selectedMode != 3) {
    if (menuItem == 2 )
    {
      drawHome();
      displayMenuItem(menuItem2, 1, true, intdiameter);
    }
    else if (menuItem == 3)
    {
      drawHome();
      displayIntStringMenuPage(menuItem3, 1, true, mode[selectedMode]);
    }
    else if (menuItem == 4 ) {

      drawHome();
      displayMenuItem(menuItem5, 1, true, intOffset);
    }
    else if (menuItem == 5 )
    {
      drawHome();
      lcd.setCursor (0, 1);
      lcd.print (">  TravSpd: ");
      if (  travelspd > 0 && travelspd < 10  )
      {
        lcd.setCursor(12, 1);
        lcd.print ("   ");
        lcd.setCursor(15, 1);
        lcd.print (travelspd);
      } else if ( travelspd > 9 && travelspd < 100 )
      {
        lcd.setCursor(12, 1);
        lcd.print ("  ");
        lcd.setCursor(14, 1);
        lcd.print (travelspd);
      } else if (travelspd > 99 && travelspd < 999)
      {
        lcd.setCursor(12, 1);
        lcd.print (" ");
        lcd.setCursor(13, 1);
        lcd.print (travelspd);
      } else if (travelspd == 0)
      {
        lcd.setCursor(12, 1);
        lcd.print ("Auto");
      }
    }
    else if (menuItem == 6 )
    {
      drawHome();
      displayMenuItem(menuItem7, 1, true, spoolRPM);
    }
    else if (menuItem == 7 )
    {
      drawHome();
      displayMenuItem(menuItem8, 1, true, fanspd);
    }
    else if (menuItem == 8 )
    {
      drawHome();
      displayMenuItem(menuItem9, 1, true, Total);
    }
  }
  // Menu using structure end



  if (selectedMode == 3) {

    //Manual Mode Setting Begin___________________________________________________________
    if (page == 2 && menuItem == 1)
    {
      //digitalWrite (enablePin, HIGH);
      displayStringMenuPage( mode[selectedMode]);
      if (up) {
        up = false;
        selectedMode--;

        if (selectedMode <= 0)
        {
          selectedMode = 0;
        }
      } else if (down) {
        down = false;
        selectedMode++;

        if (selectedMode >= 3)
        {
          selectedMode = 3;
        }

      }
    }
    if (page == 3 && menuItem == 1)
    {
      //digitalWrite (enablePin, HIGH);
      lcd.setCursor (0, 0);
      lcd.print ("Set Diameter:");
      lcd.setCursor (0, 1);
      lcd.print (intdiameter, 2);

      if (up) {
        up = false;
        diameter--;
        EEPROM.update(adressDiam, highByte(diameter));
        EEPROM.update(adressDiam + 1, lowByte(diameter));
      } else if (down) {
        down = false;
        diameter++;
        EEPROM.update(adressDiam, highByte(diameter));
        EEPROM.update(adressDiam + 1, lowByte(diameter));
      }

    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (page == 4 && menuItem == 1)
    { lcd.setCursor (0, 0);
      lcd.print ("Set Spool Begin:");
      if (mm <= 9) {
        lcd.setCursor (0, 1);
        lcd.print (mm);
        lcd.setCursor (1, 1);
        lcd.print (" ");
      }
      else if ( mm >= 10 ) {
        lcd.setCursor (0, 1);
        lcd.print (mm);
      }
      if (up) {
        up = false;
        digitalWrite(DistrpinDir, LOW);
        if (StepperPosition > 0) {
          StepperPosition = StepperPosition - StepsToTake;
        } else StepperPosition = 0;
        if ( StepperPosition != lastStepperPosition) {

          for (int x = 0; x < StepsToTake; x++) {

            digitalWrite(DistrpinStep, HIGH);
            delay(1);
            digitalWrite(DistrpinStep, LOW);
            delay(1);
            //steppini--;
          }

        }
        newposition = StepperPosition;
      } else if (down) {
        down = false;
        digitalWrite(DistrpinDir, HIGH);
        if (StepperPosition < 7900/4) {
          StepperPosition = StepperPosition + StepsToTake;
        } else StepperPosition = 7900/4;
        if ( StepperPosition != lastStepperPosition) {

          for (int x = 0; x < StepsToTake; x++) {

            digitalWrite(DistrpinStep, HIGH);
            delay(1);
            digitalWrite(DistrpinStep, LOW);
            delay(1);
            //steppini--;
          }
        }
        newposition = StepperPosition;
      }
    }
    //////////////////////////////////////////////////////////
    if (page == 5 && menuItem == 1)
    { lcd.setCursor (0, 0);
      lcd.print ("Set Spool End:");
      if (mm <= 9) {
        lcd.setCursor (0, 1);
        lcd.print (mm);
        lcd.setCursor (1, 1);
        lcd.print (" ");
      }
      else if ( mm >= 10 ) {
        lcd.setCursor (0, 1);
        lcd.print (mm);
      }

      if (up) {
        up = false;
        digitalWrite(DistrpinDir, LOW);
        if (StepperPosition > 0) {
          StepperPosition = StepperPosition - StepsToTake;
        } else StepperPosition = 0;
        if ( StepperPosition != lastStepperPosition) {

          for (int x = 0; x < StepsToTake; x++) {

            digitalWrite(DistrpinStep, HIGH);
            delay(1);
            digitalWrite(DistrpinStep, LOW);
            delay(1);
            //steppini--;
          }
        }

      } else if (down) {
        down = false;
        digitalWrite(DistrpinDir, HIGH);
        if (StepperPosition <  newpositionEnd) {
          StepperPosition = StepperPosition + StepsToTake;
        } else StepperPosition = newpositionEnd;
        if ( StepperPosition != lastStepperPosition) {

          for (int x = 0; x < StepsToTake; x++) {

            digitalWrite(DistrpinStep, HIGH);
            delay(1);
            digitalWrite(DistrpinStep, LOW);
            delay(1);
            //steppini--;
          }
        }

      }

    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (page == 6 && menuItem == 1)
    {
      lcd.setCursor (0, 0);
      lcd.print ("Set PullSpeed:");
      lcd.setCursor (0, 1);
     lcd.print (extspd2, 2);
      if (Pullinterval == 0) {
        Pullinterval = 9000;
      }

      if (up) {
        up = false;
        if (Pullinterval < 90000) {
          Pullinterval = Pullinterval + 100;
        } else
          Pullinterval = Pullinterval;
      } else if (down) {
        down = false;
        if (Pullinterval > 1000) {
          Pullinterval = Pullinterval - 100;
        } else
          Pullinterval = Pullinterval;
      }

    }
    //Manual Mode Setting End_______________________________________________________________

    if (page == 2 && menuItem == 2 )
    {
      drawHome();
      displayIntMenuPage(menuItem2, 1, intdiameter);
      if (up) {
        up = false;
        diameter--;
        EEPROM.update(adressDiam, highByte(diameter));
        EEPROM.update(adressDiam + 1, lowByte(diameter));
      } else if (down) {
        down = false;
        diameter++;
        EEPROM.update(adressDiam, highByte(diameter));
        EEPROM.update(adressDiam + 1, lowByte(diameter));
      }
    }
    else if (page == 2 && menuItem == 3)
    {
      drawHome();
      displayString2MenuPage( mode[selectedMode]);
      if (up) {
        up = false;
        selectedMode--;

        if (selectedMode <= 0)
        {
          selectedMode = 0;
        }
      } else if (down) {
        down = false;
        selectedMode++;

        if (selectedMode >= 3)
        {
          selectedMode = 3;
        }

      }
    }
    else if (page == 2 && menuItem == 4)
    {
      drawHome();
      lcd.setCursor(0, 1);
      lcd.print("Set");
      lcd.setCursor(3, 1);
      lcd.print("Pullspd");
      lcd.setCursor(11, 1);
      lcd.print ("  ");
      lcd.setCursor(12, 1);
      //extspd2 = 60 / ((pullspd * 400) / 1000) * 0.0942;
      lcd.print (extspd2, 2);
      if (Pullinterval == 0) {
        Pullinterval = 9000;
      }

      if (up) {
        up = false;
        if (Pullinterval < 90000) {
          Pullinterval = Pullinterval + 100;
        } else
          Pullinterval = Pullinterval;
      } else if (down) {
        down = false;
        if (Pullinterval > 1000) {
          Pullinterval = Pullinterval - 100;
        } else
          Pullinterval = Pullinterval;
      }

    }
    else if (page == 2 && menuItem == 5)
    {
      drawHome();
      if ( intOffset >= 0) {
        lcd.setCursor(0, 1);
        lcd.print("Set");
        lcd.setCursor(3, 1);
        lcd.print("Offset");
        lcd.setCursor(11, 1);
        lcd.print ("  ");
        lcd.setCursor(12, 1);
        lcd.print (intOffset, 2);
      } else if ( intOffset < 0) {
        lcd.setCursor(0, 1);
        lcd.print("Set");
        lcd.setCursor(3, 1);
        lcd.print("Offset");
        lcd.setCursor(11, 1);
        lcd.print (intOffset, 2);
      }
      if (up) {
        up = false;
        offset--;
        EEPROM.update (adressOffset , offset);
      } else if (down) {
        down = false;
        offset++;
        EEPROM.update (adressOffset , offset);
      }
    }
    else if (page == 2 && menuItem == 6)
    {
      drawHome();
      lcd.setCursor (0, 1);
      lcd.print ("Set TravSpd ");
      if (  travelspd > 0 && travelspd < 10  )
      {
        lcd.setCursor(12, 1);
        lcd.print ("   ");
        lcd.setCursor(15, 1);
        lcd.print (travelspd);
      } else if ( travelspd > 9 && travelspd < 100 )
      {
        lcd.setCursor(12, 1);
        lcd.print ("  ");
        lcd.setCursor(14, 1);
        lcd.print (travelspd);
      } else if (travelspd > 99 && travelspd < 999)
      {
        lcd.setCursor(12, 1);
        lcd.print (" ");
        lcd.setCursor(13, 1);
        lcd.print (travelspd);
      } else if (travelspd == 0)
      {
        lcd.setCursor(12, 1);
        lcd.print ("Auto");
      }
      if (up) {
        up = false;
        travelspd--;
      } else if (down) {
        down = false;
        travelspd++;
      }
    }
    else if (page == 2 && menuItem == 7)
    {
      drawHome();
      displayIntMenuPage(menuItem7, 1, spoolRPM);
      if (up) {
        up = false;
        spoolspd++;
      } else if (down) {
        down = false;
        spoolspd--;
      }
    }
    else if (page == 2 && menuItem == 8)
    {
      drawHome();
      displayIntMenuPage(menuItem8, 1, fanspd);
      if (up) {
        up = false;
        fanspd--;
      } else if (down) {
        down = false;
        fanspd++;
      }
    }
    else if (page == 2 && menuItem == 9)
    {
      drawHome();
      lcd.setCursor ( 0, 1);
      lcd.print ("Total meter:");
      lcd.setCursor (13, 1);
      lcd.print (Total);
    }
  }



  else if (selectedMode != 3) {
    //Preset Mode Setting Begin_____________________________________________________________
    if (page == 2 && menuItem == 1)
    {
      displayStringMenuPage( mode[selectedMode]);
      if (up) {
        up = false;
        selectedMode--;

        if (selectedMode <= 0)
        {
          selectedMode = 0;
        }
      } else if (down) {
        down = false;
        selectedMode++;

        if (selectedMode >= 3)
        {
          selectedMode = 3;
        }

      }
    }
    if (page == 3 && menuItem == 1)
    {
      lcd.setCursor (0, 0);
      lcd.print ("Set Diameter:");
      lcd.setCursor (0, 1);
      lcd.print (intdiameter, 2);
      if (up) {
        up = false;
        diameter--;
        EEPROM.update(adressDiam, highByte(diameter));
        EEPROM.update(adressDiam + 1, lowByte(diameter));
      } else if (down) {
        down = false;
        diameter++;
        EEPROM.update(adressDiam, highByte(diameter));
        EEPROM.update(adressDiam + 1, lowByte(diameter));
      }
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (page == 4 && menuItem == 1)
    {
      lcd.setCursor (0, 0);
      lcd.print ("Set Spool Begin:");
      if (mm <= 9) {
        lcd.setCursor (0, 1);
        lcd.print (mm);
        lcd.setCursor (1, 1);
        lcd.print (" ");
      }
      else if ( mm >= 10 ) {
        lcd.setCursor (0, 1);
        lcd.print (mm);
      }
      if (up) {
        up = false;
        digitalWrite(DistrpinDir, LOW);
        if (StepperPosition > 0) {
          StepperPosition = StepperPosition - StepsToTake;
        } else StepperPosition = 0;
        if ( StepperPosition != lastStepperPosition) {

          for (int x = 0; x < StepsToTake; x++) {

            digitalWrite(DistrpinStep, HIGH);
            delay(1);
            digitalWrite(DistrpinStep, LOW);
            delay(1);
            //steppini--;
          }
        }
        newposition = StepperPosition;
      } else if (down) {
        down = false;
        digitalWrite(DistrpinDir, HIGH);
        if (StepperPosition < 7900/4) {
          StepperPosition = StepperPosition + StepsToTake;
        } else StepperPosition = 7900/4;
        if ( StepperPosition != lastStepperPosition) {

          for (int x = 0; x < StepsToTake; x++) {

            digitalWrite(DistrpinStep, HIGH);
            delay(1);
            digitalWrite(DistrpinStep, LOW);
            delay(1);
            //steppini--;
          }
        }
        newposition = StepperPosition;
      }
    }
    //////////////////////////////////////////////////////////////////////////////////////
    if (page == 5 && menuItem == 1)
    {
      lcd.setCursor (0, 0);
      lcd.print ("Set Spool End:");
      if (mm <= 9) {
        lcd.setCursor (0, 1);
        lcd.print (mm);
        lcd.setCursor (1, 1);
        lcd.print (" ");
      }
      else if ( mm >= 10 ) {
        lcd.setCursor (0, 1);
        lcd.print (mm);
      }

      if (up) {
        up = false;
        digitalWrite(DistrpinDir, LOW);
        if (StepperPosition > 0) {
          StepperPosition = StepperPosition - StepsToTake;
        } else StepperPosition = 0;
        if ( StepperPosition != lastStepperPosition) {

          for (int x = 0; x < StepsToTake; x++) {

            digitalWrite(DistrpinStep, HIGH);
            delay(1);
            digitalWrite(DistrpinStep, LOW);
            delay(1);
            //steppini--;
          }
        }

      } else if (down) {
        down = false;
        digitalWrite(DistrpinDir, HIGH);
        if (StepperPosition < newpositionEnd) {
          StepperPosition = StepperPosition + StepsToTake;
        } else StepperPosition = newpositionEnd;
        if ( StepperPosition != lastStepperPosition) {

          for (int x = 0; x < StepsToTake; x++) {

            digitalWrite(DistrpinStep, HIGH);
            delay(1);
            digitalWrite(DistrpinStep, LOW);
            delay(1);
            //steppini--;
          }
        }

      }
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (page == 6 && menuItem == 1)
    {
      lcd.setCursor (0, 0);
      lcd.print ("Set PullSpeed:");
      lcd.setCursor (0, 1);
      lcd.print (pullspd);
    }
    //Preset Mode Setting End________________________________________________________________

    if (page == 2 && menuItem == 2 )
    {
      drawHome();
      lcd.setCursor(0, 1);
      lcd.print("Set");
      lcd.setCursor(3, 1);
      lcd.print("Diameter");
      lcd.setCursor(11, 1);
      lcd.print ("  ");
      lcd.setCursor(12, 1);
      lcd.print (intdiameter, 2);
      if (up) {
        up = false;
        diameter--;
        EEPROM.update(adressDiam, highByte(diameter));
        EEPROM.update(adressDiam + 1, lowByte(diameter));
      } else if (down) {
        down = false;
        diameter++;
        EEPROM.update(adressDiam, highByte(diameter));
        EEPROM.update(adressDiam + 1, lowByte(diameter));
      }
    }
    else if (page == 2 && menuItem == 3)
    {
      drawHome();
      displayString2MenuPage( mode[selectedMode]);
      if (up) {
        up = false;
        selectedMode--;

        if (selectedMode <= 0)
        {
          selectedMode = 0;
        }
      } else if (down) {
        down = false;
        selectedMode++;
        if (selectedMode >= 3)
        {
          selectedMode = 3;
        }
      }
    }
    else if (page == 2 && menuItem == 4)
    {
      drawHome();
      if ( intOffset >= 0) {
        lcd.setCursor(0, 1);
        lcd.print("Set");
        lcd.setCursor(3, 1);
        lcd.print("Offset");
        lcd.setCursor(11, 1);
        lcd.print ("  ");
        lcd.setCursor(12, 1);
        lcd.print (intOffset, 2);
      } else if ( intOffset < 0) {
        lcd.setCursor(0, 1);
        lcd.print("Set");
        lcd.setCursor(3, 1);
        lcd.print("Offset");
        lcd.setCursor(11, 1);
        lcd.print (intOffset, 2);
      }

      if (up) {
        up = false;
        offset--;
        EEPROM.update (adressOffset, offset);
      } else if (down) {
        down = false;
        offset++;
        EEPROM.update (adressOffset, offset);
      }
    }
    else if (page == 2 && menuItem == 5)
    {
      drawHome();
      lcd.setCursor (0, 1);
      lcd.print ("Set TravSpd ");
      if (  travelspd >= 0 && travelspd < 10  )
      {
        lcd.setCursor(12, 1);
        lcd.print ("   ");
        lcd.setCursor(15, 1);
        lcd.print (travelspd);
      } else if ( travelspd > 9 && travelspd < 100 )
      {
        lcd.setCursor(12, 1);
        lcd.print ("  ");
        lcd.setCursor(14, 1);
        lcd.print (travelspd);
      } else if (travelspd > 99 && travelspd < 999)
      {
        lcd.setCursor(12, 1);
        lcd.print (" ");
        lcd.setCursor(13, 1);
        lcd.print (travelspd);
      }
      if (up) {
        up = false;
        travelspd--;
      } else if (down) {
        down = false;
        travelspd++;
      }
    }
    else if (page == 2 && menuItem == 6)
    {
      drawHome();
      displayIntMenuPage(menuItem7, 1, spoolRPM);
      if (up) {
        up = false;
        spoolspd++;
      } else if (down) {
        down = false;
        spoolspd--;
      }
    }
    else if (page == 2 && menuItem == 7)
    {
      drawHome();
      displayIntMenuPage(menuItem8, 1, fanspd);
      if (up) {
        up = false;
        fanspd--;
      } else if (down) {
        down = false;
        fanspd++;
      }
    }
    else if (page == 2 && menuItem == 8)
    {
      drawHome();
      // STATS
      lcd.setCursor ( 0, 1);
      lcd.print ("Total meter:");
      lcd.setCursor (13, 1);
      lcd.print (Total);
    }

  }

  if (menuItem > 1) {
    Sensor();
  }
}

void displayIntMenuPage(String menuItem, int position, int value) {

  lcd.setCursor(0, position);
  lcd.print("Set");
  lcd.setCursor(3, position);
  lcd.print(menuItem);

  if ( value == 0 && menuItem != 2) {
    lcd.setCursor (12, position);
    lcd.print ("   ");
    lcd.setCursor(15, position);
  }
  else if (  value > 0 && value < 10 && menuItem != 2 )
  {
    lcd.setCursor(12, position);
    lcd.print ("   ");
    lcd.setCursor(15, position);
  }
  else if ( value > 9 && value < 100 && menuItem != 2)
  {
    lcd.setCursor(12, position);
    lcd.print ("  ");
    lcd.setCursor(14, position);

  }
  else if (value > 99 && value < 999 && menuItem != 2)
  {
    lcd.setCursor(12, position);
    lcd.print (" ");
    lcd.setCursor(13, position);
  }
  else if (value > 999 && value < 9999 && menuItem != 2)
  {
    lcd.setCursor(12, position);
  }
  lcd.print (value);
}
void displayStringMenuPage(String value) {
  lcd.setCursor(0, 0);
  lcd.print("Set Mode:");
  lcd.setCursor(0, 1);
  lcd.print(">");
  if (selectedMode == 0) {
    lcd.setCursor (1, 1);
    lcd.print ("    ");
    lcd.setCursor(5, 1);
  } else if (selectedMode == 1) {
    lcd.setCursor (1, 1);
    lcd.print (" ");
    lcd.setCursor(3, 1);
  } else if (selectedMode == 2) {
    lcd.setCursor (1, 1);
    lcd.print ("    ");
    lcd.setCursor(5, 1);
  } else if (selectedMode == 3) {
    lcd.setCursor (1, 1);
    lcd.print (" ");
    lcd.setCursor(3, 1);
  }

  lcd.print(value);
}
void displayString2MenuPage(String value) {
  lcd.setCursor(0, 1);
  lcd.print("Set");
  lcd.setCursor(3, 1);
  lcd.print(" Mode");

  if (selectedMode == 0) {
    lcd.setCursor (8, 1);
    lcd.print ("    ");
    lcd.setCursor(12, 1);
  } else if (selectedMode == 1) {
    lcd.setCursor (8, 1);
    lcd.print (" ");
    lcd.setCursor(10, 1);
  } else if (selectedMode == 2) {
    lcd.setCursor (8, 1);
    lcd.print ("    ");
    lcd.setCursor(12, 1);
  } else if (selectedMode == 3) {
    lcd.setCursor (8, 1);
    lcd.print (" ");
    lcd.setCursor(10, 1);
  }
  lcd.print(value);
}
void displayIntStringMenuPage(String item, int position, boolean selected, String value) {
  lcd.setCursor(0, position);
  lcd.print (">");
  lcd.setCursor(1, position);
  lcd.print( item);
  lcd.setCursor(12, position);
  lcd.print (value);
}
void displayMenuItem(String item, int position, boolean selected, int value) {
  lcd.setCursor(0, position);
  lcd.print (">");
  lcd.setCursor(1, position);
  lcd.print( item);
  if ( value == 0 && menuItem != 2 && menuItem != 4) {
    lcd.setCursor (11, position);
    lcd.print ("     ");
    lcd.setCursor(15, position);
  }
  else if (  value > 0 && value < 10 && menuItem != 2 && menuItem != 4 )
  {
    lcd.setCursor(11, position);
    lcd.print ("     ");
    lcd.setCursor(15, position);
  }
  else if ( value > 9 && value < 100 && menuItem != 2 && menuItem != 4)
  {
    lcd.setCursor(11, position);
    lcd.print ("    ");
    lcd.setCursor(14, position);

  }
  else if (value > 99 && value < 999 && menuItem != 2 && menuItem != 4)
  {
    lcd.setCursor(11, position);
    lcd.print ("   ");
    lcd.setCursor(13, position);
  }
  else if (value > 999 && value < 9999 && menuItem != 2 && menuItem != 4)
  {
    lcd.setCursor(11, position);
    lcd.print ("  ");
    lcd.setCursor(12, position);
  }
  lcd.print (value);

  if ( menuItem == 2 ) {
    lcd.setCursor(11, position);
    lcd.print ("  ");
    lcd.setCursor(12, position);
    lcd.print (intdiameter, 2);
  }

  if ( menuItem == 4 ) {
    if (intOffset >= 0) {
      lcd.setCursor(11, position);
      lcd.print ("+");
      lcd.setCursor(12, position);
      lcd.print (intOffset, 2);
    } else if (intOffset < 0) {
      lcd.setCursor(11, position);
      lcd.print (intOffset, 2);
    }
  }

}
