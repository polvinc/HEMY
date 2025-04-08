// update encoder state
#include "State.h"

#include "Status.h"
#include "../../lib/tasks/OnTask.h"
#include "../../libApp/cmd/Cmd.h"
#include "../../locales/Locale.h"
#include "../../lib/convert/Convert.h"
#include "../../libApp/encoders/Encoders.h"

void State::updateEncoders(bool now)
{
  #if ENCODERS == ON
    if (!now && millis() - lastMountPageLoadTime > 2000) return;

    char temp[80];

    // RA,Dec OnStep and Encoder positions
    double f;
    f = encoders.getOnStepAxis1();
    convert.doubleToDms(temp, f, true, true, PM_HIGH);
    strncpyex(angleAxis1Str, temp, 14);
    formatDegreesStr(angleAxis1Str);
    if (encoders.validAxis1())
    {
      f = encoders.getAxis1();
      convert.doubleToDms(temp, f, true, true, PM_HIGH);
      strncpyex(encAngleAxis1Str, temp, 14);
      formatDegreesStr(encAngleAxis1Str);

      f = encoders.getOnStepAxis1() - encoders.getAxis1();
      convert.doubleToDms(temp, f, true, true, PM_HIGHEST);
      strncpyex(diffAngleAxis1Str, temp, 18);
      formatDegreesStr(diffAngleAxis1Str);
    }
    else
    {
      strcpy(temp, " ** " L_FAULT " ** ");
      strncpyex(encAngleAxis1Str, temp, 14);
      strncpyex(diffAngleAxis1Str, temp, 18);
    }
    Y;

    f = encoders.getOnStepAxis2();
    convert.doubleToDms(temp, f, true, true, PM_HIGH);
    strncpyex(angleAxis2Str, temp, 14);
    formatDegreesStr(angleAxis2Str);
    if (encoders.validAxis2())
    {
      f = encoders.getAxis2();
      convert.doubleToDms(temp, f, true, true, PM_HIGH);
      strncpyex(encAngleAxis2Str, temp, 14);
      formatDegreesStr(encAngleAxis2Str);

      f = encoders.getOnStepAxis2() - encoders.getAxis2();
      convert.doubleToDms(temp, f, true, true, PM_HIGHEST);
      strncpyex(diffAngleAxis2Str, temp, 18);
      formatDegreesStr(diffAngleAxis2Str);
    }
    else
    {
      strcpy(temp, " ** " L_FAULT " ** ");
      strncpyex(encAngleAxis2Str, temp, 14);
      strncpyex(diffAngleAxis2Str, temp, 18);
    }
    Y;
  #else
    UNUSED(now);
  #endif
}
