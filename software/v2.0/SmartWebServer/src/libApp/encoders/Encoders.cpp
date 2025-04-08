// -------------------------------------------------------------------------------------------------------
// Handle encoders, both CW/CCW and Quadrature A/B types are supported

#include "Encoders.h"

#include "../../lib/tasks/OnTask.h"
#include "../../lib/nv/Nv.h"

#if ENCODERS == ON
  #include "../../lib/convert/Convert.h"

  #include "../status/Status.h"
  #include "../cmd/Cmd.h"
  #include "../misc/Misc.h"

  #if defined(ESP8266) || defined(ESP32)
    #include <Esp.h>
  #endif

  // bring in support for the various encoder types
  #include "../../lib/encoder/quadrature/Quadrature.h"
  #include "../../lib/encoder/quadratureEsp32/QuadratureEsp32.h"
  #include "../../lib/encoder/cwCcw/CwCcw.h"
  #include "../../lib/encoder/pulseDir/PulseDir.h"
  #include "../../lib/encoder/bissc/As37h39bb.h"
  #include "../../lib/encoder/bissc/Jtw24.h"
  #include "../../lib/encoder/bissc/Jtw26.h"

  void pollEncoders() { encoders.poll(); }

  #if AXIS1_ENCODER == AB
    Quadrature encAxis1(AXIS1_ENCODER_A_PIN, AXIS1_ENCODER_B_PIN, 1);
  #elif AXIS1_ENCODER == AB_ESP32
    QuadratureEsp32 encAxis1(AXIS1_ENCODER_A_PIN, AXIS1_ENCODER_B_PIN, 1);
  #elif AXIS1_ENCODER == CW_CCW
    CwCcw encAxis1(AXIS1_ENCODER_A_PIN, AXIS1_ENCODER_B_PIN, 1);
  #elif AXIS1_ENCODER == PULSE_DIR
    PulseDir encAxis1(AXIS1_ENCODER_A_PIN, AXIS1_ENCODER_B_PIN, 1);
  #elif AXIS1_ENCODER == AS37_H39B_B
    As37h39bb encAxis1(AXIS1_ENCODER_A_PIN, AXIS1_ENCODER_B_PIN, 1);
  #elif AXIS1_ENCODER == JTW_24BIT
    Jtw24 encAxis1(AXIS1_ENCODER_A_PIN, AXIS1_ENCODER_B_PIN, 1);
  #elif AXIS1_ENCODER == JTW_26BIT
    Jtw26 encAxis1(AXIS1_ENCODER_A_PIN, AXIS1_ENCODER_B_PIN, 1);
  #elif AXIS1_ENCODER == LIKA_ASC85
    LikaAsc85 encAxis1(AXIS1_ENCODER_A_PIN, AXIS1_ENCODER_B_PIN, 1);
  #endif

  #if AXIS2_ENCODER == AB
    Quadrature encAxis2(AXIS2_ENCODER_A_PIN, AXIS2_ENCODER_B_PIN, 2);
  #elif AXIS2_ENCODER == AB_ESP32
    QuadratureEsp32 encAxis2(AXIS2_ENCODER_A_PIN, AXIS2_ENCODER_B_PIN, 2);
  #elif AXIS2_ENCODER == CW_CCW
    CwCcw encAxis2(AXIS2_ENCODER_A_PIN, AXIS2_ENCODER_B_PIN, 2);
  #elif AXIS2_ENCODER == PULSE_DIR
    PulseDir encAxis2(AXIS2_ENCODER_A_PIN, AXIS2_ENCODER_B_PIN, 2);
  #elif AXIS2_ENCODER == AS37_H39B_B
    As37h39bb encAxis2(AXIS2_ENCODER_A_PIN, AXIS2_ENCODER_B_PIN, 2);
  #elif AXIS2_ENCODER == JTW_24BIT
    Jtw24 encAxis2(AXIS2_ENCODER_A_PIN, AXIS2_ENCODER_B_PIN, 2);
  #elif AXIS2_ENCODER == JTW_26BIT
    Jtw26 encAxis2(AXIS2_ENCODER_A_PIN, AXIS2_ENCODER_B_PIN, 2);
  #elif AXIS2_ENCODER == LIKA_ASC85
    LikaAsc85 encAxis2(AXIS2_ENCODER_A_PIN, AXIS2_ENCODER_B_PIN, 2);
  #endif
#endif

// ----------------------------------------------------------------------------------------------------------------
// background process position/rate control for encoders 

void Encoders::init() { 
  // confirm the data structure size
  if (EncoderSettingsSize < sizeof(EncoderSettings)) { nv.initError = true; DL("ERR: Encoders::init(); EncoderSettingsSize error NV subsystem writes disabled"); }

  // write the default settings to NV
  if (!nv.hasValidKey()) {
    VLF("MSG: Encoders, writing defaults to NV");
    nv.writeBytes(NV_ENCODER_SETTINGS_BASE, &settings, sizeof(EncoderSettings));
  }

  // read the settings
  nv.readBytes(NV_ENCODER_SETTINGS_BASE, &settings, sizeof(EncoderSettings));

  #if ENCODERS == ON
    encAxis1.init();
    encAxis2.init();

    #ifdef ENC_ABSOLUTE
      encAxis1.setOrigin(settings.axis1.zero);
      encAxis2.setOrigin(settings.axis2.zero);
      encAxis1.offset = settings.axis1.offset;
      encAxis2.offset = settings.axis2.offset;
    #endif

    VF("MSG: Encoders, start polling task (priority 4)... ");
    if (tasks.add(ENCODER_POLLING_RATE_MS, 0, true, 4, pollEncoders, "EncPoll")) { VLF("success"); } else { VLF("FAILED!"); }
  #endif
}

