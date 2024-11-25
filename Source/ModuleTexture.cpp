#include "Globals.h"
#include "Application.h"
#include "ModuleTexture.h"
#include "ModuleWindow.h"
#include "SDL.h"
#include "GL/glew.h"
#include "DirectXTex.h"
#include <string>

ModuleTexture::ModuleTexture()
{
	
}

// Destructor
ModuleTexture::~ModuleTexture()
{
}

// Called before render is available
bool ModuleTexture::Init()
{
	LOG("Creating Renderer context");

	DirectX::ScratchImage sImage = DirectX::ScratchImage();
	const auto path = L"Resources/baboon.ppm";

	DirectX::LoadFromDDSFile(path, DirectX::DDS_FLAGS_NONE, nullptr, sImage);

	return true;
}

update_status ModuleTexture::PreUpdate()
{
	
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleTexture::Update()
{
	return UPDATE_CONTINUE;
}

update_status ModuleTexture::PostUpdate()
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleTexture::CleanUp()
{
	return true;
}

