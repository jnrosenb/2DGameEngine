#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;
layout (location = 2) in mat4 modelMatrices;

out vec2 texuv;

uniform mat4 view;
uniform mat4 proj;

void main(void)
{
	gl_Position = proj * view * modelMatrices * vec4(pos.xyz, 1.0f);
	texuv = uv;
}