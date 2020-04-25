#pragma once
#include "GameObject.h"
#include "Textures.h"
#include "Define.h"
#include "Animations.h"
#include "Items.h"

// TORCH
#define ID_TEX_TORCH				2
#define TORCH			0
#define EFFECTEXPLORE	1


class Torch : public GameObject
{
	Items* item;
public:
	Torch();

	/*void LoadResources();*/
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* Objects = NULL, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	virtual void SetAnimationSet(LPANIMATION_SET ani_set);

};

