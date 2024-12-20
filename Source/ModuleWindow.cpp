#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow()
= default;

// Destructor
ModuleWindow::~ModuleWindow()
= default;

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface")
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError())
		ret = false;
	}
	else
	{
		SDL_DisplayMode DM;
		SDL_GetCurrentDisplayMode(0, &DM);

		activeWindowHeight = activeWindowWidth * DM.h / DM.w;
		
		Uint32 flags = SDL_WINDOW_SHOWN |  SDL_WINDOW_OPENGL;

		if(FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
			
		}
		flags |= SDL_WINDOW_RESIZABLE;

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, activeWindowWidth, activeWindowHeight, flags);
		
		if(window == nullptr)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError())
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems")

	//Destroy window
	if(window != nullptr)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::UpdateWindowWidth(const int newWidth)
{
	activeWindowWidth = newWidth;
}

void ModuleWindow::UpdateWindowHeight(const int newHeight)
{
	activeWindowHeight = newHeight;
}

