#pragma once
#include "Module.h"
#include "Globals.h"
#include <vector>

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleEditor : public Module
{
public:
	ModuleEditor();
	~ModuleEditor();

	bool Init();
	update_status PreUpdate();
	update_status Update(const float deltaTime);
	update_status PostUpdate();
	bool CleanUp();

	void Draw();

private:
	void ShowConfigurationWindow(bool* open);
	void ShowAboutWindow(bool* open);
	void ShowGraphicsWindow(bool* open);
	void ShowApplicationsWindow(bool* open);
	void ShowLogWindow(bool* open);

	bool show_demo_window = false;
	bool show_config_window = false;
	bool show_about_window = false;
	bool show_graphics_window = false;
	bool show_application_window = false;
	bool show_log_window = false;

	int brightness = 50;
	int width = 1280;
	int height = 720;

	const int tickBufferLength = 120;
	int tickBufferPtr = 0;
	std::vector<float> deltaTickBuffer;
	std::vector<float> fpsBuffer;
	
};
