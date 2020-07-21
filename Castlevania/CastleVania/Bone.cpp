#include "Bone.h"
#define BONE_SPEED				0.2f

Bone::Bone()
{
	SetState(0);
}

void Bone::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += 0.001f * dt;
	GameObject::Update(dt);
	x += dx;
	y += dy;
}

void Bone::Render()
{
	animation_set->at(state)->Render(1, nx, x, y);
}

void Bone::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case 0:
		if (nx == 1)
			vx = BONE_SPEED;
		else
			vx = -BONE_SPEED;
		vy = 0.5;
		break;
	default:
		break;
	}
}

void Bone::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + 32;
	bottom = top + 28;
}
