#include "Zombie.h"



Zombie::Zombie()
{
}

void Zombie::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == ZOMBIE_DESTROYED && animation_set->at(state)->IsOver(150) == true)
	{
		SetState(ZOMBIE_INACTIVE);
		return;
	}


	GameObject::Update(dt);

	// Check collision between zombie and ground (falling on ground)
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

		if (nx != 0)
		{
			this->nx *= -1;
			this->vx *= -1;
		}

		if (ny != 0)
		{
			this->vy = 0;
		}
		else
		{
			this->vy += ZOMBIE_GRAVITY * dt;
		}
	}

	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Zombie::Render()
{
	animation_set->at(state)->Render(1, nx, x, y);
}

void Zombie::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y + 2;
	right = left + ZOMBIE_BBOX_WIDTH;
	bottom = top + ZOMBIE_BBOX_HEIGHT;
}

void Zombie::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case ZOMBIE_ACTIVE:
		x = entryPosition.x;
		y = entryPosition.y;
		if (nx > 0) vx = ZOMBIE_WALKING_SPEED;
		else vx = -ZOMBIE_WALKING_SPEED;
		vy = 0;
		respawnTime_Start = 0;
		isRespawnWaiting = false;
		break;
	case ZOMBIE_DESTROYED:
		vx = 0;
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	case ZOMBIE_INACTIVE:
		vx = 0;
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

bool Zombie::IsAbleToActivate()
{
	DWORD now = GetTickCount();

	if (isRespawnWaiting == true && now - respawnTime_Start >= ZOMBIE_RESPAWN_TIME)
		return true;

	return false;
}
