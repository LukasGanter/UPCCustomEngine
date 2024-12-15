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
	~ModuleWindow() override;

	// Called before quitting
	bool Init() override;

	// Called before quitting
	bool CleanUp() override;

public:
	//The window we'll be rendering to
	SDL_Window* window = nullptr;

	//The surface contained by the window
	SDL_Surface* screen_surface = nullptr;

	void UpdateWindowWidth(const int newWidth);
	void UpdateWindowHeight(const int newHeight);

	const unsigned int getActiveWindowWidth() const { return activeWindowWidth; }
	const unsigned int getActiveWindowHeight() const { return activeWindowWidth; }
	const float getActiveWindowRatio() const { return static_cast<float>(activeWindowWidth) / static_cast<float>(activeWindowHeight); }

private: 

	unsigned int activeWindowWidth = SCREEN_WIDTH;
	unsigned int activeWindowHeight;
};

#endif // __ModuleWindow_H__