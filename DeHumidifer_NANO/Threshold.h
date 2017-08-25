///
/// @file		Threshold.h
/// @brief		Library header
/// @details	<#details#>
/// @n	
/// @n @b		Project DeHumidifier
/// @n @a		Developed with [embedXcode+](http://embedXcode.weebly.com)
/// 
/// @author		Marwan Butrous
/// @author		Marwan Butrous
///
/// @date		27/09/2015 18:36
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

#ifndef Threshold_cpp
#define Threshold_cpp

class Threshold {
public:
    Threshold();
    byte getThreshold(void);
    void incrementByOne(void);
    void decrementByOne(void);
    void save(void);
private:
    void initializeThresholdMemory(void);
    void setThreholdInMemory(byte);
    byte getThresholdFromMemory(void);
    byte getMarkerFromMemory(void);
};


#endif
