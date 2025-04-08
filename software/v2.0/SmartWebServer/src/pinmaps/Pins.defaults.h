// -------------------------------------------------------------------------------------------------
// Default pin assignments
#pragma once

// serial interface defaults
#ifndef SERIAL_RX
#define SERIAL_RX            OFF
#endif
#ifndef SERIAL_TX
#define SERIAL_TX            OFF
#endif
#ifndef SERIAL_SWAPPED_RX
#define SERIAL_SWAPPED_RX    OFF
#endif
#ifndef SERIAL_SWAPPED_TX
#define SERIAL_SWAPPED_TX    OFF
#endif

// status indication
#ifndef LED_STATUS_PIN      
#define LED_STATUS_PIN       OFF
#endif

// pin to force OnStep MCU into upload mode on reset
#ifndef BOOT0_PIN
#define BOOT0_PIN            OFF
#endif

// ethernet control
#ifndef ETHERNET_CS_PIN
#define ETHERNET_CS_PIN      OFF
#endif
#ifndef ETHERNET_RESET_PIN
#define ETHERNET_RESET_PIN   OFF
#endif
#ifndef SD_CARD_CS_PIN
#define SD_CARD_CS_PIN       OFF
#endif

// encoders
#ifndef AXIS1_ENCODER_A_PIN
#define AXIS1_ENCODER_A_PIN  OFF
#endif
#ifndef AXIS1_ENCODER_B_PIN
#define AXIS1_ENCODER_B_PIN  OFF
#endif
#ifndef AXIS2_ENCODER_A_PIN
#define AXIS2_ENCODER_A_PIN  OFF
#endif
#ifndef AXIS2_ENCODER_B_PIN
#define AXIS2_ENCODER_B_PIN  OFF
#endif

// virtual GPIO control
#ifndef VGPIO_PIN_0
#define VGPIO_PIN_0          OFF
#endif
#ifndef VGPIO_PIN_1
#define VGPIO_PIN_1          OFF
#endif
#ifndef VGPIO_PIN_2
#define VGPIO_PIN_2          OFF
#endif
#ifndef VGPIO_PIN_3
#define VGPIO_PIN_3          OFF
#endif
#ifndef VGPIO_PIN_4
#define VGPIO_PIN_4          OFF
#endif
#ifndef VGPIO_PIN_5
#define VGPIO_PIN_5          OFF
#endif
#ifndef VGPIO_PIN_6
#define VGPIO_PIN_6          OFF
#endif
#ifndef VGPIO_PIN_7
#define VGPIO_PIN_7          OFF
#endif
