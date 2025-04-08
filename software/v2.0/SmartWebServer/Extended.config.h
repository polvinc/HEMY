// ---------------------------------------------------------------------------------------------------------------------------------
// Extended configuration for OnStep SmartWebServer infrequently used options

//                                 *** Most users will not change anything in this file! ***

// ---------------------------------------------------------------------------------------------------------------------------------
// ADJUST THE FOLLOWING TO CONFIGURE YOUR CONTROLLER FEATURES ----------------------------------------------------------------------
// <-Req'd = always must set, <-Often = usually must set, Option = optional, Adjust = adjust as req'd, Infreq = infrequently changed

// DEBUG ------------------------------------------------------------ see https://onstep.groups.io/g/main/wiki/6-Configuration#DEBUG
// Enable additional debugging and/or status messages on the specified SERIAL_DEBUG port
// Note that the SERIAL_DEBUG port cannot be used for normal communication with OnStep

//      Parameter Name              Value        Default  Notes                                                                 Hint
#define DEBUG                         OFF //         OFF, Use ON for background error messages only, use VERBOSE for all      Infreq
                                          //              error and status messages, use PROFILER for VT100 task profiler,
                                          //              use REMOTE to forward verbose debug output to OnStep
#define SERIAL_DEBUG               Serial //      Serial, Or use any h/w serial port. Serial1 or Serial2, etc. as supported.  Option
#define SERIAL_DEBUG_BAUD            9600 //        9600, n. Where n=9600,19200,57600,115200 (common baud rates.)             Option

// NON-VOLATILE STORAGE ------------------------------------------------------------------------------------------------------------
#define NV_WIPE                       OFF //         OFF, Causes the defaults to be written back into NV (FLASH,EEPROM,etc.)  Infreq
                                          //              ***     IMPORTANT: ENABLE THIS OPTION THEN UPLOAD, WAIT A MINUTE    ***
// Warning --->                           //              ***     OR TWO THEN SET THIS OPTION TO OFF AND UPLOAD AGAIN.        ***
                                          //              ***     LEAVING THIS OPTION ENABLED CAN CAUSE EXCESSIVE NV          ***
                                          //              ***     WEAR AND DAMAGE THE MICROCONTROLLER NV SUBSYSTEM !!!        ***

// NETWORK SETTINGS ----------------------------------------------------------------------------------------------------------------

//        ***    These settings are stored in NV (EEPROM) and the SWS will not recognize setting changes below        ***
//        ***    except on the first upload or when NV is wiped, these settings can be changed at runtime however.    ***

#define PASSWORD_DEFAULT       "password" //  "password", For access to these settings, this can be changed at runtime also.  Adjust

#define AP_ENABLED                   true //        true, Wifi Access Point Enabled.                                          Adjust
#define AP_PASSWORD            "password" //  "password", Wifi Access Point password.                                         Adjust
#define AP_CHANNEL                      7 //           7, Wifi Access Point channel.                                          Adjust
#define AP_IP_ADDR          {192,168,0,1} // ..,168,0,1}, Wifi Access Point IP Address.                                       Adjust
#define AP_GW_ADDR          {192,168,0,1} // ..,168,0,1}, Wifi Access Point GATEWAY Address.                                  Adjust
#define AP_SN_MASK        {255,255,255,0} // ..55,255,0}, Wifi Access Point SUBNET Mask.                                      Adjust

#define STA_ENABLED                 false //       false, Wifi Station Enabled.                                               Adjust
#define STA_PASSWORD           "password" //  "password", Wifi Station mode password.                                         Adjust
#define STA_DHCP_ENABLED             true //        true, Wifi Station/Ethernet DHCP Enabled.                                 Adjust
#define STA_IP_ADDR         {192,168,0,1} //  ..168,0.1}, Wifi Station/Ethernet IP Address.                                   Adjust
#define STA_GW_ADDR         {192,168,0,1} // ..,168,0,1}, Wifi Station/Ethernet GATEWAY Address.                              Adjust
#define STA_SN_MASK       {255,255,255,0} // ..55,255,0}, Wifi Station/Ethernet SUBNET Mask.                                  Adjust

#define MAC {0xDE,0xAD,0xBE,0xEF,0xFE,0xEE} // ..E,0xEE}, Ethernet unique MAC address.                                        Adjust
