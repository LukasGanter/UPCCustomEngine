#include "Globals.h"
#include "Application.h"
#include "ModuleOpenGL.h"
#include "ModuleWindow.h"
#include "SDL.h"
#include "GL/glew.h"
#include "imgui/imgui.h"

ModuleOpenGL::ModuleOpenGL()
= default;

// Destructor
ModuleOpenGL::~ModuleOpenGL()
= default;

// Called before render is available
bool ModuleOpenGL::Init()
{
	LOG("Creating Renderer context")

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4); // desired version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // we want a double buffer
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // we want to have a depth buffer with 24 bits
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8); // we want to have a stencil buffer with 8 bits

	context = SDL_GL_CreateContext(App->GetWindowModule()->window);

	GLenum err = glewInit();
	// � check for errors
	LOG("Using Glew %s", glewGetString(GLEW_VERSION))
	// Should be 2.0

	LOG("Vendor: %s", glGetString(GL_VENDOR))
	LOG("Renderer: %s", glGetString(GL_RENDERER))
	LOG("OpenGL version supported %s", glGetString(GL_VERSION))
	LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION))

	glEnable(GL_DEPTH_TEST); // Enable depth test
	glEnable(GL_CULL_FACE); // Enable cull backward faces
	glFrontFace(GL_CCW); // Front faces will be counter clockwise

	return true;
}

update_status ModuleOpenGL::PreUpdate()
{
	int width = 0;
	int height = 0;
	SDL_GetWindowSize(App->GetWindowModule()->window, &width, &height);
	glViewport(0, 0, width, height);
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleOpenGL::Update(const float deltaTime)
{
	return UPDATE_CONTINUE;
}

update_status ModuleOpenGL::PostUpdate()
{
	SDL_GL_SwapWindow(App->GetWindowModule()->window); 
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleOpenGL::CleanUp()
{
	LOG("Destroying renderer")

	//Destroy window
	SDL_GL_DeleteContext(context);

	return true;
}

void ModuleOpenGL::RenderUI()
{
	int totalMemory, freeMemory, textureFreeMemory = -1;
	if (glewIsExtensionSupported("GL_NVX_gpu_memory_info")) {
		glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &totalMemory);
		glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &freeMemory);
	}

	if (glewIsExtensionSupported("GL_ATI_meminfo")) {
		glGetIntegerv(GL_TEXTURE_FREE_MEMORY_ATI, &textureFreeMemory);
	}

	ImGui::Text("Total memory available: %f mb", totalMemory / 1024.);
	ImGui::Text("Current memory available: %f mb", freeMemory / 1024.);
	ImGui::Text("Total texture memory available: %f mb", textureFreeMemory / 1024.);
	ImGui::Separator();
	ImGui::Text("SDL version: 4.6");
	ImGui::Text("SDL double buffer: 1");
	ImGui::Text("SDL depth stencil: 24");
	ImGui::Text("SDL stencil size: 8");
	ImGui::Separator();
	ImGui::Text("GLEW version: %s", glewGetString(GLEW_VERSION));
	ImGui::Text("Vendor: %s", glGetString(GL_VENDOR));
	ImGui::Text("Renderer: %s", glGetString(GL_RENDERER));
	ImGui::Text("OpenGL version supported %s", glGetString(GL_VERSION));
	ImGui::Text("GLSL: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
	
}

