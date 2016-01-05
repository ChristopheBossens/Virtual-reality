#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>

namespace Shapes
{
	class Triangle
	{
	private:
		GLuint VBO;
		GLuint VAO;
	public:
		Triangle();
		~Triangle();

		void Draw();
	};
}