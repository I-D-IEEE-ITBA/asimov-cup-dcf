/* *****************************************************************
    * FILE INFORMATION *
   ***************************************************************** */
   
/// @brief Sound management system for button feedback and game events
/// @author IEEE-ITBA | I+D | S. Lopez, A. Heir, J. Torino, I. Dib, J. Pérez
/// @copyright Copyright (c) 2024


/* *****************************************************************
    * HEADER CONFIGURATION *
   ***************************************************************** */

#ifndef SOUND_H
#define SOUND_H

//* NECESSARY LIBRARIES

#include <Arduino.h>
#include <cstdint>


//* SOUND PARAMETERS

#define SHORT_PRESS_PWM 160
#define LONG_PRESS_PWM 250
#define DOUBLE_PRESS_PWM 255

#define SHORT_PRESS_MS 150
#define LONG_PRESS_MS 250
#define DOUBLE_PRESS_MS 750


//* SOUND MANAGEMENT

enum SoundEvent 
{
    SOUND_NONE,
    SOUND_SHORT_PRESS,
    SOUND_LONG_PRESS,
    SOUND_DOUBLE_PRESS,
    SOUND_ERROR,
    SOUND_SUCCESS,
    SOUND_CUSTOM
};

struct SoundConfig 
{
    uint8_t buzzerPin;
    bool soundEnabled;
    uint8_t volumeScale;               // (0-100)
};

struct CustomSound 
{
    uint16_t pwmIntensity;             // (0-255)
    uint32_t duration;                 // in ms
    uint16_t frequency;
};

class SoundManager 
{
private:

    SoundConfig config;
    bool isPlaying;
    uint32_t playStartTime;
    uint32_t playDuration;
    uint16_t currentPWM;
    
    static const CustomSound soundPresets[];
    
    /// @brief Start playing a sound with given parameters
    /// @param pwmIntensity PWM intensity (0-255)
    /// @param duration Duration in milliseconds
    void startSound(uint16_t pwmIntensity, uint32_t duration);
    
    /// @brief Stop current sound
    void stopSound();

public:

    /// @brief Constructor for SoundManager
    /// @param buzzerPin Pin number for buzzer connection
    SoundManager(uint8_t buzzerPin);
    
    /// @brief Initialize the sound system
    void initialize();
    
    /// @brief Set sound configuration
    /// @param newConfig Configuration structure
    void setConfig(const SoundConfig& newConfig);
    
    /// @brief Get current sound configuration
    /// @return Current configuration structure
    SoundConfig getConfig() const;
    
    /// @brief Play a predefined sound event
    /// @param event Sound event to play
    void playSound(SoundEvent event);
    
    /// @brief Play a custom sound
    /// @param sound Custom sound parameters
    void playCustomSound(const CustomSound& sound);
    
    /// @brief Play sound with direct parameters
    /// @param pwmIntensity PWM intensity (0-255)
    /// @param duration Duration in milliseconds
    void playSound(uint16_t pwmIntensity, uint32_t duration);
    
    /// @brief Update sound system (call in main loop)
    void tick();
    
    /// @brief Check if sound is currently playing
    /// @return true if sound is playing
    bool isCurrentlyPlaying() const;
    
    /// @brief Set volume scale (0-100)
    /// @param volume Volume percentage
    void setVolume(uint8_t volume);
    
    /// @brief Enable or disable sound
    /// @param enabled Whether sound should be enabled
    void setSoundEnabled(bool enabled);
};


#endif // SOUND_H