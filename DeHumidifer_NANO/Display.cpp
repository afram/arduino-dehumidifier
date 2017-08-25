//
// Display.cpp 
// Library C++ code
// ----------------------------------
// Developed with embedXcode+ 
// http://embedXcode.weebly.com
//
// Project 		DeHumidifier
//
// Created by 	Marwan Butrous, 20/09/2015 16:11
// 				Marwan Butrous
//
// Copyright 	(c) Marwan Butrous, 2015
// Licence		<#license#>
//
// See 			Display.h and ReadMe.txt for references
//


// Library header
#include "Display.h"
#include "LiquidCrystal.h"

#define DISPLAY_TIMEOUT 30000 // 30 seconds
#define DISPLAY_REFRESH_TIME 300

#define LCD_D4 5
#define LCD_D5 4
#define LCD_D6 12
#define LCD_D7 9
#define LCD_RS 13
#define LCD_E 11

#define LCD_POWER_BLOCK 6

float humidityCache = 0;
float tempCache = 0;

unsigned long timeSinceLastInteraction;
unsigned long timeSinceLastRefresh;
bool screenOff = true;

LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

Display::Display() {

}

void Display::begin() {
    pinMode(LCD_POWER_BLOCK, OUTPUT);
    digitalWrite(LCD_POWER_BLOCK, HIGH);

    timeSinceLastRefresh = timeSinceLastInteraction = millis();
    lcd.begin(16, 2);
    lcd.setCursor(0, 0);
    
    lcd.print("Starting...");
}

bool Display::isOff() {
    return screenOff;
}

void Display::writeToLcd(float humidity, float temp) {
    if (millis() - timeSinceLastRefresh >= DISPLAY_REFRESH_TIME) {
        timeSinceLastRefresh = millis();
        if (humidity != humidityCache) {
            lcd.setCursor(0, 0);
            lcd.print("Humidity: ");
            lcd.print(humidity);
            lcd.print("%");
            humidityCache = humidity;
        }
        
        if (temp != tempCache) {
            lcd.setCursor(0, 1);
            lcd.print("Temp: ");
            lcd.print(temp);
            lcd.print((char)223);
            lcd.print("C");
            tempCache = temp;
        }
    }
}

bool Display::canRefreshScreen() {
    bool refresh = millis() - timeSinceLastRefresh >= DISPLAY_REFRESH_TIME;
    if (refresh) {
        timeSinceLastRefresh = millis();
        return true;
    }
    return false;
}

void Display::writeErrorToLcd() {
    if (canRefreshScreen()) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Failed to read");
        lcd.setCursor(0, 1);
        lcd.print("from DHT sensor!");
    }
}

void Display::enterMenu(byte threshold) {
    timeSinceLastInteraction = millis();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Set Humidity");
    lcd.setCursor(0, 1);
    lcd.print(threshold);
    lcd.print("%");
}

/**
 * Update the value of threshold in the menu
 */
void Display::updateThresholdMenu(byte threshold) {
    lcd.setCursor(0, 1);
    lcd.print("    ");

    lcd.setCursor(0, 1);
    lcd.print(threshold);
    lcd.print("%");
}

void Display::exitMenu() {
    timeSinceLastInteraction = millis();
    
    // reset caches so that the display refreshes main screen correctly
    humidityCache = 0;
    tempCache = 0;
}

void Display::turnOff() {
    pinMode(LCD_POWER_BLOCK, INPUT);
    lcd.noDisplay();
    screenOff = true;
}

void Display::turnOn() {
    timeSinceLastInteraction = millis();
    pinMode(LCD_POWER_BLOCK, OUTPUT);
    digitalWrite(LCD_POWER_BLOCK, HIGH);
    lcd.display();
    screenOff = false;
}

void Display::checkScreenTime() {
    unsigned long now = millis();
    if (now - timeSinceLastInteraction >= DISPLAY_TIMEOUT) {
        turnOff();
    }
}

