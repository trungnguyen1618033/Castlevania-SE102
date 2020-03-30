#pragma once

#include "GameObject.h"
#include "Textures.h"
#include "Define.h"

class Whip : public GameObject
{
public:
	Whip() : GameObject() {}

	void LoadResources();
	void Update();
	void Render();

	void SetWhipPosition(D3DXVECTOR3 simonPositon, bool isStand, int nx);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {}
};

