#version 460 
out vec4 color;
in vec2 uv0;
layout(binding=0) uniform sampler2D Diffuse;
void main()
{
color = vec4(0.0, 1.0, 1.0, 1.0);

}