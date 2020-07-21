#pragma once
#include "d3dx9.h"
#include "Define.h"
#include "Textures.h"
#include "Sprites.h"
#include "PlayScene.h"


class Player
{
protected:
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

	/*bool checkIn = false;*/

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
	void Delete();

	//void SetCheckIn(bool x) { checkIn = x; }
	//bool GetCheckIn() { return checkIn; }

	//void SaveProperties(int s, int l, int h, int e, int w);
};
