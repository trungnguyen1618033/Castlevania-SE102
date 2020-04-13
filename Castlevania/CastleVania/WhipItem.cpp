#include "WhipItem.h"

void WhipItem::Render()
{
	animation_set->at(GetState())->Render(nx, x, y);
}

void WhipItem::Update(DWORD dt, vector<LPGAMEOBJECT>* Objects, vector<LPGAMEOBJECT*>* coObject)
{
}

void WhipItem::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}
