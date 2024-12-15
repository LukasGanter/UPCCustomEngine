#include "Globals.h"
#include "Application.h"
#include "ModuleCamera.h"

#include "EngineModel.h"
#include "ModuleWindow.h"
#include "SDL.h"
#include "Geometry/Frustum.h"
#include "ModuleInput.h"
#include "ModuleModel.h"
#include "Math/float3x3.h"
#include "Math/MathConstants.h"
#include "Math/MathFunc.h"

ModuleCamera::ModuleCamera()
= default;

// Destructor
ModuleCamera::~ModuleCamera()
= default;

update_status ModuleCamera::PreUpdate()
{
	const bool shouldOrbit = App->GetInput()->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT &&
		App->GetInput()->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT;

	if (orbiting)
	{
		orbiting = shouldOrbit;
	} else
	{
		if (shouldOrbit)
		{
			StartOrbiting();
		}
		
		if (App->GetInput()->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) {
			FocusMesh();
		}
		
		float speed = 0.1f;
		if (App->GetInput()->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {
			speed *= 3;
		}

		location += lookAtDirection * speed * App->GetInput()->GetMouseWheel();
		
		// Move if left mouse is pressed and mouse is tracked
		if (App->GetInput()->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
			lookAtDirection = lookAtDirection *
				float3x3::RotateAxisAngle(right, App->GetInput()->GetMouseMotion().y / 30.);
		
			lookAtDirection = lookAtDirection *
				float3x3::RotateAxisAngle(alignDirection, App->GetInput()->GetMouseMotion().x / 30.);
		}
	
		if (App->GetInput()->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
			location += lookAtDirection * speed;
		}
	
		if (App->GetInput()->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
			location -= lookAtDirection * speed;
		}

		if (App->GetInput()->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			location -= right * speed;
		}
	
		if (App->GetInput()->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			location += right * speed;
		}

		if (App->GetInput()->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) {
			location.y += speed;
		}
		else if (App->GetInput()->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) {
			location.y -= speed;
		}
	}
	
	GenerateMatrices();

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleCamera::Update(const float deltaTime)
{
	if (orbiting)
	{
		currentOrbitingRads += deltaTime / 1000.0f;
		currentOrbitingRads = Mod(currentOrbitingRads, pi * 2.f);
		location = float3(orbitingRadius * cos(currentOrbitingRads), location.y, orbitingRadius * sin(currentOrbitingRads));
		lookAtDirection = target - location;
		lookAtDirection.Normalize();
	}
	
	return UPDATE_CONTINUE;
}

void ModuleCamera::StartOrbiting()
{
	orbitingRadius = GetDistanceForWholeMeshView();
	currentOrbitingRads = 0;
	target = GetCenterOfMesh();

	location = float3(0, GetMaxHeightForMeshView(), orbitingRadius);
	lookAtDirection = float3(0, 0, -1);
	
	orbiting = true;
}

void ModuleCamera::StopOrbiting()
{
	orbiting = false;
}

float ModuleCamera::GetDistanceForWholeMeshView()
{
	EngineModel* loadedModel = App->GetModel()->GetLoadedModel();
	if (loadedModel == nullptr) return 1;

	const float3* minPos = loadedModel->getMinPosValues();
	const float3* maxPos = loadedModel->getMaxPosValues();

	if (minPos == nullptr || maxPos == nullptr) return 1;

	return Max(minPos->Length(), maxPos->Length()) + 0.1;
}

float ModuleCamera::GetCenterHeightForWholeMeshView()
{
	EngineModel* loadedModel = App->GetModel()->GetLoadedModel();
	if (loadedModel == nullptr) return 1;

	const float3* minPos = loadedModel->getMinPosValues();
	const float3* maxPos = loadedModel->getMaxPosValues();

	if (minPos == nullptr || maxPos == nullptr) return 1;

	return (minPos->y + maxPos->y) / 2.f;
}

float ModuleCamera::GetMaxHeightForMeshView()
{
	EngineModel* loadedModel = App->GetModel()->GetLoadedModel();
	if (loadedModel == nullptr) return 1;
	
	const float3* maxPos = loadedModel->getMaxPosValues();

	if (maxPos == nullptr) return 1;

	return maxPos->y;
}

float3 ModuleCamera::GetCenterOfMesh()
{
	EngineModel* loadedModel = App->GetModel()->GetLoadedModel();
	if (loadedModel == nullptr) return float3::zero;

	const float3* minPos = loadedModel->getMinPosValues();
	const float3* maxPos = loadedModel->getMaxPosValues();

	if (minPos == nullptr || maxPos == nullptr) return float3::zero;

	return (*minPos + *maxPos) / 2.f;
}

void ModuleCamera::FocusMesh()
{
	location = float3(0, GetCenterHeightForWholeMeshView(), GetDistanceForWholeMeshView());
	lookAtDirection = float3(0, 0, -1);
}

void ModuleCamera::GenerateMatrices()
{

	model = float4x4::FromTRS(float3(0.0f, 0.0f, 0.0f),
		float4x4::RotateZ(0),
		float3(1.0f, 1.0f, 1.0f));

	float3 tempWorldRight = lookAtDirection.Normalized().Cross(alignDirection);
	up = tempWorldRight.Cross(lookAtDirection);

	Frustum frustum;
	frustum.type = PerspectiveFrustum;
	frustum.pos = location;
	frustum.front = lookAtDirection.Normalized();
	frustum.up = up;
	frustum.nearPlaneDistance = nearPlaneDistance;
	frustum.farPlaneDistance = farPlaneDistance;
	frustum.verticalFov = atanf(tanf((horizontalFOV * (pi / 180)) / 2.f) / App->GetWindow()->getActiveWindowRatio()) / 0.5f;
	//frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * (4./3.));
	frustum.horizontalFov = horizontalFOV * (pi / 180);

	right = frustum.WorldRight();
	projection = frustum.ProjectionMatrix();

	// Generates view matrix
	LookInDirection(lookAtDirection);
}

void ModuleCamera::LookAtLocation(const float3& playerPosition, const float3& lookAtPosition)
{
	LookInDirection(lookAtPosition - playerPosition);
}

void ModuleCamera::LookInDirection(const float3& viewDirection)
{
	const float3 normViewDirection = viewDirection.Normalized();

	const float4x4 TranslationMatrix = float4x4(1, 0, 0, -location.x,
		0, 1, 0, -location.y,
		0, 0, 1, -location.z,
		0, 0, 0, 1);

	const float4x4 RotationMatrix = float4x4(right.x, right.y, right.z, 0,
		up.x, up.y, up.z, 0,
		-normViewDirection.x, -normViewDirection.y, -normViewDirection.z, 0,
		0, 0, 0, 1);

	view = RotationMatrix * TranslationMatrix;
}


