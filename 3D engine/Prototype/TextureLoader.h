#pragma once

#include <SOIL.h>
#include <GL\glew.h>
#include <cstdlib>
#include <glm\glm.hpp>

namespace Core
{
	class TextureLoader {
	private:
		unsigned char* image;
		int width;
		int height;

	public:
		TextureLoader();
		~TextureLoader();

		GLuint LoadRGB(const char* filename);
		GLuint LoadBlank();
		GLuint LoadNoiseTexture(int textureWidth, int textureHeight, int maxLuminance, int smoothSize);
		GLuint LoadGratingTexture(int textureWidth, int texureHeight, int amplitude, double cyclesPerpixel, double orientation);
	};
}