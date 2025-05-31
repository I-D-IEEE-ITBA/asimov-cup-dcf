/** *************************************************************************************
   
    * @file        segments.h
    * @brief       Display management system for 7-segment LED displays
    * @author      IEEE-ITBA | I+D | S. Lopez, A. Heir, J. Torino, I. Dib, J. Pérez
    * @date        2025-05-31
    * @version     1.0

    ************************************************************************************* */


/* **************************************************************************************
    * INCLUDES AND CONFIGURATION *
   ************************************************************************************** */

#ifndef SEGMENTS_H
#define SEGMENTS_H

#include <Arduino.h>
#include <FastLED.h>


/* **************************************************************************************
    * CONSTANTS AND DEFINITIONS *
   ************************************************************************************** */

//* LED STRIP CONFIGURATION

#define NUM_LEDS 56                    // Total LEDs: 14 per digit, 2 displays per side, 2 sides
#define LEDS_PER_DIGIT 14              // LEDs required for each 7-segment digit
#define SEGMENTS_PER_DIGIT 7           // Standard 7-segment display segments
#define LEDS_PER_SEGMENT 2             // Dual LEDs per segment for brightness


//* DISPLAY POSITION INDEXES

#define DISPLAY_RIGHT_UNITS 0          // Starting LED index for right units digit
#define DISPLAY_RIGHT_TENS 14          // Starting LED index for right tens digit  
#define DISPLAY_LEFT_UNITS 28          // Starting LED index for left units digit
#define DISPLAY_LEFT_TENS 42           // Starting LED index for left tens digit


//* DEFAULT COLOR DEFINITIONS

#define COLOR_PLAYER_1 CRGB::BlueViolet
#define COLOR_PLAYER_2 CRGB::OrangeRed
#define COLOR_OFF CRGB::Black


/* **************************************************************************************
    * DATA TYPES AND STRUCTURES *
   ************************************************************************************** */

/**
 * @brief Configuration structure for display settings
 */
struct DisplayConfig 
{
    uint8_t brightness;        // LED brightness level (0-255)
    CRGB colorPlayer1;         // Color for player 1 display
    CRGB colorPlayer2;         // Color for player 2 display
    bool flipDisplay;          // Flag to flip display orientation
};


/* **************************************************************************************
    * CLASS DECLARATIONS *
   ************************************************************************************** */

/**
 * @brief Manager class for 7-segment LED displays
 * 
 * This class provides complete control over dual 7-segment LED displays
 * including number display, color management, and display orientation.
 */
class DisplayManager 
{
private:

    CRGB* leds;                                     // Pointer to LED array
    DisplayConfig config;                           // Current display configuration
    
    // Segment mapping arrays for different orientations
    static const uint8_t segmentMap[10];            // Normal orientation mapping
    static const uint8_t segmentMapFlipped[10];     // Flipped orientation mapping
    
    /**
     * @brief Shows a single digit at specified LED position
     * @param digit The digit to display (0-9)
     * @param startIdx Starting LED index for the digit
     * @param color Color to use for the digit segments
     * @return true if successful, false if invalid parameters
     */
    bool showDigitAtPosition(int digit, int startIdx, CRGB color);

public:

    /**
     * @brief Constructor for DisplayManager
     * @param ledArray Pointer to the LED array to be managed
     */
    explicit DisplayManager(CRGB* ledArray);

    /**
     * @brief Initialize the display system with default settings
     * @param brightness Initial brightness level (0-255, default: 100)
     */
    void initialize(uint8_t brightness = 100);

    /**
     * @brief Set new display configuration
     * @param newConfig New configuration structure to apply
     */
    void setConfig(const DisplayConfig& newConfig);
    
    /**
     * @brief Get current display configuration
     * @return Copy of current configuration structure
     */
    DisplayConfig getConfig() const;

    /**
     * @brief Display a number on specified side of the display
     * @param number Number to display (0-99)
     * @param isLeft true for left side display, false for right side display
     * @return true if successful, false if number is out of range
     */
    bool showNumber(int number, bool isLeft);
    
    /**
     * @brief Update both displays simultaneously
     * @param leftValue Value for left display (0-99)
     * @param rightValue Value for right display (0-99)
     */
    void updateDisplays(int leftValue, int rightValue);

    /**
     * @brief Refresh LED strip to apply changes
     */
    void refresh();

    /**
     * @brief Clear all LED displays
     */
    void clearAll();
    
    /**
     * @brief Clear display on specified side only
     * @param isLeft true for left side, false for right side
     */
    void clearSide(bool isLeft);

    /**
     * @brief Set brightness for all LEDs
     * @param brightness Brightness value (0-255)
     */
    void setBrightness(uint8_t brightness);

    /**
     * @brief Test display functionality by showing all digits sequentially
     */
    void testDisplay();
    
};

#endif // SEGMENTS_H