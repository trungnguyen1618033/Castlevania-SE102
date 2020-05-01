#pragma once
#include "GameObject.h"
#include "Textures.h"
#include "Define.h"
#include "Animations.h"

class MoneyBag: public GameObject
{
	DWORD timeAppear;   // thời gian bắt đầu xuất hiện của item, dùng để xét cho item tự huỷ
public:
	MoneyBag();

	void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

