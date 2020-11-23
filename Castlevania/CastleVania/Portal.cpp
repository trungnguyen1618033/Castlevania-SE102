#include "Portal.h"


Portal::Portal(float l, float t, float r, float b, int scene_id, int scene_back)
{
	this->scene_id = scene_id;
	this->scene_back = scene_back;
	x = l;
	y = t;
	width = r - l + 1;
	height = b - t + 1;
	
}

void Portal::Render()
{
	RenderBoundingBox();
}

void Portal::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}