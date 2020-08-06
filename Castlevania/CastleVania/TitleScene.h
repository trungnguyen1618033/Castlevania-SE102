#pragma once
#include "Game.h"
#include "Scene.h"
#include "Textures.h"
#include "Sprites.h"
#include "Animations.h"
#include "Simon.h"

class TitleScene : public Scene
{
	LPDIRECT3DTEXTURE9 view;
	Animation* bird;

public:
	TitleScene(int id, LPCWSTR filePath);
	void SwitchScene();

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
};

class TitleSceneKeyHandler : public KeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	TitleSceneKeyHandler(Scene* s) :KeyHandler(s) {};
};

