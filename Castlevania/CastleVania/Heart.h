#pragma once

#include "GameObject.h"

class Heart : public GameObject
{
	
public:
	Heart() : GameObject() {};

	void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL, bool stopMovement = false);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetState(int state);
};

