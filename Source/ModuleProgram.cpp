#include "Globals.h"
#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleWindow.h"
#include "SDL.h"
#include "GL/glew.h"

ModuleProgram::ModuleProgram()
{
	
}

// Destructor
ModuleProgram::~ModuleProgram()
{
}

// Called before render is available
bool ModuleProgram::Init()
{
	LOG("Creating Shader Program");

	createExerciseProgram();

	return true;
}

update_status ModuleProgram::PreUpdate()
{
	
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleProgram::Update(const float deltaTime)
{
	return UPDATE_CONTINUE;
}

update_status ModuleProgram::PostUpdate()
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleProgram::CleanUp()
{
	//LOG("Destroying renderer");

	glDeleteProgram(program);

	return true;
}

void ModuleProgram::createExerciseProgram()
{
	char* vertexShaderSrc = LoadShaderSource("Shader/ExampleVertexShader.vert");
	char* fragmentShaderSrc = LoadShaderSource("Shader/ExampleFragmentShader.frag");

	unsigned vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSrc);
	unsigned fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);

	program = CreateProgram(vertexShader, fragmentShader);

	free(vertexShaderSrc);
	free(fragmentShaderSrc);
}

unsigned ModuleProgram::getProgram() const
{
	return program;
}

char* ModuleProgram::LoadShaderSource(const char* shader_file_name)
{
	char* data = nullptr;
	FILE* file = nullptr;
	fopen_s(&file, shader_file_name, "rb");
	if (file)
	{
		fseek(file, 0, SEEK_END);
		int size = ftell(file);
		data = (char*)malloc(size + 1);
		fseek(file, 0, SEEK_SET);
		fread(data, 1, size, file);
		data[size] = 0;
		fclose(file);
	}
	return data;
}

unsigned ModuleProgram::CompileShader(unsigned type, const char* source)
{
	unsigned shader_id = glCreateShader(type);

	LOG("Log Info: %s", "info");
	glShaderSource(shader_id, 1, &source, 0);
	glCompileShader(shader_id);
	int res = GL_FALSE;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &res);
	if (res == GL_FALSE)
	{
		int len = 0;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &len);
		if (len > 0)
		{
			int written = 0;
			char* info = (char*)malloc(len);
			glGetShaderInfoLog(shader_id, len, &written, info);
			LOG("Log Info: %s", info);
			free(info);
		}
	}
	return shader_id;
}

unsigned ModuleProgram::CreateProgram(unsigned vtx_shader, unsigned frg_shader)
{
	unsigned program_id = glCreateProgram();
	glAttachShader(program_id, vtx_shader);
	glAttachShader(program_id, frg_shader);
	glLinkProgram(program_id);
	int res;
	glGetProgramiv(program_id, GL_LINK_STATUS, &res);
	if (res == GL_FALSE)
	{
		int len = 0;
		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &len);
		if (len > 0)
		{
			int written = 0;
			char* info = (char*)malloc(len);
			glGetProgramInfoLog(program_id, len, &written, info);
			LOG("Program Log Info: %s", info);
			free(info);
		}
	}
	glDeleteShader(vtx_shader);
	glDeleteShader(frg_shader);
	return program_id;
}

