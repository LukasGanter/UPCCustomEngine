#include "Globals.h"
#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleModel.h"
#include "ModuleCamera.h"

#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE 
#define TINYGLTF_IMPLEMENTATION /* Only in one of the includes */
#include <GL/glew.h>

#include "ModuleTexture.h"
#include "tinyGltf/tiny_gltf.h"


ModuleModel::ModuleModel()
{
	
}

// Destructor
ModuleModel::~ModuleModel()
{
	delete loadedModel;
}

// Called before render is available
bool ModuleModel::Init()
{
	LOG("Creating Base Model");

	LoadModel("Resources\\Models\\BakerHouse\\BakerHouse.gltf");

	return true;
}

update_status ModuleModel::PreUpdate()
{
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleModel::Update(const float deltaTime)
{
	const unsigned int program = App->GetProgram()->getProgram();
	glUseProgram(program);
	
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, &App->GetCamera()->model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, &App->GetCamera()->view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, &App->GetCamera()->projection[0][0]);
	
	if (loadedModel != nullptr) {
		loadedModel->Render();
	}
	return UPDATE_CONTINUE;
}

update_status ModuleModel::PostUpdate()
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleModel::CleanUp()
{
	return true;
}

void ModuleModel::LoadAssets(const std::string& filePath)
{
	if (filePath.compare(filePath.size()-5, 5, ".gltf") == 0)
	{
		LoadModel(filePath);
	} else if (filePath.compare(filePath.size()-4, 4, ".png") == 0 || filePath.compare(filePath.size()-4, 4, ".dds") == 0)
	{
		LoadTexture(filePath);
	}
}

void ModuleModel::RenderUI()
{
	if (loadedModel != nullptr)
	{
		loadedModel->RenderUI();
	}
}

void ModuleModel::LoadModel(const std::string& filePath)
{
	delete loadedModel;
	loadedModel = new EngineModel();
	const unsigned long long splitPositionFileName = filePath.find_last_of("\\");
	loadedModel->Load(filePath.substr(0, splitPositionFileName + 1), filePath.substr(splitPositionFileName + 1, filePath.length()));
	App->GetCamera()->FocusMesh();
}

void ModuleModel::LoadTexture(const std::string& filePath)
{
	App->GetTexture()->LoadTexture(filePath);	
}

