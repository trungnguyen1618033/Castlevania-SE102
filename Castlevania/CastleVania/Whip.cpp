#include "Whip.h"

Whip::Whip()
{
	SetState(WHIP_2);
	AnimationSets* animation_sets = AnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(1);
	SetAnimationSet(ani_set);
	spark = Animations::GetInstance()->Get(8001);
}

void Whip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMoving)
{

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);

		if (dynamic_cast<Torch*>(obj))
		{
			Torch* e = dynamic_cast<Torch*> (obj);

			float left, top, right, bottom;

			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom) == true)
			{
				e->SetState(EFFECTEXPLODE);
				targetTypeHit = CANDLE;
				sparkEffect.push_back({ left, top });
			}
		}
		else if (dynamic_cast<BreakWall*>(obj))
		{
			BreakWall* e = dynamic_cast<BreakWall*>(obj);

			float left, top, right, bottom;
			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom) == true)
			{
				e->SetState(BREAK);
				targetTypeHit = BREAKWALL;
			}
		}
		else if (dynamic_cast<Knight*>(obj))
		{
			Knight* e = dynamic_cast<Knight*> (obj);

			float left, top, right, bottom;

			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom) == true)
			{
				e->LoseHP(1);
				targetTypeHit = KNIGHT;

				if (e->GetState() == KNIGHT_DESTROYED)
				{
					scoreReceived += e->GetScore();
					sparkEffect.push_back({ left, top });
				}

			}
		}
		else if (dynamic_cast<Bat*>(obj))
		{
			Bat* e = dynamic_cast<Bat*> (obj);

			float left, top, right, bottom;

			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom) == true)
			{
				e->LoseHP(1);
				targetTypeHit = BAT;

				if (e->GetState() == BAT_DESTROYED)
				{
					scoreReceived += e->GetScore();
					sparkEffect.push_back({ left, top });
				}
			}
		}
		else if (dynamic_cast<Ghost*>(obj))
		{
			Ghost* e = dynamic_cast<Ghost*> (obj);

			float left, top, right, bottom;

			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom) == true)
			{
				e->LoseHP(1);
				targetTypeHit = GHOST;

				if (e->GetState() == GHOST_DESTROYED)
				{
					scoreReceived += e->GetScore();
					sparkEffect.push_back({ left, top });
				}
			}
		}
		else if (dynamic_cast<HunchBack*>(obj))
		{
			HunchBack* e = dynamic_cast<HunchBack*> (obj);

			float left, top, right, bottom;

			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom) == true)
			{
				e->LoseHP(1);
				targetTypeHit = HUNCHBACK;

				if (e->GetState() == HUNCHBACK_DESTROYED)
				{
					scoreReceived += e->GetScore();
					sparkEffect.push_back({ left, top });
				}
			}
		}
		else if (dynamic_cast<Raven*>(obj))
		{
			Raven* e = dynamic_cast<Raven*> (obj);

			float left, top, right, bottom;

			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom) == true)
			{
				e->LoseHP(1);
				targetTypeHit = RAVEN;

				if (e->GetState() == RAVEN_DESTROYED)
				{
					scoreReceived += e->GetScore();
					sparkEffect.push_back({ left, top });
				}
			}
		}
		else if (dynamic_cast<Skeleton*>(obj))
		{
			Skeleton* e = dynamic_cast<Skeleton*> (obj);

			float left, top, right, bottom;

			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom) == true)
			{
				e->LoseHP(1);
				targetTypeHit = SKELETON;

				if (e->GetState() == SKELETON_DESTROYED)
				{
					scoreReceived += e->GetScore();
					sparkEffect.push_back({ left, top });
				}
			}
		}
		else if (dynamic_cast<Bone*>(obj))
		{
			Bone* e = dynamic_cast<Bone*> (obj);

			float left, top, right, bottom;

			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom) == true)
			{
				e->SetEnable(false);
				targetTypeHit = BONE;
				sparkEffect.push_back({ left, top });
			}
		}
		else if (dynamic_cast<Zombie*>(obj))
		{
			Zombie* e = dynamic_cast<Zombie*> (obj);

			float left, top, right, bottom;

			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom) == true)
			{
				e->LoseHP(1);
				targetTypeHit = ZOMBIE;

				if (e->GetState() == ZOMBIE_DESTROYED)
				{
					scoreReceived += e->GetScore();
					sparkEffect.push_back({ left, top });
				}
			}
		}
		else if (dynamic_cast<Boss*>(obj))
		{
			Boss* e = dynamic_cast<Boss*> (obj);

			float left, top, right, bottom;

			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom) == true) // va chạm giữa roi và boss
			{
				e->LoseHP(1);
				targetTypeHit = BOSS;
				//Nguyen
				sparkEffect.push_back({ left, top });
				//
				if (e->GetState() == BOSS_DESTROYED)
				{
					scoreReceived += e->GetScore();
					//Nguyen
					//sparkEffect.push_back({ left, top });
					//
				}
			}
		}
	}
}

void Whip::Render(int currentID)
{
	RenderSpark();
	if (currentID >= 0)
		animation_set->at(state)->RenderByID(currentID, nx, x, y);
	//RenderBoundingBox();
}

void Whip::SetWhipPosition(D3DXVECTOR3 simonPositon, bool isStand)
{	
	simonPositon.x -= 90.0f;
	if (isStand == false)
		simonPositon.y += 15.0f;

	SetPosition(simonPositon.x, simonPositon.y);
}

bool Whip::CheckCollision(float obj_left, float obj_top, float obj_right, float obj_bottom)
{
	float whip_left,
		whip_top,
		whip_right,
		whip_bottom;

	GetBoundingBox(whip_left, whip_top, whip_right, whip_bottom);

	return GameObject::AABB(whip_left, whip_top, whip_right, whip_bottom, obj_left, obj_top, obj_right, obj_bottom);
}

void Whip::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	top = y + 15;
	bottom = top + WHIP_BBOX_HEIGHT;
	if (nx < 0)
	{
		if (state != WHIP_2)
			left = x + 50;
		else left = x + 20;
	}
	else if (nx > 0)
	{
		if (state != WHIP_2)
			left = (240 - 50) - WHIP_BBOX_WIDTH + x;
		else left = (240 - 20) - LONG_CHAIN_BBOX_WIDTH + x;
	}
	
	if (state != WHIP_2)
		right = left + WHIP_BBOX_WIDTH;
	else  right = left + LONG_CHAIN_BBOX_WIDTH;

}

void Whip::Upgrade()
{
	if (state == WHIP) SetState(WHIP_1);
	else if (state == WHIP_1) SetState(WHIP_2);
}

void Whip::RenderSpark()
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


