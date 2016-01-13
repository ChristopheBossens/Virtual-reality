#pragma once

#include <SOIL.h>
#include <GL\glew.h>
#include <cstdlib>
#include <glm\glm.hpp>
#include <vector>
namespace Core
{
	class TextureLoader {
	private:
		unsigned char* image;
		std::vector<GLuint> textureVector;

	public:
		TextureLoader();
		~TextureLoader();

		GLuint LoadRGB(const char* filename);
		GLuint LoadBlank();
		GLuint LoadNoiseTexture(int textureWidth, int textureHeight, int maxLuminance, int smoothSize);
		GLuint LoadGratingTexture(int textureWidth, int texureHeight, int amplitude, double cyclesPerpixel, double orientation);
	};
}