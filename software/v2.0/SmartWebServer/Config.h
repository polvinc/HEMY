/* ---------------------------------------------------------------------------------------------------------------------------------
 *                                       Configuration for the OnStep SmartWebServer
 *
 *           For more information on setting up the Smart Web Server see https://onstep.groups.io/g/main/wiki/7119
 *                               and join the OnStep Groups.io at https://groups.io/g/onstep
 * 
 *         *** Read the compiler warnings and errors, they are there to help guard against invalid configurations ***
 *
 *      For Wifi successful startup an AP will appear with an default SSID of "ONSTEP" and default password of "password".
 *      After connecting the web-site is at "192.168.0.1" and the cmd channels are at "192.168.0.1:9996 to 9999". If locked
 *      out selecting "Erase Flash: All Flash Contents" from the Arduino IDE Tools menu before uploading/flashing again can
 *      help restore access to the ESP8266 or ESP32; or see Extended.config.h to wipe NV.
 *
 *   For Ethernet DHCP and mDNS are active by default so OnStep can usually be reached at "onstepsws.local" or by checking your
 *   LAN's Router or Access Point to see what IP Address was assigned.
 *   You can also manually configure the SWS network settings and static IP in Extended.config.h so they match your LAN instead.
 *
 * ---------------------------------------------------------------------------------------------------------------------------------
 * ADJUST THE FOLLOWING TO CONFIGURE YOUR CONTROLLER FEATURES ----------------------------------------------------------------------
 *
 * <-Req'd = always must set, <-Often = usually must set, Option = optional, Adjust = adjust as req'd, Infreq = infrequently changed
 */
//      Parameter Name              Value   Default  Notes                                                                      Hint

// =================================================================================================================================
// CONTROLLER ======================================================================================================================

#define HOST_NAME            "HEMY-SWS" // p-SWS", This devices name up to 16 chars (collapses to mDNS name "onstepsws".)   Adjust

// OPERATIONAL MODE ----------------------------------------------------------------------------------------------------------------
#define OPERATIONAL_MODE             WIFI //   WIFI, Or use ETHERNET_W5100 or ETHERNET_W5500                                 <-Req'd

// SERIAL PORTS --------------------------------------------------------------------------------------------------------------------
#define SERIAL_BAUD_DEFAULT          9600 //   9600, Common baud rates for this parameter are 9600,19200,57600,115200,etc.    Infreq
                                          //         The OnStep serial port this is wired to must use the same rate above.
#define SERIAL_BAUD                115200 // 115200, Or use 19200,57600,115200,230400,460800 (not all devices support > 115200)
                                          //         (OnStep and here.)  Automatically uses 19200 if talking to a Mega2560 OnStep.
#define SERIAL_SWAP                  AUTO //   AUTO, Automatic check both, ON for swapped port or OFF for default port only.  Infreq
                                          //         this option is ignored in ETHERNET modes

// BLE GAMEPAD SETTINGS (ESP32 ONLY) ------------------------------------------------ see https://onstep.groups.io/g/main/wiki/26762
#define BLE_GAMEPAD                   OFF //    OFF, ON to allow BLE gamepad connection for ESP32 only.                       Option
#define BLE_GP_ADDR   "ff:ff:de:09:f5:cf" // f5:cf", GamePad MAC address #1                                                   Adjust
                                          //         GamePad device address must match exactly else it will not connect!
                                          //         Replace address with yours, in lower case, with colons as shown.
#define BLE_GP_ADDR1  "ff:ff:ff:ff:ff:ff" // ff:ff", GamePad MAC address #2                                                   Adjust
                                          //         Two GamePads are allowed, operating in a "handoff" mode, one at time.

// STATUS LED ----------------------------------------------------------------------------------------------------------------------
#define LED_STATUS                     ON //     ON, Enable LED flashes while connecting then steady once connected.          Infreq

// DISPLAY -------------------------------------------------------------------------------------------------------------------------
#define DISPLAY_LANGUAGE             L_en //   L_en, English. Or L_ce, L_de, L_en, L_us, L_es two letter country code.        Adjust
#define DISPLAY_WEATHER                ON //     ON, ON ambient conditions in locale default units.                           Option
#define DISPLAY_INTERNAL_TEMPERATURE   ON //    OFF, ON internal MCU temp. in locale default units.                           Option
#define DISPLAY_RESET_CONTROLS         ON //     ON, ON allows reset if supported, FWU for STM32 firmware upload pin HIGH.    Option

#define DISPLAY_SERVO_MONITOR         OFF //    OFF, ON to display the servo monitor for OnStepX (any axis.)                  Option
#define DISPLAY_SERVO_ORIGIN_CONTROLS OFF //    OFF, ON to display control to set the absolute encoder origin for servos.     Option

// ENCODER SUPPORT -----------------------------------------------------------------------------------------------------------------
// Some of these settings are stored in NV (EEPROM) as the default values when first uploaded.  Later changes below may be
// ignored unless NV is wiped or you configure to revert to the defaults again at runtime.
#define ENC_AUTO_SYNC_DEFAULT          ON //     ON, Automatically sync Encoders to OnStep.                                   Option
#define ENC_AUTO_SYNC_MEMORY          OFF //    OFF, ON Remember automatic sync setting across power cycles.                  Option
#define ENC_SYNC_DURING_GOTO          OFF //    OFF, ON high resolution encoders correct pointing even for gotos.             Option

#define AXIS1_ENCODER                 OFF //    OFF, AB, AB_ESP32, CW_CCW, PULSE_DIR, AS37_H39B_B. RA/Azm (A/MA) & (B/SLO.)   Option
#define AXIS1_ENCODER_TICKS_DEG  22.22222 // 22.222, n, (ticks/degree.) Encoder ticks per degree.                             Adjust
#define AXIS1_ENCODER_REVERSE         OFF //    OFF, ON to reverse the count direction.                                       Option
#define AXIS1_ENCODER_DIFF_LIMIT_TO   300 //    300, n, (arcsec.) Minimum diff. between encoder/OnStep for sync. to OnStep.   Adjust
#define AXIS1_ENCODER_DIFF_LIMIT_FROM OFF //    OFF, n, (arcsec.) Maximum diff. between encoder/OnStep for sync. from OnStep. Adjust
                                          //         for absolute encoders.

#define AXIS2_ENCODER                 OFF //    OFF, AB, AB_ESP32, CW_CCW, PULSE_DIR, AS37_H39B_B. Dec/Alt (A/MA) & (B/SLO.)  Option
#define AXIS2_ENCODER_TICKS_DEG  22.22222 // 22.222, n, (ticks/degree.) Encoder ticks per degree.                             Adjust
#define AXIS2_ENCODER_REVERSE         OFF //    OFF, ON to reverse the count direction.                                       Option
#define AXIS2_ENCODER_DIFF_LIMIT_TO   300 //    300, n, (arcsec.) Minimum diff. between encoder/OnStep for sync. to OnStep.   Adjust
#define AXIS2_ENCODER_DIFF_LIMIT_FROM OFF //    OFF, n, (arcsec.) Maximum diff. between encoder/OnStep for sync. from OnStep. Adjust
                                          //         for absolute encoders.

// THAT'S IT FOR USER CONFIGURATION!
// -------------------------------------------------------------------------------
#include "Extended.config.h"
