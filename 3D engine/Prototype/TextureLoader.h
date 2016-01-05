#pragma once

#include <SOIL.h>
#include <GL\glew.h>

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
	};
}