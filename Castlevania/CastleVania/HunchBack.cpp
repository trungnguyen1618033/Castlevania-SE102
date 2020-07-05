#include "HunchBack.h"




HunchBack::HunchBack()
{
	hp = 1;
	score = 500;
	attack = 2;
	respawnWaitingTime = 0;
	isJumping = false;
}

void HunchBack::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	DWORD now = GetTickCount();

	if (state == HUNCHBACK_DESTROYED && animation_set->at(state)->IsOver(300) == true)
	{
		SetState(HUNCHBACK_INACTIVE);
		return;
	}


	vy += HUNCHBACK_GRAVITY * dt;
	Enemy::Update(dt);


	// Check collision between zombie and ground (jumping on ground)
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	//DebugOut(L"%d ", coEvents.size());

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;

	/*	if (state == HUNCHBACK_JUMP)
		{
			SetState(HUNCHBACK_ACTIVE);
		}*/
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		x += dx;
		y += min_ty * dy + ny * 0.1f;

		if (ny == -1.0f)
		{
			vy = 0;

			if (state == HUNCHBACK_JUMP)
			{
				SetState(HUNCHBACK_ACTIVE);
			}
		}
	}

	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void HunchBack::Render()
{
	if (state != HUNCHBACK_INACTIVE)
		animation_set->at(state)->Render(1, nx, x, y);
	RenderActiveBoundingBox();
}

void HunchBack::SetState(int state)
{
	Enemy::SetState(state);
	switch (state)
	{
	case HUNCHBACK_ACTIVE:
		if (nx > 0) 
			vx = HUNCHBACK_RUNNING_SPEED_X;
		else 
			vx = -HUNCHBACK_RUNNING_SPEED_X;
		break;
	case HUNCHBACK_DESTROYED:
		vx = 0;
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	case HUNCHBACK_INACTIVE:
		x = entryPosition.x;
		y = entryPosition.y;
		vx = 0;
		vy = 0;
		StartRespawnTimeCounter();
		break;
	case HUNCHBACK_JUMP:
		vy = HUNCHBACK_RUNNING_SPEED_Y;
		isJumping = true;
		break;
	case HUNCHBACK_IDLE:
		vx = 0;
		vy = 0;
		respawnTime_Start = 0;
		isRespawnWaiting = false;
		isDroppedItem = false;
		break;
	default:
		break;
	}

}

void HunchBack::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + 27;  // (10/64)
	top = y;
	right = left + 10;
	bottom = top + 32	;
}

void HunchBack::GetActiveBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = entryPosition.x + 160;
	right = entryPosition.x + 180;
	top = entryPosition.y ;
	bottom = entryPosition.y + 96;
}

void HunchBack::LoseHP(int x)
{
	Enemy::LoseHP(x);

	if (hp == 0)
		SetState(HUNCHBACK_DESTROYED);
}
