#pragma once
#include "GameObject.h"

class BatIntro : public GameObject
{
public:
	BatIntro();
	~BatIntro();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL, bool stopMovement = false);
	virtual void Render();
	virtual void SetState(int state);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {}

};
