#version 460
layout(location=0) in vec3 my_vertex_position;
layout(location=1) in vec2 vertex_uv0;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec2 uv0;

void main()
{
gl_Position = proj*view*model*my_vertex_position;
uv0 = vertex_uv0;
}