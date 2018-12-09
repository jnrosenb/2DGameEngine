#version 330 core

layout (location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main(void)
{
	vec4 finalPos = proj * view * model * vec4(pos.xyz, 1.0f);
	gl_Position = vec4(finalPos.x, finalPos.y, finalPos.z, finalPos.w);
}