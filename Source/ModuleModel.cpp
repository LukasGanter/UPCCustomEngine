#include "Globals.h"
#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleModel.h"
#include "ModuleCamera.h"

#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE 
#define TINYGLTF_IMPLEMENTATION /* Only in one of the includes */
#include "tinyGltf/tiny_gltf.h"


ModuleModel::ModuleModel()
{
	
}

// Destructor
ModuleModel::~ModuleModel()
{
	
}

// Called before render is available
bool ModuleModel::Init()
{
	LOG("Creating Base Model");

	houseModel = new EngineModel();
	houseModel->Load("Resources/Models/BakerHouse/BakerHouse.gltf");

	return true;
}

update_status ModuleModel::PreUpdate()
{
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleModel::Update()
{
	const unsigned int program = App->GetProgram()->getProgram();
	glUseProgram(program);
	
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, &App->GetCamera()->model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, &App->GetCamera()->view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, &App->GetCamera()->projection[0][0]);
	
	if (houseModel != nullptr) {
		houseModel->Render();
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
	delete houseModel;
	return true;
}

