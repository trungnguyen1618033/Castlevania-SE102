#include "Bat.h"

#define BAT_FLYING_SPEED_Y	0.1f

Bat::Bat()
{
	velocityVariation = 0.01f;
	hp = 1;
	score = 200;
	attack = 2;
}

void Bat::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMoving)
{
	if (stopMoving == true)
		return;

	if (state == BAT_INACTIVE)
		return;

	if (state == BAT_DESTROYED && animation_set->at(state)->IsOver(150) == true)
	{
		SetState(BAT_INACTIVE);
		return;
	}
	if (state == BAT_IDLE)
		return;

	Enemy::Update(dt);

	vy += velocityVariation;

	if (vy >= BAT_FLYING_SPEED_Y)
		vy = 0;
	if (y > 186)
		vy = 0;


	x += dx;
	y += dy;


}

void Bat::Render()
{
	if(state !=	BAT_INACTIVE)
		animation_set->at(state)->Render(1, nx, x, y);
	RenderActiveBoundingBox();
}

void Bat::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + 11;
	top = y + 11;
	right = left + BAT_BBOX_WIDTH;
	bottom = top + BAT_BBOX_HEIGHT;
}

void Bat::GetActiveBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = entryPosition.x;
	top = entryPosition.y;
	right = entryPosition.x + 192;
	bottom = entryPosition.y + BAT_ACTIVE_BBOX_HEIGHT;
}

void Bat::LoseHP(int x)
{
	Enemy::LoseHP(x);

	if (hp == 0)
		SetState(BAT_DESTROYED);
}

void Bat::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case BAT_ACTIVE:
		if (nx > 0) 
			vx = BAT_FLYING_SPEED_X;
		else vx = 
			-BAT_FLYING_SPEED_X;
		vy = 0;
		isDroppedItem = false;
		break;
	case BAT_DESTROYED:
		vx = 0;
		vy = 0;
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		SetEnable(false);
		break;
	case BAT_INACTIVE:
		x = entryPosition.x;
		y = entryPosition.y;
		vx = 0;
		vy = 0;
		StartRespawnTimeCounter();
		break;
	case BAT_IDLE:
		vx = 0;
		vy = 0;
		break;
	default:
		break;
	}
}
