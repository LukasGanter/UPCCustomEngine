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

	void Load(const std::string& modelPath, const std::string& modelName);

	void Render() const;

private:
	
	void LoadMaterials(const std::string& modelPath, const tinygltf::Model &srcModel);

	//std::vector<std::unique_ptr<EngineMesh>> meshes;
	std::vector<EngineMesh*> meshes;
	std::vector<unsigned> meshTextures;
};
