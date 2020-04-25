#include "Torch.h"
#include "PlayScene.h"

#define TORCH_BBOX_WIDTH		16
#define TORCH_BBOX_HEIGHT		32

Torch::Torch()
{
	SetState(TORCH);
	item = new Items();
}

void Torch::Render()
{
	/*if (this->IsEnable())
		animation_set->at(GetState())->Render(nx, x, y);
	else
		this->item->Render();*/
	animation_set->at(GetState())->Render(nx, x, y);
	this->item->Render();
}

void Torch::Update(DWORD dt, vector<LPGAMEOBJECT>* Objects, vector<LPGAMEOBJECT>* coObjects)
{

	if (state == EFFECTEXPLORE && animation_set->at(state)->IsOver(150)) 	//nếu render xong hết đốm lửa rồi thì set enable = false -> biến mất
	{	
		SetEnable(false);
		item->isEnable = true;
		int ID = rand() % 6 + 2;
		item->SetState(ID);
		item->SetPosition(x + 50, y);

		//Objects->push_back(item);


		

			// Tạo một item theo id và thêm vào Objects

	}
	
}

void Torch::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + TORCH_BBOX_WIDTH;
	bottom = top + TORCH_BBOX_HEIGHT;
}

void Torch::SetAnimationSet(LPANIMATION_SET ani_set)
{
	animation_set = ani_set;
	item->SetAnimationSet(ani_set);
}

