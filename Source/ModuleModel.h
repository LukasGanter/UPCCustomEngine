#pragma once
#include "Module.h"
#include "EngineModel.h"

class ModuleModel : public Module
{
public:
	ModuleModel();
	~ModuleModel();

	bool Init();
	update_status PreUpdate();
	update_status Update(const float deltaTime);
	update_status PostUpdate();
	bool CleanUp();

	void LoadModel(const std::string& filePath);

	EngineModel* GetLoadedModel() const { return loadedModel;}

private:

	EngineModel* loadedModel = nullptr;
};
