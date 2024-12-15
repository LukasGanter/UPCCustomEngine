#pragma once
#include "EngineMesh.h"
#include <vector>

class EngineModel
{
public:
	EngineModel();
	~EngineModel();

	void Load(const std::string& modelPath, const std::string& modelName);

	void Render() const;

	const float3* getMinPosValues() const { return minPosValues; }
	const float3* getMaxPosValues() const { return maxPosValues; }

	void RenderUI() const;

private:
	static void LoadMaterials(const std::string& modelPath, const tinygltf::Model &srcModel);

	std::string modelName;
	std::vector<EngineMesh*> meshes;

	float3* minPosValues = nullptr;
	float3* maxPosValues = nullptr;
};
