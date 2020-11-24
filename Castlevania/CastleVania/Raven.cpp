#include "Raven.h"

Raven::Raven()
{
	hp = 1;
	score = 200;
	attack = 2;
	respawnWaitingTime = 7000;
}

void Raven::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMoving)
{
	if (stopMoving == true)
		return;

	if (state == RAVEN_INACTIVE)
		return;

	if (state == RAVEN_IDLE)
		return;

	if (state == RAVEN_DESTROYED && animation_set->at(state)->IsOver(150) == true)
	{
		SetState(RAVEN_INACTIVE);
		return;
	}
	if (isStopWaiting == false && (GetTickCount() - endTimeWaiting > 1500))
	{
		StartStopTimeCounter();
	}

	if (isStopWaiting == true)
	{
		if (GetTickCount() - startTimeWaiting > 1000)
		{
			//DebugOut("Fly\n");
			endTimeWaiting = GetTickCount();
			isStopWaiting = false;
			startTimeWaiting = 0;

			if (currentAttackPhase < 3) currentAttackPhase++;
			else currentAttackPhase = 0;
		}
		else
		{
			//DebugOut("Waiting\n");
			vx = vy = 0;
			return;
		}
	}

	GetVelocity();

	GameObject::Update(dt);

	x += dx;
	y += dy;
	
}

void Raven::Render()
{
	if (state != RAVEN_INACTIVE)
		animation_set->at(state)->Render(1, nx, x, y);
	//RenderActiveBoundingBox();
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
		StartStopTimeCounter();
		isDroppedItem = false;
		respawnTime_Start = 0;
		isRespawnWaiting = false;
		theFirst = false;
		break;
	case RAVEN_DESTROYED:
		vx = 0;
		vy = 0;
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		currentAttackPhase = 0;
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
	left = x + 8;
	top = y;
	right = left + RAVEN_BBOX_WIDTH;
	bottom = top + RAVEN_BBOX_HEIGHT;
}

void Raven::GetActiveBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = entryPosition.x - RAVEN_ACTIVE_BBOX_WIDTH - 32;
	right = entryPosition.x + RAVEN_ACTIVE_BBOX_WIDTH;
	top = entryPosition.y;
	bottom = entryPosition.y + RAVEN_ACTIVE_BBOX_HEIGHT;
}

void Raven::LoseHP(int x)
{
	Enemy::LoseHP(x);

	if (hp == 0)
		SetState(RAVEN_DESTROYED);
}

void Raven::GetVelocity()
{
	float dx = abs(x - simonPostion.x);
	float dy = abs(y - simonPostion.y);

	// lấy phương hướng
	int nx, ny;

	if (x < simonPostion.x)
		nx = 1;
	else
		nx = -1;

	if (y < simonPostion.y)
		ny = 1;
	else
		ny = -1;

	SetOrientation(nx);
	

	if (dx < 10 && dy < 10)
	{
		vx = nx * dx / 250;
		vy = ny * dy / 250;
	}
	else
	{
		vx = nx * dx / 1200;
		vy = ny * dy / 1200;
	}
	
	if (currentAttackPhase == 3)
	{
		vx *= 7.2f;
		vy *= 7.2f;
	}	
}
