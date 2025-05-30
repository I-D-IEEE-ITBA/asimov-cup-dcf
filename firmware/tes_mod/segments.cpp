/* *****************************************************************
    * FILE INFORMATION *
   ***************************************************************** */
   
/// @brief Display management system for 7-segment LED displays
/// @author IEEE-ITBA | I+D | S. Lopez, A. Heir, J. Torino, I. Dib, J. Pérez
/// @copyright Copyright (c) 2024


/* *****************************************************************
    * FILE CONFIGURATION *
   ***************************************************************** */

//* NECESSARY HEADERS

#include "segments.h"


/* *****************************************************************
    * SEGMENT MAPPING *
   ***************************************************************** */

//* NORMAL ORIENTATION

//      _ [1] _
//     |6|   |2|
//     |_|   |_|
//      _ [7] _
//     |5|   |3|
//     |_|   |_|
//        [4]

const uint8_t DisplayManager::segmentMap[10] = 
{
    B11111100, // 0
    B01100000, // 1
    B11011010, // 2
    B11110010, // 3
    B01100110, // 4
    B10110110, // 5
    B11111010, // 6
    B11100000, // 7
    B11111110, // 8
    B11100110  // 9
};


//* FLIPPED ORIENTATION

//      _ [4] _
//     |3|   |5|
//     |_|   |_|
//      _ [7] _
//     |2|   |6|
//     |_|   |_|
//        [1]

const uint8_t DisplayManager::segmentMapFlipped[10] = 
{
    B11111100, // 0
    B00001100, // 1
    B11011010, // 2
    B10011110, // 3
    B00101110, // 4
    B10110110, // 5
    B11110110, // 6
    B00011100, // 7
    B11111110, // 8
    B00111110  // 9
};


/* *****************************************************************
    * DISPLAY MANAGEMENT *
   ***************************************************************** */

/// @brief preset
DisplayManager::DisplayManager(CRGB* ledArray) : leds(ledArray) 
{
    // Initialize default configuration
    config.brightness = 100;
    config.colorPlayer1 = COLOR_PLAYER_1;
    config.colorPlayer2 = COLOR_PLAYER_2;
    config.flipDisplay = true;
}

/// @brief initialization
void DisplayManager::initialize(uint8_t brightness) 
{
    config.brightness = brightness;
    FastLED.setBrightness(brightness);
    clearAll();
    refresh();
}


//* CONFIGURATION UPDATE

/// @brief
void DisplayManager::setConfig(const DisplayConfig& newConfig) 
{
    config = newConfig;
    FastLED.setBrightness(config.brightness);
}

/// @brief
DisplayConfig DisplayManager::getConfig() const 
{
    return config;
}


//* DISPLAY CONTROL

/// @brief number display
bool DisplayManager::showNumber(int number, bool isLeft) 
{
    // Validate input range
    if (number < 0 || number > 99) 
    {
        return false;
    }
    
    // Extract digits
    int tens = number / 10;
    int units = number % 10;
    
    // Select color based on side
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


//* DISPLAY CLEARING

/// @brief display clearing
void DisplayManager::clearAll() 
{
    for (int i = 0; i < NUM_LEDS; i++) 
    {
        leds[i] = COLOR_OFF;
    }
    refresh();
}

/// @brief
void DisplayManager::clearSide(bool isLeft) 
{
    int startIdx = isLeft ? DISPLAY_LEFT_UNITS : DISPLAY_RIGHT_UNITS;
    int endIdx = startIdx + (LEDS_PER_DIGIT * 2); // Two digits per side
    
    for (int i = startIdx; i < endIdx; i++) 
    {
        leds[i] = COLOR_OFF;
    }
    refresh();
}

/// @brief batch update
void DisplayManager::updateDisplays(int leftValue, int rightValue) 
{
    showNumber(leftValue, true);
    showNumber(rightValue, false);
    refresh();
}


/* *****************************************************************
    * DISPLAY UTILITIES *
   ***************************************************************** */

/// @brief individual digit control
bool DisplayManager::showDigitAtPosition(int digit, int startIdx, CRGB color) 
{
    // Validate parameters
    if (digit < 0 || digit > 9 || startIdx < 0 || startIdx >= NUM_LEDS) 
    {
        return false;
    }
    
    // Select appropriate segment mapping based on configuration
    const uint8_t* mapping = config.flipDisplay ? segmentMapFlipped : segmentMap;
    
    // Process each segment
    uint8_t mask = B10000000;
    for (int i = 0; i < SEGMENTS_PER_DIGIT; i++) 
    {
        // Check if segment should be lit
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
        
        // Move to next segment
        mask >>= 1;
    }
    
    refresh();
    return true;
}


/// @brief brightness control
void DisplayManager::setBrightness(uint8_t brightness) 
{
    config.brightness = brightness;
    FastLED.setBrightness(brightness);
    refresh();
}


/// @brief display testing
void DisplayManager::testDisplay() 
{
    // Test sequence: display all digits 0-9 on both sides
    for (int digit = 0; digit <= 9; digit++) 
    {
        showDigitAtPosition(digit, DISPLAY_RIGHT_UNITS, config.colorPlayer1);
        showDigitAtPosition(digit, DISPLAY_RIGHT_TENS, config.colorPlayer1);
        showDigitAtPosition(digit, DISPLAY_LEFT_UNITS, config.colorPlayer2);
        showDigitAtPosition(digit, DISPLAY_LEFT_TENS, config.colorPlayer2);
        
        delay(500);
    }
    
    clearAll();
}


/// @brief led refresh
void DisplayManager::refresh() 
{
    FastLED.show();
}
