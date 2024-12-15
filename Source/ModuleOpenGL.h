#pragma once
#include "Module.h"
#include "Globals.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleOpenGL : public Module
{
public:
	ModuleOpenGL();
	~ModuleOpenGL() override;

	bool Init() override;
	update_status PreUpdate() override;
	update_status Update(const float deltaTime) override;
	update_status PostUpdate() override;
	bool CleanUp() override;

	void* GetContext() const { return context; }

	static void RenderUI();

private:
	void* context;
};
