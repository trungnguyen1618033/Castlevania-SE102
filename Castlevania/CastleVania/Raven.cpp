#include "Raven.h"

#define RAVEN_FLYING_SPEED_X 0.12f
#define RAVEN_FLYING_SPEED_Y 0.1f
Raven::Raven()
{
	hp = 1;
	score = 200;
	attack = 2;
	respawnWaitingTime = 0;
}

void Raven::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == RAVEN_INACTIVE)
		return;
	
	if (state == RAVEN_DESTROYED && animation_set->at(state)->IsOver(150) == true)
	{
		SetState(RAVEN_INACTIVE);
		return;
	}

	GameObject::Update(dt);

	x += dx;
	y += dy;

}

void Raven::Render()
{
	if (state != RAVEN_INACTIVE)
		animation_set->at(state)->Render(1, nx, x, y);
	RenderActiveBoundingBox();
}

void Raven::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case RAVEN_ACTIVE:
		if (nx > 0) 
			vx = RAVEN_FLYING_SPEED_X;
		else 
			vx = -RAVEN_FLYING_SPEED_X;
		vy = 0;
		isDroppedItem = false;
		respawnTime_Start = 0;
		isRespawnWaiting = false;
		break;
	case RAVEN_DESTROYED:
		vx = 0;
		vy = 0;
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	case RAVEN_INACTIVE:
		x = entryPosition.x;
		y = entryPosition.y;
		vx = 0;
		vy = 0;
		isSettedPosition = false;
		StartRespawnTimeCounter();
		break;
	case RAVEN_IDLE:
		vx = 0;
		vy = 0;
		isRespawnWaiting = false;
		isDroppedItem = false;
		break;
	default:
		break;
	}
}

void Raven::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + 11;
	top = y + 11;
	right = left + 10;
	bottom = top + 10;
}

void Raven::GetActiveBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = entryPosition.x - 192;
	top = entryPosition.y - 96 ;
	right = entryPosition.x - 160;
	bottom = entryPosition.y  + 160;
}

void Raven::LoseHP(int x)
{
	Enemy::LoseHP(x);

	if (hp == 0)
		SetState(RAVEN_DESTROYED);
}
