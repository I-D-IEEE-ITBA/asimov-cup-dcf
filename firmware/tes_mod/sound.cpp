/** *************************************************************************************
   
    * @file        sound.cpp
    * @brief       Implementation of sound management system for button feedback and 
                   game events
    * @author      IEEE-ITBA | I+D | S. Lopez, A. Heir, J. Torino, I. Dib, J. Pérez
    * @date        2025-05-31
    * @version     1.0

    ************************************************************************************* */


/* **************************************************************************************
    * INCLUDES AND CONFIGURATION *
   ************************************************************************************** */

#include "sound.h"


/* **************************************************************************************
    * CONSTANTS AND STATIC DATA *
   ************************************************************************************** */

const CustomSound SoundManager::soundPresets[] = 
{
    {0, 0, 0},                                  // SOUND_NONE
    {SHORT_PRESS_PWM, SHORT_PRESS_MS, 0},       // SOUND_SHORT_PRESS
    {LONG_PRESS_PWM, LONG_PRESS_MS, 0},         // SOUND_LONG_PRESS
    {DOUBLE_PRESS_PWM, DOUBLE_PRESS_MS, 0},     // SOUND_DOUBLE_PRESS
    {200, 100, 0},                              // SOUND_ERROR
    {180, 300, 0},                              // SOUND_SUCCESS  
    {0, 0, 0}                                   // SOUND_CUSTOM (placeholder)
};


/* **************************************************************************************
    * CLASS IMPLEMENTATION *
   ************************************************************************************** */

//* CONSTRUCTOR AND INITIALIZATION

SoundManager::SoundManager(uint8_t buzzerPin) 
{
    // Initialize configuration with default values
    config.buzzerPin = buzzerPin;
    config.soundEnabled = true;
    config.volumeScale = 100;
    
    // Initialize state variables
    isPlaying = false;
    playStartTime = 0;
    playDuration = 0;
    currentPWM = 0;
}

void SoundManager::initialize() 
{
    // Configure buzzer pin as output
    pinMode(config.buzzerPin, OUTPUT);
    
    // Ensure buzzer starts silent
    analogWrite(config.buzzerPin, 0);
    
    // Reset playing state
    isPlaying = false;
    playStartTime = 0;
    playDuration = 0;
    currentPWM = 0;
}


//* CONFIGURATION SETUP

void SoundManager::setConfig(const SoundConfig& newConfig) 
{
    config = newConfig;
    
    // Reconfigure pin if it changed
    pinMode(config.buzzerPin, OUTPUT);
    
    // If sound is disabled, stop current playback
    if (!config.soundEnabled) 
    {
        stopSound();
    }
}

SoundConfig SoundManager::getConfig() const 
{
    return config;
}


//* SOUND PLAYBACK

void SoundManager::playSound(SoundEvent event) 
{
    // Check if sound is enabled and event is valid
    if (!config.soundEnabled || event == SOUND_NONE) 
    {
        return;
    }
    
    // Validate event range
    if (event >= sizeof(soundPresets) / sizeof(soundPresets[0])) 
    {
        return;
    }
    
    // Get preset parameters and start sound
    const CustomSound& preset = soundPresets[event];
    startSound(preset.pwmIntensity, preset.duration);
}

void SoundManager::playCustomSound(const CustomSound& sound) 
{
    if (!config.soundEnabled) 
    {
        return;
    }
    
    startSound(sound.pwmIntensity, sound.duration);
}

void SoundManager::playSound(uint16_t pwmIntensity, uint32_t duration) 
{
    if (!config.soundEnabled) 
    {
        return;
    }
    
    startSound(pwmIntensity, duration);
}


//* SOUND CONTROL

void SoundManager::tick() 
{
    // Check if currently playing
    if (!isPlaying) 
    {
        return;
    }
    
    // Check if sound duration has elapsed
    if ((millis() - playStartTime) >= playDuration) 
    {
        stopSound();
    }
}

bool SoundManager::isCurrentlyPlaying() const 
{
    return isPlaying;
}

void SoundManager::startSound(uint16_t pwmIntensity, uint32_t duration) 
{
    // Stop any currently playing sound
    if (isPlaying) 
    {
        stopSound();
    }
    
    // Apply volume scaling
    currentPWM = pwmIntensity;
    uint16_t scaledPWM = (pwmIntensity * config.volumeScale) / 100;
    
    // Start new sound
    isPlaying = true;
    playStartTime = millis();
    playDuration = duration;
    
    // Start PWM output
    analogWrite(config.buzzerPin, scaledPWM);
}

void SoundManager::setSoundEnabled(bool enabled) 
{
    config.soundEnabled = enabled;
    
    // If disabling sound while playing, stop immediately
    if (!enabled && isPlaying) 
    {
        stopSound();
    }
}

void SoundManager::stopSound() 
{
    // Reset state variables
    isPlaying = false;
    playStartTime = 0;
    playDuration = 0;
    currentPWM = 0;
    
    // Turn off buzzer
    analogWrite(config.buzzerPin, 0);
}


//* VOLUME SETTINGS

void SoundManager::setVolume(uint8_t volume) 
{
    // Clamp volume to valid range
    config.volumeScale = (volume > 100) ? 100 : volume;
    
    // If currently playing, update PWM intensity
    if (isPlaying) 
    {
        uint16_t scaledPWM = (currentPWM * config.volumeScale) / 100;
        analogWrite(config.buzzerPin, scaledPWM);
    }
}
