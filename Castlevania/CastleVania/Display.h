﻿#pragma once
#include "d3dx9.h"
#include "Define.h"
#include "Textures.h"
#include "Sprites.h"
#include "PlayScene.h"


class Display
{
protected:
	Game* game;
	PlayScene * scene;
	Simon* simon;
	Boss* boss;

	ID3DXFont* font;
	string information;
	
	vector<LPSPRITE> playerHP;
	vector<LPSPRITE> enemyHP;
	vector<LPSPRITE> loseHP;
	
	Sprite* subWeaponBox;
	vector<LPSPRITE> subWeaponList;
	vector<LPSPRITE> itemList;

	LPDIRECT3DTEXTURE9 pause;
	LPDIRECT3DTEXTURE9 over;
	Sprite* choice;


	int score;
	int time;
	int stage;
	int subWeapon = -1;
	int energy;
	int life;
	int simonHP;
	int bossHP;
	int item = -1;

public:
	static Display* _instance;
	
	Display(Game* game);
	~Display();

	void Init();
	void Update(DWORD dt, bool stopwatch);
	void Render();
	void Delete();

	static Display* GetInstance();


};
