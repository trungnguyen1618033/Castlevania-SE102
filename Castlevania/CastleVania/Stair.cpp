#include "Stair.h"

// Stair
#define STAIR_BBOX_WIDTH			32
#define STAIR_BBOX_HEIGHT			32

Stair::Stair()
{
}

void Stair::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void Stair::Render()
{
	//RenderBoundingBox();
}

void Stair::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + STAIR_BBOX_WIDTH;
	b = t + STAIR_BBOX_HEIGHT;
}