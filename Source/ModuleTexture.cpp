#include "Globals.h"
#include "ModuleTexture.h"
#include "DirectXTex.h"
#include <memory>

ModuleTexture::ModuleTexture()
{
	
}

// Destructor
ModuleTexture::~ModuleTexture()
{
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
update_status ModuleTexture::Update()
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

GLuint ModuleTexture::LoadTexture(const std::string& texturePath) const
{
	// TODO Only works for ASCII strings
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
	GLuint texture;
	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, image->GetMetadata().width, image->GetMetadata().height,
		0, format, type, image->GetPixels());
	glGenerateMipmap(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);	// TODO Always Texture0 can not be correct
	
	return texture;
}

