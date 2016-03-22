#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D textureImage;
uniform vec3 spriteColor;
uniform float offsetParameter;

void main()
{
	color = texture(textureImage, vec2(TexCoords.x + offsetParameter, TexCoords.y));
}