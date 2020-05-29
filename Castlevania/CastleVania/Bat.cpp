#include "Bat.h"

Bat::Bat()
{
	nx = 1;
}

void Bat::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == BAT_DESTROYED && animation_set->at(state)->IsOver(150) == true)
	{
		SetState(BAT_INACTIVE);
		return;
	}

	GameObject::Update(dt);

	vy += velocityVariation;

	if (vy >= BAT_FLYING_SPEED_Y || vy <= -BAT_FLYING_SPEED_Y)
		velocityVariation *= -1;

	x += dx;
	y += dy;
}

void Bat::Render()
{
	animation_set->at(state)->Render(1, nx, x, y);
}

void Bat::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + 6;
	top = y;
	right = left + BAT_BBOX_WIDTH;
	bottom = top + BAT_BBOX_HEIGHT;
}

void Bat::GetActiveBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = entryPosition.x - BAT_ACTIVE_BBOX_WIDTH;
	top = entryPosition.y - BAT_ACTIVE_BBOX_HEIGHT;
	right = entryPosition.x + BAT_ACTIVE_BBOX_WIDTH;
	bottom = entryPosition.y + BAT_ACTIVE_BBOX_HEIGHT;
}

bool Bat::IsAbleToActivate()
{
	DWORD now = GetTickCount();

	if (isRespawnWaiting == true && now - respawnTime_Start >= BAT_RESPAWN_TIME)
		return true;

	return false;
}

void Bat::SetState(int state)
{
}
