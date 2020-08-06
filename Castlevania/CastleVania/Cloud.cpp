#include "Cloud.h"

Cloud::Cloud()
{
	SetState(0);

	vx = -0.02f;
	vy = -0.001f;
}

Cloud::~Cloud()
{
}

void Cloud::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject, bool stopMovement)
{
	GameObject::Update(dt);
	x += dx;
	y += dy;
}

void Cloud::Render()
{
	animation_set->at(state)->Render(0, nx, x, y);
}

void Cloud::SetState(int state)
{
	GameObject::SetState(state);
}
