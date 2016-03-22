#pragma once
#include <vector>
#include "Square.h"
#include "Camera.h"

class Scene
{
private:
	Camera sceneCamera;


public:
	Scene();

	virtual void Update(float deltaX = 0.0f, float deltaY = 0.0f, float deltaZ = 0.0f);
	virtual void Draw();
};