/* *****************************************************************
    * FILE INFORMATION *
   ***************************************************************** */
   
/// @brief Button input management system with debouncing and event detection
/// @author IEEE-ITBA | I+D | S. Lopez, A. Heir, J. Torino, I. Dib, J. Pérez
/// @copyright Copyright (c) 2024


/* *****************************************************************
    * FILE CONFIGURATION *
   ***************************************************************** */

//* NECESSARY LIBRARIES & HEADERS

#include "buttons.h"


/* *****************************************************************
    * CONSTRUCTOR *
   ***************************************************************** */

ButtonManager::ButtonManager(uint8_t pinP1, uint8_t pinP2) 
{
    // Initialize configuration with default values
    config.pinP1 = pinP1;
    config.pinP2 = pinP2;
    config.debounceDelay = DEBOUNCE_DELAY_MS;
    config.longPressDelay = LONG_PRESS_DELAY_MS;
    config.bothPressWindow = BOTH_PRESS_WINDOW_MS;
    config.invertLogic = false;
    
    // Initialize button states
    resetStates();
}


/* *****************************************************************
    * CONFIGURATION METHODS *
   ***************************************************************** */

//* INITIALIZATION

void ButtonManager::initialize() 
{
    // Configure pins as inputs with pull-up resistors
    pinMode(config.pinP1, INPUT_PULLUP);
    pinMode(config.pinP2, INPUT_PULLUP);
    
    // Reset all states
    resetStates();
}

//* CONFIGURATION MANAGEMENT

void ButtonManager::setConfig(const ButtonConfig& newConfig) 
{
    config = newConfig;
    
    // Reconfigure pins if they changed
    pinMode(config.pinP1, INPUT_PULLUP);
    pinMode(config.pinP2, INPUT_PULLUP);
}

ButtonConfig ButtonManager::getConfig() const 
{
    return config;
}


/* *****************************************************************
    * EVENT PROCESSING METHODS *
   ***************************************************************** */

//* MAIN BUTTON PROCESSING

ButtonEvent ButtonManager::processButtons() 
{
    // Process individual button states
    bool p1Changed = processButtonState(config.pinP1, stateP1);
    bool p2Changed = processButtonState(config.pinP2, stateP2);
    
    // Add processing delay
    delay(BUTTON_UPDATE_RATE);
    
    // Check for simultaneous long press first (highest priority)
    if (checkBothPress()) 
    {
        stateP1.isPressed = false;
        stateP2.isPressed = false;
        return EVENT_BOTH_LONG_PRESS;
    }
    
    // Check for individual long presses
    if (checkLongPress(stateP1) && !stateP2.isPressed) 
    {
        stateP1.isPressed = false;
        return EVENT_LONG_PRESS_P1;
    }
    
    if (checkLongPress(stateP2) && !stateP1.isPressed) 
    {
        stateP2.isPressed = false;
        return EVENT_LONG_PRESS_P2;
    }
    
    // Check for short presses (button released after being pressed)
    if (p1Changed && !stateP1.currentReading && 
        (millis() - stateP1.pressStartTime) < config.longPressDelay) 
    {
        return EVENT_SHORT_PRESS_P1;
    }
    
    if (p2Changed && !stateP2.currentReading && 
        (millis() - stateP2.pressStartTime) < config.longPressDelay) 
    {
        return EVENT_SHORT_PRESS_P2;
    }
    
    return EVENT_NONE;
}

//* BUTTON STATE QUERY

bool ButtonManager::isButtonPressed(uint8_t player) 
{
    if (player == 1) 
    {
        return stateP1.isPressed;
    } 
    else if (player == 2) 
    {
        return stateP2.isPressed;
    }
    
    return false;
}


/* *****************************************************************
    * PRIVATE METHODS *
   ***************************************************************** */

//* INDIVIDUAL BUTTON PROCESSING

bool ButtonManager::processButtonState(uint8_t pin, ButtonState& state) 
{
    bool currentReading = digitalRead(pin);
    
    // Apply logic inversion if configured
    if (config.invertLogic) 
    {
        currentReading = !currentReading;
    }
    
    bool stateChanged = false;
    
    // Check for state change with debouncing
    if (currentReading != state.previousReading && 
        millis() - state.lastDebounceTime > config.debounceDelay) 
    {
        state.lastDebounceTime = millis();
        state.currentReading = currentReading;
        
        // Button pressed
        if (state.currentReading == BUTTON_PRESSED) 
        {
            state.pressStartTime = millis();
            state.isPressed = true;
        } 
        // Button released
        else 
        {
            state.isPressed = false;
        }
        
        stateChanged = true;
    }
    
    state.previousReading = currentReading;
    return stateChanged;
}

//* LONG PRESS DETECTION

bool ButtonManager::checkLongPress(const ButtonState& state) 
{
    return state.isPressed && 
           (millis() - state.pressStartTime) > config.longPressDelay;
}

//* SIMULTANEOUS PRESS DETECTION

bool ButtonManager::checkBothPress() 
{
    // Both buttons must be pressed
    if (!stateP1.isPressed || !stateP2.isPressed) 
    {
        return false;
    }
    
    // Check if both presses started within the allowed window
    unsigned long timeDiff = abs((long)(stateP1.pressStartTime - stateP2.pressStartTime));
    if (timeDiff > config.bothPressWindow) 
    {
        return false;
    }
    
    // Both buttons must be held for long press duration
    bool p1LongPress = (millis() - stateP1.pressStartTime) > config.longPressDelay;
    bool p2LongPress = (millis() - stateP2.pressStartTime) > config.longPressDelay;
    
    return p1LongPress && p2LongPress;
}


/* *****************************************************************
    * UTILITY METHODS *
   ***************************************************************** */

//* STATE MANAGEMENT

void ButtonManager::resetStates() 
{
    // Reset player 1 button state
    stateP1.currentReading = BUTTON_RELEASED;
    stateP1.previousReading = BUTTON_RELEASED;
    stateP1.isPressed = false;
    stateP1.lastDebounceTime = 0;
    stateP1.pressStartTime = 0;
    
    // Reset player 2 button state
    stateP2.currentReading = BUTTON_RELEASED;
    stateP2.previousReading = BUTTON_RELEASED;
    stateP2.isPressed = false;
    stateP2.lastDebounceTime = 0;
    stateP2.pressStartTime = 0;
}

//* TIMING CONFIGURATION

void ButtonManager::setTimingParameters(uint16_t debounce, uint16_t longPress, uint16_t bothWindow) 
{
    config.debounceDelay = debounce;
    config.longPressDelay = longPress;
    config.bothPressWindow = bothWindow;
}