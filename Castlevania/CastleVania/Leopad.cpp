#include "Leopad.h"



Leopad::Leopad()
{
	nx = -1;
}

void Leopad::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	DWORD now = GetTickCount();

	if (state == LEOPAD_DESTROYED && animation_set->at(state)->IsOver(150) == true)
	{
		SetState(LEOPAD_INACTIVE);
		return;
	}

	vy += LEOPAD_GRAVITY * dt;
	GameObject::Update(dt);


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	//DebugOut(L"%d ", coEvents.size());

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;

		if (state == LEOPAD_ACTIVE && isJumping == false) 
		{
			isJumping = true;
			SetState(LEOPAD_JUMP);
		}
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		x += dx;
		y += min_ty * dy + ny * 0.1f;

		if (ny != 0)
		{
			vy = 0;

			if (state == LEOPAD_JUMP)
			{
				this->nx = 1;
				SetState(LEOPAD_ACTIVE);
			}
		}
	}

	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Leopad::Render()
{
	if (state != LEOPAD_INACTIVE)
	{
		if (isRespawnWaiting == false)
			animation_set->at(state)->Render(1, nx, x, y);
	}
}

void Leopad::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + 27;  // (10/64)
	top = y;
	right = left + LEOPAD_BBOX_WIDTH;
	bottom = top + LEOPAD_BBOX_HEIGHT;
}

void Leopad::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case LEOPAD_ACTIVE:
		if (nx > 0) vx = LEOPAD_RUNNING_SPEED_X;
		else vx = -LEOPAD_RUNNING_SPEED_X;
		vy = LEOPAD_RUNNING_SPEED_Y;
		respawnTime_Start = 0;
		isRespawnWaiting = false;
		break;
	case LEOPAD_DESTROYED:
		vx = 0;
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	case LEOPAD_INACTIVE:
		vx = 0;
		vy = 0;
		StartRespawnTimeCounter();
		break;
	case LEOPAD_IDLE:
		vx = 0;
		break;
	case LEOPAD_JUMP:
		vy = -LEOPAD_RUNNING_SPEED_Y;
	default:
		break;
	}
}

void Leopad::GetActiveBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = entryPosition.x - LEOPAD_ACTIVE_BBOX_WIDTH;
	right = entryPosition.x + LEOPAD_ACTIVE_BBOX_WIDTH;
	top = entryPosition.y - LEOPAD_ACTIVE_BBOX_HEIGHT;
	bottom = entryPosition.y + LEOPAD_ACTIVE_BBOX_HEIGHT;
}

bool Leopad::IsAbleToActivate()
{
	DWORD now = GetTickCount();

	if (isRespawnWaiting == true && now - respawnTime_Start >= LEOPAD_RESPAWN_TIME)
		return true;

	return false;
}
