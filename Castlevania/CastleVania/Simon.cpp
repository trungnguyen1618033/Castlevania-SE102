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
	
	if (state != ASCEND && state != DESCEND &&
		state != ASCENDING && state != DESCENDING)
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
					}
					else
					{
						y += dy;
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
	if (isUntouchable)  // Để render Simon nhấp nháy trong trạng thái isUntouchable
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
		break;
	case WALK:
		if (nx > 0) vx = SIMON_WALKING_SPEED;
		else vx = -SIMON_WALKING_SPEED;
		break;
	case DUCK:
		isStand = false;
		vx = 0;
		vy = 0;
		break;
	case JUMP:
		isStand = true;
		vy = -SIMON_JUMP_SPEED_Y;
		break;
	case ASCEND:
		isStand = true;
		if (nx > 0) vx = SIMON_STAIR_SPEED_X;
		else vx = -SIMON_STAIR_SPEED_X;
		vy = -SIMON_STAIR_SPEED_Y;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	case DESCEND:
		isStand = true;
		if (nx > 0) vx = SIMON_STAIR_SPEED_X;
		else vx = -SIMON_STAIR_SPEED_X;
		vy = SIMON_STAIR_SPEED_Y;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	case HURT:
		isStand = true;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	case DUCKING:
		isStand = false;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	case ASCENDING:
		isStand = true;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	case DESCENDING:
		isStand = true;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	case STANDING:
	case HIT:
	case UPGRADE:
		isStand = true;
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

void Simon::CheckCollisionWithEnemyActiveArea(vector<LPGAMEOBJECT>* listEnemy)
{
	float simon_l, simon_t, simon_r, simon_b;

	GetBoundingBox(simon_l, simon_t, simon_r, simon_b);

	for (UINT i = 0; i < listEnemy->size(); i++)
	{
		LPGAMEOBJECT enemy = listEnemy->at(i);

		if (enemy->GetState() == ACTIVE || enemy->GetState() == DESTROYED)
			continue;

		float enemy_l, enemy_t, enemy_r, enemy_b;
		enemy->GetActiveBoundingBox(enemy_l, enemy_t, enemy_r, enemy_b);

		if (GameObject::AABB(simon_l, simon_t, simon_r, simon_b, enemy_l, enemy_t, enemy_r, enemy_b) == true)
		{
			D3DXVECTOR2 enemyEntryPostion = enemy->GetEntryPosition();

			if (dynamic_cast<Zombie*>(enemy))
			{
				Zombie* zombie = dynamic_cast<Zombie*>(enemy);

				if (zombie->IsAbleToActivate() == true)
				{
					if ((enemyEntryPostion.x > x && enemyEntryPostion.x - x > 220 && enemyEntryPostion.x - x < 250) ||
						(enemyEntryPostion.x < x && x - enemyEntryPostion.x > 230 && x - enemyEntryPostion.x < 270))
					{
						if (enemyEntryPostion.x < x) zombie->SetOrientation(1);
						else zombie->SetOrientation(-1);

						zombie->SetState(ZOMBIE_ACTIVE);
					}
				}
			}
			else if (dynamic_cast<Leopad*>(enemy))
			{
				Leopad* leopad = dynamic_cast<Leopad*>(enemy);
				if (leopad->GetState() == LEOPAD_INACTIVE && leopad->IsAbleToActivate() == true)
					leopad->SetState(LEOPAD_IDLE);
				else if (leopad->GetState() == LEOPAD_IDLE)
					leopad->SetState(LEOPAD_ACTIVE);
			}
			else if (dynamic_cast<Bat*>(enemy))
			{
				Bat* bat = dynamic_cast<Bat*>(enemy);

				if (bat->IsAbleToActivate() == true)
				{

					// set random hướng cho dơi

					int listNx[2] = { -1, 1 };
					int rndIndex = rand() % 2;
					bat->SetOrientation(listNx[rndIndex]);
					bat->SetState(BAT_ACTIVE);
				}
			}
		}
	}
}

bool Simon::CheckCollisionWithStair(vector<LPGAMEOBJECT>* listStair)
{
	float simon_l, simon_t, simon_r, simon_b;
	GetBoundingBox(simon_l, simon_t, simon_r, simon_b);

	// thu nhỏ vùng xét va chạm, chỉ xét va chạm với chân của Simon
	simon_t += 55;
	simon_b += 5;  // bottom +5 để xét cho va chạm với bậc thang đầu tiên khi bước xuống
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

			// bậc thang ở dưới so với chân Simon->có thể di chuyển xuống.
			if (simon_b < stair_b) isMovingDown = true;

			// kiểm tra xem simon có thể di chuyển lên hay ko
			// vì mảng listStairs gồm các bậc thang liền kề nhau, nên chỉ cần kiểm tra 2 bậc là đủ.

			float upstair_x = -999, upstair_y = -999; // toạ độ của bậc thang liền kề ở phía trên (nếu có)

			if (i > 0)
			{
				listStair->at(i - 1)->GetPosition(upstair_x, upstair_y);

				float dx = abs(upstair_x - stair_l);
				float dy = upstair_y - stair_t;

				if (dx == GROUND_BBOX_WIDTH && dy == -GROUND_BBOX_HEIGHT) // vì bậc nằm trên nên dy = -...
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

			// ko có bậc thang kế tiếp, tuy nhiên cần kiểm tra simon đã đi hết sprite thang hiện tại chưa
			// (một sprite là 32x32, gồm 2 bậc thang, mỗi lần simon chỉ đi lên 1 bậc)
			if (stair_t - y < 60)
			{
				isMovingUp = true;
				return true;
			}

			isMovingUp = false;
			return true; // collision between Simon and stairs
		}

	}

	isMovingUp = false;
	isMovingDown = false;

	return false;
}

void Simon::PositionCorrection(int prevState)
{
	float stair_x, stair_y;
	stairCollided->GetPosition(stair_x, stair_y);

	if (prevState == -1)
	{
		if (state == ASCEND)
		{
			if (stairDirection == 1)
			{
				x = stair_x - 34.0f;
				y = stair_y - 31.0f;
			}
			else
			{
				x = stair_x + 6.0f;
				y = stair_y - 31.0f;
			}
		}
		else if (state == DESCEND)
		{
			if (stairDirection == 1)
			{
				x = stair_x - 10.0f;
				y = stair_y - 47.0f;
			}
			else
			{
				x = stair_x - 18.0f;
				y = stair_y - 47.0f;
			}
		}
	}
	else
	{
		if (state == ASCEND && prevState == DESCEND)
		{
			if (stairDirection == 1)
			{
				x -= 3.0f;
			}
			else
			{
				x += 3.0f;
			}
		}
		else if (state == DESCEND && prevState == ASCEND)
		{
			if (stairDirection == 1)
			{
				x += 3.0f;
			}
			else
			{
				x -= 3.0f;
			}
		}
	}
}

void Simon::StandOnStair()
{
	vx = 0;
	vy = 0;
}





