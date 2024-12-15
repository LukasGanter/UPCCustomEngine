#pragma once

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleOpenGL.h"
#include "ModuleInput.h"
#include "ModuleProgram.h"
#include "ModuleDebugDraw.h"
#include "ModuleCamera.h"
#include "ModuleEditor.h"
#include "ModuleTexture.h"
#include "ModuleModel.h"

using namespace std;

Application::Application()
{
	// Order matters: they will Init/start/update in this order
	modules.push_back(windowModule = new ModuleWindow());
	modules.push_back(renderModule = new ModuleOpenGL());
	modules.push_back(programModule = new ModuleProgram());
	modules.push_back(inputModule = new ModuleInput());
	modules.push_back(textureModule = new ModuleTexture());
	modules.push_back(debugDrawModule = new ModuleDebugDraw());
	modules.push_back(cameraModule = new ModuleCamera());
	modules.push_back(modelModule = new ModuleModel());
	modules.push_back(editorModule = new ModuleEditor());
}

Application::~Application()
{
	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
    {
        delete *it;
    }
	modules.clear();

	for (std::string* message : logMsgBuffer)
	{
		delete message;
	}
	logMsgBuffer.clear();
}

bool Application::Init()
{
	bool ret = true;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init();

	return ret;
}

update_status Application::Update(const float deltaTime)
{
	update_status ret = UPDATE_CONTINUE;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PreUpdate();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->Update(deltaTime);

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PostUpdate();

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		ret = (*it)->CleanUp();

	return ret;
}

void Application::AddLogMessage(const char* message)
{
	if (logMsgBufferPtr >= LOG_MSG_BUFFER_SIZE)
	{
		logMsgBufferPtr = 0;
	}
	if (logMsgBuffer[App->logMsgBufferPtr] != nullptr )
	{
		delete logMsgBuffer[App->logMsgBufferPtr];
	}
	logMsgBuffer[App->logMsgBufferPtr] = new std::string(message);
	logMsgBufferPtr++;
}
