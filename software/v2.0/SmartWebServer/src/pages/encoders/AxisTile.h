// -----------------------------------------------------------------------------------
// Axis tile
#pragma once

#include "../htmlHeaders.h"
#include "../htmlMessages.h"
#include "../htmlScripts.h"

extern void encAxisTile(int axis, String &data);
extern void encAxisTileAjax(String &data);
extern void encAxisTileGet();

const char html_encoderAngle[] PROGMEM =
  L_ENCODER ": <span id='enc_a%d' class='c'>%s</span><br />";

const char html_mountAngle[] PROGMEM =
  "OnStep: <span id='raw_a%d' class='c'>%s</span> (&Delta; <span id='diff_a%d' class='c'>%s</span>)<br /><br />";

const char html_indexDriverStatus[] PROGMEM =
  L_DRIVER " " L_STATUS ": <span id='dvr_stat%d' class='c'>%s</span><br />";

const char html_encAxisValueTpd[] PROGMEM =
  L_ENC_SET_TPD ": <span class='c'>%s</span><br />\n";

const char html_encAxisValueReverse[] PROGMEM =
  L_ADV_SET_REV ": <span class='c'>%s</span><br />\n";

const char html_encAxisValueSyncThreshold[] PROGMEM =
  L_ENC_SYNC_THLD ": <span class='c'>%ld arc-sec</span><br/>\n";

const char html_encFormBegin[] PROGMEM =
  "<form method='get' action='/enc.htm'>\n";

const char html_encFormEnd[] PROGMEM =
  "</form><br />\n";

const char html_encAxisTpd[] PROGMEM =
  L_ENC_SET_TPD ":<br />"
  "<input style='width: 7em;' name='a%dcpd' value='%s' type='number' min='%d' max='%ld' step='any'><br />\n";

const char html_encAxisReverse[] PROGMEM =
   L_ADV_SET_REV ", " L_ADV_BOOL ":<br />"
  "<input style='width: 7em;' name='a%drev' value='%d' type='number' min='0' max='1' step='1'><br />\n";

const char html_encAxisSyncThreshold[] PROGMEM =
  L_ENC_SYNC_THLD ", " L_ENC_SYNC_THLD_UNITS ":<br />"
  "<input style='width: 7em;' name='a%dths' value='%ld' type='number' min='0' max='9999'><br/>\n";

const char html_encUpload[] PROGMEM =
  "<button type='submit'>" L_UPLOAD "</button>\n";

const char html_encAxisRevert[] PROGMEM =
  "<button name='revert' value='%d' type='submit'>" L_REVERT "</button>\n";
