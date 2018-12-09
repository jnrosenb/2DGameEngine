#version 330 core

out vec4 frag_color;

in vec2 texuv;

uniform sampler2D ourTexture;

void main(void)
{
	frag_color = texture(ourTexture, texuv);
	if(frag_color.a < 0.2f)
        discard;
}