
#include <EEPROM.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(13, 11, 5, 4, 3, 2); // specifies the appropriate pins for the LCD

// button pins
constexpr int up = 10;    // specifies pin number for up button (LCD cursor)
constexpr int down = 9;   // specifies pin number for down button (LCD cursor)
constexpr int select = 8; // specifies pin number for select button (LCD cursor)
constexpr int purge = 7; // specifies pin number for purge button (when pressed will run the grinder for as long as it is held)
constexpr int active = 6; // specifies pin number for activate button (when pressed will run the
                          // grinder for the specified time)

// Optocoupler pins
constexpr int optoOne = 12; // specifies pin number for optocoupler 1

int presetOneVal = 0; // initialises value for preset one hold time.
int presetTwoVal = 0; // initialises value for preset two hold time.

int upVal = 0;
int downVal = 0;
int selectVal = 0;
int activeVal = 0;
int purgeVal = 0;
int upState = 0;
int downState = 0;
int selectState = 0;
int oldSelectState = 0;
int activeState = 0;
int oldActiveState = 0;
int purgeState = 0;
int lcdState = 0;
int oldlcdState = 0;

// for countdown on the LCD
unsigned long currentMillis = 0;

int x = 0;

void setup()
{
    pinMode(optoOne, OUTPUT); // specifies the optocoupler 1 pin as an output
    digitalWrite(optoOne, LOW);

    lcd.begin(16, 2);              // specifies the dimensions of the LCD to be used.
    pinMode(up, INPUT_PULLUP);     // specifies the up pin as an input pullup
    pinMode(down, INPUT_PULLUP);   // specifies the down pin as an input pullup
    pinMode(select, INPUT_PULLUP); // specifies the select pin as an input pullup
    pinMode(active, INPUT_PULLUP); // specifies the active pin as an input pullup
    pinMode(purge, INPUT_PULLUP);  // specifies the purge pin as an input pullup

    lcd.clear();

    // read the preset values saved in the eeprom
    presetOneVal = EEPROM.read(0);
    presetTwoVal = EEPROM.read(1);

    // display the preset 1 settings
    presetOneLCD();
}

void loop()
{
    buttonCheck();            // check the state of the buttons
    currentMillis = millis(); // current millis becomes millis (for countdown purposes)
    presetFunctions();        // run the main code (functions for presets and grinder motor on)
}
