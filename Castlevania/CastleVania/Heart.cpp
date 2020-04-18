#include "Heart.h"


Heart::Heart() :GameObject()
{
	vy = ITEM_FALLING_SPEED;
}


void Heart::Render()
{
	int alpha = 255;

	
}

void Heart::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject, bool stopMovement)
{
	
}

void Heart::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;

	
}

void Heart::SetState(int state)
{
	GameObject::SetState(state);

	
}
