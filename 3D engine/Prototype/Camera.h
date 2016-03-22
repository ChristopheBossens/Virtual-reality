#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

class Camera
{
private:
	glm::vec3 cameraPosition;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;

	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	float yaw;
public:
	Camera();

	float* pViewMatrix;
	float* pProjectionMatrix;

	float GetX();
	float GetY();
	float GetZ();

	void SetPosition(float x, float y, float z);
	void SetX(float x);
	void SetY(float y);
	void SetZ(float z);

	glm::mat4 GetViewMatrix();
	void SetProjectionMatrix(float fieldOfView, float aspectRatio, float nearZ = 0.1f, float farZ = 100.0f);
};