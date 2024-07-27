#include <Arduino.h>
#include <FastLED.h>

#define LED_PIN 6
#define NUM_LEDS 56 // 14 LEDs per display, 2 displays per side, 2 sides

#define DEFAULT_BRIGTHNESS 50

// Función para mostrar un número en los displays de 7 segmentos
void showNumber(int number, bool isLeft);

// Función para controlar un dígito individual
void showDigit(int digit, int startIdx);

CRGB leds[NUM_LEDS];

void setup()
{
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(DEFAULT_BRIGTHNESS);
}

void loop()
{
    showDigit(0, 0);
    delay(1000);
    showDigit(1, 0);
    delay(1000);
    showDigit(2, 0);
    delay(1000);
    showDigit(3, 0);
    delay(1000);
    showDigit(4, 0);
    delay(1000);
    showDigit(5, 0);
    delay(1000);
    showDigit(6, 0);
    delay(1000);
    showDigit(7, 0);
    delay(1000);
    showDigit(8, 0);
    delay(1000);
    showDigit(9, 0);
    delay(3000);
}

void showNumber(int number, bool isLeft)
{
    int digit1 = number / 10; // Dígito de las decenas
    int digit2 = number % 10; // Dígito de las unidades

    if (isLeft)
    {
        showDigit(digit1, 0);  // Mostrar el primer dígito en el primer display del lado izquierdo
        showDigit(digit2, 14); // Mostrar el segundo dígito en el segundo display del lado izquierdo
    }
    else
    {
        showDigit(digit1, 28); // Mostrar el primer dígito en el primer display del lado derecho
        showDigit(digit2, 42); // Mostrar el segundo dígito en el segundo display del lado derecho
    }

    FastLED.show();
}

void showDigit(int digit, int startIdx)
{
    int l_on = 0;
    int mask = B10000000;
    // Aquí se definiría la lógica para prender los segmentos correspondientes al dígito
    // Por ejemplo, para el número 1, solo se encenderían los segmentos B y C.

    // Encender los LEDs correspondientes al dígito
    // Esta es una simplificación. Deberías expandir esta lógica basándote en la representación de cada número.
    switch (digit)
    {
    case 0:               // 0,1,2,3,4,5,6,7,8,9,10,11,12
        l_on = B11111100; // Como son 7 disp, pero 8 bits, el LSB no interesa, por convencio lo dejamos como 0.
        break;
    case 1:               // 2,3
        l_on = B01100000; // Como son 7 disp, pero 8 bits, el LSB no interesa, por convencio lo dejamos como 0.
        break;
    case 2:               // 0,1,2,3,4,5,6,7,8,9,10,11,12
        l_on = B11011010; // Como son 7 disp, pero 8 bits, el LSB no interesa, por convencio lo dejamos como 0.
        break;
    case 3:               // 0,1,2,3,4,5,6,7,8,9,10,11,12
        l_on = B11110010; // Como son 7 disp, pero 8 bits, el LSB no interesa, por convencio lo dejamos como 0.
        break;
    case 4:               // 0,1,2,3,4,5,6,7,8,9,10,11,12
        l_on = B01100110; // Como son 7 disp, pero 8 bits, el LSB no interesa, por convencio lo dejamos como 0.
        break;
    case 5:               // 0,1,2,3,4,5,6,7,8,9,10,11,12
        l_on = B10110110; // Como son 7 disp, pero 8 bits, el LSB no interesa, por convencio lo dejamos como 0.
        break;
    case 6:               // 0,1,2,3,4,5,6,7,8,9,10,11,12
        l_on = B11111010; // Como son 7 disp, pero 8 bits, el LSB no interesa, por convencio lo dejamos como 0.
        break;
    case 7:               // 0,1,2,3,4,5,6,7,8,9,10,11,12
        l_on = B11100000; // Como son 7 disp, pero 8 bits, el LSB no interesa, por convencio lo dejamos como 0.
        break;
    case 8:               // 0,1,2,3,4,5,6,7,8,9,10,11,12
        l_on = B11111110; // Como son 7 disp, pero 8 bits, el LSB no interesa, por convencio lo dejamos como 0.
        break;
    case 9:               // 0,1,2,3,4,5,6,7,8,9,10,11,12
        l_on = B11100110; // Como son 7 disp, pero 8 bits, el LSB no interesa, por convencio lo dejamos como 0.
        break;
    }
    // Limpiar los LEDs del display antes de mostrar el nuevo número
    for (int i = 0; i < 7; i++)
    {
        if ((mask & l_on) > 0)
        {
            leds[startIdx + i * 2] = CRGB::Red;
            leds[startIdx + i * 2 + 1] = CRGB::Red;
            FastLED.show();
        }
        else
        {
            leds[startIdx + i * 2] = CRGB::Black;
            leds[startIdx + i * 2 + 1] = CRGB::Black;
            FastLED.show();
        }

        mask = mask >> 1;
    }
}