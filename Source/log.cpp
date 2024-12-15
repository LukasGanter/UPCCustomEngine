#pragma once
#include <iostream>
#include <ostream>

#include "Application.h"
#include "Globals.h"
#include "ModuleEditor.h"

void log(const char file[], int line, const char* format, ...)
{
	static char tmp_string[4096];
	static va_list  ap;

	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmp_string, 4096, format, ap);
	va_end(ap);
	
	App->AddLogMessage(tmp_string);
}
