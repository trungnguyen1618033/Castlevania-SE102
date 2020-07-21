﻿#include "Simon.h"

Simon::Simon() : GameObject()
{
	SetState(IDLE);

	score = 0;
	energy = 99;
	life = 3;
	subWeapon = -1;
	hp = 16;
	
}


void Simon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GameObject::Update(dt);
	
	if (isStandOnStair == false && isAutoWalk == false)
	{
		if (vy > -0.2 && vy < 0.2)
			vy += SIMON_GRAVITY_LOWER * dt;
		else
			vy += SIMON_GRAVITY * dt;
	}

	// Auto - walk
	if (isAutoWalk == true)
		DoAutoWalk();

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

	if (coEvents.size() == 0 && isAutoWalk == false)
	{
		x += dx;
		y += dy;
		if (vy > 0.2f)//SIMON_SPEED_Y_LOWER_ZONE    0.2f
			isFalling = true;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		if (isAutoWalk == false)
		{
			x += min_tx * dx + nx * 0.1f;
			y += min_ty * dy + ny * 0.1f;
		}

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Portal*>(e->obj))
			{
				//DebugOut(L"hello\n");
				Portal* p = dynamic_cast<Portal*>(e->obj);
				Game::GetInstance()->SwitchScene(p->GetSceneId());
			}
			else if (dynamic_cast<Ground*>(e->obj) || dynamic_cast<BreakWall*>(e->obj))
			{
				if (e->ny != 0)
				{
					if (e->ny == -1)
					{
						vy = 0;
						isTouchGround = true;
						isFalling = false;
					}
					else
						y += dy;
				}
				// Khi đang lên/xuống cầu thang, va chạm theo trục x sẽ không được xét tới
				if (state == ASCEND || state == DESCEND)
					if (nx != 0) 
						x -= nx * 0.1f;
			}
			else if (dynamic_cast<BlockMove*>(e->obj))
			{
				if (e->ny != 0)
				{
					if (e->ny == -1)
					{
						vy = 0;
						isTouchGround = true;
						isFalling = false;

						x += e->obj->dx * 2;
						
					}
				}
				SetState(IDLE);
			}
			else if (dynamic_cast<Knight*>(e->obj) || dynamic_cast<Bat*>(e->obj) || dynamic_cast<Ghost*>(e->obj) || dynamic_cast<HunchBack*>(e->obj) ||
				dynamic_cast<Skeleton*>(e->obj) || dynamic_cast<Raven*>(e->obj) || dynamic_cast<Zombie*>(e->obj) || dynamic_cast<Boss*>(e->obj))
			{
				if (state != UPGRADE)
				{
				
					if (dynamic_cast<Knight*>(e->obj))
					{
						Knight* knight = dynamic_cast<Knight*>(e->obj);
						DebugOut(L"collision knight\n");
						LoseHP(knight->GetAttack());
					}
					else if (dynamic_cast<Bat*>(e->obj))
					{
						Bat* bat = dynamic_cast<Bat*>(e->obj);
						bat->SetState(BAT_DESTROYED);
						DebugOut(L"collision bat\n");
						LoseHP(bat->GetAttack());
					}
					else if (dynamic_cast<Ghost*>(e->obj))
					{
						Ghost* ghost = dynamic_cast<Ghost*>(e->obj);
						LoseHP(ghost->GetAttack());
					}
					else if (dynamic_cast<HunchBack*>(e->obj))
					{
						HunchBack* hunchback = dynamic_cast<HunchBack*>(e->obj);
						LoseHP(hunchback->GetAttack());
					}
					if (dynamic_cast<Zombie*>(e->obj))
					{
						Zombie* zombie = dynamic_cast<Zombie*>(e->obj);
						LoseHP(zombie->GetAttack());
					}
					else if (dynamic_cast<Boss*>(e->obj))
					{
						Boss* boss = dynamic_cast<Boss*>(e->obj);
						LoseHP(boss->GetAttack());
					}

					if (isStandOnStair == false || hp == 0)  // Simon đứng trên cầu thang sẽ không nhấp nháy
					{
						// đặt trạng thái nhấp nháy cho simon
						if (e->nx != 0)
						{
							if (e->nx == 1 && this->nx == 1) 
								this->nx = -1;
							else if (e->nx == -1 && this->nx == -1) 
								this->nx = 1;
						}

						SetState(HURT);
					}
				}
				else
				{
					if (e->nx != 0) x += dx;
					if (e->ny != 0) y += dy;
				}
			}
		}
	}

	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++)
		delete coEvents[i];

}

