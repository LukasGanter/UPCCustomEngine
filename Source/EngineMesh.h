#pragma once
#include <vector>

#include "Math/float3.h"
#include "Math/float2.h"

//#include "tinyGltf/tiny_gltf.h"

namespace tinygltf
{
	class Model;
	struct Mesh;
	struct Primitive;
}

struct Vertex
{
	float3 Position;
	float2 TexCoords;
};

class EngineMesh
{
public:
	EngineMesh();
	~EngineMesh();

	void Load(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);

	void Draw(const std::vector<unsigned>& textures);

	const float3* GetMinPosValues() const { return minPosValues; }
	const float3* GetMaxPosValues() const { return maxPosValues; }

private:

	void LoadVBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);

	void LoadEBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);

	void CreateVAO();

	unsigned vbo;
	unsigned ebo;
	unsigned vao;

	size_t indexCount = -1;
	size_t vertexCount = -1;
	int materialIndex = -1;

	float3* minPosValues = nullptr;
	float3* maxPosValues = nullptr;

};
