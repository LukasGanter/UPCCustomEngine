#include "Globals.h"
#include "Application.h"
#include "EngineModel.h"

#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE 
#include "ModuleTexture.h"
#include "imgui/imgui.h"
#include "tinyGltf/tiny_gltf.h"

EngineModel::EngineModel()
= default;

// Destructor
EngineModel::~EngineModel()
{
	for (const auto& mesh : meshes)
		delete mesh;
	meshes.clear();
	
	delete minPosValues;
	delete maxPosValues;
}

void EngineModel::Load(const std::string& newModelPath, const std::string& newModelName)
{
	modelName = newModelName;
	tinygltf::TinyGLTF gltfContext;
	tinygltf::Model model;
	std::string error, warning;
	bool loadOk = gltfContext.LoadASCIIFromFile(&model, &error, &warning, newModelPath + modelName);
	if (!loadOk)
	{
		LOG("Error loading %s: %s", &modelName, error.c_str())
		return;
	}

	for (const auto& srcMesh : model.meshes)
	{
		for (const auto& primitive : srcMesh.primitives)
		{
			auto mesh =  new EngineMesh();
			
			mesh->Load(model, srcMesh, primitive);

			const float3* meshMinPosValues = mesh->GetMinPosValues();
			if (meshMinPosValues != nullptr) {
				if (minPosValues == nullptr)
				{
					minPosValues = new float3(*meshMinPosValues);
				} else
				{
					minPosValues = new float3(Min(*minPosValues, *meshMinPosValues));
				}
				
			}

			const float3* meshMaxPosValues = mesh->GetMaxPosValues();
			if (meshMinPosValues != nullptr)
			{
				if (maxPosValues == nullptr)
				{
					maxPosValues = new float3(*meshMaxPosValues);
				} else
				{
					maxPosValues = new float3(Max(*maxPosValues, *meshMaxPosValues));
				}
				
			}
			
			meshes.push_back(mesh);
		}
	}
	LoadMaterials(newModelPath, model);
}

void EngineModel::Render() const
{
	for (const auto& mesh : meshes)
		mesh->Draw();
}

void EngineModel::RenderUI() const
{
	if (ImGui::CollapsingHeader(modelName.c_str())) {
		for (const auto& mesh : meshes)
			mesh->RenderUI();
			ImGui::Spacing();
	}
	App->GetTextureModule()->RenderUI();
	
}

void EngineModel::LoadMaterials(const std::string& modelPath, const tinygltf::Model& srcModel)
{
	for(const auto& srcMaterial : srcModel.materials)
	{
		if(srcMaterial.pbrMetallicRoughness.baseColorTexture.index >= 0)
		{
			const tinygltf::Texture& texture = srcModel.textures[srcMaterial.pbrMetallicRoughness.baseColorTexture.index];
			const tinygltf::Image& image = srcModel.images[texture.source];
			App->GetTextureModule()->LoadTexture(modelPath + image.uri);	
		}
	}
}




