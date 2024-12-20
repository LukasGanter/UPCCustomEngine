#include "Globals.h"
#include "Application.h"
#include "EngineMesh.h"

#include "ModuleTexture.h"
#include "SDL.h"
#include "GL/glew.h"
#include "imgui/imgui.h"
#include "Math/float3.h"
#include "Time/Clock.h"
#include "tinyGltf/tiny_gltf.h"

using namespace tinygltf;

EngineMesh::EngineMesh()
= default;

// Destructor
EngineMesh::~EngineMesh()
{
	delete minPosValues;
	delete maxPosValues;
}

void EngineMesh::Load(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive)
{
	LOG("Loading mesh: %s", mesh.name.c_str())
	materialIndex = primitive.material;
	LoadVBO(model, mesh, primitive);
	LoadEBO(model, mesh, primitive);
	CreateVAO();

	meshTitle = mesh.name;
}

void EngineMesh::LoadVBO(const Model& model, const Mesh& mesh, const Primitive& primitive)
{
	
	const auto& itPos = primitive.attributes.find("POSITION");
	if (itPos != primitive.attributes.end())
	{
		LOG("Loading vertex buffer object: position")
		const Accessor& posAcc = model.accessors[itPos->second];
		vertexCount = posAcc.count;
		if (posAcc.minValues.size() == 3)
		{
			delete minPosValues;
			minPosValues = new float3(posAcc.minValues[0], posAcc.minValues[1], posAcc.minValues[2]);
		}
		if (posAcc.maxValues.size() == 3)
		{
			delete maxPosValues;
			maxPosValues = new float3(posAcc.maxValues[0], posAcc.maxValues[1], posAcc.maxValues[2]);
		}
		SDL_assert(posAcc.type == TINYGLTF_TYPE_VEC3);
		SDL_assert(posAcc.componentType == GL_FLOAT);
		const BufferView& posView = model.bufferViews[posAcc.bufferView];
		const Buffer& posBuffer = model.buffers[posView.buffer];
		const unsigned char* bufferPos = &(posBuffer.data[posAcc.byteOffset + posView.byteOffset]);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		
		const auto& itTex = primitive.attributes.find("TEXCOORD_0");
		if (itTex != primitive.attributes.end())
		{
			LOG("Loading vertex buffer object: texture coordinates")
			const Accessor& texCoordAcc = model.accessors[itTex->second];
		
			SDL_assert(texCoordAcc.count == vertexCount);
			SDL_assert(texCoordAcc.type == TINYGLTF_TYPE_VEC2);
			SDL_assert(texCoordAcc.componentType == GL_FLOAT);
		
			const BufferView& texCoordView = model.bufferViews[texCoordAcc.bufferView];
			const Buffer& texCoordBuffer = model.buffers[texCoordView.buffer];
			const unsigned char* texCoordbufferPos = &(texCoordBuffer.data[texCoordAcc.byteOffset + texCoordView.byteOffset]);

			
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertexCount, nullptr, GL_STATIC_DRAW);
		
			Vertex* ptr = static_cast<Vertex*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
			for (size_t i = 0; i < vertexCount; ++i)
			{
				const float3 pos = *reinterpret_cast<const float3*>(bufferPos);
				const float2 texCoord = *reinterpret_cast<const float2*>(texCoordbufferPos);
				ptr[i] = {pos, texCoord};
				bufferPos += posView.byteStride == 0 ? sizeof(float) * 3 : posView.byteStride;
				texCoordbufferPos += texCoordView.byteStride == 0 ? sizeof(float) * 2 : texCoordView.byteStride;
			}
			glUnmapBuffer(GL_ARRAY_BUFFER);
		} else
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * vertexCount, nullptr, GL_STATIC_DRAW);
		
			float3* ptr = static_cast<float3*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
			for (size_t i = 0; i < vertexCount; ++i)
			{
				ptr[i] = *reinterpret_cast<const float3*>(bufferPos);
				bufferPos += posView.byteStride == 0 ? sizeof(float) * 3 : posView.byteStride;
			}
			glUnmapBuffer(GL_ARRAY_BUFFER);
		}
	}
}


void EngineMesh::LoadEBO(const Model& model, const Mesh& mesh, const Primitive& primitive)
{
	if (primitive.indices >= 0)
	{
		LOG("Loading index buffer object")
		const Accessor& indAcc = model.accessors[primitive.indices];
		indexCount = indAcc.count;

		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexCount, nullptr, GL_STATIC_DRAW);
		unsigned int* ptr = static_cast<unsigned int*>(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));
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
	
	LOG("Generating vao")
	
	glGenVertexArrays(1, &vao);
	
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3+sizeof(float)*2, static_cast<void*>(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)*3+sizeof(float)*2, reinterpret_cast<void*>(sizeof(float) * 3));

	glBindVertexArray(0);
}

void EngineMesh::Draw() const
{
	const int loadedTexture = App->GetTextureModule()->getLoadedTextureID();
	if (loadedTexture != -1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, loadedTexture);
	}
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
}

void EngineMesh::RenderUI() const
{
	ImGui::Text(("Mesh: " + meshTitle).c_str());
	ImGui::Text(("Vertices: " + std::to_string(vertexCount)).c_str());
	ImGui::Text(("Triangles: " + std::to_string(indexCount / 3)).c_str());
}







