#include <Arduino.h>
#include "menu.h";
#include "notes.h";

// SET UP KEYPAD
#include <Keypad.h>
#include <Key.h>
const char rows = 4;
const char cols = 4;
char keys[rows][cols] = {
  {0,4,8,12},
  {1,5,9,13},
  {2,6,10,14},
  {3,7,11,15}
};
char rowPins[rows] = {11, 10, 9,  8}; //connect to the row pinouts of the keypad
char colPins[cols] = {12, 13, A0, A1}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );

// Configure the LCD
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>  // F Malpartida's NewLiquidCrystal library
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // Addr, En, Rw, Rs, d4, d5, d6, d7, backlighpin, polarity


/**
 * Repaint the lcd
 */
void Menu::repaint() {
    lcd.clear();
    lcd.print(Notes::getToneName(Notes::getTone()));
    lcd.print((unsigned int)Notes::getOctaveName(Notes::getTone()));
    lcd.print(" ");
    lcd.print(Notes::getCurrentScaleName());
}

/**
 * Handle a keypress
 */
void Menu::handleKeyPress(KeypadEvent key) {
    switch(key) {
        // the 0 and 1 buttons cycle the root tones
        case 0:
            Notes::decrementTone();
            break ;
        case 1:
            Notes::incrementTone();
            break ;

        // the 2 and 3 buttons cycle the scale index
        case 2:
            Notes::decrementScaleIndex();
            break ;
        case 3:
            Notes::incrementScaleIndex();
            break ;

        // 4 and 5 cycle the octave
        case 4:
            Notes::decrementOctave();
            break ;
        case 5:
            Notes::incrementOctave();
            break ;
    }
    repaint();
}

/**
 * Handle keypad events
 */
void keypadEvent(KeypadEvent key){
    switch (keypad.getState()){
        case HOLD:
            break;
        case PRESSED:
            Menu::handleKeyPress(key);
            break ;
        case RELEASED:
            break ;
    }
}

/**
 * Set up the initial menu
 */
void Menu::init() {
    lcd.begin(16, 2);
    lcd.backlight();

    keypad.addEventListener(keypadEvent); //add an event listener for this keypad
}

/**
 * Print a calibrating message
 */
void Menu::startCalibrating() {
    lcd.clear();
    lcd.print("Calibrating");
}

/**
 * Clear the screen
 */
void Menu::clear() {
    lcd.clear();
}

/**
 * Enter the menu cycle
 */
void Menu::start() {
    repaint();
}

/**
 * make sure we check the keypad
 */
void Menu::getKey() {
    keypad.getKey();
}
