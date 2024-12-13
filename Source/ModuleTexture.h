#pragma once
#include <string>

#include "Module.h"
#include "Globals.h"
#include "GL/glew.h"


struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleTexture : public Module
{
public:
	ModuleTexture();
	~ModuleTexture();

	bool Init();
	update_status PreUpdate();
	update_status Update(const float deltaTime);
	update_status PostUpdate();
	bool CleanUp();

	GLuint LoadTexture(const std::string& texturePath) const;
};
