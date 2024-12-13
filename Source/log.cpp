#pragma once
#include <iostream>
#include <ostream>

#include "Application.h"
#include "Globals.h"
#include "ModuleEditor.h"

int logMsgBufferPtr = 0;
std::vector<char*> logMsgBuffer(LOG_MSG_BUFFER_SIZE, nullptr);

void log(const char file[], int line, const char* format, ...)
{
	static char tmp_string[4096];
	static char tmp_string2[4096];
	static va_list  ap;

	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmp_string, 4096, format, ap);
	va_end(ap);
	sprintf_s(tmp_string2, 4096, "\n%s(%d) : %s", file, line, tmp_string);
	OutputDebugString(tmp_string2);
	
	if (logMsgBufferPtr >= LOG_MSG_BUFFER_SIZE)
	{
		logMsgBufferPtr = 0;
	}
	logMsgBuffer[logMsgBufferPtr] = tmp_string2;
	logMsgBufferPtr++;
}
