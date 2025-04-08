// -------------------------------------------------------------------------------------------------------
// Handle encoders, both CW/CCW and Quadrature A/B types are supported
#pragma once

#include "../../Common.h"

#if AXIS1_ENCODER > 0 && AXIS2_ENCODER > 0
  #define ENCODERS ON
#else
  #define ENCODERS OFF
#endif

#if AXIS1_ENCODER == AS37_H39B_B || AXIS1_ENCODER == JTW_24BIT || AXIS1_ENCODER == JTW_26BIT || \
    AXIS1_ENCODER == LIKA_ASC85
  #define AXIS1_ENCODER_ABSOLUTE
#endif
#if AXIS2_ENCODER == AS37_H39B_B || AXIS2_ENCODER == JTW_24BIT || AXIS2_ENCODER == JTW_26BIT || \
    AXIS2_ENCODER == LIKA_ASC85
  #define AXIS2_ENCODER_ABSOLUTE
#endif
#if defined(AXIS1_ENCODER_ABSOLUTE) || defined(AXIS2_ENCODER_ABSOLUTE)
  #define ENC_ABSOLUTE
#endif

// encoder polling rate in milli-seconds
#ifndef ENCODER_POLLING_RATE_MS
  #define ENCODER_POLLING_RATE_MS 500
#endif

// ----------------------------------------------------------------------------------------------------------------
// background process position/rate control for encoders 

#define EncoderSettingsSize 72
typedef struct EncoderAxis {
  uint32_t zero;
  uint32_t offset;
  int32_t diffTo;
  double ticksPerDeg;
  int16_t reverse;
} EncoderAxis;

typedef struct EncoderSettings {
    bool autoSync;
    EncoderAxis axis1;
    EncoderAxis axis2;
} EncoderSettings;

class Encoders {
  public:
    // prepare encoders for operation, init NV if necessary
    void init();

    EncoderSettings settings = {
      #if ENC_AUTO_SYNC_DEFAULT == ON
        true,
      #else
        false,
      #endif
      {0, 0, AXIS1_ENCODER_DIFF_LIMIT_TO, AXIS1_ENCODER_TICKS_DEG, AXIS1_ENCODER_REVERSE},
      {0, 0, AXIS2_ENCODER_DIFF_LIMIT_TO, AXIS2_ENCODER_TICKS_DEG, AXIS2_ENCODER_REVERSE}
    };

    #if ENCODERS == ON
      // automatically sync the encoders from OnStep's position when at home or parked
      void syncFromOnStep(bool force = false);

      // zero absolute encoders from OnStep's position
      #ifdef ENC_ABSOLUTE
        void originFromOnStep();
      #endif

      // automatically sync OnStep to the encoders' position
      void syncToOnStep();
      void poll();

      double getAxis1();
      double getAxis2();
      bool   validAxis1();
      bool   validAxis2();
      double getOnStepAxis1();
      double getOnStepAxis2();

      int32_t Axis1EncDiffFrom    = AXIS1_ENCODER_DIFF_LIMIT_FROM;
      int32_t Axis1EncDiffAbs     = 0;
      int32_t Axis2EncDiffFrom    = AXIS2_ENCODER_DIFF_LIMIT_FROM;
      int32_t Axis2EncDiffAbs     = 0;

    private:
      double osAxis1      = 0;
      double osAxis2      = 0;
      double enAxis1      = 0;
      double enAxis2      = 0;
      bool   enAxis1Fault = false;
      bool   enAxis2Fault = false;

    #endif
};

extern Encoders encoders;
