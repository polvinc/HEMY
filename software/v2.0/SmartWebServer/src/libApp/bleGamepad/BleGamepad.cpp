// ---------------------------------------------------------------------------------------------------------------------------------
// See Wiki for support: https://onstep.groups.io/g/main/wiki/26762
// !!! Important !!! This version requires Mode "B", press the M+B buttons simultaneously to start
// Mode "D" support is available for backward compatability, set MODE_D to ON
// All M button functions (except emergency stop) require a "double click" to activate
// The M button is not available with the new variant of MagicSee R1 GamePad.
// ---------------------------------------------------------------------------------------------------------------------------------
// BLE Gamepad support v3.0
// by Drew Bolce'

#include "BleGamepad.h"

#if defined(BLE_GAMEPAD) && (BLE_GAMEPAD == ON && ESP32)

  #include <BLEDevice.h>
  #include "../cmd/Cmd.h"
  #include "../status/Status.h"
  #include "BleConfig.h"

  #if (MODE_D == OFF) // Mode B code

    // Support for specific GamePads
    enum 
    {
    CONTROLLER_UNKNOWN = 0,
    CONTROLLER_R1,
    CONTROLLER_VRBOX,  
    CONTROLLER_MINIPLUS,
    };
  
    // Exact GamePad names (with spaces)
    char *szR1A = (char *)"Magicsee R1       ";
    char *szR1B = (char *)"Magicsee R1";  
    char *szVRBOX = (char *)"VR BOX";
    char *szVREXT = (char *)"XTREME VR REMOTE  ";
    char *szMINIPLUS = (char *)"MINI PLUS";
    static int iControllerType;

    // ===== GamePad Commands =====
    // Only commandBlind() is used to process these since it is ESP32 thread safe 
    // (unlike commandBool() etc.) and still reads any response as appropriate.
    #define FOCUS_IN          ":F-#"
    #define FOCUS_OUT         ":F+#"
    #define FOCUS_LOW         ":F3#"     // Set speed 3 (100um) for visual
    #define FOCUS_HIGH        ":F4#"     // High speed, 0.5x
    #define FOCUS_STOP        ":FQ#"
    #define SPIRAL            ":Mp#"     // Spiral search
    #define HOMEFOCUS         ":Fh#"     // Move focus to Home (1/2 max position)
    #define HOMEZERO          ":FS0#"    // Move focus to 0 position
    #define PARKFOCUS         ":FZ#"     // Set focus position to 0
    #define PARK              ":hP#"  
    #define UNPARK            ":hR#"
    #define TRACK_ON          ":Te#"
    #define TRACK_OFF         ":Td#"
    #define STOP_ALL           ":Q#"     // Emergency stop
    #define BEEP          ":SX97,2#"     // Short beep
    #define ALERT         ":SX97,3#"     // Alert, full 1 second blast
    #define CLICK         ":SX97,4#"     // Click
    #define GOTO_CURRENT      ":MS#"     // Returns to last Goto location
    #define ALIGN             ":A+#"     // Accept alignment location
    #define ALIGNSTARS        ":A3#"     // Begin alignment with 3 stars
    #define SYNC              ":CS#"     // Sync with current target 
    
    // ===== Timer values in milliseconds ======
    #define MTIMEOUT            1000     // M button "double click" time window
    #define SCANTIMER          15000     // Number of ms before rescanning
    #define FOCUSTIMER          5000     // Focus speedup timer
    #define LONGTIMER            500     // Long push timer 

    //====== GamePad Definitions ======
    enum
    {
      GP_JOY= 0,
      GP_TRIGGERS,
      GP_BTN_M,
      GP_BTN_L,
      GP_BTN_H,  
      GP_BTNAB,
      GP_BTNCD,  
      GP_NUMBYTES
    };

    // ===== GamePad Button Masks =====

    #define GP_BUTTON_UP        0x50
    #define GP_LOW_TRIGGER      0x40
    #define GP_UPR_TRIGGER      0x80
    #define GP_BUTTON_A         0x10
    #define GP_BUTTON_B         0x01
    #define GP_BUTTON_C         0x08
    #define GP_BUTTON_D         0x02
    #define GP_BUTTON_M         0x00
    #define GP_BUTTON_N         0x10
    #define GP_BUTTON_S         0x90
    #define GP_BUTTON_E         0x40
    #define GP_BUTTON_W         0x60
    #define FRESHFLAG           0x80

    // VR Box has different direction key codes
    #define VR_BUTTON_N         0x40
    #define VR_BUTTON_S         0x60
    #define VR_BUTTON_E         0x90
    #define VR_BUTTON_W         0x10 

    #define REV -2 // macro definition

    // We will connect to server(GamePad) by MAC address
    static BLEAddress *Server_BLE_Address; 
    String My_BLE_Address; 
    String My_BLE_Address1;
    String Scaned_BLE_Address;

    // We will use the name to identify GamePad type
    static BLEDevice *Server_BLE_Name; 
    String Scaned_BLE_Name;

    // All four modes send data. However each mode uses different byte positions and
    // values for each of the switches. We are using Mode "B". This allows use of all buttons.
    
    typedef void (*NotifyCallback)(BLERemoteCharacteristic*, uint8_t*, size_t, bool);

    // This is the service UUID of the mouse/joystick device (HID)
    static BLEUUID serviceUUID("00001812-0000-1000-8000-00805f9b34fb");

    // Battery Service UUID - not used for now
    //static BLEUUID BatteryServiceUUID("0000180F-0000-1000-8000-00805f9b34fb");

    // This characteristic UUID works for joystick & triggers (report)
    static BLEUUID ReportCharUUID("00002A4D-0000-1000-8000-00805f9b34fb"); 

    // Control flags
    static bool movingNorth = false;
    static bool movingSouth = false;
    static bool movingWest = false;
    static bool movingEast = false;
    static bool joyActive = false; // Joystick movement active flag
    static bool buttonPress = false; // buttons active flag
    static bool buttonAPress = false; // button A pressed 
    static bool triggerPress = false; // Focus trigger active
    static bool timerReturn = false; // Focus timer for high speed
    static bool pressedOnce = false; // Indicates a button was pushed (for M button)
    static bool focusSpd = false; // Focus speed toggle flag
    static bool spiralActive = false; // Spiral search start/stop flag
    static bool doConnect = false; // Run the connection routine
    static bool connected = false; // Fully connected to the GamePad
    static bool doScan = false; // Run the scan routine
    static bool haveDevice = false; // Found one of our GamePads
    static bool firstRate = true; // Allow rate change during alignment

    // BLE device model
    static BLEAdvertisedDevice* myDevice;
    static BLEClient *pClient = NULL;

    // Pointer to a list of characteristics of the active service,
    // sorted by characteristic UUID
    std::map<std::string, BLERemoteCharacteristic*> *pmap;
    std::map<std::string, BLERemoteCharacteristic*> :: iterator itr;

    // Pointer to a list of characteristics of the active service,
    // sorted by characteristic handle
    std::map<std::uint16_t, BLERemoteCharacteristic*> *pmapbh;
    std::map<std::uint16_t, BLERemoteCharacteristic*> :: iterator itrbh;

    // Storage for pointers to characteristics we want to work with
    BLERemoteCharacteristic *bleRcs[4];

    // This is for checking incoming data type
    static uint16_t u16Buttons;

    // This is where we store the data from the buttons and joystick
    volatile byte GpBoxData[GP_NUMBYTES];

    // ===== Timers, millisecond re-triggerable =====

    // LTimer is the time window for a long push
    volatile uint32_t LTimer = millis();

    // MTimer is the amount of time you have to do a
    // second click. 
    volatile uint32_t MTimer = millis();

    // pushTimer sets the amount of time a key must be pressed 
    // before a button state change is triggered
    volatile uint32_t pushTimer = millis();

    // scanTimer sets the amount of time between rescans on 
    // device disconnect
    volatile uint32_t scanTimer = millis();

    // joyTimer sets the joystick back to center if no activity
    // on the joystick or trigger buttons. 
    volatile uint32_t joyTimer = millis();

    // Task handles  
    TaskHandle_t HandleJS = NULL;   // handle of the joystick task
    TaskHandle_t HandleTrig = NULL;   // handle of the trigger button task
    TaskHandle_t HandleBtn = NULL;  // handle of the button task
    TaskHandle_t HandleM = NULL;  // handle of the M button task
    TaskHandle_t HandleAB = NULL;   // handle of the A/B button task
    TaskHandle_t HandleCD = NULL;   // handle of the C/D button task

    // GuideRate up/down
    int activeGuideRate;      // Scope, 2 to 9

    // Character buffer
    char bfr[80];

    // Task Stack memory allocation size
    #define TaskStackSize     4096

    // ===== Focuser functons =====
    void StartFocus()
    {
      if (FOCUS_INIT == ON) 
      {           
      onStep.commandBlind(PARKFOCUS);  //Set 0 position  
      onStep.commandBlind(HOMEFOCUS);  // Move half way
      }
    }
    void ParkFocus()
    {
      if (FOCUS_INIT == ON) 
      {           
      onStep.commandBlind(HOMEZERO); //Move focus to 0 position   
      onStep.commandBlind(PARKFOCUS); // Set 0 position
      }
    }

    // ===== LED functions =====
    void LED_STATE_A()
    {
      if (LED_ENABLE == ON) digitalWrite(LED_STATUS_PIN, LED_STATUS_OFF_STATE);
      else if (LED_ENABLE == REV) digitalWrite(LED_STATUS_PIN, LED_STATUS_ON_STATE);
    }
    void LED_STATE_B()
    {
      if (LED_ENABLE == ON) digitalWrite(LED_STATUS_PIN, LED_STATUS_ON_STATE);
      else if (LED_ENABLE == REV) digitalWrite(LED_STATUS_PIN, LED_STATUS_OFF_STATE);
    }
  
    // ===== Sound functions =====
    void Beep()
    {
      if (SOUND == ON) onStep.commandBlind(BEEP);
    }
    void Alert() 
    {
      if (SOUND == ON) onStep.commandBlind(ALERT);
    }
      void Click()
    {
      if (SOUND == ON) onStep.commandBlind(CLICK);
    }

    // ===== Direction functions =====
    void MoveNorth() // Move North
    { 
      if (!movingNorth)
      {
        movingNorth = ON;
        onStep.commandBlind(":Mn#");
      }
    }
    void MoveSouth() // Move South 
    { 
      if (!movingSouth)
      {
        movingSouth = ON;
        onStep.commandBlind(":Ms#");
      }
    }
    void MoveEast() // Move East 
    { 
      if (!movingEast)
      {
        movingEast = ON;
        onStep.commandBlind(":Me#");
      }
    }
    void MoveWest() // Move West 
    { 
      if (!movingWest)
      {
        movingWest = ON;
        onStep.commandBlind(":Mw#");
      }
    }

    //******************************************************************************
    // HID notification callback handler.
    //******************************************************************************
    static void notifyCallback(
      BLERemoteCharacteristic* pBLERemoteCharacteristic,
      uint8_t* pData,
      size_t length,
      bool isNotify)
    {
      switch (iControllerType)
      {
        case CONTROLLER_MINIPLUS:
        if (length == 9) // it always writes 9 byte reports
        {
        // Parsing code here
        }

        default: // For VR Box and MagicSee R1
        if (length == 2) // it always writes 2 byte reports
        { 
          // Emergency stop, any button, not joystick (OnStep will abort guide on joystick touch)
          if (!((0x00 == pData[0]) && (0x00 != pData[1])))
          {  
            if(!((pData[0] == 0x00) && (pData[1] == 0x50)))
            {
              if (status.inGoto || movingNorth || movingSouth || movingEast || movingWest)
              { 
                onStep.commandBlind(STOP_ALL);
                spiralActive == false;
                Alert();
                return;
              }
              if(spiralActive)
              {
                spiralActive = false; 
                onStep.commandBlind(STOP_ALL);
                Click();
                return;
              }  
            }
          }

          // UP button trap
          if ((pData[0] == 0x00) && (pData[1] == 0x50))
          {
            if (joyActive) // Joystick movement in progress
            {
              if (HandleJS)
              {
                GpBoxData[GP_JOY] = pData[1];
                vTaskResume(HandleJS);
              }
            }  
            else if (buttonPress || triggerPress) // Button has been pressed
            {
              if (HandleBtn)
              {
                GpBoxData[GP_BTN_L] = pData[0];
                GpBoxData[GP_BTN_H] = pData[1];
                vTaskResume(HandleBtn);
              }
            }
          }

          // M button, wake the M button handler task 
          else if (0x00 == pData[0] && 0x00 == pData[1])
          {
            // GpBoxData[GP_BTN_M] = pData[0];
            if (HandleM)
              vTaskResume(HandleM);
          }

          // Joystick, wake the Joy button handler task
          else if ((0x00 == pData[0]) && (0x00 != pData[1]))
          {
            GpBoxData[GP_JOY] = pData[1];
            if (HandleJS)
              vTaskResume(HandleJS);
          }

          else
          {
            // other Buttons, wake the BTN task
            GpBoxData[GP_BTN_L] = pData[0];
            GpBoxData[GP_BTN_H] = pData[1];
            if (HandleBtn)
              vTaskResume(HandleBtn);
          }
        }
      }
    }

    // End of notifyCallback

    //******************************************************************************
    // Connection state change event callback handler.
    //******************************************************************************
    class MyClientCallback: public BLEClientCallbacks
    {
      void onConnect(BLEClient* pclient)
      {
        scanTimer = 0;                                      // Reinitalize the scan timer
        LED_STATE_A();
        VLF("bleMSG: BLE GamePad connected");
      }
      
      void onDisconnect(BLEClient* pclient)
      {
        haveDevice = false;
        connected = false;
        delete myDevice; 
        scanTimer = millis() + SCANTIMER;                  // Restart the scan timer
        LED_STATE_B();
        VLF("bleMSG: BLE GamePad Disconnected");
      }
    };
    // End of MyClientCallback

    //******************************************************************************
    // Connect to a service, register for notifications from Report Characteristics.
    //******************************************************************************
    bool setupCharacteristics(BLERemoteService* pRemoteService, NotifyCallback pNotifyCallback)
    {
      // Get all the characteristics of the service using the handle as the key
      pmapbh = pRemoteService->getCharacteristicsByHandle();
      
      // Only interested in report characteristics that have the notify capability
      for (itrbh = pmapbh->begin(); itrbh != pmapbh->end(); itrbh++)
      {
        BLEUUID x = itrbh->second->getUUID();
        UNUSED(x);
        // The uuid must match the report uuid
        if (ReportCharUUID.equals(itrbh->second->getUUID()))
        {
          // Found a report characteristic
          if (itrbh->second->canNotify())
          {
            // Register for notifications from this characteristic
            itrbh->second->registerForNotify(pNotifyCallback);
          }
        }
      }
      return true;
    }
    // End of setupCharacteristics

    //******************************************************************************
    // Validate the server has the correct name and services we are looking for.
    // The server must have the HID service.
    //******************************************************************************
    bool connectToServer()
    {
      if (pClient == NULL) pClient = BLEDevice::createClient();
      pClient->setClientCallbacks(new MyClientCallback());

      // Implemented simplest security for new type GamePads. This kind security does not provide authentication
      BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT);
      BLESecurity *pSecurity = new BLESecurity();
      pSecurity->setKeySize();
      pSecurity->setAuthenticationMode(ESP_LE_AUTH_BOND);
      pSecurity->setCapability(ESP_IO_CAP_NONE);

      // Connect to the remote BLE Server
      bool _connected = pClient->connect(myDevice); // If you pass BLEAdvertisedDevice instead of address, 
                                                    // it will be recognized type of peer device address (public or private)
      if(_connected)   
      {
        // VLF("bleMSG: bleMSG: BLE says it is connected");   
        // pClient->setMTU(112); //set client to new MTU from server (default is 23 otherwise)     
      }

      if(!_connected)
      {
        delete pClient;
        delete myDevice;
        return false;
      } 
        
      // BLE servers may offer several services, each with unique characteristics
      // we can identify the type of service by using the service UUID
      // Obtain a reference to the service we are after in the remote BLE server.
      // This will return a pointer to the remote service if it has a matching service UUID
      
      BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
      if (pRemoteService == nullptr)
      {
        pClient->disconnect();
        return false;
      }
      setupCharacteristics(pRemoteService, notifyCallback);

      doConnect = false;
      connected = true;
      return true; 
    }
    // End of connectToServer

    //******************************************************************************
    // Scan for BLE servers and find the first one that advertises the service
    // we are looking for.
    //******************************************************************************
    class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks
    {
      // Called for each advertising BLE server.
      void onResult(BLEAdvertisedDevice advertisedDevice)
      {
        // We have found a server, see if it has the name we are looking for
        if (!haveDevice) 
        {
          if (advertisedDevice.haveName())
          {
            Server_BLE_Address = new BLEAddress(advertisedDevice.getAddress()); 
            Scaned_BLE_Address = Server_BLE_Address->toString().c_str();
            Scaned_BLE_Name = advertisedDevice.getName().c_str(); 

            VF("bleMSG: BLE Address = "); VL(Scaned_BLE_Address); 
            VF("bleMSG: Device Name = ");
            VF("["); 
            V(advertisedDevice.getName().c_str());  
            VLF("]");

            // Device name
            // for exact name with spaces. Set debug to ON and use SWS USB port
            Serial.print("Name = ["); Serial.print(Scaned_BLE_Name); Serial.println("]");

            if ((Scaned_BLE_Address == (My_BLE_Address)) || (Scaned_BLE_Address == (My_BLE_Address1)))
            {
              // We found a server with the correct Mac, see if it has the service we are
              // interested in (HID)
              if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID))
              {
                // Compare it to known controllers
                iControllerType = CONTROLLER_UNKNOWN;
                if (Scaned_BLE_Name == szR1A)
                iControllerType = CONTROLLER_R1; 
                else if (Scaned_BLE_Name == szR1B)
                iControllerType = CONTROLLER_R1;              
                else if (Scaned_BLE_Name == szVRBOX)
                iControllerType = CONTROLLER_VRBOX; 
                else if (Scaned_BLE_Name == szVREXT)
                iControllerType = CONTROLLER_VRBOX; 
                else if (Scaned_BLE_Name == szMINIPLUS)
                iControllerType = CONTROLLER_MINIPLUS;              

                VF("bleMSG: iControllerType = ");
                VL(iControllerType);

                doConnect = true;
                doScan = true;
                haveDevice = true;
                scanTimer = 0; // reinitalize the scan timer
                BLEDevice::getScan()->stop();
                myDevice = new BLEAdvertisedDevice(advertisedDevice);
              }           
            } 
          }
        }
      }
    };
    // End of MyAdvertisedDeviceCallbacks

    // All of these tasks are designed to run forever. The tasks are resumed when
    // a notification message is received with new data.

    //******************************************************************************
    // Joystick Task.
    //******************************************************************************
    void taskJoyStick(void *parameter)
    {
      // Forever loop
      while(true)
      {
        vTaskSuspend(NULL); // give up the CPU, wait for new data

        // We just woke up, new data is available
        if(INIT_ALIGN == ON) // Set speed for alignment
        {
          if (!status.aligning) firstRate = true; // Reset for multiple aligns
          if (status.aligning && firstRate) // Set guide rate = 48X for align
          {
            firstRate = false; // Ignore after starting align to allow rate changes        
            activeGuideRate = 7; // 48X
            char cmd[5] = ":Rn#"; cmd[2] = '0' + activeGuideRate; // set initial GuideRate
            onStep.commandBlind(cmd);
          }
        }
        if (GpBoxData[GP_JOY] == GP_BUTTON_UP) // Joystick is centered 
        { 
          joyActive = false; // Turn off JoyStick active flag

          if (movingNorth || movingSouth || movingEast || movingWest)
          { 
            if (movingNorth) {movingNorth = false; onStep.commandBlind(":Qn#");}
            if (movingSouth) {movingSouth = false; onStep.commandBlind(":Qs#");}
            if (movingEast) {movingEast = false; onStep.commandBlind(":Qe#");}
            if (movingWest) {movingWest = false; onStep.commandBlind(":Qw#");}
          }        
        }
        else // Joystick directional move
        {
          joyActive = true; // JoyStick active flag

          if (iControllerType == (CONTROLLER_UNKNOWN || CONTROLLER_R1))
          {
            if (GpBoxData[GP_JOY] == GP_BUTTON_N)
            {
              if (NS_SWAP == OFF) MoveNorth();
              else MoveSouth();
            }
            else if (GpBoxData[GP_JOY] == GP_BUTTON_S)
            {
              if (NS_SWAP == OFF) MoveSouth(); 
              else MoveNorth();
            }
            if (GpBoxData[GP_JOY] == GP_BUTTON_E)
            {
              if (EW_SWAP == OFF) MoveEast();
              else MoveWest();
            }
            else if (GpBoxData[GP_JOY] == GP_BUTTON_W)
            {
              if (EW_SWAP == OFF) MoveWest();
              else MoveEast();
            }
          }
          else if (iControllerType == CONTROLLER_VRBOX)
          {
            if (GpBoxData[GP_JOY] == VR_BUTTON_N)
            {
              if (NS_SWAP == OFF) MoveNorth();
              else MoveSouth();
            }
            else if (GpBoxData[GP_JOY] == VR_BUTTON_S)
            {
              if (NS_SWAP == OFF) MoveSouth(); 
              else MoveNorth();
            }
                
            if (GpBoxData[GP_JOY] == VR_BUTTON_E)
            {
              if (EW_SWAP == OFF) MoveEast();
              else MoveWest();
            }
            else if (GpBoxData[GP_JOY] == VR_BUTTON_W)
            {
              if (EW_SWAP == OFF) MoveWest();
              else MoveEast();
            }
          }
        }
      }
    }
    // End of taskJoyStick
    
    //******************************************************************************
    // M button task
    // Double tap of M button within MTIMEOUT miliseconds
    //******************************************************************************
    void taskButtonM(void *parameter)
    {
      while(true)
      {
        vTaskSuspend(NULL); // give up the CPU, wait for new data
        
        // We just woke up, new data is available
        if (M_SINGLE_CLICK == ON) pressedOnce = true;
        
        if (!pressedOnce) // First press, start double click timer
        {
          pressedOnce = true;
          MTimer = millis() + MTIMEOUT;
          continue;
        }
        if (M_BUTTON == ON)
        {
          if (pressedOnce)
          {
            // Set align point when doing an alignment
            if (status.aligning)
            {
              if (status.alignThisStar == status.alignLastStar) Alert();
              else Click();
              onStep.commandBlind(ALIGN);
            }
            // Unpark mount if parked        
            else if (status.parked) 
            {
              onStep.commandBlind(UNPARK);
              StartFocus();          
              Beep();          
            }
            // Start tracking if "At home"        
            else if (status.atHome && !status.tracking)
            {
              onStep.commandBlind(TRACK_ON);
              if(INIT_ALIGN == ON)
              {
              onStep.commandBlind(ALIGNSTARS);    
              }
              StartFocus();
              Alert();                   
            }
            // Start tracking if tracking off
            else if (!status.tracking)
            {
              onStep.commandBlind(TRACK_ON);
              Beep();          
            }
            // Park the scope                
            else if (!status.parked)
            {
              ParkFocus();
              onStep.commandBlind(PARK);                
            }
            pressedOnce = false;          
            MTimer = 0;
          }
        }
      }
    }
    //  End of taskButtonM

    //******************************************************************************
    // Buttons handler Task. 
    // Press of any button will resume this task. 
    //******************************************************************************
    void taskBtn(void *parameter)
  {
      while(true)
      {
        vTaskSuspend(NULL); // give up the CPU, wait for new data

        // We just woke up, new data is available
        if(GpBoxData[GP_BTN_H] == 0x50) // focus triggers, B and D buttons
        {
          if (GpBoxData[GP_BTN_L] == 0X00) // Button UP
          {
            if(buttonAPress)
            {
               if (LTimer && (LTimer < millis())) // Long press
              {
                onStep.commandBlind(SYNC);            
                Click();
              }
              else // A short click
              {
                onStep.commandBlind(GOTO_CURRENT); // Button A pressed, goto last location
              }
              LTimer = 0;
              buttonAPress = false;
              buttonPress = false;
            }

            else if (triggerPress)
            {
              // Trigger released, stop focuser 
              if (GpBoxData[GP_BTN_L] == 0x00)
              {
                onStep.commandBlind(FOCUS_STOP);        
                pushTimer = 0;
                triggerPress = false;   
                focusSpd = false;
              }
            }
            continue;
          }
          if (GpBoxData[GP_BTN_L] == GP_LOW_TRIGGER)
          {
            // The lower trigger button is pressed
            if (focusSpd && triggerPress)
              {
                onStep.commandBlind(FOCUS_HIGH); 
                focusSpd = false;
                Click();
                }
            else 
              {
                onStep.commandBlind(FOCUS_LOW);
                pushTimer = millis() + FOCUSTIMER; 
              }
            onStep.commandBlind(FOCUS_IN);         
            triggerPress = true;
            continue;
          }
          
          if (GpBoxData[GP_BTN_L] == GP_UPR_TRIGGER)
          {
            // The upper trigger button is pressed
            if (focusSpd && triggerPress)
              {
                onStep.commandBlind(FOCUS_HIGH);
                Click(); 
                focusSpd = false;
              }
            else 
              {
                onStep.commandBlind(FOCUS_LOW);
                pushTimer = millis() + FOCUSTIMER; 
              }
            onStep.commandBlind(FOCUS_OUT);         
            triggerPress = true;
            continue;
          }
        
          else if (GpBoxData[GP_BTN_L] == GP_BUTTON_B)
          {
            // Button B pressed
            if (spiralActive == false)
            {
              spiralActive = true;
              onStep.commandBlind(SPIRAL);
              Click();
            }
            else
            {
              spiralActive = false; 
              onStep.commandBlind(STOP_ALL);
              Click();
            }         
          }

          else if (GpBoxData[GP_BTN_L] == GP_BUTTON_D)
          {
            // Button D pressed
            (activeGuideRate++);
            if (activeGuideRate < 10) Click();
            if (activeGuideRate < 3) activeGuideRate = 3;
            if (activeGuideRate > 9) activeGuideRate = 9;
            char cmd[5] = ":Rn#"; cmd[2] = '0' + activeGuideRate;
            onStep.commandBlind(cmd);
          }

          if (iControllerType == CONTROLLER_VRBOX)
          { 
            if ( GpBoxData[GP_BTN_L] == GP_BUTTON_A)
            {
              LTimer = millis() + LONGTIMER;
              buttonAPress = true;
              buttonPress = true;
            }
            else if (GpBoxData[GP_BTN_L] == GP_BUTTON_C)
            {
              // button C pressed
              (activeGuideRate--);
              if (activeGuideRate > 2) Click(); 
              if (activeGuideRate < 3) activeGuideRate = 3;
              if (activeGuideRate > 9) activeGuideRate = 9;
              char cmd[5] = ":Rn#"; cmd[2] = '0' + activeGuideRate;
              onStep.commandBlind(cmd);
            }
          } 
        }

        else if ( GpBoxData[GP_BTN_L] == GP_BUTTON_A)
        {
          LTimer = millis() + LONGTIMER;
          buttonAPress = true;
          buttonPress = true;
        }

        else if (GpBoxData[GP_BTN_L] == GP_BUTTON_C)
        {
          // button C pressed
          (activeGuideRate--);
          if (activeGuideRate > 2) Click(); 
          if (activeGuideRate < 3) activeGuideRate = 3;
          if (activeGuideRate > 9) activeGuideRate = 9;
          char cmd[5] = ":Rn#"; cmd[2] = '0' + activeGuideRate;
          onStep.commandBlind(cmd);
        }       
      }
    }
    //  End of taskButtonBtn


    //******************************************************************************
    // Initial and Service routines
    //******************************************************************************

    void bleInit()
    {
      // Enable BT ble only (no classic)
      esp_bt_controller_enable(ESP_BT_MODE_BLE);
      // Free memory used by BT Classic to prevent out of memory crash
      esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);
    }

    void DoScan() 
    {
      if (scanTimer && (scanTimer < millis()))
      {
        // Retrieve a GATT Scanner and set the callback we want to use to be informed 
        // when we have detected a new device.  Specify that we want active scanning
        // and start the scan.
      
        BLEScan* pBLEScan = BLEDevice::getScan();
        pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
        pBLEScan->setInterval(1349);
        pBLEScan->setWindow(499);
        pBLEScan->setActiveScan(true);
        pBLEScan->start(1); // Scan 1 second
          
        pBLEScan->clearResults(); // Delete results fromBLEScan buffer to release memory, solves heap problem
      
        // Restart the scan timer
        scanTimer = millis() + SCANTIMER;
        VLF("bleMSG: bleMSG: Scanning for BLE GamePad");
      }
    }
    // End of DoScan.

    //******************************************************************************
    // This is the first (and after disconnect) server connection
    //******************************************************************************
    void bleConnTest() 
    {
      // If the flag "doConnect" is true then we have scanned for and found the desired
      // BLE Server with which we wish to connect.  Now we connect to it.

      if (doConnect == true) connectToServer(); 
      else
      {
        doConnect = false;
        doScan = true;
        DoScan();
      }
    }
    // End of bleConnTest

    void bleSetup()
    {
      My_BLE_Address = BLE_GP_ADDR;
      My_BLE_Address1 = BLE_GP_ADDR1;
      MTimer = 0;  
      pushTimer = 0;
      activeGuideRate = status.guideRate;
      BaseType_t xReturned;

      VLF("bleMSG: Starting BLE GamePad Services"); 

      // Create tasks to handle the joystick and buttons
      xReturned = xTaskCreate(taskJoyStick,             // task to handle activity on the joystick.
                              "Joystick",               // String with name of task.
                              TaskStackSize,            // Stack size in 32 bit words.
                              NULL,                     // Parameter passed as input of the task
                              1,                        // Priority of the task.
                              &HandleJS);               // Task handle.

      xReturned = xTaskCreate(taskBtn,             // task to handle activity on the C, D & Triggger buttons.
                              "Btn",              // String with name of task.
                              TaskStackSize,            // Stack size in 32 bit words.
                              NULL,                     // Parameter passed as input of the task
                              1,                        // Priority of the task.
                              &HandleBtn);               // Task handle.

      xReturned = xTaskCreate(taskButtonM,             // task to handle activity on the M button.
                              "ButtonsM",              // String with name of task.
                              TaskStackSize,            // Stack size in 32 bit words.
                              NULL,                     // Parameter passed as input of the task
                              1,                        // Priority of the task.
                              &HandleM);               // Task handle.

      UNUSED(xReturned);

      BLEDevice::init("");

      if (!connected)
      {
        // Retrieve a GATT Scanner and set the callback we want to use to be informed 
        // when we have detected a new device.  Specify that we want active scanning
        // and start the scan.
        
        BLEScan* pBLEScan = BLEDevice::getScan();
        pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
        pBLEScan->setInterval(1349);
        pBLEScan->setWindow(150);  
        pBLEScan->setActiveScan(true);
        pBLEScan->start(2, false); // Scan
        delay(3000); // Delay for scan
      }

      if (!connected)
      {
        doScan = true;
        scanTimer = millis() + SCANTIMER; // Restart the scan timer
      }

      bleConnTest();     
    }
    // End of bleSetup

    void bleTimers() 
    {
      if (connected)
      {
        if (MTimer && (MTimer < millis()))
        { 
          // M button double click allowed time window expired, reset 
          pressedOnce = false;
          MTimer = 0; 
        }
              
        if ((pushTimer && (pushTimer < millis())) && triggerPress)
        {
          // Focus button held down for FOCUSTIMER, speed up focuser
          focusSpd = true;
          pushTimer = 0; 
          if (HandleBtn)
            vTaskResume(HandleBtn);
        }
      }
    }
    // End of bleTimers
  #endif // End of Mode B code

