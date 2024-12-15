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
	~ModuleTexture() override;

	void RenderUI() const;

	GLuint LoadTexture(const std::string& texturePath);

	const int getLoadedTextureID() const;
	
private:

	bool textureValid = false;
	int loadedTextureWidth = -1;
	int loadedTextureHeight = -1;
	std::string loadedTextureName;
	GLuint meshTexture;
};