void Simon::Render()
{

	// Khi Simon rơi từ trên cao xuống thì luôn co chân
	if (state !=DEAD  && IsHit() == false && isFalling == true)
		state = DUCK;

	if (isUntouchable) 
	{
		int r = rand() % 2;
		if (r == 0) 
			animation_set->at(state)->Render(1, nx, x, y);
		else 
			animation_set->at(state)->Render(1, nx, x, y, 100);;
	}

	animation_set->at(state)->Render(1, nx, x, y);
}

void Simon::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case IDLE:
		vx = 0;
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
		vx = 0;
		vy = 0;
		isStandOnStair = false;
		break;
	case JUMP:
		vy = -SIMON_JUMP_SPEED_Y;
		isTouchGround = false;
		isStandOnStair = false;
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;	
	case ASCEND:
		isStand = true;
		if (nx > 0) 
			vx = SIMON_STAIR_SPEED_X;
		else 
			vx = -SIMON_STAIR_SPEED_X;
		vy = -SIMON_STAIR_SPEED_Y;
		isStandOnStair = true;
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
		isStandOnStair = true;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	case HURT:
		if (nx > 0) 
			vx = -SIMON_DEFLECT_SPEED_X;
		else 
			vx = SIMON_DEFLECT_SPEED_X;
		vy = -SIMON_DEFLECT_SPEED_Y;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		isStandOnStair = false;
		break;
	case ASCENDING:
	case DESCENDING:
		vx = 0;
		vy = 0;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	case STANDING:
	case DUCKING:
	case UPGRADE:
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		isStandOnStair = false;
		break;
	case DEAD:
		vx = 0;
		vy = 0;
		break;
	default:
		break;
	}
	
}


void Simon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	// sprite có kích thước là 60x66, bbox là 40x62
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
			case SMALL_HEART:
				energy += 1;
				break;
			case BIG_HEART:
				energy += 5;
				break;
			case WHIPITEM:
				SetState(UPGRADE);
				vx = 0;
				isGotChainItem = true;
				break;
			case MONEY_BAG_RED:
				score += 100;
				break;
			case MONEY_BAG_BLUE:
				score += 400;
				break;
			case MONEY_BAG_WHITE:
				score += 700;
				break;
			case MONEY_BAG:
				score += 1000;
				break;
			case DOUBLE_SHOT:
				break;
			case TRIPLE_SHOT:
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
	return false;
}


