#pragma once
#include "GameObject.h"
#include "Textures.h"
#include "Define.h"
#include "Animations.h"
#include "Torch.h"
#include "Simon.h"

class Weapon : public GameObject
{
	bool isHolyWater = false;
	int holyWaterCounter = 0;

public:
	Weapon();
	~Weapon() {};

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void SetState(int state);

	void HolyWaterEffect() { isHolyWater = true; holyWaterCounter = GetTickCount(); }
};

