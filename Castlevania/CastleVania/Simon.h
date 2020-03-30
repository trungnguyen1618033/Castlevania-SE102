#pragma once

#include "GameObject.h"
#include "Textures.h"
#include "Define.h"
#include "Whip.h"

using namespace std;

class Simon : public GameObject
{
	bool isStand = true;

	Whip* whip;

public:
	Simon() : GameObject() {}

	void LoadResources();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void Render();
	void SetState(int state);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	bool IsStand();
};

