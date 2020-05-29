#include "Items.h"

Items::Items() :GameObject()
{
	vy = ITEM_FALLING_SPEED;
	timeAppear = -1;
	AnimationSets* animation_sets = AnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(3);
	SetAnimationSet(ani_set);
	GetRandomItem();
}

void Items::Render()
{
	animation_set->at(GetState())->Render(1, nx, x, y);
}

void Items::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
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
		right = left + 32;
		bottom = top + 32;
		break;
	case HEART_SMALL:
		right = left + 16;
		bottom = top + 16;
		break;
	case HEART_BIG:
		right = left + 24;
		bottom = top + 20;
		break;
	case MONEYBAG:
		right = left + 30;
		bottom = top + 30;
		break;
	case KNIFE:
		right = left + 32;
		bottom = top + 18;
		break;
	case AXE:
		right = left + 30;
		bottom = top + 28;
		break;
	case BOOMERANG:
		right = left + 30;
		bottom = top + 28;
		break;
	case HOLY_WATER:
		right = left + 28;
		bottom = top + 28;
		break;
	default:
		right = left;
		bottom = top;
		break;
	}
}



void Items::GetRandomItem()
{
	srand(time(NULL));
	state = rand() % 6;
}

