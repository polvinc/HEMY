// -----------------------------------------------------------------------------------
// Display and process data from webpages

#include <Arduino.h>
#include "../Constants.h"
#include "../../Config.h"
#include "../../Extended.config.h"

void handleRoot();
void indexAjaxGet();
void indexAjax();

void handleMount();
void mountAjaxGet();
void mountAjax();
void handleLibraryHelp();

void handleRotator();
void rotatorAjaxGet();
void rotatorAjax();

void handleFocuser();
void focuserAjaxGet();
void focuserAjax();

void handleAux();
void auxAjaxGet();
void auxAjax();

#if ENCODERS == ON
  void handleEncoders();
  void encAjaxGet();
  void encAjax();
#endif

void handleNetwork();

void handleNotFound();
