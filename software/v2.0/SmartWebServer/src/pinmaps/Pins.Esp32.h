// -------------------------------------------------------------------------------------------------
// Pin map for ESP32
#pragma once

// locate WeMos D1 R32 serial port pins to match WeMos D1 Mini main and swapped port pins
#ifndef SERIAL_ONSTEP
  #define SERIAL_ONSTEP Serial1
#endif
#ifndef SERIAL_RX
  #define SERIAL_RX          1
#endif
#ifndef SERIAL_TX
  #define SERIAL_TX          3
#endif
#ifndef SERIAL_SWAPPED_RX
  #define SERIAL_SWAPPED_RX  23
#endif
#ifndef SERIAL_SWAPPED_TX
  #define SERIAL_SWAPPED_TX  5
#endif

#ifndef LED_STATUS_PIN      
  #define LED_STATUS_PIN     2   // pin 2 is a guess and is probably wrong
#endif

#define BOOT0_PIN            5   // GPIO5 to Boot0 of STM32 etc. (no swapped serial if active)

#define ETHERNET_CS_PIN      27  // pin# for W5500 chip select
#define ETHERNET_RESET_PIN   26  // pin# for controlling the reset of W5500 so it comes up properly

#define AXIS1_ENCODER_A_PIN  18  // pin# for Axis1 encoder, for A or CW  or MA/CLK
#define AXIS1_ENCODER_B_PIN  19  // pin# for Axis1 encoder, for B or CCW or SLO
#define AXIS2_ENCODER_A_PIN  22  // pin# for Axis2 encoder, for A or CW  or MA/CLK
#define AXIS2_ENCODER_B_PIN  21  // pin# for Axis2 encoder, for B or CCW or SLO

#if AXIS1_ENCODER == OFF || AXIS2_ENCODER == OFF
  #define VGPIO_PIN_0        AXIS1_ENCODER_A_PIN
  #define VGPIO_PIN_1        AXIS1_ENCODER_B_PIN
  #define VGPIO_PIN_2        AXIS2_ENCODER_A_PIN
  #define VGPIO_PIN_3        AXIS2_ENCODER_B_PIN
#endif
