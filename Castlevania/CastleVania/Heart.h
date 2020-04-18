#pragma once

#include "GameObject.h"

class Heart : public GameObject
{
	DWORD timeAppear;   // thời gian bắt đầu xuất hiện của item, dùng để xét cho item tự huỷ
public:
	Heart();

	void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL, bool stopMovement = false);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetState(int state);
};

