#pragma once
#include "Module.h"
#include "EngineModel.h"

class ModuleModel : public Module
{
public:
	ModuleModel();
	~ModuleModel() override;

	bool Init() override;
	update_status Update(const float deltaTime) override;

	void LoadAssets(const std::string& filePath);
	EngineModel* GetLoadedModel() const { return loadedModel;}

	void RenderUI() const;

private:

	void LoadModel(const std::string& filePath);

	static void LoadTexture(const std::string& filePath);
	
	EngineModel* loadedModel = nullptr;
};
