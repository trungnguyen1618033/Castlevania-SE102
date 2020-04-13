#pragma once
#include "GameObject.h"
#include "Textures.h"
#include "Define.h"
#include "Animations.h"


class WhipItem : public GameObject
{
public:
	WhipItem() : GameObject() {}


	void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* Objects = NULL, vector<LPGAMEOBJECT*>* coObject = NULL);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

