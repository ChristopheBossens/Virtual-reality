#include "TextureLoader.h"

using namespace Core;

TextureLoader::TextureLoader()
{

}

TextureLoader::~TextureLoader()
{

}

GLuint TextureLoader::LoadRGB(const char* filename)
{
	TextureLoader::image = SOIL_load_image(filename, &(TextureLoader::width), &(TextureLoader::height), 0, SOIL_LOAD_RGB);

	if (image == 0)
		return -1;

	GLuint texture;
	glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);

	return texture;
}