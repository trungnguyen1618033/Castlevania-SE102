#include "PieceWall.h"

PieceWall::PieceWall()
{
	SetState(0);
	AnimationSets* animation_sets = AnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(11);
	SetAnimationSet(ani_set);
	DebugOut(L"Init Piece wall\n");
}

void PieceWall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMoving)
{
	GameObject::Update(dt);
	vy += WALL_GRAVITY * dt;

	x += dx;
	y += dy;
}

void PieceWall::Render()
{
	animation_set->at(state)->Render(1, nx, x, y);
}

WallPieces::WallPieces(float x, float y)
{
	PieceWall* piece;

	for (int i = 1; i <= 4; i++)
	{
		piece = new PieceWall();
		piece->SetPosition(x, y);

		float vx = (float)(-100 + rand() % 200) / 1000;
		float vy = (float)(-100 + rand() % 200) / 1000;
		piece->SetSpeed(vx, vy);

		pieces.push_back(piece);
	}
}

WallPieces::~WallPieces()
{
	for (int i = 0; i < 4; i++)
		delete(pieces[i]);

	pieces.clear();
}

void WallPieces::Update(DWORD dt)
{
	for (auto piece : pieces)
		piece->Update(dt);
}

void WallPieces::Render()
{
	for (auto piece : pieces)
		piece->Render();
}
