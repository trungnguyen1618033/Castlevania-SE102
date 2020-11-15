#include "Bone.h"

Bone::Bone()
{
	SetState(0);
	AnimationSets* animation_sets = AnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(30);
	SetAnimationSet(ani_set);
}

void Bone::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMoving)
{
	if (stopMoving == true)
		return;

	vy += BONE_GRAVITY * dt;
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
			vx = num*BONE_SPEED_X;
		else
			vx = -num*BONE_SPEED_X;
		vy = -BONE_SPEED_Y;
		break;
	default:
		break;
	}
}

void Bone::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + BONE_BBOX_WIDTH;
	bottom = top + BONE_BBOX_HEIGHT;
}
