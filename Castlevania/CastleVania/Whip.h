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


	vector<vector<float>> sparkEffect; // vector lưu toạ độ để render spark khi roi đánh trúng mục tiêu
	Animation* spark;
	int startTimeRenderSpark = 0;

public:
	Whip();
	~Whip() {};
	bool isCanHit = true;
	DWORD lastTimeThrown = 0;

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL, bool stopMoving = false);
	virtual void Render() {}
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void Render(int currentID);

	void SetWhipPosition(D3DXVECTOR3 simonPositon, bool isStand);
	bool CheckCollision(float obj_left, float obj_top, float obj_right, float obj_bottom);

	void Upgrade();

	int GetScoreReceived() { return scoreReceived; }
	int GetTargetTypeHit() { return targetTypeHit; }

	void SetScoreReceived(int x) { scoreReceived = x; }
	void SetTargetTypeHit(int x) { targetTypeHit = x; }

	void RenderSpark();

};

