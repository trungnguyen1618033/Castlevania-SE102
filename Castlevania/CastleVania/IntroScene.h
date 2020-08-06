#pragma once
#include "Game.h"
#include "Define.h"
#include"Scene.h"
#include "Simon.h"
#include "BatIntro.h"
#include "Cloud.h"



class IntroScene : public Scene
{
	Simon* player;

	LPDIRECT3DTEXTURE9 view;
	vector<BatIntro*> bats;
	Cloud* cloud;

	void _ParseSection_OBJECTS(string line);

public:
	IntroScene(int id, LPCWSTR filePath);
	~IntroScene();

	virtual void Load();	
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	Simon* GetPlayer() { return player; }

};

class IntroSceneKeyHandler : public KeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	IntroSceneKeyHandler(Scene* s) :KeyHandler(s) {};
};
