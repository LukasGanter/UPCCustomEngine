#pragma once

#include<list>
#include <string>
#include <vector>

#include "Globals.h"
#include "Module.h"

class ModuleOpenGL;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleProgram;
class ModuleDebugDraw;
class ModuleCamera;
class ModuleEditor;
class ModuleTexture;
class ModuleModel;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update(const float deltaTime);
	bool CleanUp();

    ModuleOpenGL* GetOpenGLModule() const { return renderModule; }
    ModuleWindow* GetWindowModule() const { return windowModule; }
    ModuleInput*  GetInputModule() const { return inputModule; }
    ModuleProgram* GetProgramModule() const { return programModule; }
    ModuleDebugDraw* GetDebugDrawModule() const { return debugDrawModule; }
    ModuleCamera* GetCameraModule() const { return cameraModule; }
    ModuleEditor* GetEditorModule() const { return editorModule; }
    ModuleTexture* GetTextureModule() const { return textureModule; }
    ModuleModel* GetModelModule() const { return modelModule; }

	void AddLogMessage(const char* message);

	int logMsgBufferPtr = 0;
	std::vector<std::string*> logMsgBuffer = std::vector<std::string*>(LOG_MSG_BUFFER_SIZE, nullptr);

private:

    ModuleOpenGL* renderModule = nullptr;
    ModuleWindow* windowModule = nullptr;
    ModuleInput* inputModule = nullptr;
    ModuleProgram* programModule = nullptr;
    ModuleDebugDraw* debugDrawModule = nullptr;
    ModuleCamera* cameraModule = nullptr;
    ModuleEditor* editorModule = nullptr;
    ModuleTexture* textureModule = nullptr;
    ModuleModel* modelModule = nullptr;


    std::list<Module*> modules;

};

extern Application* App;
