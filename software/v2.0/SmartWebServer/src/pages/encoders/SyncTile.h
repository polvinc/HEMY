// -----------------------------------------------------------------------------------
// Encoder Sync tile
#pragma once

#include "../htmlHeaders.h"
#include "../htmlMessages.h"
#include "../htmlScripts.h"

extern void syncTile(String &data);
extern void syncTileAjax(String &data);
extern void syncTileGet();

const char html_syncOnStepNow[] PROGMEM =
  "<button onpointerdown=\"s('enc','o2e')\" type='button'>" L_ENC_SYNC_TO_ENC "</button>"
  "<br />\n";

#ifdef ENC_ABSOLUTE
  const char html_zeroEncodersNow[] PROGMEM =
    "<button onpointerdown=\"if (confirm('" L_ARE_YOU_SURE "?')) s('enc','zro')\" type='button'>" L_ENC_ORIGIN_TO_ONS "</button>"
    "<br />\n";
#endif

const char html_syncEncodersNow[] PROGMEM =
  "<button onpointerdown=\"s('enc','e2o')\" type='button'>" L_ENC_SYNC_TO_ONS "</button>"
  "<br />\n";

const char html_syncAuto[] PROGMEM = "<br />"
  L_ENC_AUTO_SYNC ": <br />"
  "<button id='enc_as_on'  class='btns_right' onpointerdown=\"s('enc','on')\"  type='button'>" L_ON "</button>"
  "<button id='enc_as_off' class='btns_left'  onpointerdown=\"s('enc','off')\" type='button'>" L_OFF "</button>"
  "<br />\n";
