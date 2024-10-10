#pragma once

#ifndef __UWP__
#error This file should not be included in a not UWP app
#endif

#include <TFE_System/system.h>

#define STUBBED(x) TFE_System::logWrite(LOG_WARNING, "STUBBED", x)
