#pragma once
#include "Module.h"
#include "Globals.h"
#include "Math/float4x4.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleCamera : public Module
{
public:
	ModuleCamera();
	~ModuleCamera();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void WindowResized(unsigned width, unsigned height);
	
	float4x4 model, view, projection;

private:

	void GenerateMatrices();
	void LookAtLocation(const float3& playerPosition, const float3& lookAtPosition);
	void LookInDirection(const float3& viewDirection);

	float3 location = float3(0, 1, 5);
	float3 lookAtDirection = float3(0, 0, -1);
	float3 right = float3(1, 0, 0);
	float3 up = float3(0, 1, 0);
	float3 alignDirection = float3(0, 1, 0);
};
