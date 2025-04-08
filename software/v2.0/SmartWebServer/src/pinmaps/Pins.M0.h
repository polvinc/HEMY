// -------------------------------------------------------------------------------------------------
// Pin map for Arduino M0
#pragma once

#ifndef SERIAL_ONSTEP
  #define SERIAL_ONSTEP Serial1
#endif

#ifndef LED_STATUS_PIN
  #define LED_STATUS_PIN     13  // pin 13 is probably the status led, a guess
#endif

#define BOOT0_PIN            10  // pin 10 to Boot0 of STM32 etc.

#define ETHERNET_RESET_PIN   9   // pin# for controlling the reset of W5500 so it comes up properly

#define AXIS1_ENCODER_A_PIN  5   // pin# for Axis1 encoder, for A or CW  or MA/CLK
#define AXIS1_ENCODER_B_PIN  6   // pin# for Axis1 encoder, for B or CCW or SLO
#define AXIS2_ENCODER_A_PIN  7   // pin# for Axis2 encoder, for A or CW  or MA/CLK
#define AXIS2_ENCODER_B_PIN  8   // pin# for Axis2 encoder, for B or CCW or SLO

#if AXIS1_ENCODER == OFF || AXIS2_ENCODER == OFF
  #define VGPIO_PIN_0        AXIS1_ENCODER_A_PIN
  #define VGPIO_PIN_1        AXIS1_ENCODER_B_PIN
  #define VGPIO_PIN_2        AXIS2_ENCODER_A_PIN
  #define VGPIO_PIN_3        AXIS2_ENCODER_B_PIN
#endif

#define SD_CARD_CS_PIN       4   // pin 4 to CS for SD adapter on Arduino Ethernet Shield
