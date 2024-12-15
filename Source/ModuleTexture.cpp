#include "Globals.h"
#include "ModuleTexture.h"
#include "DirectXTex.h"
#include <memory>

#include "imgui/imgui.h"

ModuleTexture::ModuleTexture()
{
	
}

// Destructor
ModuleTexture::~ModuleTexture()
{
	if (textureValid)
	{
		glDeleteTextures(1, &meshTexture);
	}
}

// Called before render is available
bool ModuleTexture::Init()
{
	LOG("Creating Renderer context");

	return true;
}

update_status ModuleTexture::PreUpdate()
{
	
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleTexture::Update(const float deltaTime)
{
	return UPDATE_CONTINUE;
}

update_status ModuleTexture::PostUpdate()
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleTexture::CleanUp()
{
	return true;
}

void ModuleTexture::RenderUI()
{
	if (textureValid)
	{
		if (ImGui::CollapsingHeader(loadedTextureName.c_str())) {
			ImGui::Text(("Width: " + std::to_string(loadedTextureWidth)).c_str());
			ImGui::Text(("Height: " + std::to_string(loadedTextureHeight)).c_str());
			ImGui::Image(meshTexture, ImVec2(300, 300.f * loadedTextureHeight / loadedTextureWidth));
		}
	} else
	{
		if (ImGui::CollapsingHeader("No texture loaded")) {}
	}
	
}

GLuint ModuleTexture::LoadTexture(const std::string& texturePath)
{
	if (textureValid)
	{
		glDeleteTextures(1, &meshTexture);
	textureValid = false;
	}
	
	std::wstring widestr = std::wstring(texturePath.begin(), texturePath.end());
	const wchar_t* path = widestr.c_str();
	
	const auto image = std::make_unique<DirectX::ScratchImage>();

	HRESULT loadingResult = LoadFromDDSFile(path, DirectX::DDS_FLAGS_NONE, nullptr, *image);
	if (FAILED(loadingResult))
	{
		loadingResult = LoadFromTGAFile(path, DirectX::TGA_FLAGS_NONE, nullptr, *image);
		if (FAILED(loadingResult))
		{
			loadingResult = LoadFromWICFile(path, DirectX::WIC_FLAGS_NONE, nullptr, *image);
			if (FAILED(loadingResult))
			{
				LOG("Failed to load image");
				return false;
			}
		}
	}

	loadedTextureWidth = image->GetMetadata().width;
	loadedTextureHeight = image->GetMetadata().height;
	const unsigned long long splitPositionFileName = texturePath.find_last_of("\\");
	loadedTextureName = texturePath.substr(splitPositionFileName + 1, texturePath.length());

	GLint internalFormat = GL_NONE;
	GLint format = GL_NONE;
	GLenum type = GL_NONE;
	
	switch (image->GetMetadata().format)
	{
	case DXGI_FORMAT_R8G8B8A8_UNORM:
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
		internalFormat = GL_RGBA8;
		format = GL_RGBA;
		type = GL_UNSIGNED_BYTE;
		break;
	case DXGI_FORMAT_B5G6R5_UNORM:
		internalFormat = GL_RGB8;
		format = GL_BGR;
		type = GL_UNSIGNED_BYTE;
		break;
	case DXGI_FORMAT_B8G8R8A8_UNORM:
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
		internalFormat = GL_RGBA8;
		format = GL_BGRA;
		type = GL_UNSIGNED_BYTE;
		break;
	default:
		assert(false && "Unsupported format");
	}
	
	glGenTextures(1, &meshTexture);
	glBindTexture(GL_TEXTURE_2D, meshTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, image->GetMetadata().width, image->GetMetadata().height,
		0, format, type, image->GetPixels());
	glGenerateMipmap(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);	

	textureValid = true;
	
	return meshTexture;
}

const int ModuleTexture::getLoadedTextureID() const
{
	if (textureValid)
	{
		return meshTexture;
	}
	return -1;
}


