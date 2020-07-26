#pragma once

#include "GameObject.h"
#include "Textures.h"
#include "Define.h"
#include "Animations.h"
#include "Torch.h"
#include "BreakWall.h"
#include "Bat.h"
#include "Knight.h"
#include "Ghost.h"
#include "HunchBack.h"
#include "Raven.h"
#include "Skeleton.h"
#include "Bone.h"
#include "Zombie.h"
#include "Boss.h"



class Whip : public GameObject
{
	int scoreReceived = 0;  // Điểm nhận được sau khi trúng mục tiêu
	int targetTypeHit = -1; // Loại mục tiêu đánh trúng (dùng để kiểm tra máu của boss...)
public:
	Whip();
	~Whip() {};

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL, bool stopMoving = false);
	virtual void Render() {}

	void Render(int currentID);

	void SetWhipPosition(D3DXVECTOR3 simonPositon, bool isStand);
	bool CheckCollision(float obj_left, float obj_top, float obj_right, float obj_bottom);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void Upgrade();
	void SetTargetTypeHit(int x) { targetTypeHit = x; }

};

