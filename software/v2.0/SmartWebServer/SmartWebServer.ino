/*
 * Title       OnStep Smart Web Server
 * by          Howard Dutton
 *
 * Copyright (C) 2016 to 2024 Howard Dutton
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Revision History, see GitHub
 *
 * Author: Howard Dutton
 * http://www.stellarjourney.com
 * hjd1964@gmail.com
 *
 * Description
 *
 * Web and IP Servers for OnStep and OnStepX
 *
 */

#define Product "Smart Web Server"
#define FirmwareVersionMajor  "2"
#define FirmwareVersionMinor  "08"
#define FirmwareVersionPatch  "r"

// Use Config.h to configure the SWS to your requirements

#include "src/Common.h"

#include "src/lib/tasks/OnTask.h"
#include "src/lib/nv/Nv.h"
#include "src/lib/ethernet/cmdServer/CmdServer.h"
#include "src/lib/ethernet/webServer/WebServer.h"
#include "src/lib/wifi/cmdServer/CmdServer.h"
#include "src/lib/wifi/webServer/WebServer.h"

#include "src/libApp/cmd/Cmd.h"
#include "src/libApp/bleGamepad/BleGamepad.h"
#include "src/libApp/encoders/Encoders.h"
#include "src/libApp/status/Status.h"

#include "src/pages/Pages.h"

#if DEBUG == PROFILER
  extern void profiler();
#endif

#if COMMAND_SERVER == PERSISTENT || COMMAND_SERVER == BOTH
  CmdServer persistentCmdSvr1(9996, 10L*1000L, true);
  #if OPERATIONAL_MODE != ETHERNET_W5100
    CmdServer persistentCmdSvr2(9997, 10L*1000L, true);
  #endif
  CmdServer persistentCmdSvr3(9998, 10L*1000L, true);
#endif

#if COMMAND_SERVER == STANDARD || COMMAND_SERVER == BOTH
  CmdServer cmdSvr(9999, 1L*1000L);
#endif

void systemServices() {
  nv.poll();
}

void pollWebSvr() {
  www.handleClient();
}

void pollCmdSvr() {
  #if COMMAND_SERVER == PERSISTENT || COMMAND_SERVER == BOTH
    persistentCmdSvr1.handleClient(); Y;
    #if OPERATIONAL_MODE != ETHERNET_W5100
      persistentCmdSvr2.handleClient(); Y;
    #endif
    persistentCmdSvr3.handleClient(); Y;
  #endif

  #if COMMAND_SERVER == STANDARD || COMMAND_SERVER == BOTH
    cmdSvr.handleClient(); Y;
  #endif
}

