﻿#include "Weapon.h"


Weapon::Weapon() : GameObject()
{
	state = -1;
	AnimationSets* animation_sets = AnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(4);
	SetAnimationSet(ani_set);
	spark = Animations::GetInstance()->Get(8001);
}


void Weapon::Render()
{
	RenderSpark();
	if (this->isEnable == true && state != WEAPON_STOP_WATCH)
		animation_set->at(state)->Render(1, nx, x, y);
}

void Weapon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject, bool stopMoving)
{
	if (isHolyWater == true && GetTickCount() - holyWaterCounter > HOLY_WATER_TIME_EFFECT)
	{
		isHolyWater = false;
		holyWaterCounter = 0;
		this->isEnable = false;
		return;
	}

	GameObject::Update(dt);

	switch (state)
	{
	case WEAPON_AXE:
		vy += AXE_GRAVITY * dt;
		break;
	case WEAPON_BOOMERANG:
		if (nx > 0) vx -= BOOMERANG_TURNBACK_SPEED;
		else vx += BOOMERANG_TURNBACK_SPEED;
		break;
	case WEAPON_HOLY_WATER:
		vy += HOLY_WATER_GRAVITY * dt;
		break;
	default:
		break;
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObject, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		// kiểm tra va chạm với object
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Torch*>(e->obj))
			{
				Torch* torch = dynamic_cast<Torch*>(e->obj);
				torch->SetState(EFFECTEXPLODE);
				targetTypeHit = TORCH;
				float l, t, r, b;
				e->obj->GetBoundingBox(l, t, r, b);
				sparkEffect.push_back({ l, t });

				UpdateCollisionState();
			}
			else if (dynamic_cast<Ground*>(e->obj) || dynamic_cast<BreakWall*>(e->obj))
			{
				if (state == WEAPON_HOLY_WATER && e->ny == -1)
					SetState(HOLY_WATER_BROKEN);

				x += dx;
				y += dy;
			}
			else if (dynamic_cast<Knight*>(e->obj))
			{
				Knight* knight = dynamic_cast<Knight*>(e->obj);
				knight->LoseHP(2);
				targetTypeHit = KNIGHT;

				if (knight->GetState() == KNIGHT_DESTROYED)
				{
					scoreReceived += knight->GetScore();
					float l, t, r, b;
					e->obj->GetBoundingBox(l, t, r, b);
					sparkEffect.push_back({ l, t });
				}

				UpdateCollisionState();
			}
			else if (dynamic_cast<Bat*>(e->obj))
			{
				Bat* bat = dynamic_cast<Bat*>(e->obj);
				bat->LoseHP(2);
				targetTypeHit = BAT;

				if (bat->GetState() == BAT_DESTROYED)
				{
					scoreReceived += bat->GetScore();
					float l, t, r, b;
					e->obj->GetBoundingBox(l, t, r, b);
					sparkEffect.push_back({ l, t });
				}

				UpdateCollisionState();
			}
			else if (dynamic_cast<Ghost*>(e->obj))
			{
				Ghost* ghost = dynamic_cast<Ghost*>(e->obj);
				ghost->LoseHP(2);
				targetTypeHit = GHOST;

				if (ghost->GetState() == GHOST_DESTROYED)
				{
					scoreReceived += ghost->GetScore();
					float l, t, r, b;
					e->obj->GetBoundingBox(l, t, r, b);
					sparkEffect.push_back({ l, t });
				}

				UpdateCollisionState();
			}
			else if (dynamic_cast<HunchBack*>(e->obj))
			{
				HunchBack* hunchback = dynamic_cast<HunchBack*>(e->obj);
				hunchback->LoseHP(2);
				targetTypeHit = HUNCHBACK;

				if (hunchback->GetState() == HUNCHBACK_DESTROYED)
				{
					scoreReceived += hunchback->GetScore();
					float l, t, r, b;
					e->obj->GetBoundingBox(l, t, r, b);
					sparkEffect.push_back({ l, t });
				}

				UpdateCollisionState();
			}
			else if (dynamic_cast<Raven*>(e->obj))
			{
				Raven* raven = dynamic_cast<Raven*>(e->obj);
				raven->LoseHP(2);
				targetTypeHit = RAVEN;

				if (raven->GetState() == HUNCHBACK_DESTROYED)
				{
					scoreReceived += raven->GetScore();
					float l, t, r, b;
					e->obj->GetBoundingBox(l, t, r, b);
					sparkEffect.push_back({ l, t });
				}

				UpdateCollisionState();
			}
			else if (dynamic_cast<Skeleton*>(e->obj))
			{
				Skeleton* skeleton = dynamic_cast<Skeleton*>(e->obj);
				skeleton->LoseHP(2);
				targetTypeHit = SKELETON;

				if (skeleton->GetState() == SKELETON_DESTROYED)
				{
					scoreReceived += skeleton->GetScore();
					float l, t, r, b;
					e->obj->GetBoundingBox(l, t, r, b);
					sparkEffect.push_back({ l, t });
				}

				UpdateCollisionState();
			}
			else if (dynamic_cast<Bone*>(e->obj))
			{
				Bone* bone = dynamic_cast<Bone*>(e->obj);
				bone->SetEnable(false);
				targetTypeHit = BONE;
				float l, t, r, b;
				e->obj->GetBoundingBox(l, t, r, b);
				sparkEffect.push_back({ l, t });
				UpdateCollisionState();
			}
			else if (dynamic_cast<Zombie*>(e->obj))
			{
				Zombie* zombie = dynamic_cast<Zombie*>(e->obj);
				zombie->LoseHP(2);

				targetTypeHit = ZOMBIE;

				if (zombie->GetState() == ZOMBIE_DESTROYED)
				{
					scoreReceived += zombie->GetScore();
					float l, t, r, b;
					e->obj->GetBoundingBox(l, t, r, b);
					sparkEffect.push_back({ l, t });
				}

				UpdateCollisionState();
			}
			else if (dynamic_cast<Boss*>(e->obj))
			{
				Boss* boss = dynamic_cast<Boss*>(e->obj);
				boss->LoseHP(2);
				targetTypeHit = BOSS;
				//Nguyen
				float l, t, r, b;
				e->obj->GetBoundingBox(l, t, r, b);
				sparkEffect.push_back({ l, t });
				//
				if (boss->GetState() == BOSS_DESTROYED)
				{
					scoreReceived += boss->GetScore();
					/*float l, t, r, b;
					e->obj->GetBoundingBox(l, t, r, b);*/
					//sparkEffect.push_back({ l, t });
				}

				UpdateCollisionState();
			}
			else if (dynamic_cast<Simon*>(e->obj))
			{
				if (state == WEAPON_BOOMERANG)
					SetEnable(false);
			}
		}
	}

	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Weapon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	
	switch (state)
	{
	case WEAPON_KNIFE:
		right = left + KNIFE_BBOX_WIDTH;
		bottom = top + KNIFE_BBOX_HEIGHT;
	case WEAPON_AXE:
		right = left + AXE_BBOX_WIDTH;
		bottom = top + AXE_BBOX_HEIGHT;
	case WEAPON_BOOMERANG:
		right = left + AXE_BBOX_WIDTH;
		bottom = top + AXE_BBOX_HEIGHT;
	case WEAPON_HOLY_WATER:
		right = left + KNIFE_BBOX_HEIGHT;
		bottom = top + AXE_BBOX_HEIGHT;
		break;
	default:
		right = left;
		bottom = top;
		break;
	}
}

