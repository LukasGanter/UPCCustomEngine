#pragma once

#include<list>
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
	update_status Update();
	bool CleanUp();

    ModuleOpenGL* GetOpenGL() const { return render; }
    ModuleWindow* GetWindow() const { return window; }
    ModuleInput*  GetInput() const { return input; }
    ModuleProgram* GetProgram() const { return program; }
    ModuleDebugDraw* GetDebugDraw() const { return debugDraw; }
    ModuleCamera* GetCamera() const { return camera; }
    ModuleEditor* GetEditor() const { return editor; }
    ModuleTexture* GetTexture() const { return texture; }
    ModuleModel* GetModel() const { return model; }

private:

    ModuleOpenGL* render = nullptr;
    ModuleWindow* window = nullptr;
    ModuleInput* input = nullptr;
    ModuleProgram* program = nullptr;
    ModuleDebugDraw* debugDraw = nullptr;
    ModuleCamera* camera = nullptr;
    ModuleEditor* editor = nullptr;
    ModuleTexture* texture = nullptr;
    ModuleModel* model = nullptr;


    std::list<Module*> modules;

};

extern Application* App;
