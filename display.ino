#include "./display.h"

namespace {

using namespace display;

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

unsigned long lastUpdate = 0;

boolean inSetup = true;

enum class Screen {
    SetupMode,
    SetupDiam,
    SetupSpoolStart,
    SetupSpoolEnd,
    Stats,
    Mode,
    WidthOffset,
    PullSpeed,
    SpoolSpeed
};
Screen currScreen = Screen::SetupMode;
boolean adjusting = false;

void clearLine(int l)
{
    lcd.setCursor(0, l);
    lcd.print("                ");
}

void redrawStatus()
{
    lcd.setCursor(0, 0);
    if (!stepper::isEnabled()) {
        lcd.print(strings::spooler_ready);
    } else {
        lcd.setCursor(0, 0);
        int col = 0;

        lcd.print(char(Logo::Diameter));
        col++;
        String val = String(abs(sensor::width()), 2);
        lcd.print(val);
        col += val.length();

        lcd.print(" ");
        lcd.print(char(Logo::Speed));
        col += 2;
        val = String(stepper::pull::speed(), 2);
        lcd.print(val);
        col += val.length();

        lcd.print(char(Logo::MM));
        col++;

        for (; col < 16; col++)
            lcd.print(" ");
    }
}

void redrawMenu();

// Shorthand for the setup screens, which are all pretty similar
// adjust gets called with either 1 (up) or -1 (down) to set the new value
// display gets called to return the formatted value
void redrawSetupScreen(
    String& prompt,
    void (*adjust)(int),
    String (*display)())
{
    if (encoder::up || encoder::down) {
        adjust(encoder::up ? 1 : -1);
        clearLine(1);
    }

    lcd.setCursor(0, 0);
    lcd.print(prompt);
    lcd.setCursor(0, 1);
    lcd.print(display());
}

void redrawSetup()
{
    bool firstRedrawOnScreen = false;
    if (encoder::clicked) {
        currScreen = Screen(int(currScreen) + 1);
        encoder::clicked = false;
        firstRedrawOnScreen = true;
        lcd.clear();
    }

    if (currScreen == Screen::SetupMode) {
        redrawSetupScreen(
            strings::setup_mode,
            [](int adj) { pid::setMode(pid::PIDMode(clamp(0, int(pid::mode()) + adj, pid::MAX_MODE))); },
            []() { return strings::modes[pid::mode()]; });
    } else if (currScreen == Screen::SetupDiam) {
        if (firstRedrawOnScreen)
            pid::setSetpoint(175);

        redrawSetupScreen(
            strings::setup_diameter,
            [](int adj) { pid::setSetpoint(clamp(0, pid::setpoint() + (adj * MM_PER_CLICK), 300)); },
            []() { return String(pid::setpoint_mm(), 2); });
    } else if (currScreen == Screen::SetupSpoolStart) {
        using namespace stepper;

        if (firstRedrawOnScreen) {
            distrib::setStartPos(0);
            distrib::goToStart();
        }

        redrawSetupScreen(
            strings::setup_distrib_start,
            [](int adj) { distrib::setStartPos(clamp(0, adj > 0 ? distrib::startPos() + STEPS_PER_CLICK : distrib::startPos() - STEPS_PER_CLICK, distrib::MAX_POS)); distrib::goToStart(); },
            []() { return String(distrib::startPos()); });
    } else if (currScreen == Screen::SetupSpoolEnd) {
        using namespace stepper;

        if (firstRedrawOnScreen) {
            distrib::setEndPos(distrib::MAX_POS);
            distrib::goToEnd();
        }

        redrawSetupScreen(
            strings::setup_distrib_end,
            [](int adj) { distrib::setEndPos(clamp(distrib::startPos(), adj > 0 ? distrib::endPos() + STEPS_PER_CLICK : distrib::endPos() - STEPS_PER_CLICK, distrib::MAX_POS)); distrib::goToEnd(); },
            []() { return String(distrib::endPos()); });
    } else if (currScreen > Screen::SetupSpoolEnd) {
        inSetup = false;
        lcd.clear();
        redrawStatus();
        redrawMenu();
    }
}

// Shorthand for the setup screens, which are all pretty similar
// adjust gets called with either 1 (up) or -1 (down) to set the new value
// display gets called to return the formatted value
void redrawMenuScreen(
    String& prefix,
    String& setPrefix,
    void (*adjust)(int),
    String (*display)(),
    boolean readOnly = false)
{
    if (encoder::clicked && !readOnly) {
        adjusting = !adjusting;
        encoder::clicked = false;
    }

    if (adjusting && (encoder::up || encoder::down)) {
        adjust(encoder::up ? 1 : -1);
        clearLine(1);
    }

    lcd.setCursor(0, 1);
    lcd.print(adjusting ? setPrefix : prefix);

    // Right align displayed value
    String val = display();
    lcd.setCursor(16 - val.length(), 1);
    lcd.print(val);
}

void redrawMenu()
{
    if (!adjusting && (encoder::up || encoder::down)) {
        currScreen = Screen(clamp(int(Screen::Stats), int(currScreen) + (encoder::up ? 1 : -1), int(Screen::SpoolSpeed)));
        clearLine(1);
    }

    if (currScreen == Screen::Stats) {
        redrawMenuScreen(
            strings::stats, strings::stats,
            [](int) {},
            []() { return String(stepper::pull::total(), 2); },
            true);
    } else if (currScreen == Screen::Mode) {
        redrawMenuScreen(
            strings::mode, strings::set_mode,
            [](int adj) { pid::setMode(pid::PIDMode(clamp(0, int(pid::mode()) + adj, pid::MAX_MODE))); },
            []() { return strings::modes[pid::mode()]; });
    } else if (currScreen == Screen::WidthOffset) {
        redrawMenuScreen(
            strings::width_offset, strings::set_width_offset,
            [](int adj) { sensor::setOffset(clamp(0, sensor::offset() + adj, 100)); },
            []() { return String(sensor::offset_mm(), 2); });

    } else if (currScreen == Screen::PullSpeed) {
        using namespace stepper;

        redrawMenuScreen(
            strings::pull_speed, strings::set_pull_speed,
            [](int adj) { pull::setInterval(clamp(12, pull::interval() + adj, 128)); },
            []() { return String(pull::interval()); },
            pid::mode() != pid::PIDMode::Manual);
    } else if (currScreen == Screen::SpoolSpeed) {
        using namespace stepper;

        redrawMenuScreen(
            strings::spool_speed, strings::set_spool_speed,
            [](int adj) { spool::setInterval(clamp(10, spool::interval() + adj, 1000)); },
            []() { return String(spool::interval()); });
    }
}

}

