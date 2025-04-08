// -----------------------------------------------------------------------------------
// Encoders
#include "Encoders.h"

#include "../Page.h"
#include "../Pages.common.h"

#if ENCODERS == ON

void processEncodersGet();

void handleEncoders()
{
  char temp[400] = "";

  state.updateMount(true);

  SERIAL_ONSTEP.setTimeout(webTimeout);
  onStep.serialRecvFlush();

  processEncodersGet();

  www.setContentLength(CONTENT_LENGTH_UNKNOWN);
  www.sendHeader("Cache-Control", "no-cache");
  www.send(200, "text/html", String());

  String data = FPSTR(html_head_begin);

  // send a standard http response header
  data.concat(FPSTR(html_main_css_begin));
  www.sendContentAndClear(data);
  data.concat(FPSTR(html_main_css_core));
  data.concat(FPSTR(html_main_css_control));
  www.sendContentAndClear(data);
  data.concat(FPSTR(html_main_css_buttons));
  data.concat(FPSTR(html_main_css_collapse));
  data.concat(FPSTR(html_main_css_end));
  data.concat(FPSTR(html_head_end));
  www.sendContentAndClear(data);

  // show this page
  data.concat(FPSTR(html_body_begin));
  www.sendContentAndClear(data);
  pageHeader(PAGE_ENCODERS);
  data.concat(FPSTR(html_onstep_page_begin));

  // OnStep wasn't found, show warning and info.
  if (!status.onStepFound)
  {
    data.concat(FPSTR(html_bad_comms_message));
    data.concat(FPSTR(html_page_and_body_end));
    www.sendContentAndClear(data);
    www.sendContent("");
    return;
  }

  // scripts
  sprintf_P(temp, html_script_ajax_get, "enc-ajax-get.txt");
  data.concat(temp);

  data.concat(F("<script>var ajaxPage='enc-ajax.txt';</script>\n"));
  www.sendContentAndClear(data);
  data.concat(FPSTR(html_script_ajax));
  www.sendContentAndClear(data);

  // page contents
  syncTile(data);
  encAxisTile(0, data);
  encAxisTile(1, data);

  data.concat(F("<br class=\"clear\" />\n"));
  data.concat(FPSTR(html_page_and_body_end));

  data.concat(FPSTR(html_script_collapsible));

  www.sendContentAndClear(data);
  www.sendContent("");
}

void encAjaxGet()
{
  www.setContentLength(CONTENT_LENGTH_UNKNOWN);
  www.sendHeader("Cache-Control", "no-cache");
  www.send(200, "text/plain", String());

  processEncodersGet();

  www.sendContent("");
}

void encAjax()
{
  String data = "";

  www.setContentLength(CONTENT_LENGTH_UNKNOWN);
  www.sendHeader("Cache-Control", "no-cache");
  www.send(200, "text/plain", String());

  syncTileAjax(data);
  encAxisTileAjax(data);
  
  www.sendContent("");

  state.lastMountPageLoadTime = millis();
}

void processEncodersGet()
{
  String v;

  syncTileGet();
  encAxisTileGet();

  state.lastMountPageLoadTime = millis();
}
#endif
