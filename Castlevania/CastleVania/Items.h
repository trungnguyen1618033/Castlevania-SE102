#pragma once
#include "GameObject.h"
#include "Textures.h"
#include "Define.h"
#include "Animations.h"


class Items : public GameObject
{
	DWORD timeAppear;   
public:
	Items();
	~Items() {};

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void GetRandomItem();

};


