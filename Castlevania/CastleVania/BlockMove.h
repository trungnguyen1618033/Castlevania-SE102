#pragma once
#include "GameObject.h"
#include "Textures.h"
#include "Define.h"
#include "Animations.h"

class BlockMove: public GameObject
{

public:
	BlockMove();
	~BlockMove() {};

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL, bool stopMoving = false);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

};
