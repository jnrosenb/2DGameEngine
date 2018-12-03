#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 center_size;
layout (location = 2) in vec4 color;
layout (location = 3) in vec4 vertexPosInfo;

uniform mat4 model; //Maybe unnecessary
uniform mat4 view;
uniform mat4 proj;

out vec4 outColor;

void main(void)
{
	vec3 scaledPos = pos * center_size.w;
	vec3 correctedPos = scaledPos + vertexPosInfo.xyz + center_size.xyz;

	gl_Position = proj * view * vec4(correctedPos, 1.0f);
	
	outColor = color;
}