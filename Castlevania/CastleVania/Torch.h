#pragma once
#include "GameObject.h"
#include "Textures.h"
#include "Define.h"
#include "Animations.h"

// TORCH
#define ID_TEX_TORCH				2

class Torch : public GameObject
{
public:
	Torch() : GameObject() {}

	/*void LoadResources();*/
	void Render();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {}

};

