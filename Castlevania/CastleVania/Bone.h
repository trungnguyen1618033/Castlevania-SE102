#pragma once
#include "GameObject.h"

class Bone : public GameObject
{
public:
	Bone();
	~Bone() {};
	float num = 1;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL, bool stopMoving = false);
	virtual void SetNum(float num) {
		this->num = num;
	}
	void Render();

	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

