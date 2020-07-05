#include "Items.h"

#define ITEM_FALLING_SPEED_X			0.15f
#define ITEM_FALLING_SPEED_X_VARIATION	0.01f
#define ITEM_FALLING_SPEED_Y			0.15f
#define ITEM_SMALLHEART_FALLING_SPEED_Y	0.1f
#define ITEM_TIME_DESTROYED				4000

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
	if (state == SMALL_HEART && vy != 0)
	{
		vx += velocityVariation_x;
		if (vx >= ITEM_FALLING_SPEED_X || vx <= -ITEM_FALLING_SPEED_X)
			velocityVariation_x *= -1; // đổi chiều
	}

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
		if (ny != 0) 
		{
			vx = 0;
			vy = 0;
		}
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
	case SMALL_HEART:
		right = left + 16;
		bottom = top + 16;
		break;
	case BIG_HEART:
		right = left + 24;
		bottom = top + 20;
		break;
	case MONEY_BAG_RED:
	case MONEY_BAG_BLUE:
	case MONEY_BAG_WHITE:
	case MONEY_BAG:
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
	case DOUBLE_SHOT:
	case TRIPLE_SHOT:
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
	state = rand() % 12;
}

void Items::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case SMALL_HEART:
		velocityVariation_x = ITEM_FALLING_SPEED_X_VARIATION;
		vx = 0;
		vy = ITEM_SMALLHEART_FALLING_SPEED_Y;
		break;
	default:
		vx = 0;
		vy = ITEM_FALLING_SPEED_Y;
		break;
	}
}

