#include "Globals.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "ModuleProgram.h"
#include "SDL.h"
#include "GL/glew.h"
#include "Geometry/Frustum.h"
#include "ModuleInput.h"
#include "Math/float3x3.h"

ModuleCamera::ModuleCamera()
{
	
}

// Destructor
ModuleCamera::~ModuleCamera()
{
}

// Called before render is available
bool ModuleCamera::Init()
{
	LOG("Creating Renderer exercise");

	float vtx_data[] = { -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f };

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // set vbo active
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_data), vtx_data, GL_STATIC_DRAW);

	return true;
}

update_status ModuleCamera::PreUpdate()
{
	float speed = 0.1f;
	if (App->GetInput()->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {
		speed *= 3;
	}
	
	// Move if left mouse is pressed and mouse is tracked
	if (App->GetInput()->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
		lookAtDirection = lookAtDirection *
			float3x3::RotateAxisAngle(alignDirection, App->GetInput()->GetMouseMotion().x / 30.);

		lookAtDirection = lookAtDirection *
			float3x3::RotateAxisAngle(float3(1, 0, 0), App->GetInput()->GetMouseMotion().y / 30.);
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

	GenerateMatrices();

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleCamera::Update()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	// size = 3 float per vertex
	// stride = 0 is equivalent to stride = sizeof(float)*3
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	RenderTriangle();

	return UPDATE_CONTINUE;
}

void ModuleCamera::GenerateMatrices()
{

	model = float4x4::FromTRS(float3(2.0f, 0.0f, 0.0f),
		float4x4::RotateZ(M_PI / 4.0f),
		float3(2.0f, 1.0f, 1.0f));

	float3 tempWorldRight = lookAtDirection.Normalized().Cross(alignDirection);
	up = tempWorldRight.Cross(lookAtDirection);

	Frustum frustum;
	frustum.type = PerspectiveFrustum;
	frustum.pos = location;
	frustum.front = lookAtDirection.Normalized();
	frustum.up = up;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = M_PI / 4.0f;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * (4./3.));

	right = frustum.WorldRight();
	projection = frustum.ProjectionMatrix();

	// Generates view matrix
	LookInDirection(lookAtDirection);
}

void ModuleCamera::RenderTriangle()
{
	const unsigned int program = App->GetProgram()->getProgram();
	// TODO: retrieve model view and projection
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, &projection[0][0]);
		// TODO: bind buffer and vertex attributes
	glDrawArrays(GL_TRIANGLES, 0, 3);
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

update_status ModuleCamera::PostUpdate()
{
	SDL_GL_SwapWindow(App->GetWindow()->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleCamera::CleanUp()
{
	LOG("Destroying renderer");

	glDeleteBuffers(1, &vbo);

	return true;
}

void ModuleCamera::WindowResized(unsigned width, unsigned height)
{
}

