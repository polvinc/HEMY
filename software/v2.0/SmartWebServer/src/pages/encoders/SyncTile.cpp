// -----------------------------------------------------------------------------------
// Encoder Sync tile
#include "SyncTile.h"

#include "../../lib/nv/Nv.h"

#include "../KeyValue.h"
#include "../Pages.common.h"

#if ENCODERS == ON

// create the related webpage tile
void syncTile(String &data)
{
  char temp[240] = "";

  sprintf_P(temp, html_tile_beg, "22em", "15em", "Sync Control");
  data.concat(temp);
  data.concat(F("<br /><hr>"));

  sprintf_P(temp, html_form_begin, "enc.htm");
  data.concat(temp);

  data.concat(FPSTR(html_syncOnStepNow));
  data.concat(FPSTR(html_syncEncodersNow));

  data.concat(FPSTR(html_syncAuto));
  www.sendContentAndClear(data);

  data.concat(F("<br />"));

  #ifdef ENC_ABSOLUTE
    data.concat(FPSTR(html_zeroEncodersNow));
    www.sendContentAndClear(data);
  #endif

  data.concat(F("<hr>"));

  data.concat(FPSTR(html_form_end));

  data.concat(FPSTR(html_tile_end));
  www.sendContentAndClear(data);
}

// use Ajax key/value pairs to pass related data to the web client in the background
void syncTileAjax(String &data)
{
  data.concat(keyValueToggleBoolSelected("enc_as_on", "enc_as_off", encoders.settings.autoSync));
  www.sendContentAndClear(data);
}

// pass related data back to OnStep
extern void syncTileGet()
{
  String v;

  v = www.arg("enc");
  if (!v.equals(EmptyStr))
  {
    if (v.equals("o2e")) encoders.syncFromOnStep();
    if (v.equals("e2o")) encoders.syncToOnStep();
    #ifdef ENC_ABSOLUTE
      if (v.equals("zro")) encoders.originFromOnStep();
    #endif
    if (v.equals("on")) encoders.settings.autoSync = true;
    if (v.equals("off")) encoders.settings.autoSync = false;
    nv.updateBytes(NV_ENCODER_SETTINGS_BASE, &encoders.settings, sizeof(EncoderSettings));
  }
}

#endif
