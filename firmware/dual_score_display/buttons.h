/** *************************************************************************************
   
    * @file        buttons.h
    * @brief       Button input management system with debouncing and event detection
    * @author      IEEE-ITBA | I+D | S. Lopez, A. Heir, J. Torino, I. Dib, J. Pérez
    * @date        2025-05-31
    * @version     1.0

    ************************************************************************************* */


/* **************************************************************************************
    * INCLUDES AND CONFIGURATION *
   ************************************************************************************** */

#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>


/* **************************************************************************************
    * CONSTANTS AND DEFINITIONS *
   ************************************************************************************** */

//* TIMING CONSTANTS

#define DEBOUNCE_DELAY_MS 50           // Button debounce delay in milliseconds
#define LONG_PRESS_DELAY_MS 600        // Long press threshold in milliseconds
#define BOTH_PRESS_WINDOW_MS 500       // Window for simultaneous button press detection
#define BUTTON_UPDATE_RATE 50          // Button processing update rate in milliseconds


//* BUTTON STATE DEFINITIONS

#define BUTTON_PRESSED LOW             // Active button state (pull-up logic)
#define BUTTON_RELEASED HIGH           // Inactive button state (pull-up logic)


/* **************************************************************************************
    * DATA TYPES AND STRUCTURES *
   ************************************************************************************** */

/**
 * @brief Enumeration of button events
 */
enum ButtonEvent 
{
    EVENT_NONE,                        // No button event occurred
    EVENT_SHORT_PRESS_P1,              // Player 1 short press event
    EVENT_SHORT_PRESS_P2,              // Player 2 short press event
    EVENT_LONG_PRESS_P1,               // Player 1 long press event
    EVENT_LONG_PRESS_P2,               // Player 2 long press event
    EVENT_BOTH_LONG_PRESS              // Both buttons long press event
};

/**
 * @brief Configuration structure for button system settings
 */
struct ButtonConfig 
{
    uint8_t pinP1;                     // Pin number for player 1 button
    uint8_t pinP2;                     // Pin number for player 2 button
    uint16_t debounceDelay;            // Debounce delay in milliseconds
    uint16_t longPressDelay;           // Long press threshold in milliseconds
    uint16_t bothPressWindow;          // Simultaneous press detection window
    bool invertLogic;                  // Invert button logic if needed
};

/**
 * @brief Structure to track individual button state
 */
struct ButtonState 
{
    bool currentReading;               // Current button reading
    bool previousReading;              // Previous button reading
    bool isPressed;                    // Current pressed state
    unsigned long lastDebounceTime;    // Last debounce timestamp
    unsigned long pressStartTime;      // Press start timestamp
};


/* **************************************************************************************
    * CLASS DECLARATIONS *
   ************************************************************************************** */

/**
 * @brief Manager class for button input handling and event detection
 * 
 * This class provides complete button management including debouncing,
 * short/long press detection, and simultaneous button press handling.
 */
class ButtonManager 
{
private:
    
    ButtonConfig config;               // Current button configuration
    ButtonState stateP1;               // Player 1 button state
    ButtonState stateP2;               // Player 2 button state
    
    /**
     * @brief Process individual button state with debouncing
     * @param pin Button pin to read
     * @param state Button state structure to update
     * @return true if button state changed, false otherwise
     */
    bool processButtonState(uint8_t pin, ButtonState& state);
    
    /**
     * @brief Check for long press condition on a button
     * @param state Button state to check
     * @return true if long press detected, false otherwise
     */
    bool checkLongPress(const ButtonState& state);
    
    /**
     * @brief Check for simultaneous button press condition
     * @return true if both buttons pressed within detection window
     */
    bool checkBothPress();

public:
    
    /**
     * @brief Constructor for ButtonManager
     * @param pinP1 Pin number for player 1 button
     * @param pinP2 Pin number for player 2 button
     */
    explicit ButtonManager(uint8_t pinP1, uint8_t pinP2);
    
    /**
     * @brief Initialize the button system with default settings
     */
    void initialize();
    
    /**
     * @brief Set new button system configuration
     * @param newConfig New configuration structure to apply
     */
    void setConfig(const ButtonConfig& newConfig);
    
    /**
     * @brief Get current button system configuration
     * @return Copy of current configuration structure
     */
    ButtonConfig getConfig() const;

    /**
     * @brief Process button inputs and detect events
     * @return Button event that occurred (EVENT_NONE if no event)
     */
    ButtonEvent processButtons();
    
    /**
     * @brief Check if specific player button is currently pressed
     * @param player Player number (1 or 2)
     * @return true if button is pressed, false otherwise
     */
    bool isButtonPressed(uint8_t player);
    
    /**
     * @brief Reset all button states to default values
     */
    void resetStates();
    
    /**
     * @brief Set custom timing parameters for button detection
     * @param debounce Debounce delay in milliseconds
     * @param longPress Long press threshold in milliseconds
     * @param bothWindow Simultaneous press detection window in milliseconds
     */
    void setTimingParameters(uint16_t debounce, uint16_t longPress, uint16_t bothWindow);
};

#endif // BUTTONS_H