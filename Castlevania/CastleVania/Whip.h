#pragma once

#include "GameObject.h"
#include "Textures.h"
#include "Define.h"
#include "Animations.h"
#include "Torch.h"



class Whip : public GameObject
{
	DWORD timeCheck;
public:
	Whip();
	~Whip() {};

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render() {}

	void Render(int currentID);

	void SetWhipPosition(D3DXVECTOR3 simonPositon, bool isStand);
	bool CheckCollision(float obj_left, float obj_top, float obj_right, float obj_bottom);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void Upgrade();

};

