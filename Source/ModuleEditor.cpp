#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"

#include "ModuleCamera.h"
#include "ModuleModel.h"
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

	deltaTickBuffer = std::vector<float>( tickBufferLength);
	fpsBuffer = std::vector<float>( tickBufferLength);

	return true;
}

update_status ModuleEditor::PreUpdate()
{
	
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleEditor::Update(const float deltaTime)
{
	if (tickBufferPtr >= tickBufferLength)
	{
		tickBufferPtr = 0;
	}
	deltaTickBuffer[tickBufferPtr] = deltaTime;
	fpsBuffer[tickBufferPtr] = 1000. / deltaTime;
	tickBufferPtr++;
	
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

update_status ModuleEditor::Draw()
{
	update_status ret = UPDATE_CONTINUE;
	
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("General")) {
			ImGui::TextLinkOpenURL("Github repository", "https://github.com/LukasGanter/UPCCustomEngine");
			if (ImGui::MenuItem("About")) {
				show_about_window = !show_about_window;
			}
			if (ImGui::Button("Quit"))
			{
				ret = UPDATE_STOP;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window")) {
			if (ImGui::MenuItem("Graphics")) {
				show_graphics_window = !show_graphics_window;
			}
			if (ImGui::MenuItem("Application Profiler")) {
				show_application_window = !show_application_window;
			}
			if (ImGui::MenuItem("Logs")) {
				show_log_window = !show_log_window;
			}
			if (ImGui::MenuItem("Properties")) {
				show_properties_window = !show_properties_window;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	

	if (show_about_window)
		ShowAboutWindow();

	if (show_graphics_window)
		ShowGraphicsWindow();

	if (show_application_window)
		ShowApplicationsWindow();

	if (show_log_window)
		ShowLogWindow();

	if (show_properties_window)
		ShowPropertiesWindow();

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
	
	return ret;
}

void ModuleEditor::ShowAboutWindow()
{
	ImGui::Begin("About");
	ImGui::Text("Developer: Lukas Ganter");
	ImGui::Text("Engine version: v0.1");
	ImGui::TextLinkOpenURL("Github repository", "https://github.com/LukasGanter/UPCCustomEngine");
	if (ImGui::Button("Close")) {
		show_about_window = false;
	}
	ImGui::End();
}

void ModuleEditor::ShowGraphicsWindow()
{
	ImGui::Begin("Graphic settings");
	ImGui::SliderFloat("Horizontal FOV", &App->GetCamera()->horizontalFOV, 0, 180);
	ImGui::SliderFloat("Near clipping plane", &App->GetCamera()->nearPlaneDistance, 0.001f, App->GetCamera()->farPlaneDistance);
	ImGui::SliderFloat("Far clipping plane", &App->GetCamera()->farPlaneDistance, App->GetCamera()->nearPlaneDistance + 0.001f, 1000);
	if (ImGui::Button("Close")) {
		show_graphics_window = false;
	}
	ImGui::End();
}

void ModuleEditor::ShowApplicationsWindow()
{
	ImGui::Begin("Application options");
	
	ImGui::PlotHistogram("##framerate", &fpsBuffer[0], fpsBuffer.size(), tickBufferPtr, "Framerate", 0.0f, 100.f, ImVec2(310, 100));
	ImGui::PlotHistogram("##milliseconds", &deltaTickBuffer[0], deltaTickBuffer.size(), tickBufferPtr, "Milliseconds", 0.0f, 40.f, ImVec2(310, 100));
	
	App->GetOpenGL()->RenderUI();
	
	if (ImGui::Button("Close")) {
		show_application_window = false;
	}
	ImGui::End();
}

void ModuleEditor::ShowLogWindow()
{
	ImGui::Begin("Logs");
	unsigned int workingPtr = App->logMsgBufferPtr + 1;
	for (unsigned int i = 0; i < App->logMsgBuffer.size(); i++)
	{
		if (workingPtr >= App->logMsgBuffer.size())
		{
			workingPtr = 0;
		}
		const char* message = App->logMsgBuffer[workingPtr];
		if (message != nullptr)
		{
			ImGui::Text(message);
		}
		workingPtr++;
	}
	if (ImGui::Button("Close")) {
		show_log_window = false;
	}
	ImGui::End();
}

void ModuleEditor::ShowPropertiesWindow()
{
	ImGui::Begin("Properties", &show_properties_window, ImGuiWindowFlags_MenuBar);
	App->GetModel()->RenderUI();
	ImGui::End();
}
