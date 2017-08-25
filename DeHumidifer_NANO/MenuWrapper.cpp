//
// menu.cpp
// C++ code
// ----------------------------------
// Developed with embedXcode+
// http://embedXcode.weebly.com
//
// Project 		DeHumidifier
//
// Created by 	Marwan Butrous, 20/09/2015 00:52
// 				Marwan Butrous
//
// Copyright 	(c) Marwan Butrous, 2015
// Licence		<#license#>
//
// See 			menu.h and ReadMe.txt for references
//


// Core library for code-sense - IDE-based
#if defined(WIRING) // Wiring specific
#   include "Wiring.h"
#elif defined(MAPLE_IDE) // Maple specific
#   include "WProgram.h"
#elif defined(ROBOTIS) // Robotis specific
#   include "libpandora_types.h"
#   include "pandora.h"
#elif defined(MPIDE) // chipKIT specific
#   include "WProgram.h"
#elif defined(DIGISPARK) // Digispark specific
#   include "Arduino.h"
#elif defined(ENERGIA) // LaunchPad specific
#   include "Energia.h"
#elif defined(LITTLEROBOTFRIENDS) // LittleRobotFriends specific
#   include "LRF.h"
#elif defined(MICRODUINO) // Microduino specific
#   include "Arduino.h"
#elif defined(TEENSYDUINO) // Teensy specific
#   include "Arduino.h"
#elif defined(REDBEARLAB) // RedBearLab specific
#   include "Arduino.h"
#elif defined(RFDUINO) // RFduino specific
#   include "Arduino.h"
#elif defined(SPARK) // Spark specific
#   include "application.h"
#elif defined(ARDUINO) // Arduino 1.0 and 1.5 specific
#   include "Arduino.h"
#else // error
#   error Platform not defined
#endif // end IDE

/*
 MENU library for the Dehumidifier
 
 */

#include "MenuWrapper.h"
#include "Display.h"
#include "Threshold.h"


#define BTN_MENU_SELECT_PIN 3
#define BTN_UP_DOWN_PIN 2

int menuButtonState;             // the current reading from the input pin
int lastMenuButtonState = HIGH;   // the previous reading from the input pin

int upDownButtonState;
int lastUpDownButtonState = HIGH;

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastMenuDebounceTime = 0;  // the last time the output pin was toggled
long lastUpDownDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

bool inMenu = false;
bool menuLongPressed = false;
bool upDownLongPressed = false;

bool menuButtonIsPressed = false;
bool downHasBeenPressed = false;

int longPressDelay = 1000;


MenuWrapper::MenuWrapper(Display display, Threshold threshold) {
    _display = display;
    _threshold = threshold;
}

void MenuWrapper::begin(void) {
    pinMode(BTN_MENU_SELECT_PIN, INPUT_PULLUP);
    pinMode(BTN_UP_DOWN_PIN, INPUT_PULLUP);
}

bool MenuWrapper::isInMenu() {
    return inMenu;
}

void MenuWrapper::check() {
    checkMenuButton();
    checkUpDownButton();
}

void MenuWrapper::checkMenuButton() {
    // read the state of the switch into a local variable:
    int reading = digitalRead(BTN_MENU_SELECT_PIN);
    long time = millis();
    
    // check to see if you just pressed the button
    // (i.e. the input went from LOW to HIGH),  and you've waited
    // long enough since the last press to ignore any noise:
    
    // If the switch changed, due to noise or pressing:
    if (reading != lastMenuButtonState) {
        // reset the debouncing timer
        lastMenuDebounceTime = time;
    }
    
    if ((time - lastMenuDebounceTime) > debounceDelay) {
        // whatever the reading is at, it's been there for longer
        // than the debounce delay, so take it as the actual current state:
        
        
        
        // if the button state has changed:
        if (reading != menuButtonState) {
            menuButtonState = reading;
            
            // only toggle the LED if the new button state is LOW
            if (menuButtonState == LOW) {
                menuButtonIsPressed = true;
            }
            
            if (menuButtonState == HIGH) {
                if (menuButtonIsPressed && !menuLongPressed && !downHasBeenPressed) {
                    menuButtonPressed();
                }
                
                menuLongPressed = false;
                downHasBeenPressed = false;
                menuButtonIsPressed = false;
            }
        }
        
        if (menuButtonState == LOW && time - lastMenuDebounceTime > longPressDelay) {
            menuLongPress();
        }
    }
    
    // save the reading.  Next time through the loop,
    // it'll be the lastButtonState:
    lastMenuButtonState = reading;
}

void MenuWrapper::checkUpDownButton() {
    // read the state of the switch into a local variable:
    int reading = digitalRead(BTN_UP_DOWN_PIN);
    
    // check to see if you just pressed the button
    // (i.e. the input went from LOW to HIGH),  and you've waited
    // long enough since the last press to ignore any noise:
    
    // If the switch changed, due to noise or pressing:
    if (reading != lastUpDownButtonState) {
        // reset the debouncing timer
        lastUpDownDebounceTime = millis();
    }
    
    if ((millis() - lastUpDownDebounceTime) > debounceDelay) {
        // whatever the reading is at, it's been there for longer
        // than the debounce delay, so take it as the actual current state:
        
        // if the button state has changed:
        if (reading != upDownButtonState) {
            upDownButtonState = reading;
            
            // only toggle the LED if the new button state is LOW
            if (upDownButtonState == LOW) {
                upDownButtonPressed();
            }
            
            if (upDownButtonState == HIGH) {
                upDownLongPressed = false;
            }
        }
        
        if (upDownButtonState == LOW && millis() - lastUpDownDebounceTime > longPressDelay) {
            upDownLongPress();
        }
    }
    
    // save the reading.  Next time through the loop,
    // it'll be the lastButtonState:
    lastUpDownButtonState = reading;
}

// private
void MenuWrapper::menuButtonPressed() {
    if (!_display.isOff()) {
        if (!inMenu) {
            inMenu = true;
            _display.enterMenu(_threshold.getThreshold());
            Serial.println("Go into menu");
        }
        else {
            inMenu = false;
            _display.exitMenu();
            _threshold.save();
            Serial.println("Out of menu");
        }
        
    }
    else {
        _display.turnOn();
    }
}

void MenuWrapper::upDownButtonPressed() {
    if (!_display.isOff() && inMenu) {
        if (menuButtonIsPressed) {
            downHasBeenPressed = true;
            Serial.println("DOWN");
            _threshold.decrementByOne();
            _display.updateThresholdMenu(_threshold.getThreshold());
        }
        else {
            Serial.println("UP");
            _threshold.incrementByOne();
            _display.updateThresholdMenu(_threshold.getThreshold());
        }
    }
    else {
        _display.turnOn();
    }
}

void MenuWrapper::menuLongPress() {
    if (!menuLongPressed && !downHasBeenPressed) {
        menuLongPressed = true;
        Serial.println("MENU LONG PRESS");
    }
}

void MenuWrapper::upDownLongPress() {
    if (!upDownLongPressed) {
        upDownLongPressed = true;
    }
    
    if (menuButtonIsPressed) {
        Serial.println("DOWN LONG PRESS");
        _threshold.decrementByOne();
        _display.updateThresholdMenu(_threshold.getThreshold());
    }
    else {
        Serial.println("UP LONG PRESS");
        _threshold.incrementByOne();
        _display.updateThresholdMenu(_threshold.getThreshold());
    }
}
