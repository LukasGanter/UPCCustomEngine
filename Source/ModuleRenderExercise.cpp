#include "Globals.h"
#include "Application.h"
#include "ModuleRenderExercise.h"
#include "ModuleWindow.h"
#include "ModuleProgram.h"
#include "SDL.h"
#include "GL/glew.h"
#include "Geometry/Frustum.h"
#include "ModuleInput.h"
#

ModuleRenderExercise::ModuleRenderExercise()
{
	
}

// Destructor
ModuleRenderExercise::~ModuleRenderExercise()
{
}

// Called before render is available
bool ModuleRenderExercise::Init()
{
	LOG("Creating Renderer exercise");

	GenerateMatrices();

	float vtx_data[] = { -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f };

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // set vbo active
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_data), vtx_data, GL_STATIC_DRAW);

	return true;
}

update_status ModuleRenderExercise::PreUpdate()
{
	float speed = 0.2f;

	if (App->GetInput()->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		posX -= speed;
	}
	else if (App->GetInput()->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		posX += speed;
	}

	if (App->GetInput()->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {
		posZ -= speed;
	}
	else if (App->GetInput()->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
		posZ += speed;
	}

	if (App->GetInput()->GetKey(SDL_SCANCODE_Z) == KEY_REPEAT) {
		posY -= speed;
	}
	else if (App->GetInput()->GetKey(SDL_SCANCODE_X) == KEY_REPEAT) {
		posY += speed;
	}

	LookAt(float3(posX, posY, posZ), float3(0, 0, 0));

	//LookAt(float3(animatedRenderOffsetX, 2, 12), float3(0, 0, 0));
	//animatedRenderOffsetX += 0.05f;
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRenderExercise::Update()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	// size = 3 float per vertex
	// stride = 0 is equivalent to stride = sizeof(float)*3
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	RenderTriangle();

	return UPDATE_CONTINUE;
}

void ModuleRenderExercise::GenerateMatrices()
{
	model = float4x4::FromTRS(float3(2.0f, 0.0f, 0.0f),
		float4x4::RotateZ(M_PI / 4.0f),
		float3(2.0f, 1.0f, 1.0f));

	Frustum frustum = Frustum();
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3(animatedRenderOffsetX, 2, 12);
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = M_PI / 4.0f;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * (4./3.));

	//view = frustum.ViewMatrix();
	LookAt(float3(animatedRenderOffsetX, 2, 12), float3(animatedRenderOffsetX, 2, 1));
	projection = frustum.ProjectionMatrix();

}

void ModuleRenderExercise::RenderTriangle()
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

void ModuleRenderExercise::LookAt(const float3& playerPosition, const float3& lookAtPosition)
{
	const float3 viewDirection = (lookAtPosition - playerPosition);
	const float3 normViewDirection = viewDirection.Normalized();

	const float3 alignDirection = float3(0, 1, 0);

	const float3 cross = normViewDirection.Cross(alignDirection);

	const float4x4 TranslationMatrix = float4x4(1, 0, 0, -playerPosition.x, 
		0, 1, 0, -playerPosition.y, 
		0, 0, 1, -playerPosition.z, 
		0, 0, 0, 1);

	const float4x4 RotationMatrix = float4x4(cross.x, cross.y, cross.z, 0, 
		alignDirection.x, alignDirection.y, alignDirection.z, 0, 
		-normViewDirection.x, -normViewDirection.y, -normViewDirection.z, 0,
		0, 0, 0, 1);

	view = RotationMatrix * TranslationMatrix;

}

update_status ModuleRenderExercise::PostUpdate()
{
	SDL_GL_SwapWindow(App->GetWindow()->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderExercise::CleanUp()
{
	LOG("Destroying renderer");

	glDeleteBuffers(1, &vbo);

	return true;
}

void ModuleRenderExercise::WindowResized(unsigned width, unsigned height)
{
}

