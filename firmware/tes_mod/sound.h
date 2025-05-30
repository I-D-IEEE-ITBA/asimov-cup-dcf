/** *************************************************************************************
   
    * @file        sound.h
    * @brief       Sound management system for button feedback and game events
    * @author      IEEE-ITBA | I+D | S. Lopez, A. Heir, J. Torino, I. Dib, J. Pérez
    * @date        2025-05-31
    * @version     1.0

    ************************************************************************************* */


/* **************************************************************************************
    * INCLUDES AND CONFIGURATION *
   ************************************************************************************** */

#ifndef SOUND_H
#define SOUND_H

#include <Arduino.h>
#include <cstdint>


/* **************************************************************************************
    * CONSTANTS AND DEFINITIONS *
   ************************************************************************************** */

//* SOUND PRESET PARAMETERS

#define SHORT_PRESS_PWM 160            // PWM intensity for short button press
#define LONG_PRESS_PWM 250             // PWM intensity for long button press  
#define DOUBLE_PRESS_PWM 255           // PWM intensity for double button press

#define SHORT_PRESS_MS 150             // Duration for short button press sound
#define LONG_PRESS_MS 250              // Duration for long button press sound
#define DOUBLE_PRESS_MS 750            // Duration for double button press sound


/* **************************************************************************************
    * DATA TYPES AND STRUCTURES *
   ************************************************************************************** */

/**
 * @brief Enumeration of available sound events
 */
enum SoundEvent 
{
    SOUND_NONE,                        // No sound/silence
    SOUND_SHORT_PRESS,                 // Short button press feedback
    SOUND_LONG_PRESS,                  // Long button press feedback
    SOUND_DOUBLE_PRESS,                // Double button press feedback
    SOUND_ERROR,                       // Error notification sound
    SOUND_SUCCESS,                     // Success notification sound
    SOUND_CUSTOM                       // Custom user-defined sound
};

/**
 * @brief Configuration structure for sound system settings
 */
struct SoundConfig 
{
    uint8_t buzzerPin;                 // Pin number for buzzer connection
    bool soundEnabled;                 // Enable/disable sound output
    uint8_t volumeScale;               // Volume scaling percentage (0-100)
};

/**
 * @brief Structure for custom sound parameters
 */
struct CustomSound 
{
    uint16_t pwmIntensity;             // PWM intensity value (0-255)
    uint32_t duration;                 // Sound duration in milliseconds
    uint16_t frequency;                // Sound frequency (reserved for future use)
};


/* **************************************************************************************
    * CLASS DECLARATIONS *
   ************************************************************************************** */

/**
 * @brief Manager class for sound output and buzzer control
 * 
 * This class provides complete control over sound feedback including
 * predefined sound events, custom sounds, and volume management.
 */
class SoundManager 
{
private:

    SoundConfig config;                             // Current sound configuration
    bool isPlaying;                                 // Current playback state flag
    uint32_t playStartTime;                         // Playback start timestamp
    uint32_t playDuration;                          // Current sound duration
    uint16_t currentPWM;                            // Current PWM intensity value
    
    // Predefined sound configurations array
    static const CustomSound soundPresets[];
    
    /**
     * @brief Start playing a sound with specified parameters
     * @param pwmIntensity PWM intensity value (0-255)
     * @param duration Sound duration in milliseconds
     */
    void startSound(uint16_t pwmIntensity, uint32_t duration);
    
    /**
     * @brief Stop current sound playback immediately
     */
    void stopSound();

public:

    /**
     * @brief Constructor for SoundManager
     * @param buzzerPin Pin number for buzzer connection
     */
    explicit SoundManager(uint8_t buzzerPin);
    
    /**
     * @brief Initialize the sound system with default settings
     */
    void initialize();
    
    /**
     * @brief Set new sound system configuration
     * @param newConfig New configuration structure to apply
     */
    void setConfig(const SoundConfig& newConfig);
    
    /**
     * @brief Get current sound system configuration
     * @return Copy of current configuration structure
     */
    SoundConfig getConfig() const;
    
    /**
     * @brief Play a predefined sound event
     * @param event Sound event type to play
     */
    void playSound(SoundEvent event);
    
    /**
     * @brief Play a custom sound with specified parameters
     * @param sound Custom sound parameter structure
     */
    void playCustomSound(const CustomSound& sound);
    
    /**
     * @brief Play sound with direct parameter specification
     * @param pwmIntensity PWM intensity value (0-255)
     * @param duration Sound duration in milliseconds
     */
    void playSound(uint16_t pwmIntensity, uint32_t duration);
    
    /**
     * @brief Update sound system state (call in main loop)
     */
    void tick();
    
    /**
     * @brief Check if sound is currently being played
     * @return true if sound is playing, false otherwise
     */
    bool isCurrentlyPlaying() const;

    /**
     * @brief Enable or disable sound output
     * @param enabled true to enable sound, false to disable
     */
    void setSoundEnabled(bool enabled);
    
    /**
     * @brief Set volume scaling percentage
     * @param volume Volume percentage (0-100)
     */
    void setVolume(uint8_t volume);
};

#endif // SOUND_H