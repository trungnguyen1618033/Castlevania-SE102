#include "Whip.h"

#define WHIP_BBOX_WIDTH				25
#define WHIP_BBOX_HEIGHT			10
#define WHIP_ANIMATION				13

void Whip::Render(int currentID)
{
	animation_set->at(WHIP_ANIMATION)->RenderByID(currentID, nx, x, y);
}

void Whip::SetWhipPosition(D3DXVECTOR3 simonPositon, bool isStand)
{	
	if (nx == 1)
		simonPositon.x -= 10.0f;
	else
		simonPositon.x -= 22.0f;
	if (isStand == false)
		simonPositon.y += 4.0f;

	SetPosition(simonPositon.x, simonPositon.y);
}

bool Whip::CheckCollision(float obj_left, float obj_top, float obj_right, float obj_bottom)
{
	float whip_left,
		whip_top,
		whip_right,
		whip_bottom;

	GetBoundingBox(whip_left, whip_top, whip_right, whip_bottom);

	return GameObject::AABB(whip_left, whip_top, whip_right, whip_bottom, obj_left, obj_top, obj_right, obj_bottom);
}

void Whip::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	top = y + 5;
	bottom = top + WHIP_BBOX_HEIGHT;
	if (nx < 0)
	{
		left = x + 5;
	}
	else if (nx > 0)
	{
		left = x + WHIP_BBOX_WIDTH + 10;
	}
	
	right = left + WHIP_BBOX_WIDTH;

}


