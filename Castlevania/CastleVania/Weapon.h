#pragma once
#include "GameObject.h"
#include "Textures.h"
#include "Define.h"
#include "Animations.h"
#include "Torch.h"
#include "Simon.h"

class Weapon : public GameObject
{
	int scoreReceived = 0;  // Điểm nhận được sau khi trúng mục tiêu
	int targetTypeHit = -1; // Loại mục tiêu đánh trúng (dùng để kiểm tra máu của boss...)

	bool isHolyWater = false;
	int holyWaterCounter = 0;

public:
	Weapon();
	~Weapon() {};

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL, bool stopMoving = false);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void SetState(int state);

	void HolyWaterEffect() { isHolyWater = true; holyWaterCounter = GetTickCount(); }

	void UpdateCollisionState();

	int GetTargetTypeHit() { return targetTypeHit; }
	void SetTargetTypeHit(int x) { targetTypeHit = x; }
};

