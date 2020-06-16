#include "Simon.h"

Simon::Simon() : GameObject()
{
	SetState(IDLE);

	score = 0;
	energy = 99;
	life = 3;
	subWeapon = -1;
	HP = 10;
	
}


void Simon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GameObject::Update(dt);
	
	if (state != ASCEND && state != DESCEND && state != ASCENDING && state != DESCENDING)
	{
		if (vy < -0.2f || vy > 0.2f)
			vy += SIMON_GRAVITY * dt;
		else vy += SIMON_GRAVITY_LOWER * dt;
	}

	if (GetTickCount() - untouchable_start > SIMON_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		isUntouchable = false;
	}
	// Check collision between Simon and other objects
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
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		x += min_tx * dx + nx * 0.1f;
		y += min_ty * dy + ny * 0.1f;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Torch*>(e->obj))
			{	
				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
			}
			else if (dynamic_cast<Portal*>(e->obj))
			{
				DebugOut(L"hello\n");
				Portal* p = dynamic_cast<Portal*>(e->obj);
				Game::GetInstance()->SwitchScene(p->GetSceneId());
			}
			else if (dynamic_cast<Ground*>(e->obj))
			{
				if (ny != 0)
				{
					if (ny == -1)
					{
						vy = 0;
						isTouchGround = true;
					}
					else
					{
						y += dy;
						isTouchGround = false;
					}
				}
			}
			else
			{
				if (nx != 0) vx = 0;
				if (ny != 0) vy = 0;
			}
		}
	}

	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++)
		delete coEvents[i];

}

void Simon::Render()
{
	if (isUntouchable) 
	{
		int r = rand() % 2;

		if (r == 0) animation_set->at(state)->Render(1, nx, x, y);
		else animation_set->at(state)->Render(1, nx, x, y, 100);;
	}
	else
	{
		animation_set->at(state)->Render(1, nx, x, y);
	}
}

void Simon::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case IDLE:
		isStand = true;
		vx = 0;
		isMovingDown = false;
		isMovingUp = false;
		isStandOnStair = false;
		break;
	case WALK:
		if (nx > 0)
			vx = SIMON_WALKING_SPEED;
		else 
			vx = -SIMON_WALKING_SPEED;
		isStandOnStair = false;
		break;
	case DUCK:
		isStand = false;
		vx = 0;
		vy = 0;
		isStandOnStair = false;
		break;
	case JUMP:
		isStand = true;
		vy = -SIMON_JUMP_SPEED_Y;
		isTouchGround = false;
		isStandOnStair = false;
		break;
	case ASCEND:
		isStand = true;
		if (nx > 0) 
			vx = SIMON_STAIR_SPEED_X;
		else 
			vx = -SIMON_STAIR_SPEED_X;
		vy = -SIMON_STAIR_SPEED_Y;
		isTouchGround = false;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	case DESCEND:
		isStand = true;
		if (nx > 0) 
			vx = SIMON_STAIR_SPEED_X;
		else 
			vx = -SIMON_STAIR_SPEED_X;
		vy = SIMON_STAIR_SPEED_Y;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	case HURT:
		isStand = true;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		isStandOnStair = false;
		break;
	case DUCKING:
		isStand = false;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		isStandOnStair = false;
		break;
	case ASCENDING:
	case DESCENDING:
		isStand = true;
		vx = 0;
		vy = 0;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		isStand = true;
		break;
	case STANDING:
	case HIT:
	case UPGRADE:
		isStand = true;
		isStandOnStair = false;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	default:
		break;
	}
	
}


void Simon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + 15;
	top = y + 2;
	right = left + SIMON_BBOX_WIDTH;
	bottom = top + SIMON_BBOX_HEIGHT;
}

bool Simon::CheckCollisionWithItem(vector<LPGAMEOBJECT>* listItem)
{
	float simon_l, simon_t, simon_r, simon_b;
	float item_l, item_t, item_r, item_b;

	GetBoundingBox(simon_l, simon_t, simon_r, simon_b);

	for (UINT i = 0; i < listItem->size(); i++)
	{
		if (listItem->at(i)->IsEnable() == false)
			continue;

		listItem->at(i)->GetBoundingBox(item_l, item_t, item_r, item_b);

		if (GameObject::AABB(simon_l, simon_t, simon_r, simon_b, item_l, item_t, item_r, item_b) == true)
		{
			listItem->at(i)->isEnable = false;

			int idItem = listItem->at(i)->GetState();

			switch (idItem)
			{
			case KNIFE:
				subWeapon = idItem;
				break;
			case AXE:
				subWeapon = 1;
				break;
			case BOOMERANG:
				subWeapon = 2;
				break;
			case HEART_BIG:
				energy += 5;
				break;
			case HEART_SMALL:
				energy += 1;
				break;
			case WHIPITEM:
				SetState(UPGRADE);
				vx = 0;
				isGotChainItem = true;
				break;
			case MONEYBAG:
				score += 1000;
				break;
			case HOLY_WATER:
				subWeapon = 3;
				break;
			default:
				break;
			}

			return true;
		}
	}
}


bool Simon::CheckCollisionWithStair(vector<LPGAMEOBJECT>* listStair)
{
	float simon_l, simon_t, simon_r, simon_b;
	GetBoundingBox(simon_l, simon_t, simon_r, simon_b);

	simon_t += 55;
	simon_b += 5; 
	simon_l += 5;
	simon_r -= 5;

	for (UINT i = 0; i < listStair->size(); i++)
	{
		float stair_l, stair_t, stair_r, stair_b;
		listStair->at(i)->GetBoundingBox(stair_l, stair_t, stair_r, stair_b);


		if (GameObject::AABB(simon_l, simon_t, simon_r, simon_b, stair_l, stair_t, stair_r, stair_b) == true)
		{
			//DebugOut(L"collision\n");

			if (listStair->at(i)->GetState() == 0) stairDirection = 1;
			else stairDirection = -1;

			stairCollided = listStair->at(i);

			if (simon_b < stair_b) isMovingDown = true;

			float upstair_x = -999, upstair_y = -999;

			if (i > 0)
			{
				listStair->at(i - 1)->GetPosition(upstair_x, upstair_y);

				float dx = abs(upstair_x - stair_l);
				float dy = upstair_y - stair_t;

				if (dx == GROUND_BBOX_WIDTH && dy == -GROUND_BBOX_HEIGHT)
				{
					isMovingUp = true;
					return true;
				}
			}

			if (i < listStair->size() - 1)
			{
				listStair->at(i + 1)->GetPosition(upstair_x, upstair_y);

				float dx = abs(upstair_x - stair_l);
				float dy = upstair_y - stair_t;

				if (dx == GROUND_BBOX_WIDTH && dy == -GROUND_BBOX_HEIGHT)
				{
					isMovingUp = true;
					return true;
				}
			}

			if (stair_t - y < 60)
			{
				isMovingUp = true;
				return true;
			}

			isMovingUp = false;
			return true; // collision between Simon and stairs
		}

	}

	return false;
}


void Simon::StandOnStair()
{
	vx = 0;
	vy = 0;
}

bool Simon::IsHit()
{
	return state == STANDING || state == DUCKING || state == ASCENDING || state == DESCENDING;
}





