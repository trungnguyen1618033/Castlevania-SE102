#include "Ground.h"

Ground::Ground() : GameObject()
{

}

void Ground::Render()
{
	/*RenderBoundingBox();*/
}

void Ground::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + GROUND_BBOX_WIDTH;
	b = t + GROUND_BBOX_HEIGHT;
}