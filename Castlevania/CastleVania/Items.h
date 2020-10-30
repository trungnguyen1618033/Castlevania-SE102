#pragma once
#include "GameObject.h"
#include "Textures.h"
#include "Define.h"
#include "Animations.h"


class Items : public GameObject
{
	DWORD timeAppear;
	float velocityVariation_x;	// biến thiên vận tốc theo chiều x để cho small heart bay lảo đảo
public:
	Items();
	~Items() {};

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL, bool stopMoving = false);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void SetState(int state);

};


