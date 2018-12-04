#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 center_size;
layout (location = 2) in vec4 color;
layout (location = 3) in vec4 vertexPosInfo;
layout (location = 4) in vec4 uv;

uniform mat4 model; //Maybe unnecessary
uniform mat4 view;
uniform mat4 proj;

out vec4 outColor;
out vec2 frag_uv;

void main(void)
{
	//Rotation experiment
	float r = vertexPosInfo.w;
	mat3 rot = {
		vec3( cos(r), sin(r), 0.0f),
		vec3(-sin(r), cos(r), 0.0f),
		vec3( 0.0f,   0.0f,   1.0f)	
	};
	vec3 rotatedPos = rot * pos;
	vec3 scaledPos = rotatedPos * center_size.w;

	//vec3 scaledPos = pos * center_size.w;
	vec3 correctedPos = scaledPos + vertexPosInfo.xyz + center_size.xyz;
	gl_Position = proj * view * vec4(correctedPos, 1.0f);
	
	outColor = color;

	//UV calculation experiment (INNEFFICIENT FOR NOW)
	vec2 baseUV = vec2(pos.x + 0.5f, pos.y + 0.5f) / uv.w;
	float delta = 1.0f / uv.w;
	int currentCorrected = int(uv.x) + int(uv.z);
	int row =  int( currentCorrected / int(uv.w) );
	int col =  int( currentCorrected % int(uv.w) );
	frag_uv = vec2(baseUV.x + col * delta, (baseUV.y + row * delta));
}