// -------------------------------------------------------------------------------------------------
// Loads pinmap model for current configuration
#pragma once

#if defined(CONFIG_IDF_TARGET_ESP32S2)
  #include "Pins.Esp32S2.h"
  #define PINMAP_STR "ESP32-S2"

#elif defined(CONFIG_IDF_TARGET_ESP32C3)
  #include "Pins.Esp32C3.h"
  #define PINMAP_STR "ESP32-C3"

#elif defined(ESP32)
  #include "Pins.Esp32.h"
  #define PINMAP_STR "ESP32"

#elif defined(ESP8266)
  #include "Pins.Esp8266.h"
  #define PINMAP_STR "ESP8266"

#elif defined(__M0__)
  #include "Pins.M0.h"
  #define PINMAP_STR "ARDUINO M0"

#elif defined(__TEENSYDUINO__)
  #include "Pins.Teensy.h"
  #define PINMAP_STR "TEENSY"

#endif

// all unassigned pins OFF
#include "Pins.defaults.h"

#if LED_STATUS_ON_STATE == HIGH
  #define LED_STATUS_OFF_STATE LOW
#else
  #define LED_STATUS_OFF_STATE HIGH
#endif
