

/**
 * @file main.cpp
 * @brief Este archivo contiene el código principal del firmware para el proyecto Asimov Cup DCF.
 *
 * El firmware controla la funcionalidad de los botones, los displays de 7 segmentos y los LEDs RGB.
 * Los botones permiten incrementar o decrementar contadores, mientras que los displays muestran los valores actuales.
 * Los LEDs RGB se utilizan para mostrar dígitos en los displays.
 *
 * @author IEEE-ITBA | I+D
 * @date 2024-07
 */

#include <Arduino.h>
#include <FastLED.h>
#include <string.h>

// Definiciones de depuración
#define DEBUG

#ifdef DEBUG
#ifndef BAUD_RATE
#define BAUD_RATE 115200
#endif // !BAUD_RATE
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#endif // DEBUG

// Definiciones de pines
#define PIN_RGB_DATA 3
#define PIN_BUZZER 5
#define PIN_SW0 7
#define PIN_SW1 8
#define PIN_SW2 9
#define PIN_LED_BUILTIN LED_BUILTIN
#define PIN_MISC_LED 6

// Definiciones de botones
#define BUTTON_P1 PIN_SW0
#define BUTTON_P2 PIN_SW1
#define BUTTON_PRESSED LOW

// Definiciones de LEDs
#define NUM_LEDS 56 // 14 LEDs por dígito, 2 displays por lado, 2 lados
#define DEFAULT_BRIGTHNESS 50

// Definiciones de tiempos
#define DEBOUNCE_DELAY_MS 50
#define LONG_PRESS_DELAY_MS 1000
#define BOTH_PRESS_WINDOW_MS 500 // Ventana de tiempo para detectar ambos botones presionados
#define UPDATE_RATE 100

// Enumeración de eventos de botones
enum ButtonEvents
{
    NONE,
    SHORT_PRESS_P1,
    SHORT_PRESS_P2,
    LONG_PRESS_P1,
    LONG_PRESS_P2,
    BOTH_LONG_PRESS
};

// Declaraciones de funciones -------------------------------------------------

// Actualiza los displays de 7 segmentos con los valores actuales
void updateDisplays();

// Procesa el estado de los botones y devuelve el evento correspondiente
ButtonEvents processButtons();

// Muestra un número en los displays de 7 segmentos
bool showNumber(int number, bool isLeft);

// Muestra un dígito en una posición específica del array de LEDs
bool showDigit(int digit, int startIdx, CRGB color = CRGB::BlueViolet);

// ------------------------------------------------- Declaraciones de funciones

// Variables globales
CRGB leds[NUM_LEDS];
int count_p1 = 0;
int count_p2 = 0;

void setup()
{
#ifdef DEBUG
    Serial.begin(BAUD_RATE);
#endif

    // Inicialización de los módulos RGB
    FastLED.addLeds<WS2812B, PIN_RGB_DATA, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(DEFAULT_BRIGTHNESS);

    // Configuración de los pines de los botones como entradas
    pinMode(PIN_SW0, INPUT_PULLUP);
    pinMode(PIN_SW1, INPUT_PULLUP);
    pinMode(PIN_SW2, INPUT_PULLUP);

    // Configuración de otros pines
    digitalWrite(PIN_MISC_LED, HIGH);
    digitalWrite(PIN_LED_BUILTIN, LOW);
    pinMode(PIN_BUZZER, OUTPUT);
    pinMode(PIN_MISC_LED, OUTPUT);
    pinMode(PIN_LED_BUILTIN, OUTPUT);

    delay(10);

    CRGB temp[NUM_LEDS];
    memset(temp, CRGB::Black, sizeof(temp));
    memcpy(leds, temp, sizeof(leds));
    FastLED.show();
    
    delay(10);

    updateDisplays();

    delay(10);
}

void loop()
{
    ButtonEvents event = processButtons();

    switch (event)
    {
    case SHORT_PRESS_P1:
        DEBUG_PRINTLN("Pulsación corta en P1");
        count_p1++;
        count_p1 = (count_p1 > 99) ? 99 : count_p1;
        analogWrite(PIN_BUZZER, 126);
        break;

    case LONG_PRESS_P1:
        DEBUG_PRINTLN("Pulsación larga en P1");
        count_p1--;
        count_p1 = (count_p1 < 0) ? 0 : count_p1;
        break;

    case SHORT_PRESS_P2:
        DEBUG_PRINTLN("Pulsación corta en P2");
        count_p2++;
        count_p2 = (count_p2 > 99) ? 99 : count_p2;
        break;

    case LONG_PRESS_P2:
        DEBUG_PRINTLN("Pulsación larga en P2");
        count_p2--;
        count_p2 = (count_p2 < 0) ? 0 : count_p2;
        break;

    case BOTH_LONG_PRESS:
        DEBUG_PRINTLN("Ambos botones presionados por mucho tiempo");
        count_p1 = 0;
        count_p2 = 0;
        break;

    default:
        break;
    }

    if (event != NONE)
    {
        DEBUG_PRINT("Contador P1: ");
        DEBUG_PRINT(count_p1);
        DEBUG_PRINT(" | Contador P2: ");
        DEBUG_PRINTLN(count_p2);

        updateDisplays();
    }
}

