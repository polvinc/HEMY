// -------------------------------------------------------------------------------------------------
// Pin map for ESP32S2
#pragma once

// locate ESP32-S2 serial port pins
#ifndef SERIAL_ONSTEP
  #define SERIAL_ONSTEP Serial1
#endif
#ifndef SERIAL_RX
  #define SERIAL_RX          11
#endif
#ifndef SERIAL_TX
  #define SERIAL_TX          12
#endif
#ifndef SERIAL_SWAPPED_RX
  #define SERIAL_SWAPPED_RX  12
#endif
#ifndef SERIAL_SWAPPED_TX
  #define SERIAL_SWAPPED_TX  11
#endif

#ifndef LED_STATUS_PIN      
  #define LED_STATUS_PIN     15  // Change in config.h if needed
#endif

#define BOOT0_PIN            5   // GPIO5 to Boot0 of STM32 etc. (no swapped serial if active)

#define ETHERNET_RESET_PIN   9   // pin# for controlling the reset of W5500 so it comes up properly
#define SD_CARD_CS_PIN      OFF  // pin 4 to CS for SD adapter on Arduino Ethernet Shield

#define AXIS1_ENCODER_A_PIN  7   // pin# for Axis1 encoder, for A or CW  or MA/CLK
#define AXIS1_ENCODER_B_PIN  9   // pin# for Axis1 encoder, for B or CCW or SLO
#define AXIS2_ENCODER_A_PIN  35  // pin# for Axis1 encoder, for A or CW  or MA/CLK
#define AXIS2_ENCODER_B_PIN  33  // pin# for Axis1 encoder, for B or CCW or SLO

#if AXIS1_ENCODER == OFF || AXIS2_ENCODER == OFF
  #define VGPIO_PIN_0        AXIS1_ENCODER_A_PIN
  #define VGPIO_PIN_1        AXIS1_ENCODER_B_PIN
  #define VGPIO_PIN_2        AXIS2_ENCODER_A_PIN
  #define VGPIO_PIN_3        AXIS2_ENCODER_B_PIN
#endif
