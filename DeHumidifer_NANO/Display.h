///
/// @file		Display.h
/// @brief		Library header
/// @details	<#details#>
/// @n	
/// @n @b		Project DeHumidifier
/// @n @a		Developed with [embedXcode+](http://embedXcode.weebly.com)
/// 
/// @author		Marwan Butrous
/// @author		Marwan Butrous
///
/// @date		20/09/2015 16:11
/// @version	<#version#>
/// 
/// @copyright	(c) Marwan Butrous, 2015
/// @copyright	<#license#>
///
/// @see		ReadMe.txt for references
///


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

#ifndef Display_cpp
#define Display_cpp

class Display {
public:
    Display();
    void writeToLcd(float, float);
    void writeErrorToLcd(void);
    void begin(void);
    void checkScreenTime(void);
    void turnOff(void);
    void turnOn(void);
    void enterMenu(byte);
    void exitMenu(void);
    void updateThresholdMenu(byte);
    bool isOff(void);
private:
    bool canRefreshScreen(void);
};

#endif

#define LCD_D4 5
#define LCD_D5 4
#define LCD_D6 3
#define LCD_D7 2
#define LCD_RS 12
#define LCD_E 11

#define DHT_POWER_BLOCK 10
#define LCD_POWER_BLOCK 9
