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
	update_status Update(const float deltaTime);
	update_status PostUpdate();
	bool CleanUp();
	void WindowResized(unsigned width, unsigned height);
	
	float4x4 model, view, projection;

	float nearPlaneDistance = 0.1f;
	float farPlaneDistance = 100.0f;
	float horizontalFOV = 90.0f;
	
	void FocusMesh();

private:

	float GetDistanceForWholeMeshView() const;
	float GetCenterHeightForWholeMeshView() const;
	float GetMaxHeightForMeshView() const;
	float3 GetCenterOfMesh() const;
	
	void StartOrbiting();
	void StopOrbiting();

	void GenerateMatrices();
	void LookAtLocation(const float3& playerPosition, const float3& lookAtPosition);
	void LookInDirection(const float3& viewDirection);

	float3 location = float3(0, 2, 7);
	float3 lookAtDirection = float3(0, 0, -1);
	float3 right = float3::nan;
	float3 up = float3::nan;
	float3 alignDirection = float3(0, 1, 0);

	bool orbiting = false;
	float orbitingRadius;
	float currentOrbitingRads;
	float3 target = float3::zero;
};
