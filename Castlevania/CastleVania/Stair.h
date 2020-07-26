#pragma once

#include "GameObject.h"
#include "Sprites.h"
#include "Textures.h"

class Stair : public GameObject
{
public:
	Stair();
	~Stair() {};

	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