void updateDisplays()
{
    showNumber(count_p1, true);
    showNumber(count_p2, false);
}

ButtonEvents processButtons()
{
    static unsigned long lastDebounceTime_p1 = 0;
    static unsigned long lastDebounceTime_p2 = 0;
    static unsigned long pressStartTime_p1 = 0;
    static unsigned long pressStartTime_p2 = 0;
    static bool buttonPressed_p1 = false;
    static bool buttonPressed_p2 = false;
    static bool reading_p1 = !BUTTON_PRESSED;
    static bool reading_p2 = !BUTTON_PRESSED;
    static bool previous_reading_p1 = !BUTTON_PRESSED;
    static bool previous_reading_p2 = !BUTTON_PRESSED;

    bool current_reading_p1 = digitalRead(BUTTON_P1);
    bool current_reading_p2 = digitalRead(BUTTON_P2);

    // sleep interval to allow proper press detection
    delay(UPDATE_RATE);

    // Check P1 short press
    if (current_reading_p1 != previous_reading_p1 && millis() - lastDebounceTime_p1 > DEBOUNCE_DELAY_MS)
    {
        lastDebounceTime_p1 = millis();
        reading_p1 = current_reading_p1;

        if (reading_p1 == BUTTON_PRESSED)
        {
            pressStartTime_p1 = millis(); // start counting time
            buttonPressed_p1 = true;
        }
        else // was short press!
        {
            buttonPressed_p1 = false;
            previous_reading_p1 = current_reading_p1;

            if (millis() - pressStartTime_p1 < LONG_PRESS_DELAY_MS)
            {
                return SHORT_PRESS_P1;
            }
        }
    }

    // Check P2 short press
    if (current_reading_p2 != previous_reading_p2 && millis() - lastDebounceTime_p2 > DEBOUNCE_DELAY_MS)
    {
        lastDebounceTime_p2 = millis();
        reading_p2 = current_reading_p2;

        if (reading_p2 == BUTTON_PRESSED)
        {
            pressStartTime_p2 = millis(); // start counting time
            buttonPressed_p2 = true;
        }
        else // was short press!
        {
            buttonPressed_p2 = false;
            previous_reading_p2 = current_reading_p2;

            if (millis() - pressStartTime_p2 < LONG_PRESS_DELAY_MS)
            {
                return SHORT_PRESS_P2;
            }
        }
    }

    // Check both buttons long press
    if (buttonPressed_p1 && buttonPressed_p2 &&
        (abs(pressStartTime_p1 - pressStartTime_p2) < BOTH_PRESS_WINDOW_MS) &&
        (millis() - pressStartTime_p1) > LONG_PRESS_DELAY_MS &&
        (millis() - pressStartTime_p2) > LONG_PRESS_DELAY_MS)
    {
        buttonPressed_p1 = false;
        buttonPressed_p2 = false;
        return BOTH_LONG_PRESS;
    }

    // Check P1 long press
    if (buttonPressed_p1 && !buttonPressed_p2 && ((millis() - pressStartTime_p1) > LONG_PRESS_DELAY_MS))
    {
        buttonPressed_p1 = false;
        return LONG_PRESS_P1;
    }

    // Check P2 long press
    if (buttonPressed_p2 && !buttonPressed_p1 && ((millis() - pressStartTime_p2) > LONG_PRESS_DELAY_MS))
    {
        buttonPressed_p2 = false;
        return LONG_PRESS_P2;
    }

    previous_reading_p1 = current_reading_p1;
    previous_reading_p2 = current_reading_p2;

    return NONE;
}

bool showNumber(int number, bool isLeft)
{
    if (number < 0 || number > 99)
    {
        return false;
    }

    int digit1 = number / 10; // Dígito de las decenas
    int digit2 = number % 10; // Dígito de las unidades

    if (isLeft)
    {
        showDigit(digit1, 0, CRGB::OrangeRed);  // Mostrar el primer dígito en el primer display del lado izquierdo
        showDigit(digit2, 14, CRGB::OrangeRed); // Mostrar el segundo dígito en el segundo display del lado izquierdo
    }
    else
    {
        showDigit(digit1, 28, CRGB::BlueViolet); // Mostrar el primer dígito en el primer display del lado derecho
        showDigit(digit2, 42, CRGB::BlueViolet); // Mostrar el segundo dígito en el segundo display del lado derecho
    }

    return true;
}

bool showDigit(int digit, int startIdx, CRGB color)
{
    static const uint8_t segmentMap[] = {
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

    if (digit < 0 || digit > 9 || startIdx < 0 || startIdx >= NUM_LEDS)
    {
        return false;
    }

    uint8_t mask = B10000000;
    for (int i = 0; i < 7; i++)
    {
        if (segmentMap[digit] & mask)
        {
            leds[startIdx + i * 2] = color;
            leds[startIdx + i * 2 + 1] = color;
        }
        else
        {
            leds[startIdx + i * 2] = CRGB::Black;
            leds[startIdx + i * 2 + 1] = CRGB::Black;
        }

        mask >>= 1;
    }

    FastLED.show();

    return true;
}