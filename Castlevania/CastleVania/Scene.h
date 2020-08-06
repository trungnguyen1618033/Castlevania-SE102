#pragma once

#include <d3dx9.h>
#include "KeyEventHandler.h"
#include <Windows.h>
#include <unordered_map>
#include <stdlib.h>
using namespace std;

class Scene
{
protected:
	KeyEventHandler* key_handler;
	int id;
	LPCWSTR sceneFilePath;
	

public:
	
	Scene(int id, LPCWSTR filePath);

	KeyEventHandler* GetKeyEventHandler() { return key_handler; }
	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0;
	int GetId() { return id; }

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	
};
typedef Scene* LPSCENE;

class KeyHandler : public KeyEventHandler
{
protected:
	Scene* scene;

public:
	virtual void KeyState(BYTE* states) = 0;
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;
	KeyHandler(Scene* s) :KeyEventHandler() { scene = s; }
};

