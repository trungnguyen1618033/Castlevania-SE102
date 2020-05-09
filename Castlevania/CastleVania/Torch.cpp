#include "Torch.h"
#include "PlayScene.h"

#define TORCH_BBOX_WIDTH		16
#define TORCH_BBOX_HEIGHT		32


Torch::Torch()
{
	SetState(TORCH);
	explode = false;
	vy = ITEM_FALLING_SPEED;
	timeAppear = -1;
}

void Torch::Render()
{

	animation_set->at(GetState())->Render(nx, x, y);

}

void Torch::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	if (state == EFFECTEXPLODE && animation_set->at(state)->IsOver(150))
	{	
		int ID = rand() % 5 + 2;
		SetState(ID);
		SetPosition(x, y);
		explode = true;

	}

	if (explode)
	{
		if (timeAppear == -1)
		{
			timeAppear = GetTickCount();
		}
		else
		{
			DWORD now = GetTickCount();

			if (now - timeAppear > ITEM_TIME_DESTROYED)
			{
				isEnable = false;
				return;
			}
		}

		GameObject::Update(dt);

		// Check collision between item and ground (falling on ground)
		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();

		CalcPotentialCollisions(coObjects, coEvents);

		if (coEvents.size() == 0)
		{
			x += dx;
			y += dy;
		}
		else
		{
			float min_tx, min_ty, nx = 0, ny;

			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

			y += min_ty * dy + ny * 0.1f;
			if (ny != 0) vy = 0;
		}

		// clean up collision events
		for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
	
}

void Torch::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;

	switch (state)
	{
	case TORCH:
		right = left + TORCH_BBOX_WIDTH;
		bottom = top + TORCH_BBOX_HEIGHT;
		break;
	case EFFECTEXPLODE:
		right = left + TORCH_BBOX_WIDTH;
		bottom = top + TORCH_BBOX_HEIGHT;
		break;
	case WHIPITEM:
		right = left + 16;
		bottom = top + 16;
		break;
	case HEART_SMALL:
		right = left + 16;
		bottom = top + 16;
		break;
	case HEART_BIG:
		right = left + 16;
		bottom = top + 16;
		break;
	case MONEYBAG:
		right = left + 16;
		bottom = top + 16;
		break;
	case KNIFE:
		right = left + 16;
		bottom = top + 16;
		break;
	default:
		break;
	}
	
}

void Torch::SetAnimationSet(LPANIMATION_SET ani_set)
{
	animation_set = ani_set;
}


