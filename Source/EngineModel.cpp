#include "Globals.h"
#include "Application.h"
#include "EngineModel.h"

#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE 
#include "tinyGltf/tiny_gltf.h"

EngineModel::EngineModel()
{
	
}

// Destructor
EngineModel::~EngineModel()
{
	for (const auto& mesh : meshes)
		delete mesh;
	meshes.clear();
}

void EngineModel::Load(const char* assetFileName)
{
	tinygltf::TinyGLTF gltfContext;
	tinygltf::Model model;
	std::string error, warning;
	bool loadOk = gltfContext.LoadASCIIFromFile(&model, &error, &warning, assetFileName);
	if (!loadOk)
	{
		LOG("Error loading %s: %s", assetFileName, error.c_str());
		return;
	}

	for (const auto& srcMesh : model.meshes)
	{
		for (const auto& primitive : srcMesh.primitives)
		{
			//auto mesh = std::make_unique<EngineMesh>();
			auto mesh =  new EngineMesh();
			mesh->Load(model, srcMesh, primitive);

			meshes.push_back(mesh);
		}
	}
	
}

void EngineModel::Render()
{
	for (const auto& mesh : meshes)
		mesh->Draw();
}




