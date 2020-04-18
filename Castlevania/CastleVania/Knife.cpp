#include "Knife.h"

#define KNIFE_SPEED			0.3f
#define KNIFE_BBOX_WIDTH			16
#define KNIFE_BBOX_HEIGHT			16		

Knife::Knife() : GameObject()
{
	vx = KNIFE_SPEED;
}


void Knife::Render()
{
	animation_set->at(state)->Render(nx, x, y);
}

void Knife::Update(DWORD dt, vector<LPGAMEOBJECT>* Objects, vector<LPGAMEOBJECT*>* coObject)
{
	if (nx < 0) vx = -KNIFE_SPEED;
	else vx = KNIFE_SPEED;

	GameObject::Update(dt);
	x += dx;
}

void Knife::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + KNIFE_BBOX_WIDTH;
	bottom = top + KNIFE_BBOX_HEIGHT;
}
