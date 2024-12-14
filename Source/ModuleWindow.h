#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow();

	// Destructor
	virtual ~ModuleWindow();

	// Called before quitting
	bool Init();

	// Called before quitting
	bool CleanUp();

public:
	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screen_surface = NULL;

	void UpdateWindowWidth(const int newWidth);
	void UpdateWindowHeight(const int newHeight);

	const unsigned int getActiveWindowWidth() const { return activeWindowWidth; }
	const unsigned int getActiveWindowHeight() const { return activeWindowWidth; }
	const float getActiveWindowRatio() const { return (float)activeWindowWidth / (float)activeWindowHeight; }

private: 

	unsigned int activeWindowWidth = SCREEN_WIDTH;
	unsigned int activeWindowHeight;
};

#endif // __ModuleWindow_H__