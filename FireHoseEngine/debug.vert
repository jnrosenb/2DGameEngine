#version 330 core

layout (location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main(void)
{
	gl_Position = proj * view * model * vec4(pos.xyz, 1.0f);
}