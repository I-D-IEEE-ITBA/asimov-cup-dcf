/** *************************************************************************************
   
    * @file        segments.cpp
    * @brief       Implementation of display management system for 7-segment LED displays
    * @author      IEEE-ITBA | I+D | S. Lopez, A. Heir, J. Torino, I. Dib, J. Pérez
    * @date        2025-05-31
    * @version     1.0

    ************************************************************************************* */


/* **************************************************************************************
    * INCLUDES AND CONFIGURATION *
   ************************************************************************************** */

#include "segments.h"


/* **************************************************************************************
    * CONSTANTS AND STATIC DATA *
   ************************************************************************************** */

//* NORMAL ORIENTATION SEGMENT MAPPING

//      _ [1] _
//     |6|   |2|
//     |_|   |_|
//      _ [7] _
//     |5|   |3|
//     |_|   |_|
//        [4]

const uint8_t DisplayManager::segmentMap[10] = 
{
    B11111100, // 0: segments 1,2,3,4,5,6
    B01100000, // 1: segments 2,3
    B11011010, // 2: segments 1,2,4,5,7
    B11110010, // 3: segments 1,2,3,4,7
    B01100110, // 4: segments 2,3,6,7
    B10110110, // 5: segments 1,3,4,6,7
    B11111010, // 6: segments 1,3,4,5,6,7
    B11100000, // 7: segments 1,2,3
    B11111110, // 8: segments 1,2,3,4,5,6,7
    B11100110  // 9: segments 1,2,3,6,7
};


//* FLIPPED ORIENTATION SEGMENT MAPPING

//      _ [4] _
//     |3|   |5|
//     |_|   |_|
//      _ [7] _
//     |2|   |6|
//     |_|   |_|
//        [1]

const uint8_t DisplayManager::segmentMapFlipped[10] = 
{
    B11111100, // 0: segments 1,2,3,4,5,6
    B00001100, // 1: segments 2,3
    B11011010, // 2: segments 1,2,4,5,7
    B10011110, // 3: segments 1,2,3,4,7
    B00101110, // 4: segments 2,3,6,7
    B10110110, // 5: segments 1,3,4,6,7
    B11110110, // 6: segments 1,3,4,5,6,7
    B00011100, // 7: segments 1,2,3
    B11111110, // 8: segments 1,2,3,4,5,6,7
    B00111110  // 9: segments 1,2,3,6,7
};


/* **************************************************************************************
    * CLASS IMPLEMENTATION *
   ************************************************************************************** */

//* CONSTRUCTOR AND INITIALIZATION

DisplayManager::DisplayManager(CRGB* ledArray) : leds(ledArray) 
{
    // Initialize with default configuration values
    config.brightness = 100;
    config.colorPlayer1 = COLOR_PLAYER_1;
    config.colorPlayer2 = COLOR_PLAYER_2;
    config.flipDisplay = true;
}

void DisplayManager::initialize(uint8_t brightness) 
{
    config.brightness = brightness;
    FastLED.setBrightness(brightness);
    clearAll();
    refresh();
}


//* CONFIGURATION SETUP

void DisplayManager::setConfig(const DisplayConfig& newConfig) 
{
    config = newConfig;
    FastLED.setBrightness(config.brightness);
}

DisplayConfig DisplayManager::getConfig() const 
{
    return config;
}

void DisplayManager::setBrightness(uint8_t brightness) 
{
    config.brightness = brightness;
    FastLED.setBrightness(brightness);
    refresh();
}


//* DISPLAY CONTROL

bool DisplayManager::showNumber(int number, bool isLeft) 
{
    // Validate input range
    if (number < 0 || number > 99) 
    {
        return false;
    }
    
    // Extract individual digits
    int tens = number / 10;
    int units = number % 10;
    
    // Select color based on display side
    CRGB color = isLeft ? config.colorPlayer2 : config.colorPlayer1;
    
    // Display logic based on orientation
    // Current logic assumes display order: [4, 3, 2, 1]
    // For order [1, 2, 3, 4], adjust startIdx by ±28 and swap digit order
    
    if (isLeft) 
    {
        showDigitAtPosition(units, DISPLAY_LEFT_UNITS, color);
        showDigitAtPosition(tens, DISPLAY_LEFT_TENS, color);
    } 
    else 
    {
        showDigitAtPosition(units, DISPLAY_RIGHT_UNITS, color);
        showDigitAtPosition(tens, DISPLAY_RIGHT_TENS, color);
    }
    
    return true;
}

void DisplayManager::updateDisplays(int leftValue, int rightValue) 
{
    showNumber(leftValue, true);
    showNumber(rightValue, false);
    refresh();
}

bool DisplayManager::showDigitAtPosition(int digit, int startIdx, CRGB color) 
{
    // Validate input parameters
    if (digit < 0 || digit > 9 || startIdx < 0 || startIdx >= NUM_LEDS) 
    {
        return false;
    }
    
    // Select appropriate segment mapping based on orientation setting
    const uint8_t* mapping = config.flipDisplay ? segmentMapFlipped : segmentMap;
    
    // Process each segment using bit masking
    uint8_t mask = B10000000;
    for (int i = 0; i < SEGMENTS_PER_DIGIT; i++) 
    {
        // Check if current segment should be illuminated
        if (mapping[digit] & mask) 
        {
            // Light up both LEDs for this segment
            leds[startIdx + i * LEDS_PER_SEGMENT] = color;
            leds[startIdx + i * LEDS_PER_SEGMENT + 1] = color;
        }

        else 
        {
            // Turn off both LEDs for this segment
            leds[startIdx + i * LEDS_PER_SEGMENT] = COLOR_OFF;
            leds[startIdx + i * LEDS_PER_SEGMENT + 1] = COLOR_OFF;
        }
        
        // Move to next segment bit
        mask >>= 1;
    }
    
    refresh();
    return true;
}

void DisplayManager::refresh() 
{
    FastLED.show();
}


//* DISPLAY CLEARING

void DisplayManager::clearAll() 
{
    // Turn off all LEDs in the strip
    for (int i = 0; i < NUM_LEDS; i++) 
    {
        leds[i] = COLOR_OFF;
    }

    refresh();
}

void DisplayManager::clearSide(bool isLeft) 
{
    // Calculate range for the specified side
    int startIdx = isLeft ? DISPLAY_LEFT_UNITS : DISPLAY_RIGHT_UNITS;
    int endIdx = startIdx + (LEDS_PER_DIGIT * 2); // Two digits per side
    
    // Clear LEDs for the specified range
    for (int i = startIdx; i < endIdx; i++) 
    {
        leds[i] = COLOR_OFF;
    }
    refresh();
}


//* DISPLAY TEST

void DisplayManager::testDisplay() 
{
    // Cycle through all digits 0-9 on both displays
    for (int digit = 0; digit <= 9; digit++) 
    {
        showDigitAtPosition(digit, DISPLAY_RIGHT_UNITS, config.colorPlayer1);
        showDigitAtPosition(digit, DISPLAY_RIGHT_TENS, config.colorPlayer1);
        showDigitAtPosition(digit, DISPLAY_LEFT_UNITS, config.colorPlayer2);
        showDigitAtPosition(digit, DISPLAY_LEFT_TENS, config.colorPlayer2);
        
        delay(500); // Wait between digit changes
    }
    
    clearAll();
}
