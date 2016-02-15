#include "TextureLoader.h"

using namespace Engine;

TextureLoader::TextureLoader()
{

}

TextureLoader::~TextureLoader()
{
	for (std::vector<GLuint>::iterator it = textureVector.begin(); it != textureVector.end(); ++it)
		glDeleteTextures(1, &(*it));
}

GLuint TextureLoader::LoadRGB(const char* filename)
{
	int width, height;
	TextureLoader::image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGB);

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

	textureVector.push_back(texture);
	return texture;
}

GLuint TextureLoader::LoadBlank()
{
	unsigned char buffer[4 * 4] = { 255, 255, 255, 255,
								 255, 255, 255, 255,
								 255, 255, 255, 255,
								 255, 255, 255, 255 };
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 4, 4, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, buffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	textureVector.push_back(textureID);
	return textureID;
}

GLuint TextureLoader::LoadNoiseTexture(int textureWidth, int textureHeight, int maxLuminance, int smoothSize)
{
	// Need an uneven smooth size
	if (smoothSize % 2 == 0)
		return -1;

	GLubyte* generatorImage = new GLubyte[(textureHeight + smoothSize)*(textureWidth+smoothSize)];
	GLubyte* noiseImage = new GLubyte[textureHeight*textureWidth];

	for (int i = 0; i < ((textureHeight + smoothSize)*(textureWidth+smoothSize)); ++i)
		generatorImage[i] = rand() % maxLuminance;

	int smoothOffset = (smoothSize - 1) / 2;
	int idx;
	for (int i = smoothOffset; i < (textureHeight + smoothOffset); ++i)
	{
		for (int j = smoothOffset; j < (textureWidth + smoothOffset);++j)
		{
			GLubyte totalNoiseValue = 0;
			
			for (int x = -smoothOffset; x <= smoothOffset;++x)
			{
				for (int y = -smoothOffset; y <= smoothOffset;++y)
				{
					idx = (i+x)*(textureWidth + smoothOffset) + (j+y);
					totalNoiseValue += (generatorImage[idx]/(smoothSize*smoothSize));
				}
			}
			idx = (i - smoothOffset)*(textureWidth) + (j - smoothOffset);
			noiseImage[idx] = totalNoiseValue;
		}
	}
	GLuint textureID;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, textureWidth, textureHeight, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, noiseImage);
	glBindTexture(GL_TEXTURE_2D, 0);

	delete[] generatorImage;
	delete[] noiseImage;

	textureVector.push_back(textureID);
	return textureID;
}

GLuint TextureLoader::LoadGratingTexture(int textureWidth, int textureHeight, int amplitude, double cyclesPerPixel, double orientation)
{
	GLubyte* gratingImage = new GLubyte[textureHeight*textureWidth];

	int idx;
	for (int i = 0; i < textureHeight; ++i)
	{
		for (int j = 0; j < textureWidth; ++j)
		{
			double xRotated = j*glm::cos(orientation) + i*glm::sin(orientation);
			idx = (i*textureWidth) + j;
			gratingImage[idx] = 127 + (int)(amplitude * glm::cos(2.0 * 3.1458*cyclesPerPixel*xRotated));
		}
	}

	GLuint textureID;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, textureWidth, textureHeight, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, gratingImage);
	glBindTexture(GL_TEXTURE_2D, 0);

	delete[] gratingImage;

	textureVector.push_back(textureID);
	return textureID;
}
