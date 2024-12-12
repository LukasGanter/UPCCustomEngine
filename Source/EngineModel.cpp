#include "Globals.h"
#include "Application.h"
#include "EngineModel.h"

#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE 
#include "ModuleTexture.h"
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

	for (const auto& texture : meshTextures)
		glDeleteTextures(1, &texture);
	meshTextures.clear();
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
			auto mesh =  new EngineMesh();
			mesh->Load(model, srcMesh, primitive);

			meshes.push_back(mesh);
		}
	}
	LoadMaterials(model);
}

void EngineModel::Render() const
{
	for (const auto& mesh : meshes)
		mesh->Draw(meshTextures);
}

void EngineModel::LoadMaterials(const tinygltf::Model& srcModel)
{
	for(const auto& srcMaterial : srcModel.materials)
	{
		unsigned int textureId = 0;
		if(srcMaterial.pbrMetallicRoughness.baseColorTexture.index >= 0)
		{
			const tinygltf::Texture& texture = srcModel.textures[srcMaterial.pbrMetallicRoughness.baseColorTexture.index];
			const tinygltf::Image& image = srcModel.images[texture.source];
			textureId = App->GetTexture()->LoadTexture("Resources/Models/BakerHouse/" + image.uri);	// TODO Remove hard coded path
		}
		meshTextures.push_back(textureId);
	}
}




