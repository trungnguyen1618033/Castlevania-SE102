#include "Stair.h"


Stair::Stair()
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