bool Simon::CheckCollisionWithStair(vector<LPGAMEOBJECT>* listStair)
{
	canMoveUpStair = false;
	canMoveDownStair = false;

	float simon_l, simon_t, simon_r, simon_b;
	GetBoundingBox(simon_l, simon_t, simon_r, simon_b);

	// thu nhỏ vùng xét va chạm, chỉ xét va chạm với chân của Simon
	simon_t += 50;
	simon_b += 5;  // bottom +5 để xét cho va chạm với bậc thang đầu tiên khi bước xuống

	for (UINT i = 0; i < listStair->size(); i++)
	{
		float stair_l, stair_t, stair_r, stair_b;
		listStair->at(i)->GetBoundingBox(stair_l, stair_t, stair_r, stair_b);


		if (GameObject::AABB(simon_l, simon_t, simon_r, simon_b, stair_l, stair_t, stair_r, stair_b) == true)
		{
			//DebugOut(L"collision\n");

			if (listStair->at(i)->GetState() == 0) 
				stairDirection = 1;
			else 
				stairDirection = -1;

			stairCollided = listStair->at(i);

			// bậc thang ở dưới so với chân Simon->có thể di chuyển xuống.
			
			if (y >= stair_t - 35) 
				canMoveUpStair = true;
			if (simon_b < stair_b)
				canMoveDownStair = true;

			// kiểm tra xem simon có thể di chuyển lên/xuống hay ko
			// (dựa vào toạ độ của 2 bậc liền kề hơn/kém nhau 32)

			float upstair_x = -999, upstair_y = -999; // toạ độ của bậc thang liền kề

			for (UINT j = 0; j < listStair->size(); j++)
			{
				if (i == j)
					continue;

				listStair->at(j)->GetPosition(upstair_x, upstair_y);

				float dx = abs(upstair_x - stair_l);
				float dy = upstair_y - stair_t;

				if (dx == GROUND_BBOX_WIDTH && dy == -GROUND_BBOX_HEIGHT) // vì bậc nằm trên nên dy = -...
					canMoveUpStair = true;

				if (dx == GROUND_BBOX_WIDTH && dy == GROUND_BBOX_HEIGHT) // vì bậc nằm duoi nên dy = +...
					canMoveDownStair = true;
			}
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

void Simon::AutoWalk(float distance, int new_state, int new_nx)
{
	isAutoWalk = true;
	autoWalkDistance = distance;
	stateAfterAutoWalk = new_state;
	nxAfterAutoWalk = new_nx;
}

void Simon::DoAutoWalk()
{
	if (abs(dx) <= abs(autoWalkDistance))
	{
		x += dx;
		y += dy;
		autoWalkDistance -= dx;
	}
	else
	{
		x += autoWalkDistance;
		state = stateAfterAutoWalk;
		nx = nxAfterAutoWalk;

		SetState(state);
		if (state == DESCEND) y += 1.0f; // + 1.0f để đảm bảo simon sẽ va chạm với bậc thang trong lần update kế tiếp

		isAutoWalk = false;
		autoWalkDistance = 0;
		stateAfterAutoWalk = -1;
		nxAfterAutoWalk = 0;
	}
}

bool Simon::IsHit()
{
	return state == STANDING || state == DUCKING || state == ASCENDING || state == DESCENDING;
}

void Simon::CheckCollisionWithEnemyActiveArea(vector<LPGAMEOBJECT>* listObjects)
{
	float simon_l, simon_t, simon_r, simon_b;

	GetBoundingBox(simon_l, simon_t, simon_r, simon_b);

	for (UINT i = 0; i < listObjects->size(); i++)
	{
		Enemy* enemy = dynamic_cast<Enemy*>(listObjects->at(i));

		if (enemy == NULL)
			continue;

		// Không cần xét vùng active nữa khi nó đang active / destroyed
		if (enemy->GetState() == ACTIVE || enemy->GetState() == DESTROYED)
			continue;

		float enemy_l, enemy_t, enemy_r, enemy_b;
		enemy->GetActiveBoundingBox(enemy_l, enemy_t, enemy_r, enemy_b);

		if (GameObject::AABB(simon_l, simon_t, simon_r, simon_b, enemy_l, enemy_t, enemy_r, enemy_b) == true)
		{
			D3DXVECTOR2 enemyEntryPostion = enemy->GetEntryPosition();

			if (dynamic_cast<Ghost*>(enemy))
			{
				Ghost* ghost = dynamic_cast<Ghost*>(enemy);
				ghost->SetState(GHOST_ACTIVE);
			}
			else if (dynamic_cast<HunchBack*>(enemy))
			{
				HunchBack* hunchback = dynamic_cast<HunchBack*>(enemy);
				if (hunchback->GetState() == HUNCHBACK_IDLE)
					hunchback->SetState(HUNCHBACK_ACTIVE);
			}
			else if (dynamic_cast<Bat*>(enemy))
			{
				Bat* bat = dynamic_cast<Bat*>(enemy);
				if (bat->IsEnable() == true)
					bat->SetState(BAT_ACTIVE);
			}
			else if (dynamic_cast<Raven*>(enemy))
			{
				Raven* raven = dynamic_cast<Raven*>(enemy);
				if (raven->IsEnable() == true)
					raven->SetState(RAVEN_ACTIVE);
			}
			else if (dynamic_cast<Zombie*>(enemy))
			{
				Zombie* zombie = dynamic_cast<Zombie*>(enemy);
				if (zombie->GetState() == ZOMBIE_INACTIVE && zombie->IsAbleToActivate())
					zombie->SetState(ZOMBIE_ACTIVE);
			}
			else if (dynamic_cast<Skeleton*>(enemy))
			{
				Skeleton* skeleton = dynamic_cast<Skeleton*>(enemy);

				if (skeleton->GetState() == SKELETON_INACTIVE)
					skeleton->SetState(SKELETON_ACTIVE);
			}
			else if (dynamic_cast<Boss*>(enemy))
			{
				Boss* boss = dynamic_cast<Boss*>(enemy);
				boss->SetState(BOSS_ACTIVE);
			}
		}
	}
}

void Simon::LoseHP(int x)
{
	hp -= x;

	if (hp <= 0)
		hp = 0;
}

void Simon::SetSimon(int s, int l, int h, int e, int w)
{
	score = s;
	life = l;
	hp = h;
	energy = e;
	subWeapon = s;
}







