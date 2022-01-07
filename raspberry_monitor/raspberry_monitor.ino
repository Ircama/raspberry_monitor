/*
 * Used Libraries:
 * https://github.com/notisrac/SH1106Lib (No buffer SH1106 OLED display driver)
 * https://playground.arduino.cc/Main/SoftwareI2CLibrary/ (required library of SH1106Lib - I2C Master)
 * https://github.com/notisrac/TinyPrint (required library of SH1106Lib)
 */

// Global configuration
#define LED_ACT_PIN 3
#define RESET_PIN   4
#define MAX_LED_ACT_FROZEN_PERIOD 120000 // milliseconds

#define USE_SH1106 1
#define USE_INTERNAL_LED 1
#define ENABLE_BOOT_MONITOR 1

/*-------------------------------------------------------------------------------------------------------*/

// softwareI2C lib
#define I2C_HARDWARE 1
#define I2C_TIMEOUT 10
#define I2C_MAXWAIT 10
#define I2C_PULLUP 1
#define I2C_FASTMODE 1
#define SDA_PORT PORTC
#define SDA_PIN 4 // = A4
#define SCL_PORT PORTC
#define SCL_PIN 5 // = A5

#if defined USE_SH1106
// tinyprint lib
#define TP_PRINTLINES 0
#define TP_FLASHSTRINGHELPER 1
#define TP_NUMBERS 1
#define TP_FLOAT 1
#define TP_WINDOWSLINEENDS 0

#include "SH1106Lib.h"
#include "glcdfont.h" // FONT5X7_H https://github.com/adafruit/Adafruit-GFX-Library/blob/master/glcdfont.c
#include <TinyPrint.h>
SH1106Lib display;
#endif

#ifdef ENABLE_BOOT_MONITOR
unsigned long last_led_act_changed = millis();
#endif

/*-------------------------------------------------------------------------------------------------------*/
void setup() {
  #if defined USE_SH1106
  display.initialize();
  display.clearDisplay();
  display.setFont(font, 5, 7);
  display.setTextWrap(true);
  display.setTextColor(WHITE, TRANSPARENT);
  display.clearDisplay();
  display.setCursor(0, 4);
  display.print("System powering");
  display.setCursor(0, 24);
  display.print("on now...");
  display.setCursor(0, 44);
  display.print("PLEASE WAIT...");
  #endif // USE_SH1106

  #ifdef USE_INTERNAL_LED
  pinMode(LED_BUILTIN, OUTPUT);
  #endif

  #ifdef ENABLE_BOOT_MONITOR
  pinMode(LED_ACT_PIN, INPUT);    // sets the digital pin 7 as input
  pinMode(RESET_PIN, INPUT_PULLUP); // add a pull-up resistor via software to this pin
  attachInterrupt(digitalPinToInterrupt(LED_ACT_PIN), updateChangeFlagVector, CHANGE);
  #endif
} // setup

void updateChangeFlagVector() {
  last_led_act_changed = millis();
}

#ifdef USE_INTERNAL_LED
bool blink = 0;
#endif

void led_act_watchdog() { // Check system activity
  #ifdef USE_INTERNAL_LED
  if (blink=!blink)
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  else
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  #endif

  #ifdef ENABLE_BOOT_MONITOR
  if (millis() - last_led_act_changed > MAX_LED_ACT_FROZEN_PERIOD) { // Drives a reset

    #if defined USE_SH1106
    // writes the splash page
    display.clearDisplay();
    display.setCursor(0, 4);
    display.print("System restarting");
    display.setCursor(0, 24);
    display.print("after no activity");
    display.setCursor(0, 44);
    display.print("PLEASE WAIT...");
    #endif // USE_SH1106

    #ifdef USE_INTERNAL_LED
    for (int i=0;i<10;i++) { // quickly blinks the LED 10 times
      digitalWrite(LED_BUILTIN, HIGH);
      delay(50);
      digitalWrite(LED_BUILTIN, LOW);
      delay(50);
    }
    #endif

    // to drive a reset, set the RESET_PIN to low for 1 second
    pinMode(RESET_PIN, OUTPUT);
    digitalWrite(RESET_PIN, LOW);
    delay(1000);
    digitalWrite(RESET_PIN, HIGH);
    pinMode(RESET_PIN, INPUT_PULLUP);

    last_led_act_changed = millis();

  }
  #endif
}

/*-------------------------------------------------------------------------------------------------------*/
void loop() {
  led_act_watchdog();
  delay(1000);
}
