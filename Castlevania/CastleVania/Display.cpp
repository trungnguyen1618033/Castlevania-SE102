#include "Display.h"
#define FILEPATH_FONT				L"font\\prstart.ttf"
#define DEFAULT_TIME_PLAY			300
#define ID_TEX_HP					97
#define ID_TEX_RECT					98
#define FILEPATH_TEX_HP				L"textures\\HP.png"
#define FILEPATH_TEX_RECT			L"textures\\Rect.png"


Display::Display(Game* game, PlayScene* scene)
{
	this->game = game;
	this->scene = scene;
	this->simon = scene->GetPlayer();
	this->boss = scene->GetBoss();

	if (boss == NULL)
		bossHP = 16;

	time = 0;
}


Display::~Display()
{
}

void Display::Init()
{
	// textures HP 
	Textures* textures = Textures::GetInstance();
	textures->Add(ID_TEX_HP, FILEPATH_TEX_HP, D3DCOLOR_XRGB(255, 255, 255));
	textures->Add(ID_TEX_RECT, FILEPATH_TEX_RECT, D3DCOLOR_XRGB(255, 255, 255));

	LPDIRECT3DTEXTURE9 texHP = textures->Get(ID_TEX_HP);
	LPDIRECT3DTEXTURE9 texRect = textures->Get(ID_TEX_RECT);

	for (int i = 0; i < 16; i++)
	{
		Sprite* player = new Sprite(10000, 0, 0, 8, 15, texHP);
		playerHP.push_back(player);

		Sprite* lose = new Sprite(10001, 8, 0, 16, 15, texHP);
		loseHP.push_back(lose);

		Sprite* enemy = new Sprite(10002, 16, 0, 24, 15, texHP);
		enemyHP.push_back(enemy);
	}

	// sprite weapon
	subWeaponBox = new Sprite(110, 0, 0, 95, 40, texRect);

	Sprites* sprites = Sprites::GetInstance();
	subWeaponList.push_back(sprites->Get(301)); 
	subWeaponList.push_back(sprites->Get(801));
	subWeaponList.push_back(sprites->Get(901));
	subWeaponList.push_back(sprites->Get(1401));
	subWeaponList.push_back(sprites->Get(407));

	// Khởi tạo list item (double shot, triple shot)
	itemList.push_back(sprites->Get(408));
	itemList.push_back(sprites->Get(409));

	// Font
	font = NULL;
	AddFontResourceEx(FILEPATH_FONT, FR_PRIVATE, NULL);

	HRESULT hr = D3DXCreateFont(
		game->GetDirect3DDevice(), 16, 0, FW_NORMAL, 1, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, FF_DONTCARE, L"Press Start", &font);

	if (hr != DI_OK)
	{
		DebugOut(L"[ERROR] Create font failed\n");
		return;
	}

	information = "SCORE-000000 TIME 0000 SCENE 00\n";
	information += "PLAYER                  -00\n";
	information += "ENEMY                   -00\n";

}

void Display::Update(DWORD dt, bool stopwatch)
{
	score = simon->GetScore();
	energy = simon->GetEnergy();
	life = simon->GetLife();
	simonHP = simon->GetHP();
	subWeapon = simon->GetSubWeapon();

	if(boss != NULL)
		bossHP = boss->GetHP();

	switch (scene->GetId())
	{
	case 0:
		this->stage = 1;
		break;
	case 1:
	case 2:
		this->stage = 2;
		break;
	case 3:
	case 4:
		this->stage = 3;
		break;
	case 5:
		this->stage = 4;
		break;
	default:
		break;
	}

	if (scene->IsDoubleShot()) 
		item = 0;		// double shot
	else if (scene->IsTripleShot()) 
		item = 1;		// trip shot
	else 
		item = -1;

	if (stopwatch == false && scene->isGamePause == false) // khi sử dụng stop watch thì không đếm thời gian
		time += dt;
	
	int remainTime = DEFAULT_TIME_PLAY - time / 1000;
	if (remainTime <= 0)
	{
		remainTime = 0;

		if (simon->isTouchGround == true && simon->GetState() != DEAD)
			simon->SetState(DEAD);
	}


	// Chuẩn hoá chuỗi
	string score_str = to_string(score);
	while (score_str.length() < 6) score_str = "0" + score_str;

	string time_str = to_string(remainTime);
	while (time_str.length() < 4) time_str = "0" + time_str;

	string scene_str = to_string(stage);
	while (scene_str.length() < 2) scene_str = "0" + scene_str;

	string energy_str = to_string(energy);
	while (energy_str.length() < 2) energy_str = "0" + energy_str;

	string life_str = to_string(life);
	while (life_str.length() < 2) life_str = "0" + life_str;

	// display
	information = "SCORE-" + score_str + " TIME " + time_str + " SCENE " + scene_str + "\n";
	information += "PLAYER                  -" + energy_str + "\n";
	information += "ENEMY                   -" + life_str + "\n";
}



void Display::Render()
{
	if (game->GetChangeScene() == true)
		return;

	if (scene->isGamePause == true)
	{
		pause = Textures::GetInstance()->Get(90);
		game->Draw(0, 0, 240, 210, pause, 0, 0, 34, 52);
	}

	RECT rect;
	SetRect(&rect, 0, 15, SCREEN_WIDTH, 80);

	if (font != NULL)
	{
		font->DrawTextA(NULL, information.c_str(), -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
	}

	// draw subWeaponBox
	subWeaponBox->Draw(0, -1, 288, 32);

	if (subWeapon != -1) // simon get subweapon
	{
		
		subWeaponList[subWeapon]->Draw(0, -1, 305, 38);
	}

	if (item != -1)
	{
		itemList[item]->Draw(0, -1, 450, 38);
	}

	for (int i = 0; i < simonHP; i++)
	{
		playerHP[i]->Draw(0, -1, 105 + i * 9, 31);
		enemyHP[i]->Draw(0, -1, 106 + i * 9, 47);
	}

	for (int i = simonHP; i < 16; i++)
	{
		loseHP[i]->Draw(0, -1, 105 + i * 9, 31);
	}

	for (int i = 0; i < 16; i++)
	{
		enemyHP[i]->Draw(0, -1, 106 + i * 9, 47);
	}

	for (int i = bossHP; i < 16; i++)
		loseHP[i]->Draw(0, -1, 106 + i * 9, 47);

}

void Display::Delete()
{
	subWeaponList.clear();
	playerHP.clear();
	enemyHP.clear();
	loseHP.clear();
}




