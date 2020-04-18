#pragma once

#include "GameObject.h"
#include "Textures.h"
#include "Define.h"
#include "Animations.h"

#define ID_TEX_WHIP					1
#define NORMAL_WHIP	0

class Whip : public GameObject
{
public:
	Whip() : GameObject() {}

	/*void LoadResources();*/
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* Objects = NULL, vector<LPGAMEOBJECT>* coObjects = NULL) {};
	virtual void Render() {}

	void Render(int currentID);

	void SetWhipPosition(D3DXVECTOR3 simonPositon, bool isStand);
	bool CheckCollision(float obj_left, float obj_top, float obj_right, float obj_bottom);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

