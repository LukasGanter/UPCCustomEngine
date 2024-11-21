#pragma once
#include "Module.h"
#include "Globals.h"
#include "Math/float4x4.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRenderExercise : public Module
{
public:
	ModuleRenderExercise();
	~ModuleRenderExercise();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void WindowResized(unsigned width, unsigned height);

	const float4x4& getModelMatrix() const {
		return model;
	}

	const float4x4& getViewMatrix() const {
		return view;
	}

	const float4x4& getProjectionMatrix() const {
		return projection;
	}

private:

	void GenerateMatrices();
	void RenderTriangle();
	void LookAt(const float3& playerPosition, const float3& lookAtPosition);

	unsigned vbo;
	float animatedRenderOffsetX = -10.;

	float posX = 0;
	float posY = 0;
	float posZ = 0;
	
	float4x4 model, view, projection;
};
