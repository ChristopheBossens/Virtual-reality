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
		GLuint texID;

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
		void SetScaling(float scaleFactor);
		void SetColor(float r, float g, float b);

		// Manipulator functions for adjusting individual vertices
		void SetTopLeft(float x, float y, float z);
		void SetTopRight(float x, float y, float z);
		void SetBottomLeft(float x, float y, float z);
		void SetBottomRight(float x, float y, float z);

		// 
		void Apply();
		void MatchTextureToScale();
		void SetTextureID(GLuint texID);
	};
}