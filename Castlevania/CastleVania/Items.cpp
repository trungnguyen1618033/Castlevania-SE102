#include "Items.h"

#define KNIFE			0
#define WHIPITEM		1
#define HEART			2
#define MONEYBAG		3
#define ITEM_TIME_DESTROYED			5000

Items::Items() :GameObject()
{
	vy = ITEM_FALLING_SPEED;
	timeAppear = -1;
}

void Items::Render()
{
	animation_set->at(GetState())->Render(nx, x, y);
}

void Items::Update(DWORD dt, vector<LPGAMEOBJECT>* Objects, vector<LPGAMEOBJECT>* coObject)
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

	CalcPotentialCollisions(coObject, coEvents);

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

void Items::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;

	switch (state)
	{
	case KNIFE:
		right = left + 16;
		bottom = top + 16;
		break;
	case WHIPITEM: 
		right = left + 16;
		bottom = top + 16;
		break;
	case HEART: 
		right = left +	16;
		bottom = top + 16;
		break;
	case MONEYBAG: 
		right = left + 16;
		bottom = top + 16;
		break;
	default:
		break;
	}
}

void Items::GetRandomItem()
{
	state = rand() % 3;
	
}

void Items::SetItem(int idItem)
{
	state = idItem;
}
