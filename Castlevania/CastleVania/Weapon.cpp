#include "Weapon.h"

#define KNIFE_SPEED					0.2f
#define KNIFE_BBOX_WIDTH			32
#define KNIFE_BBOX_HEIGHT			32
#define AXE_BBOX_WIDTH				30
#define AXE_BBOX_HEIGHT				28

#define KNIFE				0
#define	AXE					1
#define BOOMERANG			2
#define HOLY_WATER			3
#define HOLY_WATER_BROKEN	4

#define AXE_SPEED_X			0.2f
#define AXE_SPEED_Y			0.5f
#define AXE_GRAVITY			0.001f

#define BOOMERANG_SPEED		0.5f
#define BOOMERANG_TURNBACK_SPEED		0.01f

#define HOLY_WATER_GRAVITY	0.001f
#define HOLY_WATER_SPEED_X	0.25f
#define HOLY_WATER_SPEED_Y	0.1f
#define HOLY_WATER_TIME_EFFECT	1000

Weapon::Weapon() : GameObject()
{
	SetState(0);
}


void Weapon::Render()
{
	animation_set->at(GetState())->Render(1, nx, x, y);
}

void Weapon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	if (isHolyWater == true &&
		GetTickCount() - holyWaterCounter > HOLY_WATER_TIME_EFFECT)
	{
		isHolyWater = false;
		holyWaterCounter = 0;
		this->isEnable = false;
		return;
	}

	GameObject::Update(dt);

	switch (state)
	{
	case AXE:
		vy += AXE_GRAVITY * dt;
		break;
	case BOOMERANG:
		if (nx > 0) vx -= BOOMERANG_TURNBACK_SPEED;
		else vx += BOOMERANG_TURNBACK_SPEED;
		break;
	case HOLY_WATER:
		vy += HOLY_WATER_GRAVITY * dt;
		break;
	default:
		break;
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
		// kiểm tra va chạm với object
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Torch*>(e->obj))
			{
				Torch* torch = dynamic_cast<Torch*>(e->obj);
				torch->SetState(EFFECTEXPLODE);
				this->isEnable = false;
				if (state == KNIFE || state == AXE || state == BOOMERANG)
					this->isEnable = false;
			}
			else if (dynamic_cast<Simon*>(e->obj))
			{
				if (state == BOOMERANG)
					SetEnable(false);
			}
			else if (dynamic_cast<Ground*>(e->obj))
			{
				if (state == AXE)
					SetEnable(false);
				if (state == HOLY_WATER && e->ny == -1)
					SetState(HOLY_WATER_BROKEN);

				x += dx;
				y += dy;
			}
		}
	}

	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Weapon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	
	switch (state)
	{
	case KNIFE:
		right = left + KNIFE_BBOX_WIDTH;
		bottom = top + KNIFE_BBOX_HEIGHT;
	case AXE:
		right = left + AXE_BBOX_WIDTH;
		bottom = top + AXE_BBOX_HEIGHT;
	case BOOMERANG:
		right = left + AXE_BBOX_WIDTH;
		bottom = top + AXE_BBOX_HEIGHT;
	case HOLY_WATER:
		right = left + KNIFE_BBOX_HEIGHT;
		bottom = top + AXE_BBOX_HEIGHT;
		break;
	default:
		right = left;
		bottom = top;
		break;
	}
}

void Weapon::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case KNIFE:
		if (nx > 0) vx = KNIFE_SPEED;
		else vx = -KNIFE_SPEED;
		vy = 0;
		break;
	case AXE:
		if (nx > 0) vx = AXE_SPEED_X;
		else vx = -AXE_SPEED_X;
		vy = -AXE_SPEED_Y;
		break;
	case BOOMERANG:
		vx = nx * BOOMERANG_SPEED;
		vy = 0;
		break;
	case HOLY_WATER:
		vx = nx * HOLY_WATER_SPEED_X;
		vy = -HOLY_WATER_SPEED_Y;
		break;
	case HOLY_WATER_BROKEN:
		vx = 0;
		vy = 0;
		HolyWaterEffect();
		break;
	default:
		break;
	}
}
