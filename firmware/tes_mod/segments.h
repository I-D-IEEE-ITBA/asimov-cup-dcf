/* *****************************************************************
    * FILE INFORMATION *
   ***************************************************************** */
   
/// @brief Display management system for 7-segment LED displays
/// @author IEEE-ITBA | I+D | S. Lopez, A. Heir, J. Torino, I. Dib, J. Pérez
/// @copyright Copyright (c) 2024


/* *****************************************************************
    * HEADER CONFIGURATION *
   ***************************************************************** */

#ifndef SEGMENTS_H
#define SEGMENTS_H


//* NECESSARY LIBRARIES

#include <Arduino.h>
#include <FastLED.h>
#include <cstdint>


//* LED STRIP CONFIGURATION

#define NUM_LEDS 56                    // 14 LEDs per digit, 2 displays per side, 2 sides
#define LEDS_PER_DIGIT 14
#define SEGMENTS_PER_DIGIT 7
#define LEDS_PER_SEGMENT 2


//* DISPLAY POSITIONS

#define DISPLAY_RIGHT_UNITS 0          // Starting index for right display units digit
#define DISPLAY_RIGHT_TENS 14          // Starting index for right display tens digit  
#define DISPLAY_LEFT_UNITS 28          // Starting index for left display units digit
#define DISPLAY_LEFT_TENS 42           // Starting index for left display tens digit


//* DEFAULT COLORS

#define COLOR_PLAYER_1 CRGB::BlueViolet
#define COLOR_PLAYER_2 CRGB::OrangeRed
#define COLOR_OFF CRGB::Black


//* DISPLAY MANAGEMENT

struct DisplayConfig 
{
    uint8_t brightness;                // LED brightness (0-255)
    CRGB colorPlayer1;
    CRGB colorPlayer2;
    bool flipDisplay;
};

class DisplayManager 
{
private:

    CRGB* leds;
    DisplayConfig config;
    
    static const uint8_t segmentMap[10];        // Normal orientation segment mapping
    static const uint8_t segmentMapFlipped[10]; // Flipped orientation segment mapping
    
    /// @brief Shows a single digit at specified position
    /// @param digit The digit to display (0-9)
    /// @param startIdx Starting LED index for the digit
    /// @param color Color to use for the digit
    /// @return true if successful, false if invalid parameters
    bool showDigitAtPosition(int digit, int startIdx, CRGB color);

public:
    
    /// @brief Constructor for DisplayManager
    /// @param ledArray Pointer to the LED array
    DisplayManager(CRGB* ledArray);

    /// @brief Initialize the display system
    /// @param brightness Initial brightness (0-255)
    void initialize(uint8_t brightness = 100);
    
    /// @brief Set display configuration
    /// @param newConfig Configuration structure
    void setConfig(const DisplayConfig& newConfig);
    
    /// @brief Get current display configuration
    /// @return Current configuration structure
    DisplayConfig getConfig() const;

    /// @brief Show a number on specified side
    /// @param number Number to display (0-99)
    /// @param isLeft true for left side, false for right side
    /// @return true if successful, false if invalid number
    bool showNumber(int number, bool isLeft);
    
    /// @brief Clear all displays
    void clearAll();
    
    /// @brief Clear specific side display
    /// @param isLeft true for left side, false for right side
    void clearSide(bool isLeft);
    
    /// @brief Update displays with current values
    /// @param leftValue Value for left display (0-99)
    /// @param rightValue Value for right display (0-99)
    void updateDisplays(int leftValue, int rightValue);
    
    /// @brief Set brightness for all LEDs
    /// @param brightness Brightness value (0-255)
    void setBrightness(uint8_t brightness);
    
    /// @brief Test display by showing all digits
    void testDisplay();
    
    /// @brief Refresh LED strip
    void refresh();
};


#endif // SEGMENTS_H