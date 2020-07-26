#include "Torch.h"
#include "PlayScene.h"

#define TORCH_BBOX_WIDTH		32
#define TORCH_BBOX_HEIGHT		64

Torch::Torch()
{
	timeAppear = -1;
}

void Torch::Render()
{
	animation_set->at(GetState())->Render(1, nx, x, y);
	RenderBoundingBox();
}

void Torch::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMoving)
{
	if (state == EFFECTEXPLODE && animation_set->at(state)->IsOver(300) == true)
	{
		this->SetEnable(false);
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
	case CANDLE:
		right = left + 16;
		bottom = top + 32;
		break;
	default:
		right = left;
		bottom = top;
		break;
	}
	
}

void Torch::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case TORCH:
	case CANDLE:
		break;
	case EFFECTEXPLODE:
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	default:
		break;
	}
	
}


