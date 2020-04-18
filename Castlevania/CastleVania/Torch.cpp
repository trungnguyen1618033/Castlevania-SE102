#include "Torch.h"

#define TORCH_BBOX_WIDTH		16
#define TORCH_BBOX_HEIGHT		32

Torch::Torch()
{
	SetState(TORCH);
}

void Torch::Render()
{
	animation_set->at(GetState())->Render(nx, x, y);
	RenderBoundingBox();
}

void Torch::Update(DWORD dt, vector<LPGAMEOBJECT>* Objects, vector<LPGAMEOBJECT>* coObjects)
{
	
	if (state == EFFECTEXPLORE && animation_set->at(state)->IsOver(150)) 	//nếu render xong hết đốm lửa rồi thì set enable = false -> biến mất
	{
		this->isEnable = false;

			// Tạo một item theo id và thêm vào Objects

			Items* item = new Items();
			item->isEnable = true;
			int ID = rand() % 3;
			item->SetState(ID);
			item->SetPosition(x, y);
			Objects->push_back(item);
		
	}
	
}

void Torch::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + TORCH_BBOX_WIDTH;
	bottom = y + TORCH_BBOX_HEIGHT;
}
