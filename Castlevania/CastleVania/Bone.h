#pragma once
#include "GameObject.h"

class Bone : public GameObject
{
public:
	Bone();
	~Bone() {};

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();

	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

