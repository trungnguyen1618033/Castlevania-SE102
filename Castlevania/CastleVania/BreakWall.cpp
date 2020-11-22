#include "BreakWall.h"

BreakWall::BreakWall()
{
	SetState(0);
	AnimationSets* animation_sets = AnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(10);
	SetAnimationSet(ani_set);
}

void BreakWall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMoving)
{
	if (state == BREAK && isBreakToPieces == false)
	{
		Game::gameSound->playSound(BRICKBREAK);
		isBreakToPieces = true;
		pieces = new WallPieces(x, y);
	}

	if (pieces != NULL)
		pieces->Update(dt);
}

void BreakWall::Render()
{
	if (state == NORMAL)
		return;

	animation_set->at(state)->Render(1, -1, x, y);

	if (isBreakToPieces == true && pieces != NULL)
	{
		if (pieces->GetStartTimeRender() == 0)
			pieces->SetStartTimeRender(GetTickCount());
		else if (GetTickCount() - pieces->GetStartTimeRender() > WALLPIECES_TIME)
		{
			delete pieces;
			pieces = NULL;
			return;
		}

		pieces->Render();
	}
	
}

void BreakWall::SetState(int state)
{
	GameObject::SetState(state);
}

void BreakWall::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + GROUND_BBOX_WIDTH;
	bottom = top + GROUND_BBOX_HEIGHT;
}
