#include "Knight.h"


#define KNIGHT_SPEED	0.03f

Knight::Knight()
{
	hp = 3;
	score = 400;
	attack = 2;
	respawnWaitingTime = 0;
}

void Knight::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMoving)
{
	if (stopMoving == true)
		return;
	
	if (state == KNIGHT_INACTIVE)
		return;

	if (state == KNIGHT_DESTROYED && animation_set->at(state)->IsOver(150) == true)
	{
		SetState(KNIGHT_INACTIVE);
		return;
	}
	if (x < left) {
		nx = 1;
		vx = KNIGHT_SPEED;
	}
	else if (x > right) {
		nx = -1;
		vx = -KNIGHT_SPEED;
	}
	else
	{
		if (x > left && x < right)
		{
			if (nx == 1)
				vx = KNIGHT_SPEED;
			else
				vx = -KNIGHT_SPEED;
		}
	}
	
	Enemy::Update(dt);
	x += dx;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		x += min_tx * dx + nx * 0.1f;
		y += min_ty * dy + ny * 0.1f;

		if (nx != 0 && ny == 0)
		{
			this->nx *= -1;
			this->vx *= -1;
		}
		else if (ny == -1.0f)
		{
			vy = 0;
		}
	}

	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Knight::Render()
{
	if (state != KNIGHT_INACTIVE)
		animation_set->at(state)->Render(1, nx, x, y);
	
}

void Knight::SetState(int state)
{
	Enemy::SetState(state);

	switch (state)
	{
	case KNIGHT_ACTIVE:
		if (nx > 0) 
			vx = KNIGHT_SPEED;
		else 
			vx = -KNIGHT_SPEED;
		vy = 0;
		isDroppedItem = false;
		respawnTime_Start = 0;
		isRespawnWaiting = false;
		break;
	case KNIGHT_DESTROYED:
		vx = 0;
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		SetEnable(false);
		break;
	case KNIGHT_INACTIVE:
		x = entryPosition.x;
		y = entryPosition.y;
		vx = 0;
		StartRespawnTimeCounter();
		break;
	default:
		break;
	}
}

void Knight::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + ZOMBIE_BBOX_WIDTH;
	bottom = top + ZOMBIE_BBOX_HEIGHT;
}

void Knight::GetActiveBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = entryPosition.x;
	right = entryPosition.x;
	top = entryPosition.y;
	bottom = entryPosition.y;
}

void Knight::LoseHP(int x)
{
	Enemy::LoseHP(x);
	if (hp == 0)
		SetState(KNIGHT_DESTROYED);
}

void Knight::SetLeft(int l)
{
	left = l;
}

void Knight::SetRight(int r)
{
	right = r;
}

