/* *****************************************************************
    * FILE INFORMATION *
   ***************************************************************** */
   
/// @brief Button input management system with debouncing and event detection
/// @author IEEE-ITBA | I+D | S. Lopez, A. Heir, J. Torino, I. Dib, J. Pérez
/// @copyright Copyright (c) 2024


/* *****************************************************************
    * HEADER CONFIGURATION *
   ***************************************************************** */

#ifndef BUTTONS_H
#define BUTTONS_H


//* NECESSARY LIBRARIES

#include <Arduino.h>
#include <cstdint>


//* TIMING CONSTANTS

#define DEBOUNCE_DELAY_MS 50
#define LONG_PRESS_DELAY_MS 600
#define BOTH_PRESS_WINDOW_MS 500
#define BUTTON_UPDATE_RATE 50


//* BUTTON STATES

#define BUTTON_PRESSED LOW
#define BUTTON_RELEASED HIGH


//* BUTTON MANAGEMENT

enum ButtonEvent 
{
    EVENT_NONE,
    EVENT_SHORT_PRESS_P1,
    EVENT_SHORT_PRESS_P2,
    EVENT_LONG_PRESS_P1,
    EVENT_LONG_PRESS_P2,
    EVENT_BOTH_LONG_PRESS
};

struct ButtonConfig 
{
    uint8_t pinP1;
    uint8_t pinP2;
    uint16_t debounceDelay;
    uint16_t longPressDelay;
    uint16_t bothPressWindow;
    bool invertLogic;
};

struct ButtonState 
{
    bool currentReading;
    bool previousReading;
    bool isPressed;
    unsigned long lastDebounceTime;
    unsigned long pressStartTime;
};

class ButtonManager 
{
private:
    
    ButtonConfig config;
    ButtonState stateP1;
    ButtonState stateP2;
    
    /// @brief Process individual button state
    /// @param pin Button pin to read
    /// @param state Button state structure to update
    /// @return true if state changed, false otherwise
    bool processButtonState(uint8_t pin, ButtonState& state);
    
    /// @brief Check for long press condition
    /// @param state Button state to check
    /// @return true if long press detected
    bool checkLongPress(const ButtonState& state);
    
    /// @brief Check for simultaneous button press
    /// @return true if both buttons pressed within window
    bool checkBothPress();

public:
    
    /// @brief Constructor for ButtonManager
    /// @param pinP1 Pin number for player 1 button
    /// @param pinP2 Pin number for player 2 button
    ButtonManager(uint8_t pinP1, uint8_t pinP2);
    
    /// @brief Initialize the button system
    void initialize();
    
    /// @brief Set button configuration
    /// @param newConfig Configuration structure
    void setConfig(const ButtonConfig& newConfig);
    
    /// @brief Get current button configuration
    /// @return Current configuration structure
    ButtonConfig getConfig() const;

    /// @brief Process button inputs and return event
    /// @return Button event that occurred
    ButtonEvent processButtons();
    
    /// @brief Check if specific button is currently pressed
    /// @param player Player number (1 or 2)
    /// @return true if button is pressed
    bool isButtonPressed(uint8_t player);
    
    /// @brief Reset all button states
    void resetStates();
    
    /// @brief Set custom timing parameters
    /// @param debounce Debounce delay in milliseconds
    /// @param longPress Long press threshold in milliseconds
    /// @param bothWindow Simultaneous press window in milliseconds
    void setTimingParameters(uint16_t debounce, uint16_t longPress, uint16_t bothWindow);
};


#endif // BUTTONS_H