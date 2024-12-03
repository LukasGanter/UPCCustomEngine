#pragma once

//#include "tinyGltf/tiny_gltf.h"

namespace tinygltf
{
	class Model;
	struct Mesh;
	struct Primitive;
}


class EngineMesh
{
public:
	EngineMesh();
	~EngineMesh();

	void Load(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);

	void Draw();

private:

	void LoadVBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);

	void LoadEBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);

	void CreateVAO();

	unsigned vbo;
	unsigned ebo;
	unsigned vao;

	int indexCount = -1;

};
