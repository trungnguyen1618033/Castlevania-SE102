#include "Whip.h"


void Whip::LoadResources()
{
	Textures* texture = Textures::GetInstance();

	texture->Add(ID_TEX_WHIP, FILEPATH_TEX_WHIP, D3DCOLOR_XRGB(255, 255, 255));

	Sprites* sprites = Sprites::GetInstance();
	Animations* animations = Animations::GetInstance();

	LPDIRECT3DTEXTURE9 texWhip = texture->Get(ID_TEX_WHIP);

	sprites->Add(201, 0, 0, 64, 32, texWhip);
	sprites->Add(202, 64, 0, 128, 32 , texWhip);
	sprites->Add(203, 128, 0, 192, 32, texWhip);


	LPANIMATION ani;

	ani = new Animation();
	ani->Add(201, 150);
	ani->Add(202, 150);
	ani->Add(203, 200);
	animations->Add(NORMAL_WHIP, ani);

	AddAnimation(NORMAL_WHIP);
}

void Whip::Update()
{
}

void Whip::Render()
{
	animations[state]->Render(nx, x, y);
}

void Whip::SetWhipPosition(D3DXVECTOR3 simonPositon, bool isStand, int nx)
{	
	if (nx == 1)
		simonPositon.x -= 10.0f;
	else
		simonPositon.x -= 22.0f;
	if (isStand == false)
		simonPositon.y += 4.0f;

	SetPosition(simonPositon.x, simonPositon.y);
}


