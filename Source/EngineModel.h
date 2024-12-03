#pragma once
#include "EngineMesh.h"
#include <vector>
#include <memory>

class EngineModel
{
public:
	EngineModel();
	~EngineModel();

	void Load(const char* assetFileName);

	void Render();

private:

	//std::vector<std::unique_ptr<EngineMesh>> meshes;
	std::vector<EngineMesh*> meshes;
};