// ---------------------------------------------------------------------------------------------------------------------------------
  // BLE Gamepad support V2.0
  // by Drew Bolce'
  // for backward compatability
// ---------------------------------------------------------------------------------------------------------------------------------
  #if (MODE_D == ON) // mode D code, V2.0

    //******************************************************************************
    // Configuration settings
    //******************************************************************************

    #define REV -2 // macro definition

    //******************************************************************************

    // ===== GamePad Commands =====
    // Only commandBlind() is used to process these since it is ESP32 thread safe 
    // (unlike commandBool() etc.) and still reads any response as appropriate.
    #define FOCUS_IN          ":F-#"
    #define FOCUS_OUT         ":F+#"
    #define FOCUS_LOW         ":F3#"     // Set speed 3 (100um) for visual
    #define FOCUS_HIGH        ":F4#"     // High speed, 0.5x
    #define FOCUS_STOP        ":FQ#"
    #define SPIRAL            ":Mp#"     // Spiral search
    #define HOMEFOCUS         ":Fh#"     // Move focus to Home (1/2 max position)
    #define HOMEZERO          ":FS0#"    // Move focus to 0 position
    #define PARKFOCUS         ":FZ#"     // Set focus position to 0
    #define PARK              ":hP#"  
    #define UNPARK            ":hR#"
    #define TRACK_ON          ":Te#"
    #define TRACK_OFF         ":Td#"
    #define STOP_ALL           ":Q#"     // Emergency stop
    #define BEEP          ":SX97,2#"     // Short beep
    #define ALERT         ":SX97,3#"     // Alert, full 1 second blast
    #define CLICK         ":SX97,4#"     // Click
    #define GOTO_CURRENT      ":MS#"     // Returns to last Goto location
    #define ALIGN             ":A+#"     // Accept alignment location

    #define JoyStickDeadZone       0     // Increase if motors not stopping when in center position

    // ===== Timer values in milliseconds ======
    #define JOYTIMEOUT           100     // Joystick no activity timeout
    #define SCANTIMER          15000     // Number of ms before rescanning
    #define FOCUSTIMER          5000     // Focus speedup timer

    //------ GamePad Definitions -----
    enum
    {
      GP_TRIGGERS = 0,
      GP_JOYX,
      GP_JOYY,
      GP_BTNAB,
      GP_BTNCDM,
      GP_NUMBYTES
    };

    // ===== GamePad Button Masks =====

    #define GP_BUTTON_UP        0x00
    #define GP_LOW_TRIGGER      0x01
    #define GP_UPR_TRIGGER      0x02
    #define GP_BUTTON_A         0x01
    #define GP_BUTTON_B         0x02
    #define GP_BUTTON_C         0x01
    #define GP_BUTTON_D         0x02
    #define GP_BUTTON_M         0x00
    #define FRESHFLAG           0x80

    // We will connect to server(GamePad) by MAC address
    static BLEAddress *Server_BLE_Address; 
    String My_BLE_Address; 
    String My_BLE_Address1;
    String Scaned_BLE_Address;

    // All four modes send data. However each mode uses different byte positions and
    // values for each of the switches. We are using Mode "D". This allows use of all buttons.
    
    typedef void (*NotifyCallback)(BLERemoteCharacteristic*, uint8_t*, size_t, bool);

    // This is the service UUID of the mouse/joystick device (HID)
    static BLEUUID serviceUUID("00001812-0000-1000-8000-00805f9b34fb");

    // Battery Service UUID - not used for now
    //static BLEUUID BatteryServiceUUID("0000180F-0000-1000-8000-00805f9b34fb");

    // This characteristic UUID works for joystick & triggers (report)
    static BLEUUID ReportCharUUID("00002A4D-0000-1000-8000-00805f9b34fb"); 

    // Control flags
    static bool movingNorth = false;
    static bool movingSouth = false;
    static bool movingWest = false;
    static bool movingEast = false;
    static bool triggerPress = false; // Focus triggers
    static bool timerReturn = false; // Focus timer for high speed
    static bool pressedOnce = false; // We use the second signal
    static bool FocusSpd = false; // Focus speed toggle flag
    static bool SpiralInProgess = false; // Spiral search start/stop flag
    static bool CDMpressedOnce = false; // A sort of debounce
    static bool doConnect = false; // Run the connection routine
    static bool Connected = false; // Fully connected to the GamePad
    static bool doScan = false; // Run the scan routine
    static bool havedevice = false; // Found one of our GamePads
    static bool firstrate = true; // Allow rate change during alignment

    // BLE device model
    static BLEAdvertisedDevice* myDevice;
    static BLEClient *pClient = NULL;

    // Pointer to a list of characteristics of the active service,
    // sorted by characteristic UUID
    std::map<std::string, BLERemoteCharacteristic*> *pmap;
    std::map<std::string, BLERemoteCharacteristic*> :: iterator itr;

    // Pointer to a list of characteristics of the active service,
    // sorted by characteristic handle
    std::map<std::uint16_t, BLERemoteCharacteristic*> *pmapbh;
    std::map<std::uint16_t, BLERemoteCharacteristic*> :: iterator itrbh;

    // Storage for pointers to characteristics we want to work with
    BLERemoteCharacteristic *bleRcs[4];

    // This is for checking incoming data type
    static uint16_t u16Buttons;
    
    // This is where we store the data from the buttons and joystick
    volatile byte   VrBoxData[GP_NUMBYTES];

    // ------ Timers, millisecond re-triggerable ------ 

    // joyTimer sets the joystick back to center if there is
    // no activity on the joystick or trigger buttons. 
    volatile uint32_t joyTimer = millis();

    // pushTimer sets the amount of time a key must be pressed 
    // before a button state change is triggered
    volatile uint32_t pushTimer = millis();

    // scanTimer sets the amount of time between rescans on 
    // device disconnect
    volatile uint32_t scanTimer = millis();

    // Task handles  
    TaskHandle_t HandleJS = NULL;   // handle of the joystick task
    TaskHandle_t HandleAB = NULL;   // handle of the A/B button task
    TaskHandle_t HandleCDM = NULL;  // handle of the C/D/M button task

    // GuideRate up/down
    int activeGuideRate = 7;        // Scope, 2 to 9, 7 = 48X

    // Character buffer
    char bfr[80];

    // Task Stack memory allocation size
    #define TaskStackSize     4096

    // ----- Focuser functons -----
    void StartFocus()
    {
      if (FOCUS_INIT == ON) 
      {           
      onStep.commandBlind(PARKFOCUS);  //Set 0 position  
      onStep.commandBlind(HOMEFOCUS); // Move half way
      }
    }
    void ParkFocus()
    {
      if (FOCUS_INIT == ON) 
      {           
      onStep.commandBlind(HOMEZERO); //Move focus to 0 position   
      onStep.commandBlind(PARKFOCUS); // Set 0 position
      }
    }

    // ------ LED functions -----
    void LED_STATE_A()
    {
      if (LED_ENABLE == ON) digitalWrite(LED_STATUS_PIN, LED_STATUS_OFF_STATE);
      else if (LED_ENABLE == REV) digitalWrite(LED_STATUS_PIN, LED_STATUS_ON_STATE);
    }
    void LED_STATE_B()
    {
      if (LED_ENABLE == ON) digitalWrite(LED_STATUS_PIN, LED_STATUS_ON_STATE);
      else if (LED_ENABLE == REV) digitalWrite(LED_STATUS_PIN, LED_STATUS_OFF_STATE);
    }
  
    // ------ Sound functions -----
    void Beep()
    {
      if (SOUND == ON) onStep.commandBlind(BEEP);
    }
    void Alert() 
    {
      if (SOUND == ON) onStep.commandBlind(ALERT);
    }
      void Click()
    {
      if (SOUND == ON) onStep.commandBlind(CLICK);
    }

    // ------ Direction functions ------
    void MoveNorth() // Move North
    {
        if (!movingNorth)
        {
          movingNorth = ON;
          onStep.commandBlind(":Mn#");
        }
    }
    void MoveSouth() // Move South 
    {
        if (!movingSouth)
        {
          movingSouth = ON;
          onStep.commandBlind(":Ms#");
        }
    }
    void MoveEast() // Move East 
    {
        if (!movingEast)
        {
          movingEast = ON;
          onStep.commandBlind(":Me#");
        }
    }
    void MoveWest() // Move West 
    {
        if (!movingWest)
        {
          movingWest = ON;
          onStep.commandBlind(":Mw#");
        }
    }

    //******************************************************************************
    // HID notification callback handler.
    //******************************************************************************
    static void notifyCallback(
      BLERemoteCharacteristic* pBLERemoteCharacteristic,
      uint8_t* pData,
      size_t length,
      bool isNotify)
    {
      // We are getting the two trigger buttons in the first byte, joyX & joyY in 2nd & 3rd bytes
      // A four byte report is the joystick/trigger buttons.
      // A two byte report is either the A/B buttons or the C/D buttons
      // Low nibble equal to 0x50 indicates A/B buttons.
      if (4 == length)
      {
        // Copy data to VrBoxData
        for (int i = GP_TRIGGERS; i < GP_BTNAB; i++)
          VrBoxData[i] = pData[i];
    
        // Wake up the joystick/trigger buttons handler task
        if (HandleJS)
          vTaskResume(HandleJS);
          
        // Restart the timers
        joyTimer = millis() + JOYTIMEOUT;
      }
      else if (2 == length)
      {
        // Show the received data
        if (0x50 == pData[1])
        {
          // A/B button report, wake the A/B button handler task
          VrBoxData[GP_BTNAB] = pData[0];
          if (HandleAB)
            vTaskResume(HandleAB);
        }
        else
        {
          // C/D button report, wake the C/D/M button handler task
          VrBoxData[GP_BTNCDM] = pData[0];
          if (HandleCDM)
            vTaskResume(HandleCDM);
        }
      }
    }
    // End of notifyCallback

    //******************************************************************************
    // Connection state change event callback handler.
    //******************************************************************************
    class MyClientCallback: public BLEClientCallbacks
    {
      void onConnect(BLEClient* pclient)
      {
        scanTimer = 0;                                      // Reinitalize the scan timer
        LED_STATE_A();
        VLF("bleMSG: BLE GamePad Connected");
      }
      
      void onDisconnect(BLEClient* pclient)
      {
        havedevice = false;
        Connected = false;
        delete myDevice; 
        scanTimer = millis() + SCANTIMER;                  // Restart the scan timer
        LED_STATE_B();
        VLF("bleMSG: BLE GamePad Disconnected");
      }
    };
    // End of MyClientCallback

    //******************************************************************************
    // Connect to a service, register for notifications from Report Characteristics.
    //******************************************************************************
    bool setupCharacteristics(BLERemoteService* pRemoteService, NotifyCallback pNotifyCallback)
    {
      // Get all the characteristics of the service using the handle as the key
      pmapbh = pRemoteService->getCharacteristicsByHandle();
      
      // Only interested in report characteristics that have the notify capability
      for (itrbh = pmapbh->begin(); itrbh != pmapbh->end(); itrbh++)
      {
        BLEUUID x = itrbh->second->getUUID();
        UNUSED(x);
        // The uuid must match the report uuid
        if (ReportCharUUID.equals(itrbh->second->getUUID()))
        {
          // Found a report characteristic
          if (itrbh->second->canNotify())
          {
            // Register for notifications from this characteristic
            itrbh->second->registerForNotify(pNotifyCallback);
          }
        }
      }
      return true;
    }
    // End of setupCharacteristics

    //******************************************************************************
    // Validate the server has the correct name and services we are looking for.
    // The server must have the HID service.
    //******************************************************************************
    bool connectToServer()
    {

      if (pClient == NULL) pClient = BLEDevice::createClient();
      pClient->setClientCallbacks(new MyClientCallback());

      // Connect to the remote BLE Server
      bool _connected = pClient->connect(myDevice); // If you pass BLEAdvertisedDevice instead of address, 
                                                    // it will be recognized type of peer device address (public or private)
      if(!_connected)
      {
        delete pClient;
        delete myDevice;
        return false;
      } 
        
      // BLE servers may offer several services, each with unique characteristics
      // we can identify the type of service by using the service UUID
      // Obtain a reference to the service we are after in the remote BLE server.
      // This will return a pointer to the remote service if it has a matching service UUID
      
      BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
      if (pRemoteService == nullptr)
      {
        pClient->disconnect();
        return false;
      }
      setupCharacteristics(pRemoteService, notifyCallback);

      doConnect = false;
      Connected = true;
      return true; 
    }
    // End of connectToServer

    //******************************************************************************
    // Scan for BLE servers and find the first one that advertises the service we are looking for.
    //******************************************************************************
    class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks
    {
      // Called for each advertising BLE server.
      void onResult(BLEAdvertisedDevice advertisedDevice)
      {
        // We have found a server, see if it has the name we are looking for
        if (!havedevice) 
        {
          if (advertisedDevice.haveName())
          {
            Server_BLE_Address = new BLEAddress(advertisedDevice.getAddress()); 
            Scaned_BLE_Address = Server_BLE_Address->toString().c_str();
            if ((Scaned_BLE_Address == (My_BLE_Address)) || (Scaned_BLE_Address == (My_BLE_Address1)))
            {
              // We found a server with the correct Mac, see if it has the service we are
              // interested in (HID)
              if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID))
              {
                doConnect = true;
                doScan = true;
                havedevice = true;
                scanTimer = 0; // reinitalize the scan timer
                BLEDevice::getScan()->stop();
                myDevice = new BLEAdvertisedDevice(advertisedDevice);
              }           
            } 
          }
        }
      }
    };
    // End of MyAdvertisedDeviceCallbacks

    // All of these tasks are designed to run forever. The tasks are resumed when
    // a notification message is received with new data.
    //******************************************************************************
    // Joystick handler Task.
    // Moving the joystick off center causes this task to be resumed about every
    // 15ms. Press of either trigger button will also resume this task.
    //******************************************************************************
    void taskJoyStick(void *parameter)
    {
      int8_t  x;
      int8_t  y;
      uint8_t triggers;

      // Forever loop
      while(true)
      {
        // give up the CPU, wait for new data
        vTaskSuspend(NULL);

        // We just woke up, new data is available, convert joystick data to
        // signed 8 bit integers
        x = (int8_t)VrBoxData[GP_JOYX];
        y = (int8_t)VrBoxData[GP_JOYY];
        triggers = VrBoxData[GP_TRIGGERS];

        if (!status.aligning) firstrate = true; // Reset for multiple aligns

        // set guide rate = 48X for align
        if (status.aligning && firstrate)
        {
          firstrate = false; // Ignore after starting align to allow rate changes        
          activeGuideRate = 7; // 48X
          char cmd[5] = ":Rn#"; cmd[2] = '0' + activeGuideRate; // set initial GuideRate
          onStep.commandBlind(cmd);
        }

        // Trigger released, stop focuser 
        if (triggerPress && (VrBoxData[GP_TRIGGERS] == 0))
        {
          onStep.commandBlind(FOCUS_STOP);        
          pushTimer = 0;
          triggerPress = false;   
          FocusSpd = false;
          continue;          
        }
        
        if (triggers & GP_LOW_TRIGGER)
        {
        // The lower trigger button is pressed
        if (FocusSpd && triggerPress)
          {
          onStep.commandBlind(FOCUS_HIGH); 
          FocusSpd = false;
          }
        else 
          {
          onStep.commandBlind(FOCUS_LOW);
          pushTimer = millis() + FOCUSTIMER; 
          }
          onStep.commandBlind(FOCUS_IN);         
          triggerPress = true;
          pressedOnce = true;
          continue;
        }
        
        if (triggers & GP_UPR_TRIGGER)
        {
          // The upper trigger button is pressed
          if (FocusSpd && triggerPress)
            {
            onStep.commandBlind(FOCUS_HIGH); 
            FocusSpd = false;
            }
        else 
          {
          onStep.commandBlind(FOCUS_LOW);
          pushTimer = millis() + FOCUSTIMER; 
          }
          onStep.commandBlind(FOCUS_OUT);         
          triggerPress = true;
          pressedOnce = true;
          continue;
        }

        if (pressedOnce)
        {
          pressedOnce = false;
          continue;
        }

        // Joystick has been centered for JOYTIMEOUT ms 
        if (timerReturn) 
        {
          if (movingNorth || movingSouth || movingEast || movingWest)
          {
            if (movingNorth) {movingNorth = false; onStep.commandBlind(":Qn#");}
            if (movingSouth) {movingSouth = false; onStep.commandBlind(":Qs#");}
            if (movingEast) {movingEast = false; onStep.commandBlind(":Qe#");}
            if (movingWest) {movingWest = false; onStep.commandBlind(":Qw#");}
            delay(25);
            if (movingNorth || movingSouth || movingEast || movingWest)
            { 
              onStep.commandBlind(STOP_ALL); 
              delay(50);
            } 
            timerReturn = false; 
            continue;          
          }        
        }

        // Joystick directional moves
        if (y < -JoyStickDeadZone)
        {
          if (NS_SWAP == OFF) MoveNorth();
          else MoveSouth();
        }
        else if (y > JoyStickDeadZone)
        {
          if (NS_SWAP == OFF) MoveSouth(); 
          else MoveNorth();
        }
            
        if (x < -JoyStickDeadZone)
        {
          if (EW_SWAP == OFF) MoveEast();
          else MoveWest();
        }
        else if (x > JoyStickDeadZone)
        {
          if (EW_SWAP == OFF) MoveWest();
          else MoveEast();
        }
      }
    }
    // End of taskJoyStick

    //******************************************************************************
    // A & B Buttons handler Task.
    // Press of either the A or B button will resume this task. 
    //******************************************************************************
    void taskButtonAB(void *parameter)
    {
      uint8_t buttons;
    
      while(true)
      {
        // Give up the CPU, wait for new data
        vTaskSuspend(NULL);
        
        // We just woke up, new data is available
        buttons = VrBoxData[GP_BTNAB];

        if (buttons & GP_BUTTON_A) onStep.commandBlind(GOTO_CURRENT); // Button A pressed, goto last location

        if (buttons & GP_BUTTON_B)
        {
          // Button B pressed, Spiral search
          if (SpiralInProgess == false)
          {
            SpiralInProgess = true;
            onStep.commandBlind(SPIRAL);
          }
          else
          {
            SpiralInProgess = false; 
            onStep.commandBlind(STOP_ALL);
          }
        }
      }
    } 
    //  End of taskButtonAB

    //******************************************************************************
    // C, D & M Buttons handler Task. 
    // Press of either the C, D or M button will resume this task. 
    //******************************************************************************
    void taskButtonCDM(void *parameter)
    {
      uint8_t buttons;
      
      while(true)
      {
        // Give up the CPU
        vTaskSuspend(NULL);

        // we just woke up, new data is available
        buttons = VrBoxData[GP_BTNCDM];

        if (CDMpressedOnce) // Ignore first callback 
        {
          CDMpressedOnce = false;        
          continue;
        }

        if (buttons & GP_BUTTON_C)
        {
          // button C pressed
          CDMpressedOnce = true; 
          (activeGuideRate--);
          if (activeGuideRate > 2) Click(); 
          if (activeGuideRate < 3) activeGuideRate = 3;
          if (activeGuideRate > 9) activeGuideRate = 9;
          char cmd[5] = ":Rn#"; cmd[2] = '0' + activeGuideRate;
          onStep.commandBlind(cmd);
        }

        if (buttons & GP_BUTTON_D)
        {
          // Button D pressed
          CDMpressedOnce = true; 
          (activeGuideRate++);
          if (activeGuideRate < 10) Click();
          if (activeGuideRate < 3) activeGuideRate = 3;
          if (activeGuideRate > 9) activeGuideRate = 9;
          char cmd[5] = ":Rn#"; cmd[2] = '0' + activeGuideRate;
          onStep.commandBlind(cmd);
        } 

        if (GP_BUTTON_M == buttons) // Button M pressed, conditional response to state
        {
          // Emergency stop
          if (status.inGoto || movingNorth || movingSouth || movingEast || movingWest)
          {
            onStep.commandBlind(STOP_ALL);
            continue;
          }
          // Set align point when doing an alignment
          else if (status.aligning)
          {
            onStep.commandBlind(ALIGN);
            Click(); 
            continue;
          }
          // Unpark mount if parked        
          else if (status.parked) 
          {
            onStep.commandBlind(UNPARK);
            StartFocus();          
            Beep();          
          }
          // Start tracking if "At home"        
          else if (status.atHome && !status.tracking)
          {
            onStep.commandBlind(TRACK_ON);
            StartFocus();
            Alert();                   
          }
          // Start tracking if tracking off
          else if (!status.tracking)
          {
            onStep.commandBlind(TRACK_ON);
            Beep();          
          }
          // Park the scope                
          else if (!status.parked)
          {
            ParkFocus();
            onStep.commandBlind(PARK);                
          }
        }
      } 
    }
    //  End of taskButtonCDM

    //******************************************************************************
    // Initial and Service routines
    //******************************************************************************

    void bleInit()
    {
      // Enable BT ble only (no classic)
      esp_bt_controller_enable(ESP_BT_MODE_BLE);
      // Free memory used by BT Classic to prevent out of memory crash
      esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);
    }

    void DoScan() 
    {
      if (scanTimer && (scanTimer < millis()))
      {
        // Retrieve a GATT Scanner and set the callback we want to use to be informed 
        // when we have detected a new device.  Specify that we want active scanning
        // and start the scan.
      
        BLEScan* pBLEScan = BLEDevice::getScan();
        pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
        pBLEScan->setInterval(1349);
        pBLEScan->setWindow(499);
        pBLEScan->setActiveScan(true);
        pBLEScan->start(1); // scan 1 second
          
        pBLEScan->clearResults(); // Delete results fromBLEScan buffer to release memory, solves heap problem
      
        // Restart the scan timer
        scanTimer = millis() + SCANTIMER;
        VLF("bleMSG: swsMSG: Scanning for BLE GamePad");
      }
    }
    // End of DoScan.

    //******************************************************************************
    // This is the first (and after disconnect) server connection
    //******************************************************************************
    void bleConnTest() 
    {
      // If the flag "doConnect" is true then we have scanned for and found the desired
      // BLE Server with which we wish to connect.  Now we connect to it.

      if (doConnect == true) connectToServer(); 
      else
      {
        doConnect = false;
        doScan = true;
        DoScan();
      }
    }
    // End of bleConnTest

    void bleSetup()
    {
      My_BLE_Address = BLE_GP_ADDR;
      My_BLE_Address1 = BLE_GP_ADDR1;
      joyTimer = 0;  
      pushTimer = 0;
      activeGuideRate = status.guideRate;
      BaseType_t xReturned;

      VLF("bleMSG: V2.0 Mode D"); 
      VLF("bleMSG: Starting BLE GamePad Services"); 

      // Create tasks to handle the joystick and buttons
      xReturned = xTaskCreate(taskJoyStick,             // task to handle activity on the joystick.
                              "Joystick",               // String with name of task.
                              TaskStackSize,            // Stack size in 32 bit words.
                              NULL,                     // Parameter passed as input of the task
                              1,                        // Priority of the task.
                              &HandleJS);               // Task handle.
    
      xReturned = xTaskCreate(taskButtonAB,             // task to handle activity on the A & B buttons.
                              "ButtonsAB",              // String with name of task.
                              TaskStackSize,            // Stack size in 32 bit words.
                              NULL,                     // Parameter passed as input of the task
                              1,                        // Priority of the task.
                              &HandleAB);               // Task handle.

      xReturned = xTaskCreate(taskButtonCDM,             // task to handle activity on the C, D & M buttons.
                              "ButtonsCDM",              // String with name of task.
                              TaskStackSize,            // Stack size in 32 bit words.
                              NULL,                     // Parameter passed as input of the task
                              1,                        // Priority of the task.
                              &HandleCDM);               // Task handle.

      UNUSED(xReturned);

      BLEDevice::init("");

      if (!Connected)
      {
        // Retrieve a GATT Scanner and set the callback we want to use to be informed 
        // when we have detected a new device.  Specify that we want active scanning
        // and start the scan.
        
        BLEScan* pBLEScan = BLEDevice::getScan();
        pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
        pBLEScan->setInterval(1349);
        pBLEScan->setWindow(150);  
        pBLEScan->setActiveScan(true);
        pBLEScan->start(2, false); // Scan
        delay(3000); // Delay for scan
      }

      if (!Connected)
      {
        doScan = true;
        scanTimer = millis() + SCANTIMER; // Restart the scan timer
      }

      bleConnTest();     
    }
    // End of bleSetup

    void bleTimers() 
    {
      if (Connected)
      {
        if (joyTimer && (joyTimer < millis()) && (movingNorth || movingSouth || movingEast || movingWest))
        { 
          // No joystick notification for JOYTIMEOUT mS, center the joystick
          timerReturn = true;
          joyTimer = 0; 
          VrBoxData[GP_JOYX] = VrBoxData[GP_JOYY] = 0;               
          vTaskResume(HandleJS);
        }
              
        if (pushTimer && (pushTimer < millis()) && triggerPress)
        {
          // Focus button held down for FOCUSTIMER, speed up focuser
          FocusSpd = true;
          timerReturn = true;
          pushTimer = 0;                  
          vTaskResume(HandleJS);
        }
        else FocusSpd = false;
      }
    }
    // End of bleTimers

  #endif // End of mode D code
#endif 
