#include "Globals.h"
#include "Application.h"
#include "ModuleProgram.h"
#include "EngineMesh.h"
#include "SDL.h"
#include "GL/glew.h"
#include "Math/float3.h"
#include "tinyGltf/tiny_gltf.h"

using namespace tinygltf;

EngineMesh::EngineMesh()
{
	
}

// Destructor
EngineMesh::~EngineMesh()
{
}

void EngineMesh::Load(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive)
{
	LoadVBO(model, mesh, primitive);
	LoadEBO(model, mesh, primitive);
	CreateVAO();
}

void EngineMesh::LoadVBO(const Model& model, const Mesh& mesh, const Primitive& primitive)
{
	const auto& itPos = primitive.attributes.find("POSITION");
	if (itPos != primitive.attributes.end())
	{
		const Accessor& posAcc = model.accessors[itPos->second];
		SDL_assert(posAcc.type == TINYGLTF_TYPE_VEC3);
		SDL_assert(posAcc.componentType == GL_FLOAT);
		const BufferView& posView = model.bufferViews[posAcc.bufferView];
		const Buffer& posBuffer = model.buffers[posView.buffer];
		const unsigned char* bufferPos = &(posBuffer.data[posAcc.byteOffset + posView.byteOffset]);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * posAcc.count, nullptr, GL_STATIC_DRAW);

		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			printf("%i\n", err);
		}
		float3* ptr = reinterpret_cast<float3*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
		for (size_t i = 0; i < posAcc.count; ++i)
		{
			ptr[i] = *reinterpret_cast<const float3*>(bufferPos);
			bufferPos += posView.byteStride;
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}
}


void EngineMesh::LoadEBO(const Model& model, const Mesh& mesh, const Primitive& primitive)
{
	if (primitive.indices >= 0)
	{
		const Accessor& indAcc = model.accessors[primitive.indices];
		indexCount = indAcc.count;

		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexCount, nullptr, GL_STATIC_DRAW);
		unsigned int* ptr = reinterpret_cast<unsigned int*>(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));
		const BufferView& indView = model.bufferViews[indAcc.bufferView];
		const unsigned char* buffer = &(model.buffers[indView.buffer].data[indAcc.byteOffset +
			indView.byteOffset]);

		if (indAcc.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT)
		{
			const uint32_t* bufferInd = reinterpret_cast<const uint32_t*>(buffer);
			for (uint32_t i = 0; i < indexCount; ++i) ptr[i] = bufferInd[i];
		}
		else if (indAcc.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT)
		{
			const short* bufferInd = reinterpret_cast<const short*>(buffer);
			for (short i = 0; i < indexCount; ++i) ptr[i] = bufferInd[i];
		}
		else if (indAcc.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE)
		{
			const byte* bufferInd = reinterpret_cast<const byte*>(buffer);
			for (byte i = 0; i < indexCount; ++i) ptr[i] = bufferInd[i];
		}

		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	}
}


void EngineMesh::CreateVAO()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * indexCount));

	glBindVertexArray(0);
}

void EngineMesh::Draw(const std::vector<unsigned>& textures)
{
	glUseProgram(App->GetProgram()->getProgram());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);	// TODO Replace hardcoded 0 with the MaterialIndex
	glUniform1i(glGetUniformLocation(App->GetProgram()->getProgram(), "diffuse"), 0);
	
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
}







