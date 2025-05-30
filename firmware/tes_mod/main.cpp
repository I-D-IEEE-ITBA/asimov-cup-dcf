/* *****************************************************************
    * FILE INFORMATION *
   ***************************************************************** */
   
/// @brief Main firmware file for Asimov Cup DCF project with modular architecture
/// @author IEEE-ITBA | I+D | S. Lopez, A. Heir, J. Torino, I. Dib, J. Pérez
/// @date 2025-06
/// @version 2.0 - Modular Architecture
/// @copyright Copyright (c) 2024

/* This firmware controls button functionality, 7-segment displays, and RGB LEDs
   using a modular approach with separate managers for different subsystems.
   Buttons allow incrementing/decrementing counters, displays show current values.
   RGB LEDs are used to display digits on the 7-segment displays.                   */


/* *****************************************************************
    * FILE CONFIGURATION *
   ***************************************************************** */

//* NECESSARY LIBRARIES & HEADERS

#include <Arduino.h>
#include <FastLED.h>

#include "buttons.h"
#include "segments.h"
#include "sound.h"


//* DEBUG CONFIGURATION

#define DEBUG

#ifdef DEBUG
    #ifndef BAUD_RATE
        #define BAUD_RATE 115200
    #endif // BAUD_RATE
    #define DEBUG_PRINT(x) Serial.print(x)
    #define DEBUG_PRINTLN(x) Serial.println(x)
#else
    #define DEBUG_PRINT(x)
    #define DEBUG_PRINTLN(x)
#endif // DEBUG


/* *****************************************************************
    * HARDWARE SETUP *
   ***************************************************************** */

//* PINS, LEDS AND COUNTERS

#define PIN_RGB_DATA 3                 // WS2812B LED strip data pin
#define PIN_BUZZER 5                   // Buzzer/speaker pin
#define PIN_SW0 7                      // Player 1 button pin
#define PIN_SW1 8                      // Player 2 button pin
#define PIN_SW2 9                      // Reserved button pin
#define PIN_LED_BUILTIN LED_BUILTIN    // Built-in LED pin
#define PIN_MISC_LED 6                 // Miscellaneous LED pin

#define DEFAULT_BRIGHTNESS 100         // Default LED brightness

// Player counters
int count_p1 = 0;
int count_p2 = 0;


//* CLASS MANAGERS

CRGB leds[NUM_LEDS];
ButtonManager buttonManager(PIN_SW0, PIN_SW1);
DisplayManager displayManager(leds);
SoundManager soundManager(PIN_BUZZER);


/* *****************************************************************
    * CORE LOGIC *
   ***************************************************************** */

/// @brief 
void setup() 
{
    #ifdef DEBUG
        Serial.begin(BAUD_RATE);
        DEBUG_PRINTLN("Asimov Cup DCF - Starting initialization...");
    #endif

    initializeHardware();
    updateDisplays();
    
    DEBUG_PRINTLN("Initialization complete. System ready.");
    delay(100);
}

/// @brief 
void loop() 
{
    handleButtonEvents();
    soundManager.tick();                 // Update sound system
    delay(10);
}


/* *****************************************************************
    * HARDWARE MANAGEMENT *
   ***************************************************************** */

/// @brief Initialize all hardware and subsystems
void initializeHardware() 
{
    FastLED.addLeds<WS2812B, PIN_RGB_DATA, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    displayManager.initialize(DEFAULT_BRIGHTNESS);
    buttonManager.initialize();
    soundManager.initialize();
    
    // Configure additional pins
    pinMode(PIN_SW2, INPUT_PULLUP);      // Reserve button (not used)
    pinMode(PIN_MISC_LED, OUTPUT);       // Miscellaneous LED
    pinMode(PIN_LED_BUILTIN, OUTPUT);    // Built-in LED
    
    // Set initial LED states
    digitalWrite(PIN_MISC_LED, HIGH);
    digitalWrite(PIN_LED_BUILTIN, LOW);
    
    DEBUG_PRINTLN("Hardware initialization complete.");
}


/// @brief Process button events and update counters
void handleButtonEvents() 
{
    ButtonEvent event = buttonManager.processButtons();
    
    switch (event) 
    {
        case EVENT_SHORT_PRESS_P1:
            DEBUG_PRINTLN("Short press P1");
            count_p1++;
            count_p1 = (count_p1 > 99) ? 99 : count_p1;  // Clamp to maximum
            soundManager.playSound(SOUND_SHORT_PRESS);
            updateDisplays();
            break;
            
        case EVENT_LONG_PRESS_P1:
            DEBUG_PRINTLN("Long press P1");
            count_p1--;
            count_p1 = (count_p1 < 0) ? 0 : count_p1;    // Clamp to minimum
            soundManager.playSound(SOUND_LONG_PRESS);
            updateDisplays();
            break;
            
        case EVENT_SHORT_PRESS_P2:
            DEBUG_PRINTLN("Short press P2");
            count_p2++;
            count_p2 = (count_p2 > 99) ? 99 : count_p2;  // Clamp to maximum
            soundManager.playSound(SOUND_SHORT_PRESS);
            updateDisplays();
            break;
            
        case EVENT_LONG_PRESS_P2:
            DEBUG_PRINTLN("Long press P2");
            count_p2--;
            count_p2 = (count_p2 < 0) ? 0 : count_p2;    // Clamp to minimum
            soundManager.playSound(SOUND_LONG_PRESS);
            updateDisplays();
            break;
            
        case EVENT_BOTH_LONG_PRESS:
            DEBUG_PRINTLN("Both buttons long press - Reset counters");
            count_p1 = 0;
            count_p2 = 0;
            soundManager.playSound(SOUND_DOUBLE_PRESS);
            updateDisplays();
            break;
            
        default:
            // No event occurred, continue normal operation
            break;
    }
    
    // Debug output for counter values when changed
    if (event != EVENT_NONE) 
    {
        DEBUG_PRINT("Counter P1: ");
        DEBUG_PRINT(count_p1);
        DEBUG_PRINT(" | Counter P2: ");
        DEBUG_PRINTLN(count_p2);
    }
}


/// @brief Update display with current counter values
void updateDisplays() 
{
    displayManager.updateDisplays(count_p1, count_p2);
}
