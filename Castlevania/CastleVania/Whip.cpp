#include "Whip.h"



Whip::Whip()
{
	SetState(WHIP);
	timeCheck = -1;
}

void Whip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	DebugOut(L"\n test \t");

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);

		if (dynamic_cast<Torch*>(obj))
		{
			Torch* e = dynamic_cast<Torch*> (obj);

			float left, top, right, bottom;

			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom) == true)
			{
				//DebugOut(L"\nframe :%d \t", animation_set->at(GetState())->IsRenderingLastFrame());
				e->SetState(EFFECTEXPLODE);
			}
		}
	
	}
}

void Whip::Render(int currentID)
{
	animation_set->at(state)->RenderByID(currentID, nx, x, y);
	

}

void Whip::SetWhipPosition(D3DXVECTOR3 simonPositon, bool isStand)
{	
	simonPositon.x -= 90.0f;
	if (isStand == false)
		simonPositon.y += 15.0f;

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
	top = y + 15;
	bottom = top + WHIP_BBOX_HEIGHT;
	if (nx < 0)
	{
		if (state != WHIP_2)
			left = x + 50;
		else left = x + 20;
	}
	else if (nx > 0)
	{
		if (state != WHIP_2)
			left = (240 - 50) - WHIP_BBOX_WIDTH + x;
		else left = (240 - 20) - LONG_CHAIN_BBOX_WIDTH + x;
	}
	
	if (state != WHIP_2)
		right = left + WHIP_BBOX_WIDTH;
	else  right = left + LONG_CHAIN_BBOX_WIDTH;

}

void Whip::Upgrade()
{
	if (state == WHIP) SetState(WHIP_1);
	else if (state == WHIP_1) SetState(WHIP_2);
}


