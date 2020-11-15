#include "Skeleton.h"

Skeleton::Skeleton(float min, float max)
{
	hp = 2;
	score = 300;
	respawnWaitingTime = 5000;
	isJumping = false;
	xMin = min;
	xMax = max;
}

void Skeleton::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMoving)
{
	if (stopMoving == true)
		return;

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

	if (this->state == SKELETON_ACTIVE)
	{

		if (x <= xMin)
		{
			x = xMin;
			velocityVariation = 1;
		}
		if (x >= xMax)
		{
			x = xMax;
			velocityVariation = -1;
		}
		vx += velocityVariation * 0.01f;
		GetOrientation();

		
		/*if (vx > 0.14f) 
			velocityVariation = -1;
		else if (vx < -0.14f)  
			velocityVariation = 1;*/
	}
	Enemy::Update(dt);
	vy += SKELETON_GRAVITY * dt;

	// Check collision between zombie and ground (jumping on ground)
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

		x += dx;
		y += min_ty * dy + ny * 0.1f;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Ground*>(e->obj))
			{
				if (e->ny != 0)
				{
					if (e->ny == -1)
					{
						vy = 0;

						if (state == SKELETON_JUMP) {
							// jump xong chạm đất -> walk
							SetState(SKELETON_ACTIVE);
						}
					}
					else
						y += dy;
				}
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
	//RenderActiveBoundingBox();
}

void Skeleton::SetState(int state)
{
	Enemy::SetState(state);

	switch (state)
	{
	case SKELETON_ACTIVE:
		vx = vy = 0;
		lastTimeThrown = GetTickCount();
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
		vx = 0.09;
		vy = -0.5;
		isDroppedItem = false;
		respawnTime_Start = 0;
		isRespawnWaiting = false;
		isJumping = true;
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
	left = x + 11;
	top = y + 2;  
	right = left + SKELETON_BBOX_WIDTH;
	bottom = top + SKELETON_BBOX_HEIGHT;
}

void Skeleton::GetActiveBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = entryPosition.x - RAVEN_ACTIVE_BBOX_WIDTH;
	right = entryPosition.x + RAVEN_ACTIVE_BBOX_WIDTH;
	top = entryPosition.y - RAVEN_ACTIVE_BBOX_HEIGHT;
	bottom = entryPosition.y + RAVEN_ACTIVE_BBOX_HEIGHT;
}

void Skeleton::LoseHP(int x)
{
	Enemy::LoseHP(x);

	if (hp == 0)
		SetState(SKELETON_DESTROYED);
}

bool Skeleton::CanHit()
{
	float dx = abs(x - simonPostion.x);
	if (dx > 60 && dx < 160 && state == SKELETON_ACTIVE)
	{
		if (numThrow == 3){
			if (GetTickCount() - lastTimeThrown > 3000) {
				numThrow = 1;
				return true;
			}
		}
		else if (GetTickCount() - lastTimeThrown > 1000)
		{
			numThrow++;
			return true;
		}
	}
	return false;
}

void Skeleton::Hit(Grid* grid)
{
	// Tạo bone
	auto bone = new Bone();
	bone->SetPosition(x + 5, y + 10);
	bone->SetOrientation(nx);
	bone->SetEnable(true);
	// Co the tang case -> tang numThrow theo
	switch (numThrow)
	{
	case 1:
		bone->SetNum(1.0f);
		break;
	case 2:
		bone->SetNum(0.5f);
		break;
	case 3:		
		bone->SetNum(1.5f);
		break;
	default:
		break;
	}
	bone->SetState(0);

	if (y > 480)
		Unit* unit = new Unit(grid, bone, x + 5, y);
	Unit* unit = new Unit(grid, bone, x + 5, y + 10);
}

void Skeleton::GetOrientation()
{
	// lấy phương hướng
	int nx;

	if (x - simonPostion.x < 32 && vx < 0)
	{
		nx = 1;
		SetOrientation(nx);
	}
	else if (x > simonPostion.x && vx > 0)
	{
		nx = -1;
		SetOrientation(nx);
	}
}
