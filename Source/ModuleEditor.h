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
	~ModuleEditor() override;

	bool Init() override;
	update_status Update(const float deltaTime) override;
	bool CleanUp() override;

	update_status Draw();

private:
	void ShowAboutWindow();
	void ShowGraphicsWindow();
	void ShowApplicationsWindow();
	void ShowLogWindow();
	void ShowPropertiesWindow();

	bool show_about_window = false;
	bool show_graphics_window = false;
	bool show_application_window = false;
	bool show_log_window = false;
	bool show_properties_window = false;
	
	int tickBufferPtr = 0;
	std::vector<float> deltaTickBuffer;
	std::vector<float> fpsBuffer;
	
};
