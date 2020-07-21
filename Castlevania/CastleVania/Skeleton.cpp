#include "Skeleton.h"

#define SKELETON_JUMP_SPEED_Y		0.7f
#define SKELETON_GRAVITY				0.0015f
#define SKELETON_WALKING_SPEED_X		0.1f

#define SKELETON_BBOX_WIDTH			10
#define SKELETON_BBOX_HEIGHT			60

Skeleton::Skeleton()
{
	hp = 1;
	score = 300;
	attack = 2 + rand() % 3;
	respawnWaitingTime = 5000;
	isJumping = false;
}

void Skeleton::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	DWORD now = GetTickCount();

	if (state == SKELETON_DESTROYED && animation_set->at(state)->IsOver(300) == true)
	{
		SetState(SKELETON_INACTIVE);
		return;
	}

	if (state == SKELETON_HIT && animation_set->at(state)->IsOver(200) == true)
	{
		SetState(SKELETON_ACTIVE);
		return;
	}

	if (state == SKELETON_INACTIVE)
		return;

	Enemy::Update(dt);
	vy += SKELETON_GRAVITY * dt;

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

		if (state == SKELETON_ACTIVE && isJumping == false) // không va chạm với ground và chưa nhảy -> nhảy
		{
			isJumping = true;
			SetState(SKELETON_JUMP);
		}
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

			if (state == SKELETON_JUMP)
			{
				(this->nx) *= -1;
				SetState(SKELETON_ACTIVE);
			}
		}
	}

	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Skeleton::Render()
{
	if (state != SKELETON_INACTIVE)
		animation_set->at(state)->Render(1, nx, x, y);
	RenderActiveBoundingBox();
}

void Skeleton::SetState(int state)
{
	Enemy::SetState(state);

	switch (state)
	{
	case SKELETON_ACTIVE:
		if (nx > 0) vx = SKELETON_WALKING_SPEED_X;
		else vx = -SKELETON_WALKING_SPEED_X;
		break;
	case SKELETON_DESTROYED:
		vx = vy = 0;
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	case SKELETON_INACTIVE:
		x = entryPosition.x;
		y = entryPosition.y;
		vx = vy = 0;
		isSettedPosition = false;
		StartRespawnTimeCounter();
		break;
	case SKELETON_JUMP:
		vx = 0.1;
		vy = -0.7;
		isDroppedItem = false;
		respawnTime_Start = 0;
		isRespawnWaiting = false;
		break;
	case SKELETON_HIT:
		vx = vy = 0;
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	default:
		break;
	}
}

void Skeleton::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + 11; // 10,32
	top = y + 2;  // 60,64
	right = left + SKELETON_BBOX_WIDTH;
	bottom = top + SKELETON_BBOX_HEIGHT;
}

void Skeleton::GetActiveBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = entryPosition.x + 224;
	right = entryPosition.x + 256;
	top = entryPosition.y ;
	bottom = entryPosition.y + 160;
}

void Skeleton::LoseHP(int x)
{
	Enemy::LoseHP(x);

	if (hp == 0)
		SetState(SKELETON_DESTROYED);
}
