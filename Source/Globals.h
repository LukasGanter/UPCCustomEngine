#pragma once
#define NOMINMAX

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
#define FULLSCREEN false
#define VSYNC true
#define TITLE "UPCCustomEngine"
#define FPS 60
#define LOG_MSG_BUFFER_SIZE 40
#define FPS_TICK_BUFFER_SIZE 120