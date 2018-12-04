#version 330 core

in vec4 outColor;
in vec2 frag_uv;

out vec4 frag_color;

uniform sampler2D sheet;


void main(void)
{
	//frag_color = outColor;
	frag_color = texture(sheet, frag_uv.xy) * outColor;
	if(frag_color.a < 0.2f) //CHECK
        discard;			 //CHECK
}