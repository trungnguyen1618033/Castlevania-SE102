#include "BatIntro.h"

BatIntro::BatIntro()
{
	SetState(0);

	vx = (float)(-50 + rand() % 100) / 1000;
	vy = (float)(-50 + rand() % 100) / 1000;

	
}

BatIntro::~BatIntro()
{
}

void BatIntro::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMovement)
{
	GameObject::Update(dt);

	x += dx;
	y += dy;
}

void BatIntro::Render()
{
	animation_set->at(state)->Render(0, nx, x, y);
}

void BatIntro::SetState(int state)
{
	GameObject::SetState(state);
}
