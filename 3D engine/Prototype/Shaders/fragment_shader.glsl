#version 330 core
in vec3 outColor;
in vec2 outTexCoord;

out vec4 color;

uniform sampler2D textureSampler;
uniform float textureOffset;

void main()
{
	//outTexCoord.x += textureOffset;
	color = texture(textureSampler, vec2(outTexCoord.x + textureOffset, outTexCoord.y)) * vec4(outColor, 1.0f);
}