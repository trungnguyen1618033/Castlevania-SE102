#include "Ghost.h"

Ghost::Ghost()
{
	hp = 1;
	score = 300;
	attack = 2;
	respawnWaitingTime = 5000;
	velocityVariation = 0.001f;

}

void Ghost::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMoving)
{
	if (stopMoving == true)
		return;

	if (state == GHOST_INACTIVE)
		return;

	if (state == GHOST_DESTROYED && animation_set->at(state)->IsOver(150) == true)
	{
		SetState(GHOST_INACTIVE);
		return;
	}
	GetOrientation();

	Enemy::Update(dt);

	vy += velocityVariation;

	if (vy > 0.025f || vy < -0.025f) {
		velocityVariation = -velocityVariation;
	}

	x += dx;
	y += dy;
}

void Ghost::Render()
{
	if (state != GHOST_INACTIVE)
		animation_set->at(state)->Render(1, nx, x, y);
	//RenderActiveBoundingBox();
}

void Ghost::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case GHOST_ACTIVE:
		if (nx > 0) 
			vx = GHOST_FLYING_SPEED_X;
		else 
			vx = -GHOST_FLYING_SPEED_X;
		isDroppedItem = false;
		respawnTime_Start = 0;
		isRespawnWaiting = false;
		break;
	case GHOST_DESTROYED:
		vx = 0;
		vy = 0;
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	case GHOST_INACTIVE:
		x = entryPosition.x;
		y = entryPosition.y;
		vx = 0;
		vy = 0;
		isSettedPosition = false;
		StartRespawnTimeCounter();
		break;
	default:
		break;
	}
}

void Ghost::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + 11;
	top = y + 11;
	right = left + 10;
	bottom = top + 10;
}

void Ghost::GetActiveBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = entryPosition.x - 166;
	top = entryPosition.y - 60;
	right = entryPosition.x - 150;
	bottom = entryPosition.y + 60;
}

void Ghost::LoseHP(int x)
{
	Enemy::LoseHP(x);
	if (hp == 0)
		SetState(GHOST_DESTROYED);
}

void Ghost::GetOrientation()
{
	// lấy phương hướng
	int nx;

	if (x - simonPostion.x < 32 && vx < 0)
	{
		nx = 1;
		vx *= -1;
		SetOrientation(nx);
	}
	else if (x > simonPostion.x && vx > 0)
	{
		nx = -1;
		vx *= -1;
		SetOrientation(nx);
	}
		
	

}

