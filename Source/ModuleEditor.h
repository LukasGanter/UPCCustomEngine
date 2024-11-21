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
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void Draw();

private:
	void ShowConfigurationWindow(bool* open);
	void ShowAboutWindow(bool* open);

	bool show_demo_window = false;
	bool show_config_window = false;
	bool show_about_window = false;

	int brightness = 50;
	int width = 1280;
	int height = 720;
};
