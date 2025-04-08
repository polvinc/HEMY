// Common includes
#pragma once

// enable lib support for the command and web servers
#define WEB_SERVER ON

#include <Arduino.h>

#include "lib/Macros.h"
#include "lib/Constants.h"

#include "Constants.h"
#include "../Config.h"
#include "Config.defaults.h"
#include "HAL/HAL.h"
#include "lib/debug/Debug.h"

#include "pinmaps/Models.h"

#include "libApp/status/Version.h"
#include "libApp/status/State.h"
