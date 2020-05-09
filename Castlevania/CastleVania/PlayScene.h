#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Simon.h"
#include "Torch.h"
#include "Portal.h"
#include "TileMap.h"
#include "Knife.h"
#include "Items.h"
#include"Ground.h"
#include "TileMap.h"

class PlayScene : public Scene
{
protected:
	Simon* player;					// A play scene has to have player, right? 
	Whip* whip;
	Knife* knife;
	//Items* items;
	TileMaps* tilemaps = TileMaps::GetInstance();


	vector<LPGAMEOBJECT> objects;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);

	
public:
	PlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();	

	Simon* GetPlayer() { return player; }
	Knife* GetKnife() { return this->knife; }

	/*void CreateItem(double x, double y);*/

	/*friend class SceneKeyHandler;*/
};

class PlaySceneKeyHandler : public KeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	PlaySceneKeyHandler(Scene* s) :KeyHandler(s) {};
};