void setup(void) {
  #if OPERATIONAL_MODE == WIFI
    WiFi.disconnect();
    WiFi.softAPdisconnect(true);
  #endif

  strcpy(firmwareVersion.str, FirmwareVersionMajor "." FirmwareVersionMinor FirmwareVersionPatch);

  // start debug serial port
  if (DEBUG == ON || DEBUG == VERBOSE || DEBUG == PROFILER) SERIAL_DEBUG.begin(SERIAL_DEBUG_BAUD);
  delay(2000);

  #ifdef PIN_INIT
    PIN_INIT();
  #endif

  VF("MSG: SmartWebServer "); VL(firmwareVersion.str);
  VF("MSG: MCU = "); VF(MCU_STR); V(", "); VF("Pinmap = "); VLF(PINMAP_STR);

  delay(2000);

  // call gamepad BLE initialization
  #if (BLE_GAMEPAD == ON && ESP32)
    VLF("MSG: Init BLE");
    bleInit();
  #endif

  // call hardware specific initialization
  VLF("MSG: Init HAL");
  HAL_INIT();
  nv.init();

  #if LED_STATUS != OFF
    pinMode(LED_STATUS_PIN, OUTPUT);
  #endif

  // attempt to connect to OnStep
  int serialSwap = OFF;
  serialSwap = SERIAL_SWAP;
  if (serialSwap == AUTO) serialSwap = AUTO_OFF;

  long serial_baud = SERIAL_BAUD;
  onStep.serialBegin(SERIAL_BAUD_DEFAULT, serialSwap);
  uint8_t tb = 1;

Again:
  VLF("MSG: Clearing serial channel");
  onStep.clearSerialChannel();

  // look for OnStep
  VLF("MSG: Attempting to contact OnStep");
  SERIAL_ONSTEP.print(":GVP#"); delay(100);
  String s = SERIAL_ONSTEP.readString();
  if (s == "On-Step#") {
    // if there is more than one baud rate specified
    if (SERIAL_BAUD != SERIAL_BAUD_DEFAULT) {
      // get fastest baud rate, Mega2560 returns '4' for 19200 baud recommended
      SERIAL_ONSTEP.print(":GB#"); delay(100);
      if (SERIAL_ONSTEP.available() != 1) { onStep.serialRecvFlush(); goto Again; }
      if (SERIAL_ONSTEP.read() == '4' && serial_baud > 19200) serial_baud = 19200;
      // set faster baud rate
      SERIAL_ONSTEP.print(onStep.highSpeedCommsStr(serial_baud)); delay(100);
      if (SERIAL_ONSTEP.available() != 1) { onStep.serialRecvFlush(); goto Again; }
      if (SERIAL_ONSTEP.read() != '1') goto Again;
    }

    // we're all set, just change the baud rate to match OnStep
    onStep.serialBegin(serial_baud, serialSwap);

    #if DEBUG == REMOTE
      debugRemoteConnected = true;
    #endif
    
    VLF("MSG: OnStep Connection established");
  } else {
    if (DEBUG == ON || DEBUG == VERBOSE) { VF("MSG: No valid reply found ("); V(s); VL(")"); }
    #if LED_STATUS == ON
      digitalWrite(LED_STATUS_PIN, LED_STATUS_OFF_STATE);
    #endif
    // got nothing back, toggle baud rate and/or swap ports
    onStep.serialRecvFlush();
    tb++;
    if (tb == 16) { tb = 1; if (serialSwap == AUTO_OFF) serialSwap = AUTO_ON; else if (serialSwap == AUTO_ON) serialSwap = AUTO_OFF; }
    if (tb == 1) onStep.serialBegin(SERIAL_BAUD_DEFAULT, serialSwap);
    if (tb == 6) onStep.serialBegin(serial_baud, serialSwap);
    if (tb == 11) { if (SERIAL_BAUD_DEFAULT == 9600) { onStep.serialBegin(19200, serialSwap); } else tb = 15; }
    goto Again;
  }
  onStep.clearSerialChannel();

  // System services
  // add task for system services, runs at 10ms intervals so commiting 1KB of NV takes about 10 seconds
  VF("MSG: Setup, starting system services");
  VF(" task (rate 10ms priority 7)... ");
  if (tasks.add(10, 0, true, 7, systemServices, "SysSvcs")) { VL("success"); } else { VL("FAILED!"); }

  // get NV ready
  if (!nv.isKeyValid(INIT_NV_KEY)) {
    VF("MSG: NV, invalid key wipe "); V(nv.size); VLF(" bytes");
    if (nv.verify()) { VLF("MSG: NV, ready for reset to defaults"); }
  } else { VLF("MSG: NV, correct key found"); }

  // get the command and web timeouts
  if (!nv.hasValidKey()) {
    nv.write(NV_TIMEOUT_CMD, (int16_t)cmdTimeout);
    nv.write(NV_TIMEOUT_WEB, (int16_t)webTimeout);
  }
  cmdTimeout = nv.readUI(NV_TIMEOUT_CMD);
  webTimeout = nv.readUI(NV_TIMEOUT_WEB);

  // bring network servers up
  #if OPERATIONAL_MODE == WIFI
    VLF("MSG: Init WiFi");
    wifiManager.init();
  #else
    VLF("MSG: Init Ethernet");
    ethernetManager.init();
  #endif

  // ready encoders
  VLF("MSG: Initialize Encoders");
  encoders.init();

  // init is done, write the NV key if necessary
  if (!nv.hasValidKey()) {
    nv.writeKey((uint32_t)INIT_NV_KEY);
    nv.wait();
    if (!nv.isKeyValid(INIT_NV_KEY)) { DLF("ERR: NV, failed to read back key!"); } else { VLF("MSG: NV, reset complete"); }
  }

  VLF("MSG: Set webpage handlers");
  www.on("/index.htm", handleRoot);
  www.on("/index-ajax-get.txt", indexAjaxGet);
  www.on("/index.txt", indexAjax);

  www.on("/mount.htm", handleMount);
  www.on("/mount-ajax-get.txt", mountAjaxGet);
  www.on("/mount-ajax.txt", mountAjax);
  www.on("/libraryHelp.htm", handleLibraryHelp);

  www.on("/rotator.htm", handleRotator);
  www.on("/rotator-ajax-get.txt", rotatorAjaxGet);
  www.on("/rotator-ajax.txt", rotatorAjax);

  www.on("/focuser.htm", handleFocuser);
  www.on("/focuser-ajax-get.txt", focuserAjaxGet);
  www.on("/focuser-ajax.txt", focuserAjax);

  www.on("/auxiliary.htm", handleAux);
  www.on("/auxiliary-ajax-get.txt", auxAjaxGet);
  www.on("/auxiliary-ajax.txt", auxAjax);

  #if ENCODERS == ON
    www.on("/enc.htm", handleEncoders);
    www.on("/enc-ajax-get.txt", encAjaxGet);
    www.on("/enc-ajax.txt", encAjax);
  #endif

  www.on("/net.htm", handleNetwork);

  www.on("/", handleRoot);
  
  www.onNotFound(handleNotFound);

  #if COMMAND_SERVER == PERSISTENT || COMMAND_SERVER == BOTH
    #if OPERATIONAL_MODE != ETHERNET_W5100
      VLF("MSG: Starting port 9996 cmd server");
      persistentCmdSvr3.begin();
      VLF("MSG: Starting port 9997 cmd server");
      persistentCmdSvr2.begin();
    #endif
    VLF("MSG: Starting port 9998 cmd server");
    persistentCmdSvr1.begin();
  #endif

  #if COMMAND_SERVER == STANDARD || COMMAND_SERVER == BOTH
    VLF("MSG: Starting port 9999 cmd server");
    cmdSvr.begin();
  #endif

  VLF("MSG: Starting port 80 web server");
  www.begin();

  // allow time for the background servers to come up
  delay(2000);

  // clear the serial channel one last time
  onStep.clearSerialChannel();

  if (status.onStepFound) {
    status.update();
    delay(100);
  }

  state.init();

  VF("MSG: Setup, starting cmd channel polling");
  VF(" task (rate 5ms priority 2)... ");
  if (tasks.add(5, 0, true, 2, pollCmdSvr, "cmdPoll")) { VL("success"); } else { VL("FAILED!"); }

  VF("MSG: Setup, starting web server polling");
  VF(" task (rate 20ms priority 3)... ");
  if (tasks.add(20, 0, true, 3, pollWebSvr, "webPoll")) { VL("success"); } else { VL("FAILED!"); }

  // start task manager debug events
  #if DEBUG == PROFILER
    tasks.add(142, 0, true, 7, profiler, "Profilr");
  #endif

  #if (BLE_GAMEPAD == ON && ESP32)
    bleSetup();
  #endif

  VLF("MSG: SmartWebServer ready");
}

void loop(void) {
  #if (BLE_GAMEPAD == ON && ESP32)
    bleTimers(); Y;
    bleConnTest(); Y;
  #endif

  tasks.yield();
}
