#pragma once

#include "GameObject.h"
#include "Sprites.h"
#include "Textures.h"

class Ground : public GameObject
{
public:
	Ground();

	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};


