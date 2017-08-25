//
// Threshold.cpp 
// Library C++ code
// ----------------------------------
// Developed with embedXcode+ 
// http://embedXcode.weebly.com
//
// Project 		DeHumidifier
//
// Created by 	Marwan Butrous, 27/09/2015 18:36
// 				Marwan Butrous
//
// Copyright 	(c) Marwan Butrous, 2015
// Licence		<#license#>
//
// See 			Threshold.h and ReadMe.txt for references
//


// Library header
#include "Threshold.h"
#include "EEPROM.h"

byte thresholdGlobal;

// Code

Threshold::Threshold() {
    initializeThresholdMemory();
}

byte Threshold::getThreshold() {
    return thresholdGlobal;
}

void Threshold::initializeThresholdMemory(void) {
    // set default threshold if not already set
    byte marker = getMarkerFromMemory();
    if (marker == 42) {
        thresholdGlobal = getThresholdFromMemory();
    }
    else {
        EEPROM.write(0, 42);
        thresholdGlobal = 70;
        setThreholdInMemory(thresholdGlobal);
    }
}

void Threshold::setThreholdInMemory(byte thresholdValue) {
    EEPROM.write(1, thresholdValue);
}

byte Threshold::getThresholdFromMemory(void) {
    return EEPROM.read(1);
}

/**
 * To ensure we can work on any microcontroller, the first byte in memory will be our marker. It will always equal 42
 * if this marker isn't equal to 42, then it's a new installation, and we should bootstrap our memory.
 * threshold lives at memory address 1 (marker at 0)
 */
byte Threshold::getMarkerFromMemory(void) {
    return EEPROM.read(0);
}

void Threshold::incrementByOne() {
    if (thresholdGlobal < 100) {
        thresholdGlobal++;
    }
}

void Threshold::decrementByOne() {
    if (thresholdGlobal > 0) {
        thresholdGlobal--;
    }
}

void Threshold::save() {
    setThreholdInMemory(thresholdGlobal);
}