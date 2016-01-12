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

		float xPosition, yPosition, zPosition;
		float xRotation, yRotation, zRotation;
		float xScaling, yScaling;

		GLfloat* vertices;

		void UpdateVBO();
	public:
		Square();
		~Square();

		void Draw();
		void SetPosition(float x, float y, float z);
		void SetRotation(float x, float y, float z);
		void SetScaling(float x, float y);
		void SetColor(float r, float g, float b);
		void MatchTextureToScale();
	};
}