#include "Simon.h"

Simon::Simon() : GameObject()
{
	untouchable = 0;
	SetState(IDLE);
	whip = new Whip();
	
}


void Simon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GameObject::Update(dt);

	/*vy += SIMON_GRAVITY;
	if (y > 128)
	{
		vy = 0;
		y = 128.0f;	
	}*/

	// simple collision with border map
	if (x < 0) x = 0;
	if (x >= 736) x = 736;
	
	if (vy < -0.1f || vy > 0.1f)
		vy += SIMON_GRAVITY*dt;
	else vy += SIMON_GRAVITY_LOWER*dt;
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
				if (e->obj->explode)
				{
					e->obj->SetEnable(false);
				}
				else
				{
					if (e->nx != 0) x += dx;
					if (e->ny != 0) y += dy;
				}
			}
			else if (dynamic_cast<Ground*>(e->obj))
			{
				if (ny != 0) vy = 0;
			}
			else if (dynamic_cast<Items*>(e->obj))
			{
				e->obj->isEnable = false;
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
	if (state == STANDING || state == DUCKING)	
	{
		// lấy vị trí của simon để set vị trí cho roi

		whip->SetOrientation(nx);
		whip->SetWhipPosition(D3DXVECTOR3(x, y, 0), isStand);


		//DebugOut(L"current: %d\n", animations[state]->GetCurrentFrame());

		if (animation_set->at(state)->GetCurrentFrame() == animation_set->at(state)->GetFramesSize() - 1) // chỉ xét va chạm khi render tới sprite cuối cùng của roi
		{
			for (UINT i = 0; i < coObjects->size(); i++)
			{
				LPGAMEOBJECT obj = coObjects->at(i);
				if (dynamic_cast<Torch*>(obj))
				{
					Torch* e = dynamic_cast<Torch*> (obj);

					float left, top, right, bottom;

					e->GetBoundingBox(left, top, right, bottom);

				/*	DebugOut(L"%f %f %f %f\n", left, top, right, bottom);*/

					if (whip->CheckCollision(left, top, right, bottom) == true) // va chạm giữa roi và nến
					{
						DebugOut(L"collision\n");
						e->SetState(EFFECTEXPLODE);
						e->animation_set->at(EFFECTEXPLODE)->SetAniStartTime(GetTickCount());
					}
				}
			}
		}
	}
}

void Simon::Render()
{
	animation_set->at(state)->Render(nx, x, y);
	/*DebugOut(L"[INFO] State: %d\n", ani);*/

	if (state == STANDING || state == DUCKING) {		// lấy vị trí của simon để thực hiện gắn roi
		whip->Render(animation_set->at(state)->GetCurrentFrame());
	}
	/*else if (state == THROW) {
		knife->Render(animation_set->at(state)->GetCurrentFrame());
	}*/
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
		break;
	case DESCEND:
		isStand = true;
		break;
	case HURT:
		isStand = true;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	case DEATH:
		isStand = true;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	case STANDING:
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
	case DESCENDING:
		isStand = true;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
	case THROW:
		isStand = true;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		/*DebugOut(L"id animation_set=%d", state);*/
	default:
		break;
	}
	RenderBoundingBox();
}

void Simon::SetWhipAnimationSet(LPANIMATION_SET ani_set)
{
	whip->SetAnimationSet(ani_set);
}


void Simon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + 7;
	top = y;
	right = left + SIMON_BBOX_WIDTH;
	bottom = top + SIMON_BBOX_HEIGHT;
}





