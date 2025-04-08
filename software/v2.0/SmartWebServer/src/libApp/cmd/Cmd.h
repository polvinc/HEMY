// -----------------------------------------------------------------------------------
// Help with commands, etc.
#pragma once

#include "../../Common.h"

enum CommandErrors {
  CE_NONE, CE_0, CE_CMD_UNKNOWN, CE_REPLY_UNKNOWN, CE_PARAM_RANGE, CE_PARAM_FORM,
  CE_ALIGN_FAIL, CE_ALIGN_NOT_ACTIVE, CE_NOT_PARKED_OR_AT_HOME, CE_PARKED,
  CE_PARK_FAILED, CE_NOT_PARKED, CE_NO_PARK_POSITION_SET, CE_GOTO_FAIL, CE_LIBRARY_FULL,
  CE_GOTO_ERR_BELOW_HORIZON, CE_GOTO_ERR_ABOVE_OVERHEAD, CE_SLEW_ERR_IN_STANDBY, 
  CE_SLEW_ERR_IN_PARK, CE_GOTO_ERR_GOTO, CE_GOTO_ERR_OUTSIDE_LIMITS, CE_SLEW_ERR_HARDWARE_FAULT,
  CE_MOUNT_IN_MOTION, CE_GOTO_ERR_UNSPECIFIED, CE_NULL};

class OnStepCmd {
  public:
    // start serial comms at the specified rate and with swapping enabled or disabled or automatic
    void serialBegin(long baudRate, int swap);

    // returns OnStep command to set a given baud rate
    const char* highSpeedCommsStr(long baud);

    // flushes both the send and recv. buffers
    char serialRecvFlush();

    // sends 3x empty frames to OnStep to purge noise that might intefere with commands on the interface 
    void clearSerialChannel();

    // low level smart LX200 aware command and response (up to 80 chars) over serial (includes any '#' frame char)
    bool processCommand(const char* cmd, char* response, long timeOutMs);

    // send command to OnStep and get any response (up 80 chars, no '#' frame char)
    bool command(const char* command, char* response);

    // send command to OnStep, expects no reply
    bool commandBlind(const char* command);

    // send command to OnStep for debugging, expects a boolean reply
    bool commandEcho(const char* command);

    // send command to OnStep, expects a boolean reply
    bool commandBool(const char* command);

    // send command to OnStep, expects a string reply (no '#' frame char)
    char *commandString(const char* command);

    // turns OnStep command error number into descriptive string
    char* commandErrorToStr(int e);

  private:
};

// timeout period for the web
extern int webTimeout;

// timeout period for the command channel(s)
extern int cmdTimeout;

extern OnStepCmd onStep;
