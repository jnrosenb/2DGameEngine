#version 330 core

in vec4 outColor;

out vec4 frag_color;

void main(void)
{
	frag_color = outColor;
}