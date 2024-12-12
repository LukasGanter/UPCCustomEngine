#version 460 
out vec4 color;
in vec2 uv0;
layout(binding=0) uniform sampler2D my_texture;
void main()
{
color = texture2D(my_texture, uv0);

}