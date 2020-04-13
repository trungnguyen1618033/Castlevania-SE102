#include "MoneyBag.h"

void MoneyBag::Render()
{
	animation_set->at(GetState())->Render(nx, x, y);
}

void MoneyBag::Update(DWORD dt, vector<LPGAMEOBJECT>* Objects, vector<LPGAMEOBJECT*>* coObject)
{
}

void MoneyBag::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}
