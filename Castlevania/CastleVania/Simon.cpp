#include "Simon.h"


Simon* Simon::_instance = NULL;

Simon::Simon() : GameObject()
{
	SetState(IDLE);
	score = 0;
	energy = SIMON_ENERGY;
	life = 3;
	subWeapon = -1;
	hp = SIMON_HP;
}


void Simon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMoving)
{

	if (state == BEHIND)
		return;
	
	if (dt > 64)
		dt = 16;
	GameObject::Update(dt);
	
	// Update vy
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

	// Reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > SIMON_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		isUntouchable = false;
	}
	
	if (GetTickCount() - invisibleTime_start > SIMON_INVISIBLE_TIME)
	{
		invisibleTime_start = 0;
		isInvisible = false;
	}


	if (coObjects == NULL)
	{
		
		if (isAutoWalk == false)
		{
			x += dx;
			y += dy;

		}
		return;
	}

		
	// Check collision between Simon and other objects
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	if (y > 480.f) {
		SetState(DEAD);
	}
	if (coEvents.size() == 0 && isAutoWalk == false)
	{
		
		x += dx;
		y += dy;
		if (vy > 0.2f)
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
				Portal* p = dynamic_cast<Portal*>(e->obj);
				if (p->GetSceneBack() != -1)
					Game::GetInstance()->BackScene(p->GetSceneId());
				else	
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
					if (hp == 0)
					{
						SetState(DEAD);
						return;
					}
				}
				// Khi đang lên/xuống cầu thang, va chạm theo trục x sẽ không được xét tới
				if (state == ASCEND || state == DESCEND)
					if (nx != 0) 
						x -= nx * 0.1f;
			}
			else if (dynamic_cast<AreaDead*>(e->obj))
			{
				AreaDead* area = dynamic_cast<AreaDead*>(e->obj);
				SetState(DEAD);
				return;
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
				dynamic_cast<Skeleton*>(e->obj) || dynamic_cast<Raven*>(e->obj) || dynamic_cast<Zombie*>(e->obj) || dynamic_cast<Boss*>(e->obj) || dynamic_cast<Bone*>(e->obj))
			{
				DebugOut("Enemy\n");
				if (state != UPGRADE && isUntouchable == false && isInvisible == false)
				{
					StartUntouchable();
					if (dynamic_cast<Knight*>(e->obj))
					{
						DebugOut("Knight\n");
						Knight* knight = dynamic_cast<Knight*>(e->obj);
						LoseHP(knight->GetAttack());
					}
					else if (dynamic_cast<Bat*>(e->obj))
					{
						Bat* bat = dynamic_cast<Bat*>(e->obj);
						bat->SetState(BAT_DESTROYED);
						LoseHP(bat->GetAttack());
					}
					else if (dynamic_cast<Raven*>(e->obj))
					{
						Raven* raven = dynamic_cast<Raven*>(e->obj);
						raven->SetState(RAVEN_DESTROYED);
						LoseHP(raven->GetAttack());
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
					else if (dynamic_cast<Zombie*>(e->obj))
					{
						Zombie* zombie = dynamic_cast<Zombie*>(e->obj);
						LoseHP(zombie->GetAttack());
					}
					else if (dynamic_cast<Bone*>(e->obj))
					{
						LoseHP(1);
						e->obj->SetEnable(false);
					}
					else if (dynamic_cast<Boss*>(e->obj))
					{
						Boss* boss = dynamic_cast<Boss*>(e->obj);
						LoseHP(boss->GetAttack());
					}
					
					

					if (isStandOnStair == false || hp == 0)  // Simon đứng trên cầu thang sẽ không nhấp nháy
					{
						// đặt trạng thái nhấp nháy cho simon
						/*if (e->nx != 0)
						{
							if (e->nx == 1 && this->nx == 1) 
								this->nx = -1;
							else if (e->nx == -1 && this->nx == -1) 
								this->nx = 1;
						}*/

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

	int tempState = state;	// Sử dụng biến tạm để không thay đổi trạng thái gốc của Simon
	int alpha = 255;
	float ratio = 0;

	// Khi Simon rơi từ trên cao xuống thì luôn co chân
	if (state != DEAD  && IsHit() == false && isFalling == true)
		tempState = DUCK;

	if (isUntouchable == true)  // Để render Simon nhấp nháy trong trạng thái isUntouchable
		alpha = rand() % 255;
	else if (GetTickCount()- invisibleTime_start < SIMON_INVISIBLE_TIME) // Simon tàng hình
	{
		ratio = (float)(GetTickCount() - invisibleTime_start) / SIMON_INVISIBLE_TIME;

		if (ratio < 0.3f)			// trong suốt
			alpha = 0;
		else if (ratio < 0.6f)		// nhấp nháy
			alpha = 200 * (rand() % 100 < 20);
		else						// render animation của invisible
		{
			switch (tempState)
			{
			case IDLE:	
				tempState = IDLE_INVISIBLE; 
				break;
			case WALK:	
				tempState = WALK_INVISIBLE; 
				break;
			case DUCK:	
				tempState = DUCK_INVISIBLE;
				break;
			case JUMP:	
				tempState = JUMP_INVISIBLE; 
				break;
			case ASCEND:	
				tempState = ASCEND_INVISIBLE; 
				break;
			case DESCEND:	
				tempState = DESCEND_INVISIBLE; 
				break;
			case ASCENDING:
				tempState = ASCENDING_INVISIBLE; 
				break;
			case DESCENDING:	
				tempState = DESCENDING_INVISIBLE; 
				break;
			case STANDING: 
				tempState = STANDING_INVISIBLE; 
				break;
			case DUCKING:	
				tempState = DUCKING_INVISIBLE; 
				break;
			default:
				break;
			}
		}
	}
	if (state == -1)
		return;

	animation_set->at(tempState)->Render(1, nx, x, y, alpha);
	int currentFrame = animation_set->at(tempState)->GetCurrentFrame();
	animation_set->at(state)->SetCurrentFrame(currentFrame);
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
			vx = -SIMON_HURT_SPEED_X;
		else 
			vx = SIMON_HURT_SPEED_X;
		vy = -SIMON_HURT_SPEED_Y;
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
		isUntouchable = false;
		isInvisible = false;
		vx = 0;
		vy = 0;
		life -= 1;
		break;
	case BEHIND:
		vx = 0;
		vy = 0;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
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
				Game::gameSound->playSound(EATWEAPON_CHICKEN);
				break;
			case AXE:
				subWeapon = 1;
				Game::gameSound->playSound(EATWEAPON_CHICKEN);
				break;
			case BOOMERANG:
				subWeapon = 2;
				Game::gameSound->playSound(EATWEAPON_CHICKEN);
				break;
			case HOLY_WATER:
				subWeapon = 3;
				Game::gameSound->playSound(EATHEART);
				break;
			case STOP_WATCH:
				subWeapon = 4;
				Game::gameSound->playSound(EATHEART);
				break;
			case SMALL_HEART:
				energy += 1;
				Game::gameSound->playSound(EATHEART);
				break;
			case BIG_HEART:
				energy += 5;
				Game::gameSound->playSound(EATHEART);
				break;
			case WHIPITEM:
				SetState(UPGRADE);
				vx = 0;
				isGotChainItem = true;
				Game::gameSound->playSound(EATWEAPON_CHICKEN);
				break;
			case MONEY_BAG_RED:
				score += 100;
				Game::gameSound->playSound(EATMONEY);
				break;
			case MONEY_BAG_BLUE:
				score += 400;
				Game::gameSound->playSound(EATMONEY);
				break;
			case MONEY_BAG_WHITE:
				score += 700;
				Game::gameSound->playSound(EATMONEY);
				break;
			case MONEY_BAG:
				score += 1000;
				Game::gameSound->playSound(EATMONEY);
				break;
			case DOUBLE_SHOT:
				stateShot = idItem;
				isGotDoubleShotItem = true;
				Game::gameSound->playSound(EATHEART);
				break;
			case TRIPLE_SHOT:
				stateShot = idItem;
				isGotTripleShotItem = true;
				Game::gameSound->playSound(EATHEART);
				break;
			case MAGIC_CRYSTAL:
				isGotMagicCrystalItem = true;
				Game::gameSound->stopSound(BOSS_FIGHT);
				Game::gameSound->playSound(EATCRYSTAL);
				break;
			case FOOD:
				hp += 2;
				if (hp > SIMON_HP)
					hp = SIMON_HP;
				Game::gameSound->playSound(EATWEAPON_CHICKEN);
				break;
			case INVISIBLE_BOTLE:
				StartInvisible();
				Game::gameSound->playSound(EATPOTION);
				break;
			case CROSS:
				isGotCrossItem = true;
				Game::gameSound->playSound(EATCROSS);
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

			
		if (GameObject::AABB(simon_l, simon_t, simon_r, simon_b, stair_l - 96, stair_t, stair_r, stair_b) == true)
		{
			//DebugOut("collision\n");

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
			return true; // va chạm Simon và stairs
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
		if (state == DESCEND) 
			y += 1.0f; // + 1.0f để đảm bảo simon sẽ va chạm với bậc thang trong lần update kế tiếp

		isAutoWalk = false;
		autoWalkDistance = 0;
		stateAfterAutoWalk = -1;
		nxAfterAutoWalk = 0;
	}
	/*DebugOut("3: x: %f, y: %f\n", x, y);*/
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

			if (dynamic_cast<Bat*>(enemy))
			{
				Bat* bat = dynamic_cast<Bat*>(enemy);
				if (bat->IsEnable() == true)
					bat->SetState(BAT_ACTIVE);
			}
			else if (dynamic_cast<Ghost*>(enemy))
			{
				Ghost* ghost = dynamic_cast<Ghost*>(enemy);
				if(ghost->GetState() == GHOST_INACTIVE && ghost->IsAbleToActivate())
					ghost->SetState(GHOST_ACTIVE);
			}
			else if (dynamic_cast<HunchBack*>(enemy))
			{
				HunchBack* hunchback = dynamic_cast<HunchBack*>(enemy);
				if (hunchback->GetState() == HUNCHBACK_IDLE)
					hunchback->SetState(HUNCHBACK_ACTIVE);
			}
			else if (dynamic_cast<Raven*>(enemy))
			{
				Raven* raven = dynamic_cast<Raven*>(enemy);
				if (raven->GetState() == RAVEN_IDLE)
					raven->SetState(RAVEN_ACTIVE);
			}
			else if (dynamic_cast<Zombie*>(enemy))
			{
				Zombie* zombie = dynamic_cast<Zombie*>(enemy);
				if (zombie->GetState() == ZOMBIE_INACTIVE && zombie->IsAbleToActivate())
				{
					zombie->isSettedPosition = true;
					float simon_x, simon_y;
					GetPosition(simon_x, simon_y);

					int nx = zombie->GetEntryPosition().x < simon_x ? 1 : -1;
					zombie->SetOrientation(nx);

					// Cần random một khoảng nhỏ để tránh việc các zombie spawn cùng lúc, tại cùng một vị trí
					int randomDistance = rand() % 20;

					float x, y;
					y = zombie->GetEntryPosition().y;

					if (nx == -1)
						x = Game::GetInstance()->GetCameraPositon().x + SCREEN_WIDTH - (ENEMY_DEFAULT_BBOX_WIDTH + randomDistance);
					else
						x = Game::GetInstance()->GetCameraPositon().x + (ENEMY_DEFAULT_BBOX_WIDTH + randomDistance);

					zombie->SetPosition(x, y);
					zombie->SetState(ZOMBIE_ACTIVE);
				}
					
			}
			else if (dynamic_cast<Skeleton*>(enemy))
			{
				Skeleton* skeleton = dynamic_cast<Skeleton*>(enemy);
				if (skeleton->GetState() == SKELETON_INACTIVE && skeleton->IsAbleToActivate())
					skeleton->SetState(SKELETON_JUMP);
			}
			else if (dynamic_cast<Boss*>(enemy))
			{
				Boss* boss = dynamic_cast<Boss*>(enemy);
				boss->SetState(BOSS_ACTIVE);
			}
		}
	}
}

Simon* Simon::GetInstance()
{
	if (_instance == NULL) _instance = new Simon();
	return _instance;
}

void Simon::AddHP(int x)
{
	hp += x;

	if (hp >= 16)
		hp = 16;
}

void Simon::LoseHP(int x)
{
	hp -= x;

	if (hp <= 0)
		hp = 0;
}








