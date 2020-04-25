#include "Items.h"

//
//#define WHIPITEM		2
//#define HEART_SMALL		3
//#define HEART_BIG		4
//#define MONEYBAG		5
//#define KNIFE			6

#define WHIPITEM		0
#define HEART_SMALL		1
#define HEART_BIG		2
#define MONEYBAG		3
#define KNIFE			4

#define ITEM_TIME_DESTROYED			5000

Items::Items() :GameObject()
{
	/*vx = 0.3;*/
	vy = ITEM_FALLING_SPEED;
	timeAppear = -1;
	SetState(0);
}

void Items::Render()
{
	animation_set->at(2)->Render(nx, x, y);
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
	case WHIPITEM: 
		right = left + 16;
		bottom = top + 16;
		break;
	case HEART_SMALL: 
		right = left +	16;
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

void Items::GetRandomItem()
{
	state = rand() % 4;
	
}

void Items::SetItem(int idItem)
{
	state = idItem;
}
