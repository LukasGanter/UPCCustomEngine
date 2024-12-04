#pragma once
#include "EngineMesh.h"
#include <vector>
#include <memory>
#include "GL/glew.h"

class EngineModel
{
public:
	EngineModel();
	~EngineModel();

	void Load(const char* assetFileName);

	void Render() const;

private:
	
	void LoadMaterials(const tinygltf::Model &srcModel);

	//std::vector<std::unique_ptr<EngineMesh>> meshes;
	std::vector<EngineMesh*> meshes;
	std::vector<unsigned> meshTextures;
};
