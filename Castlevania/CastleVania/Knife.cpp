#include "Knife.h"



void Knife::Render()
{
	animation_set->at(GetState())->Render(nx, x, y);
}

void Knife::Update(DWORD dt, vector<LPGAMEOBJECT>* Objects, vector<LPGAMEOBJECT*>* coObject)
{
}

void Knife::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}
