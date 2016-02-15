#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace Engine
{
	struct Character {
		GLuint TextureID;
		glm::ivec2 Size;
		glm::ivec2 Bearing;
		GLuint advance;
	};

	class TextRenderer
	{
	private:
		std::map<GLchar, Character> characters;
		glm::mat4 projection;

		GLuint VAO;
		GLuint VBO;
		GLuint shaderProgram;

		void InitTextures();
		void InitBuffers();

	public:
		TextRenderer();
		~TextRenderer();

		void Init();
		void RenderText(GLuint shaderProgram, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
	};
}