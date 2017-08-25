//
// File			Menu.h
// Header
//
// Details		<#details#>
//
// Project		 DeHumidifier
// Developed with [embedXcode](http://embedXcode.weebly.com)
//
// Author		Marwan Butrous
// 				Marwan Butrous
//
// Date			20/09/2015 00:53
// Version		<#version#>
//
// Copyright	© Marwan Butrous, 2015
// Licence    <#license#>
//
// See			ReadMe.txt for references
//


// Core library for code-sense - IDE-based
#if defined(WIRING) // Wiring specific
#   include "Wiring.h"
#elif defined(MAPLE_IDE) // Maple specific
#   include "WProgram.h"
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
#elif defined(SPARK) // Spark specific
#   include "application.h"
#elif defined(ARDUINO) // Arduino 1.0 and 1.5 specific
#   include "Arduino.h"
#else // error
#   error Platform not defined
#endif // end IDE


/* DHT library
 
 MIT license
 written by Adafruit Industries
 */
#ifndef MenuWrapper_h
#define MenuWrapper_h

#include "Display.h"
#include "Threshold.h"

// Uncomment to enable printing out nice debug messages.
//#define MENU_DEBUG

// Define where debug output will be printed.
#define DEBUG_PRINTER Serial

// Setup debug printing macros.
#ifdef MENU_DEBUG
#define DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
#define DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#else
#define DEBUG_PRINT(...) {}
#define DEBUG_PRINTLN(...) {}
#endif


class MenuWrapper {
public:
    MenuWrapper(Display, Threshold);
    void begin(void);
    void check(void);
    bool isInMenu(void);
private:
    void menuButtonPressed(void);
    void upDownButtonPressed(void);
    void menuLongPress(void);
    void upDownLongPress(void);
    void checkMenuButton(void);
    void checkUpDownButton(void);
    Display _display;
    Threshold _threshold;
};

#endif
