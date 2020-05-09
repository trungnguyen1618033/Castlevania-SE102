#pragma once
#include "GameObject.h"
#include "Textures.h"
#include "Define.h"
#include "Animations.h"
#include "Items.h"

// TORCH
#define ID_TEX_TORCH				2
#define TORCH			0
#define EFFECTEXPLODE		1
#define WHIPITEM		2
#define HEART_SMALL		3
#define HEART_BIG		4
#define MONEYBAG		5
#define KNIFE			6

#define ITEM_TIME_DESTROYED			5000



class Torch : public GameObject
{
	DWORD timeAppear;   
public:
	
	Torch();

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	virtual void SetAnimationSet(LPANIMATION_SET ani_set);


};

