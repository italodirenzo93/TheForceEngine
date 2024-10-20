#pragma once

#ifndef __UWP__
#error This file should not be included in a not UWP app
#endif

#include <TFE_System/system.h>
#include <string>

#define STUBBED(x) TFE_System::logWrite(LOG_WARNING, "STUBBED", x)

inline std::string ToStdString(Platform::String^ str)
{
	std::wstring ws(str->Data());
	return std::string(ws.begin(), ws.end());
}
