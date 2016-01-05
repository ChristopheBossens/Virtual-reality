#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>

namespace Shapes
{
	class Square
	{
	private:
		GLuint VBO;
		GLuint EBO;
		GLuint VAO;

	public:
		Square();
		~Square();

		void Draw();
	};
}