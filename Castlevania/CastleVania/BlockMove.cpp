#include "BlockMove.h"



BlockMove::BlockMove() : GameObject()
{
	nx = 1;
	vx = SPEED_BLOCK;
}

void BlockMove::Render()
{
	animation_set->at(GetState())->Render(1, nx, x, y);
}

void BlockMove::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMoving)
{
	GameObject::Update(dt);

	//DebugOut("dx_b: %d\n", this->dx);
	x += dx;

	if (x <= 64)
	{
		nx = 1;
		vx = SPEED_BLOCK;
	}
	else if (x >= 128)
	{
		nx = -1;
		vx = -SPEED_BLOCK;
	}


}

void BlockMove::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + BLOCK_BBOX_WIDTH;
	bottom = top + BLOCK_BBOX_HEIGHT;
}
