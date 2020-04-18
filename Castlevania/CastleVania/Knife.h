#pragma once
#include "GameObject.h"
#include "Textures.h"
#include "Define.h"
#include "Animations.h"


class Knife : public GameObject
{
public:
	Knife();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* Objects = NULL, vector<LPGAMEOBJECT*>* coObject = NULL);
	void Render();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

