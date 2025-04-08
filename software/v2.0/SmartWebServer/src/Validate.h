#pragma once

#if DISPLAY_RESET_CONTROLS == FWU
  #if SERIAL_SWAP != OFF
    #error "Configuration (Config.h): Setting DISPLAY_RESET_CONTROLS FWU must be used with SERIAL_SWAP OFF only"
  #endif
#endif

#if ESP32
  #if (AP_ENABLED == true && STA_ENABLED == true)
    #error "AP and Station modes cannot be enabled at the same time with ESP32."
  #endif
#endif
