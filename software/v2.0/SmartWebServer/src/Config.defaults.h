// -----------------------------------------------------------------------------------
// Configuration defaults
#pragma once

// host name for this microcontroller, by default used for the following if enabled/supported:
// PRODUCT_DESCRIPTION    the user friendly name for this device, appears on websites etc.
// HOST_NAME              the name ESP WiFi provides to any DHCP server (Ethernet doesn't support this)
// MDNS_NAME              the name mDNS (Multicast DNS) clients see for IP address resolution
// AP_SSID                the SSID WiFi clients see when the ESP WiFi Soft Access Point is enabled
#ifndef HOST_NAME
#define HOST_NAME                      "OnStep-SWS"
#endif

// use the HAL specified default NV driver
#ifndef NV_DRIVER
#define NV_DRIVER                      NV_DEFAULT
#endif

#ifndef LED_STATUS_ON_STATE
#define LED_STATUS_ON_STATE            LOW       // use HIGH so indicator stays on when connected as needed
#endif

// display
#ifndef DISPLAY_SPECIAL_CHARS
#define DISPLAY_SPECIAL_CHARS          ON        // for standard ASCII special symbols (compatibility)
#endif
#ifndef DISPLAY_HIGH_PRECISION_COORDS
#define DISPLAY_HIGH_PRECISION_COORDS  OFF       // for high precision coordinate display on status page
#endif

// drive configuration
#ifndef DRIVE_CONFIGURATION
#define DRIVE_CONFIGURATION            ON        // to display/modify mount, rotator, focuser settings
#endif
#ifndef DRIVE_MAIN_AXES_MICROSTEPS
#define DRIVE_MAIN_AXES_MICROSTEPS     ON        // to display Axis1/2 Microsteps if available
#endif
#ifndef DRIVE_MAIN_AXES_CURRENT
#define DRIVE_MAIN_AXES_CURRENT        ON        // to display Axis1/2 IRUN if available
#endif
#ifndef DRIVE_MAIN_AXES_REVERSE
#define DRIVE_MAIN_AXES_REVERSE        ON        // to display Axis1/2 Reverse if available
#endif

// enable and customize WiFi/Ethernet functionality
// for other default IP settings see the file:
// src/lib/wifi/WifiManager.defaults.h

#ifndef MDNS_SERVER
#define MDNS_SERVER                    ON
#endif

#ifndef MDNS_NAME
#define MDNS_NAME                      HOST_NAME
#endif

#ifndef COMMAND_SERVER
#define COMMAND_SERVER                 BOTH      // BOTH, for STANDARD (port 9999) and PERSISTENT (ports 9996 to 9998)
#endif                                           // or disable with OFF

#ifndef STA_AP_FALLBACK
#define STA_AP_FALLBACK                true      // activate SoftAP if station fails to connect
#endif

#ifndef STA_AUTO_RECONNECT
#define STA_AUTO_RECONNECT             true      // automatically reconnect if connection is dropped
#endif

// onstep reset control related
#ifndef RESET_PIN
#define RESET_PIN                      OFF
#endif

#ifndef RESET_PIN_STATE
#define RESET_PIN_STATE                LOW
#endif

// background ajax page update timing (in milliseconds, 50ms minimum)
#ifndef AJAX_PAGE_UPDATE_RATE_MS
#define AJAX_PAGE_UPDATE_RATE_MS       500       // normal ajax page update update rate
#endif

#ifndef STATE_POLLING_RATE_MS
#define STATE_POLLING_RATE_MS          500       // time between updates for most OnStep state information
#endif

// background scanning of OnStep state (in milliseconds, 200ms minimum)
#ifndef STATE_SLOW_POLLING_RATE_MS
#define STATE_SLOW_POLLING_RATE_MS     1000      // as above except for less critical items
#endif

#ifndef STATE_FAST_POLLING_RATE_MS
#define STATE_FAST_POLLING_RATE_MS     500       // as above except for more critical items
#endif

#ifndef STATE_GPIO_POLLING_RATE_MS
#define STATE_GPIO_POLLING_RATE_MS     100       // time between updates of OnStep GPIO pin state
#endif

// home offset
#ifndef HOME_OFFSET_RANGE_AXIS1
#define HOME_OFFSET_RANGE_AXIS1        7200      // allow adjusting home offset up to +/- 2 degrees
#endif
#ifndef HOME_OFFSET_RANGE_AXIS2
#define HOME_OFFSET_RANGE_AXIS2        7200      // allow adjusting home offset up to +/- 2 degrees
#endif

// encoders
#ifndef ENC_AUTO_SYNC_DEFAULT
#define ENC_AUTO_SYNC_DEFAULT          ON        // automatically sync Encoders to OnStep
#endif
#ifndef ENC_AUTO_SYNC_MEMORY
#define ENC_AUTO_SYNC_MEMORY           OFF       // remember automatic sync setting across power cycles
#endif
#ifndef ENC_SYNC_DURING_GOTO
#define ENC_SYNC_DURING_GOTO           OFF       // high resolution encoders correct pointing even during an align
#endif

#ifndef AXIS1_ENCODER
#define AXIS1_ENCODER                  OFF       // AB, AB_ESP32, CW_CCW, PULSE_DIR, AS37_H39B_B. RA/Azm (A/MA) & (B/SLO)
#endif
#ifndef AXIS1_ENCODER_TICKS_DEG
#define AXIS1_ENCODER_TICKS_DEG        22.222    // encoder ticks per degree
#endif
#ifndef AXIS1_ENCODER_REVERSE
#define AXIS1_ENCODER_REVERSE          OFF       // reverse the count direction
#endif
#ifndef AXIS1_ENCODER_DIFF_LIMIT_TO
#define AXIS1_ENCODER_DIFF_LIMIT_TO    0         // minimum diff. between encoder/OnStep for sync. to OnStep
#endif
#ifndef AXIS1_ENCODER_DIFF_LIMIT_FROM
#define AXIS1_ENCODER_DIFF_LIMIT_FROM  0         // maximum diff. between encoder/OnStep for sync. from OnStep
#endif

#ifndef AXIS2_ENCODER
#define AXIS2_ENCODER                  OFF       // AB, AB_ESP32, CW_CCW, PULSE_DIR, AS37_H39B_B. RA/Azm (A/MA) & (B/SLO)
#endif
#ifndef AXIS2_ENCODER_TICKS_DEG
#define AXIS2_ENCODER_TICKS_DEG        22.222    // encoder ticks per degree
#endif
#ifndef AXIS2_ENCODER_REVERSE
#define AXIS2_ENCODER_REVERSE          OFF       // reverse the count direction
#endif
#ifndef AXIS2_ENCODER_DIFF_LIMIT_TO
#define AXIS2_ENCODER_DIFF_LIMIT_TO    0         // minimum diff. between encoder/OnStep for sync. to OnStep
#endif
#ifndef AXIS2_ENCODER_DIFF_LIMIT_FROM
#define AXIS2_ENCODER_DIFF_LIMIT_FROM  0         // maximum diff. between encoder/OnStep for sync. from OnStep
#endif
