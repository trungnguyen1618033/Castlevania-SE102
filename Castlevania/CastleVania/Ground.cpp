#include "Ground.h"

#define GROUND_BBOX_WIDTH 32
#define GROUND_BBOX_HEIGHT 32

Ground::Ground() : GameObject()
{


}

void Ground::Render()
{	
}

void Ground::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + GROUND_BBOX_WIDTH;
	b = t + GROUND_BBOX_HEIGHT;
}