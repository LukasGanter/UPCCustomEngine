#version 460 color = texture2D(mytexture, uv0);
out vec4 color;
in vec2 uv0;
layout(binding=0) uniform sampler2D mytexture;
void main()
{
color = vec4(0.0, 1.0, 1.0, 1.0);
}