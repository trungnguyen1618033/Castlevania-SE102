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
#include "WhipItem.h"
#include "Heart.h"
#include "MoneyBag.h"

class ManageScene: public Scene
{
protected:
	Simon* player;					// A play scene has to have player, right? 
	

	vector<LPGAMEOBJECT> objects;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
public:
	ManageScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();	

	friend class SceneKeyHandler;
};

class SceneKeyHandler : public KeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	SceneKeyHandler(Scene* s) :KeyHandler(s) {};
};

