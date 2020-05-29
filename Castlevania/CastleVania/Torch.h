#pragma once
#include "GameObject.h"
#include "Textures.h"
#include "Define.h"
#include "Animations.h"
#include "Items.h"

// TORCH
#define TORCH			0
#define CANDLE			1
#define EFFECTEXPLODE	2


class Torch : public GameObject
{
	DWORD timeAppear;   
public:
	
	Torch();

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetState(int state);

};

