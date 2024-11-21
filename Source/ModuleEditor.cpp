#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleOpenGL.h"
#include "SDL.h"
#include "GL/glew.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_opengl3.h"

ModuleEditor::ModuleEditor()
{
	
}

// Destructor
ModuleEditor::~ModuleEditor()
{
}

// Called before render is available
bool ModuleEditor::Init()
{
	LOG("Creating Editor Program");
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

	ImGui_ImplSDL2_InitForOpenGL(App->GetWindow()->window, App->GetOpenGL()->GetContext());
	ImGui_ImplOpenGL3_Init("#version 460");

	return true;
}

update_status ModuleEditor::PreUpdate()
{
	
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleEditor::Update()
{
	Draw();

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate()
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleEditor::CleanUp()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	//LOG("Destroying renderer");
	return true;
}

void ModuleEditor::Draw()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("Menu")) {
			if (ImGui::MenuItem("Gui Demo")) {
				show_demo_window = !show_demo_window;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window")) {
			if (ImGui::MenuItem("Configuration")) {
				show_config_window = !show_config_window;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	

	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	if (show_config_window)
		ShowConfigurationWindow(&show_demo_window);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	   // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
	   //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	}
}

void ModuleEditor::ShowConfigurationWindow(bool* open)
{
	if (ImGui::CollapsingHeader("Window options")) {
		ImGui::SliderInt("Brightness", &brightness, 0, 100);
		ImGui::SliderInt("Width", &width, 0, 1920);
		ImGui::SliderInt("Height", &height, 0, 1080);
	}
	if (ImGui::Button("Close")) {
		show_config_window = false;
	}
}

void ModuleEditor::ShowAboutWindow(bool* open)
{
	//ImGui
}