void Weapon::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case WEAPON_KNIFE:
		if (nx > 0)
			vx = KNIFE_SPEED;
		else 
			vx = -KNIFE_SPEED;
		vy = 0;
		Game::gameSound->playSound(SOUNDKNIFE);
		break;
	case WEAPON_AXE:
		if (nx > 0) 
			vx = AXE_SPEED_X;
		else 
			vx = -AXE_SPEED_X;
		vy = -AXE_SPEED_Y;
		Game::gameSound->playSound(WHIPATTACK_AXE);
		break;
	case WEAPON_BOOMERANG:
		vx = nx * BOOMERANG_SPEED;
		vy = 0;
		Game::gameSound->playSound(WHIPATTACK_AXE);
		break;
	case WEAPON_HOLY_WATER:
		vx = nx * HOLY_WATER_SPEED_X;
		vy = -HOLY_WATER_SPEED_Y;
		Game::gameSound->playSound(DOWNWATER);
		break;
	case HOLY_WATER_BROKEN:
		vx = 0;
		vy = 0;
		HolyWaterEffect();
		Game::gameSound->playSound(HOLYWATERBREAK);
		break;
	default:
		break;
	}
}

void Weapon::UpdateCollisionState()
{
	if (state == WEAPON_KNIFE || state == WEAPON_BOOMERANG)
		this->isEnable = false;
	else
	{
		x += dx;
		y += dy;
	}
}

void Weapon::RenderSpark()
{
	if (sparkEffect.size() > 0)
	{
		if (startTimeRenderSpark == 0)
			startTimeRenderSpark = GetTickCount();
		else if (GetTickCount() - startTimeRenderSpark > 100)
		{
			startTimeRenderSpark = 0;
			sparkEffect.clear();
		}

		for (auto effect : sparkEffect)
			spark->Render(1, -1, effect[0], effect[1]);
	}
}