namespace display {

void init()
{
    lcd.begin(16, 2);
    lcd.createChar(char(Logo::Diameter), logos::diameter);
    lcd.createChar(char(Logo::Speed), logos::speed);
    lcd.createChar(char(Logo::MM), logos::mm);
    lcd.createChar(char(Logo::Metr1), logos::metr1);
    lcd.createChar(char(Logo::Metr2), logos::metr2);
    lcd.createChar(char(Logo::Extruder1), logos::extruder1);
    lcd.createChar(char(Logo::Extruder2), logos::extruder2);
}

void showSplash()
{
    lcd.setCursor(0, 0);
    lcd.print("Felfil Spooler");
    lcd.setCursor(0, 1);
    lcd.print("Calibrating...");
}

void clear()
{
    lcd.clear();
}

void loop(boolean receivedInput)
{
    // Don't redraw unless enough time has passed, or we've been forced to (by input usually)
    unsigned long currentMillis = millis();
    if (!receivedInput && currentMillis - lastUpdate < interval)
        return;

    // TODO: Hold to reset

    if (inSetup) {
        redrawSetup();
    } else {
        redrawStatus();
        if (receivedInput)
            redrawMenu();
    }

    lastUpdate = currentMillis;
}

boolean isInSetup()
{
    return inSetup;
}

}

// switch (encoder::btn) {
// case ClickEncoder::Clicked:
//     lcd.clear();
//     break;
// case ClickEncoder::Held:
//     lcd.clear();
//     lcd.setCursor(0, 0);
//     lcd.print("Resetting...");
//     stepper::distrib::reset();
//     menu_curr_item = 1;
//     menu_page = 2;
//     lcd.clear();
//     break;
// case ClickEncoder::DoubleClicked:
//     if (pid::mode < 3) {
//         if (menu_page == 2 && menu_curr_item == 8) {
//             stepper::pull::resetCounter();
//         }
//     } else if (pid::mode == 3) {
//         if (menu_page == 2 && menu_curr_item == 9) {
//             stepper::pull::resetCounter();
//         }
//     }
//     break;
// default:
//     break;
// }
