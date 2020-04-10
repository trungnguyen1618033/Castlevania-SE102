#include "Simon.h"

Simon::Simon() : GameObject()
{
	untouchable = 0;
	SetState(IDLE);

	
}

//void Simon::LoadResources()
//{
//	Textures* texture = Textures::GetInstance();
//
//	texture->Add(ID_TEX_SIMON, FILEPATH_TEX_SIMON, D3DCOLOR_XRGB(255, 255, 255));
//
//	Sprites* sprites = Sprites::GetInstance();
//	Animations* animations = Animations::GetInstance();
//
//	LPDIRECT3DTEXTURE9 texSimon = texture->Get(ID_TEX_SIMON);
//
//	sprites->Add(1, 0, 0, 32, 32, texSimon); // idle
//
//	sprites->Add(2, 32, 0, 64, 32, texSimon); // walk
//	sprites->Add(3, 64, 0, 96, 32, texSimon);
//	sprites->Add(4, 96, 0, 128, 32, texSimon);
//
//	sprites->Add(5, 128, 0, 160, 32, texSimon); // duck and jump
//
//	sprites->Add(6, 0, 32, 32, 64, texSimon); // ascend stairs
//	sprites->Add(7, 32, 32, 64, 64, texSimon);
//
//	sprites->Add(8, 64, 32, 96, 64, texSimon); // descend stairs
//	sprites->Add(9, 96, 32, 128, 64, texSimon);
//
//	sprites->Add(10, 128, 32, 160, 64, texSimon); // hurt
//
//	sprites->Add(11, 0, 64, 32, 96, texSimon);	// death
//	sprites->Add(12, 32, 64, 64, 96, texSimon);
//
//	sprites->Add(13, 64, 64, 96, 96, texSimon);
//
//	sprites->Add(14, 96, 64, 128, 96, texSimon); // standing and jumping
//	sprites->Add(15, 128, 64, 160, 96, texSimon);
//	sprites->Add(16, 0, 96, 32, 128, texSimon);
//
//	sprites->Add(17, 32, 96, 64, 128, texSimon); // ducking
//	sprites->Add(18, 64, 96, 96, 128, texSimon);
//	sprites->Add(19, 96, 96, 128, 128, texSimon);
//
//	sprites->Add(20, 128, 96, 160, 128, texSimon); // ascending
//	sprites->Add(21, 0, 128, 32, 160, texSimon);
//	sprites->Add(22, 32, 128, 64, 160, texSimon);
//
//	sprites->Add(23, 64, 128, 96, 160, texSimon); // descending
//	sprites->Add(24, 96, 128, 128, 160, texSimon);
//	sprites->Add(25, 128, 128, 160, 160, texSimon);
//
//
//
//	LPANIMATION ani;
//
//	ani = new Animation();
//	ani->Add(1);
//	animations->Add(IDLE, ani);
//
//	ani = new Animation();
//	ani->Add(2);
//	ani->Add(3);
//	ani->Add(4);
//	animations->Add(WALK, ani);
//
//	ani = new Animation();
//	ani->Add(5);
//	animations->Add(DUCK, ani);
//
//	ani = new Animation();
//	ani->Add(5);
//	animations->Add(JUMP, ani);
//
//	ani = new Animation();
//	ani->Add(6, 150);
//	ani->Add(7, 150);
//	animations->Add(ASCEND, ani);
//
//	ani = new Animation();	
//	ani->Add(8, 150);
//	ani->Add(9, 150);
//	animations->Add(DESCEND, ani);
//
//	ani = new Animation();
//	ani->Add(10, 100);
//	ani->Add(10, 100);
//	ani->Add(10, 100);
//	animations->Add(HURT, ani);
//
//	ani = new Animation();
//	ani->Add(11, 150);
//	ani->Add(12, 150);
//	animations->Add(DEATH, ani);
//
//	ani = new Animation();
//	ani->Add(14, 150);
//	ani->Add(15, 150);
//	ani->Add(16, 200);
//	animations->Add(STANDING, ani);
//
//	ani = new Animation();
//	ani->Add(17, 150);
//	ani->Add(18, 150);
//	ani->Add(19, 200);
//	animations->Add(DUCKING, ani);
//
//	ani = new Animation();
//	ani->Add(20, 150);
//	ani->Add(21, 150);
//	ani->Add(22, 200);
//	animations->Add(ASCENDING, ani);
//
//	ani = new Animation();
//	ani->Add(23, 150);
//	ani->Add(24, 150);
//	ani->Add(25, 200);
//	animations->Add(DESCENDING, ani);
//
//	AddAnimation(IDLE);
//	AddAnimation(WALK);
//	AddAnimation(DUCK);
//	AddAnimation(JUMP);
//	AddAnimation(ASCEND);
//	AddAnimation(DESCEND);
//	AddAnimation(HURT);
//	AddAnimation(DEATH);
//	AddAnimation(STANDING);
//	AddAnimation(DUCKING);
//	AddAnimation(ASCENDING);
//	AddAnimation(DESCENDING);
//
//	SetPosition(10.0f, 150.0f);
//	whip = new Whip();
//	whip->LoadResources();
//}

void Simon::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	GameObject::Update(dt);

	vy += SIMON_GRAVITY;
	if (y > 128)
	{
		vy = 0;
		y = 128.0f;
	}
}

void Simon::Render()
{

	if (state == STANDING || state == DUCKING) {		// lấy vị trí của simon để thực hiện gắn roi
		D3DXVECTOR3 simonPositon;
		GetPosition(simonPositon.x, simonPositon.y);

		whip->SetOrientation(nx);
		whip->SetWhipPosition(simonPositon, isStand, nx);
		whip->Render();
	}

	int ani = GetState();
	animation_set->at(ani)->Render(nx, x, y);
	DebugOut(L"[INFO] State: %d\n", ani);
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
		if (y == 128)
			vy = -SIMON_JUMP_SPEED_Y;
		break;
	case ASCEND:
		isStand = true;
		vy = -SIMON_JUMP_SPEED_Y;
		break;
	case DESCEND:
		isStand = true;
		vy = SIMON_JUMP_SPEED_Y;
		break;
	case HURT:
		isStand = true;
		/*animations[HURT]->Reset();*/
		animation_set->at(HURT)->Reset();
		break;
	case DEATH:
		isStand = true;
		/*animations[DEATH]->Reset();*/
		animation_set->at(DEATH)->Reset();
		break;
	case STANDING:
		isStand = true;
		/*animations[STANDING]->Reset();*/
		animation_set->at(STANDING)->Reset();
		break;
	case DUCKING:
		isStand = false;
		/*animations[DUCKING]->Reset();*/
		animation_set->at(DUCKING)->Reset();
		break;
	case ASCENDING:
		isStand = false;
		/*animations[ASCENDING]->Reset();*/
		animation_set->at(ASCENDING)->Reset();
	case DESCENDING:
		isStand = false;
		/*animations[DESCENDING]->Reset();*/
		animation_set->at(DESCENDING)->Reset();
	default:
		break;
	}
}

void Simon::SetWhipAnimationSet(LPANIMATION_SET ani_set)
{
	whip = new Whip();
	whip->SetAnimationSet(ani_set);
}

bool Simon::IsStand()
{
	return this->y == 128.0f;
}

void Simon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + SIMON_BBOX_WIDTH;
	bottom = y + SIMON_BBOX_HEIGHT;
}





