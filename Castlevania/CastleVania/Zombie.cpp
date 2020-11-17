#include "Zombie.h"

Zombie::Zombie()
{
	hp = 1;
	score = 100;
	attack = 2;
	respawnWaitingTime = 5000;
}

Zombie::~Zombie()
{
}

void Zombie::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMoving)
{
	if (stopMoving == true)
		return;

	if (state == ZOMBIE_INACTIVE)
		return;

	if (state == ZOMBIE_DESTROYED && animation_set->at(state)->IsOver(150) == true)
	{
		SetState(ZOMBIE_INACTIVE);
		return;
	}

	Enemy::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		vy += ZOMBIE_GRAVITY * dt;
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

void Zombie::Render()
{
	if(state != ZOMBIE_INACTIVE)
		animation_set->at(state)->Render(1, nx, x, y);
	//RenderActiveBoundingBox();
}

void Zombie::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + 11; // (10/32)
	top = y + 2; // (60,64)
	right = left + ZOMBIE_BBOX_WIDTH;
	bottom = top + ZOMBIE_BBOX_HEIGHT;
}

void Zombie::SetState(int state)
{
	Enemy::SetState(state);

	switch (state)
	{
	case ZOMBIE_ACTIVE:
		if (nx > 0) 
			vx = ZOMBIE_WALKING_SPEED;
		else 
			vx = -ZOMBIE_WALKING_SPEED;
		vy = 0;
		isDroppedItem = false;
		respawnTime_Start = 0;
		isRespawnWaiting = false;
		break;
	case ZOMBIE_DESTROYED:
		vx = 0;
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		theFirst = false;
		break;
	case ZOMBIE_INACTIVE:
		x = entryPosition.x;
		y = entryPosition.y;
		vx = 0;
		isSettedPosition = false;
		StartRespawnTimeCounter();
		break;
	default:
		break;
	}
}

void Zombie::GetActiveBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = entryPosition.x - ZOMBIE_ACTIVE_BBOX_WIDTH;
	right = entryPosition.x + ZOMBIE_ACTIVE_BBOX_WIDTH;
	top = entryPosition.y - ZOMBIE_ACTIVE_BBOX_HEIGHT;
	bottom = entryPosition.y + ZOMBIE_ACTIVE_BBOX_HEIGHT;
}

void Zombie::LoseHP(int x)
{
	Enemy::LoseHP(x);

	if (hp == 0)
		SetState(ZOMBIE_DESTROYED);
}
