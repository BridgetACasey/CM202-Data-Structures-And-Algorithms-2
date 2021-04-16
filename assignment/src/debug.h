//@BridgetACasey

#pragma once

#include <stdarg.h>
#include <stdio.h>
#include "windows.h"

void debugPrint(const char* message, ...)
{
    va_list args;
    char buffer[256] = { 0 };

    va_start(args, message);

    vsprintf_s(buffer, message, args);

    OutputDebugStringA(buffer);

    va_end(args);
}