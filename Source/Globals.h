#pragma once
#define NOMINMAX
#include <windows.h>
#include <stdio.h>
#include <vector>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Configuration -----------
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define FULLSCREEN false
#define VSYNC true
#define TITLE "Custom game engine"
#define FPS 1000
#define LOG_MSG_BUFFER_SIZE 40

extern int logMsgBufferPtr;
extern std::vector<char*> logMsgBuffer;