#if ENCODERS == ON
  void Encoders::syncFromOnStep(bool force) {
    if (Axis1EncDiffFrom == OFF || force || fabs(osAxis1 - enAxis1) <= (double)(Axis1EncDiffFrom/3600.0)) {
      encAxis1.write(settings.axis1.reverse == ON ? -osAxis1*settings.axis1.ticksPerDeg : osAxis1*settings.axis1.ticksPerDeg);
      settings.axis1.offset = encAxis1.offset;
    }
    if (Axis2EncDiffFrom == OFF || force || fabs(osAxis2 - enAxis2) <= (double)(Axis2EncDiffFrom/3600.0)) {
      encAxis2.write(settings.axis2.reverse == ON ? -osAxis2*settings.axis2.ticksPerDeg : osAxis2*settings.axis2.ticksPerDeg);
      settings.axis2.offset = encAxis2.offset;
    }
    nv.updateBytes(NV_ENCODER_SETTINGS_BASE, &settings, sizeof(EncoderSettings));
  }

  #ifdef ENC_ABSOLUTE
    void Encoders::originFromOnStep() {
      encAxis1.origin = 0;
      encAxis2.origin = 0;
      encAxis1.offset = 0;
      encAxis2.offset = 0;

      settings.axis1.zero = (uint32_t)(-encAxis1.read());
      settings.axis2.zero = (uint32_t)(-encAxis2.read());
      encAxis1.setOrigin(settings.axis1.zero);
      encAxis2.setOrigin(settings.axis2.zero);

      syncFromOnStep(true);
      settings.axis1.offset = encAxis1.offset;
      settings.axis2.offset = encAxis2.offset;

      nv.updateBytes(NV_ENCODER_SETTINGS_BASE, &settings, sizeof(EncoderSettings));
    }
  #endif

  void Encoders::syncToOnStep() {
    char cmd[60], cmd1[30];
    if (status.getVersionMajor() >= 10) {
      sprintF(cmd, ":SX44,%0.6f,", enAxis1);
      sprintF(cmd1, "%0.6f#", enAxis2); // 28
      strcat(cmd, cmd1);
      onStep.commandBool(cmd);
    } else {
      sprintF(cmd, ":SX40,%0.6f#", enAxis1); // 17
      onStep.commandBool(cmd);
      sprintF(cmd, ":SX41,%0.6f#", enAxis2); // 17
      onStep.commandBool(cmd);
      onStep.commandBool(":SX42,1#"); // 8
    }
  }

  // check encoders and auto sync OnStep if diff is too great
  void Encoders::poll() {
    char *conv_end;

    if (status.mountFound != SD_TRUE) return;

    char result[80];
    if (onStep.command(":GX42#", result) && strlen(result) > 1) {
      double f = strtod(result, &conv_end);
      if (&result[0] != conv_end && f >= -999.9 && f <= 999.9) osAxis1 = f;
    }
    if (onStep.command(":GX43#", result) && strlen(result) > 1) {
      double f = strtod(result, &conv_end);
      if (&result[0] != conv_end && f >= -999.9 && f <= 999.9) osAxis2 = f;
    }

    long pos = encAxis1.read();
    if (pos == INT32_MAX) { enAxis1Fault = true; pos = 0; } else enAxis1Fault = false;
    enAxis1 = (double)pos/settings.axis1.ticksPerDeg;
    if (settings.axis1.reverse == ON) enAxis1 = -enAxis1;

    pos = encAxis2.read();
    if (pos == INT32_MAX) { enAxis2Fault = true; pos = 0; } else enAxis2Fault = false;
    enAxis2 = (double)pos/settings.axis2.ticksPerDeg;
    if (settings.axis2.reverse == ON) enAxis2 = -enAxis2;

    // determine if goto pointing correction is allowed
    bool syncDuringGoto = false;
    if (ENC_SYNC_DURING_GOTO == ON && status.getVersionMajor() * 100 + status.getVersionMinor() >= 1015) syncDuringGoto = true;

    if (settings.autoSync && status.onStepFound && !enAxis1Fault && !enAxis2Fault) {
      if (
          #ifdef ENC_ABSOLUTE
            (status.getVersionMajor() * 100 + status.getVersionMinor() < 1015 && (status.atHome || status.parked)) ||
          #else
            (status.atHome || status.parked) ||
          #endif
          status.syncToEncodersOnly || (!syncDuringGoto && status.aligning))
      {
        syncFromOnStep();
        if (status.syncToEncodersOnly) onStep.commandBool(":SX43,1#");
      } else
      if ((syncDuringGoto || !status.inGoto) && !status.guiding)
      {
        if ((fabs(osAxis1 - enAxis1) > (double)(settings.axis1.diffTo/3600.0)) ||
            (fabs(osAxis2 - enAxis2) > (double)(settings.axis2.diffTo/3600.0))) syncToOnStep();
      }
    }
  }

  double Encoders::getAxis1() { return enAxis1; }
  double Encoders::getAxis2() { return enAxis2; }
  bool   Encoders::validAxis1() { return !enAxis1Fault; }
  bool   Encoders::validAxis2() { return !enAxis2Fault; }
  double Encoders::getOnStepAxis1() { return osAxis1; }
  double Encoders::getOnStepAxis2() { return osAxis2; }
#endif

Encoders encoders;

