#pragma once
#include "d3dx9.h"
#include "Define.h"
#include "Textures.h"
#include "Sprites.h"
#include "PlayScene.h"


class Player
{
private:
	Game* game;
	PlayScene * scene;
	Simon* simon;


	vector<LPSPRITE> playerHP;
	vector<LPSPRITE> enemyHP;
	vector<LPSPRITE> loseHP;

	ID3DXFont* font;
	string information;
	
	Sprite* subWeaponBox;
	vector<LPSPRITE> subWeaponList;

	int score;
	int time;
	int stage;
	int subWeapon;
	int energy;
	int life;
	int simonHP;
	int bossHP;


public:
	Player(Game* game, PlayScene * scene);
	~Player();

	void Init();
	void Update(DWORD dt);
	void Render();
};
