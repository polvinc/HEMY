// -----------------------------------------------------------------------------------
// Axis tile
#include "AxisTile.h"

#include "../../lib/nv/Nv.h"

#include "../KeyValue.h"
#include "../Pages.common.h"

#if ENCODERS == ON

// create the related webpage tile
void encAxisTile(int axis, String &data)
{
  char temp[240] = "";
  char ticksPerDegStr[32] = "";
  float ticksPerDeg = 0;
  bool reverse = false;
  long diffTo = 0;

  if (axis == 0)
  {
    ticksPerDeg = encoders.settings.axis1.ticksPerDeg;
    reverse = encoders.settings.axis1.reverse == ON;
    diffTo = encoders.settings.axis1.diffTo;
    sprintf_P(temp, html_tile_text_beg, "22em", "15em", "Axis1 RA/Azm");
  }
  else if (axis == 1)
  {
    ticksPerDeg = encoders.settings.axis2.ticksPerDeg;
    reverse = encoders.settings.axis2.reverse == ON;
    diffTo = encoders.settings.axis2.diffTo;
    sprintf_P(temp, html_tile_text_beg, "22em", "15em", "Axis2 Dec/Alt");
  }
  data.concat(temp);
  data.concat(F("<br /><hr>"));

  if (axis == 0)
  {
    sprintf_P(temp, html_encoderAngle, 1, state.encAngleAxis1Str);
    data.concat(temp);
    sprintf_P(temp, html_mountAngle, 1, state.angleAxis1Str, 1, state.diffAngleAxis1Str);
    data.concat(temp);
  }
  else if (axis == 1)
  {
    sprintf_P(temp, html_encoderAngle, 2, state.encAngleAxis2Str);
    data.concat(temp);
    sprintf_P(temp, html_mountAngle, 2, state.angleAxis2Str, 2, state.diffAngleAxis2Str);
    data.concat(temp);
  }

  dtostrf(ticksPerDeg, 1, 3, ticksPerDegStr);
  stripNum(ticksPerDegStr);

  sprintf_P(temp, html_encAxisValueTpd, ticksPerDegStr);
  data.concat(temp);
  sprintf_P(temp, html_encAxisValueReverse, reverse ? "true" : "false");
  data.concat(temp);
  sprintf_P(temp, html_encAxisValueSyncThreshold, diffTo);
  data.concat(temp);
  www.sendContentAndClear(data);

  data.concat(F("<hr>"));

  sprintf_P(temp, html_collapsable_beg, L_SETTINGS "...");
  data.concat(temp);
  data.concat(FPSTR(html_encFormBegin));

  sprintf_P(temp, html_encAxisTpd, axis + 1, ticksPerDegStr, 1, 100000L);
  data.concat(temp);
  sprintf_P(temp, html_encAxisReverse, axis + 1, reverse ? 1 : 0);
  data.concat(temp);
  sprintf_P(temp, html_encAxisSyncThreshold, axis + 1, diffTo);
  data.concat(temp);
  www.sendContentAndClear(data);

  data.concat(FPSTR(html_encUpload));
  sprintf_P(temp, html_encAxisRevert, axis + 1);
  data.concat(temp);

  data.concat(FPSTR(html_encFormEnd));
  data.concat(FPSTR(html_collapsable_end));
  data.concat(FPSTR(html_tile_end));
  www.sendContentAndClear(data);
}

// use Ajax key/value pairs to pass related data to the web client in the background
void encAxisTileAjax(String &data)
{
  data.concat(keyValueString("diff_a1", state.diffAngleAxis1Str));
  data.concat(keyValueString("enc_a1", state.encAngleAxis1Str));
  data.concat(keyValueString("raw_a1", state.angleAxis1Str));
  data.concat(keyValueString("diff_a2", state.diffAngleAxis2Str));
  data.concat(keyValueString("enc_a2", state.encAngleAxis2Str));
  data.concat(keyValueString("raw_a2", state.angleAxis2Str));
  www.sendContentAndClear(data);
}

// pass related data back to OnStep
extern void encAxisTileGet()
{
  String v;
  bool updateNV = false;

  // Max. limits
  v = www.arg("a1ths");
  if (!v.equals(EmptyStr))
  {
    int16_t i;
    if (convert.atoi2((char *)v.c_str(), &i) && (i >= 0 && i <= 9999))
    {
      encoders.settings.axis1.diffTo = i;
      updateNV = true;
    }
  }
  v = www.arg("a2ths");
  if (!v.equals(EmptyStr))
  {
    int16_t i;
    if (convert.atoi2((char *)v.c_str(), &i) && (i >= 0 && i <= 9999))
    {
      encoders.settings.axis2.diffTo = i;
      updateNV = true;
    }
  }

  // Counts per degree
  v = www.arg("a1cpd");
  if (!v.equals(EmptyStr))
  {
    double d = v.toFloat();
    if (d >= 1.0 && d <= 1000000.0)
    {
      encoders.settings.axis1.ticksPerDeg = d;
      updateNV = true;
    }
  }
  v = www.arg("a2cpd");
  if (!v.equals(EmptyStr))
  {
    double d = v.toFloat();
    if (d >= 1.0 && d <= 1000000.0)
    {
      encoders.settings.axis2.ticksPerDeg = d;
      updateNV = true;
    }
  }

  v = www.arg("a1rev");
  if (!v.equals(EmptyStr))
  {
    if (v.equals("0"))
    {
      encoders.settings.axis1.reverse = OFF;
      updateNV = true;
    }
    if (v.equals("1"))
    {
      encoders.settings.axis1.reverse = ON;
      updateNV = true;
    }
  }

  v = www.arg("a2rev");
  if (!v.equals(EmptyStr))
  {
    if (v.equals("0"))
    {
      encoders.settings.axis2.reverse = OFF;
      updateNV = true;
    }
    if (v.equals("1"))
    {
      encoders.settings.axis2.reverse = ON;
      updateNV = true;
    }
  }

  v = www.arg("revert");
  if (!v.equals(EmptyStr))
  {
    if (v.equals("1"))
    {
      encoders.settings.axis1.ticksPerDeg = AXIS1_ENCODER_TICKS_DEG;
      encoders.settings.axis1.reverse = AXIS1_ENCODER_REVERSE;
      encoders.settings.axis1.diffTo = AXIS1_ENCODER_DIFF_LIMIT_TO;
      updateNV = true;
    }
    if (v.equals("2"))
    {
      encoders.settings.axis2.ticksPerDeg = AXIS2_ENCODER_TICKS_DEG;
      encoders.settings.axis2.reverse = AXIS1_ENCODER_REVERSE;
      encoders.settings.axis2.diffTo = AXIS2_ENCODER_DIFF_LIMIT_TO;
      updateNV = true;
    }
  }

  if (updateNV)
  {
    nv.writeBytes(NV_ENCODER_SETTINGS_BASE, &encoders.settings, sizeof(EncoderSettings));
  }
}

#endif
