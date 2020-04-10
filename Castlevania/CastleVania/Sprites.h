#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>
#include "Game.h"
#include "Debug.h"

using namespace std;

/*
	A class contains id of a sprite and its position in texture.
*/
class Sprite
{
	int id;			// Sprite ID 
	int left, top, right, bottom;

	LPDIRECT3DTEXTURE9 texture;

public:
	Sprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex);
	void Draw(int nx, float x, float y, int alpha = 255);
};

typedef Sprite* LPSPRITE;

/*
	Sprite Manager Class
*/
class Sprites
{
	static Sprites* _instance;
	unordered_map<int, LPSPRITE> sprites;		// list of Sprite Pointer by Id

public:
	void Add(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex);
	LPSPRITE Get(int id) { return sprites[id]; }
	void Sprites::Clear();

	static Sprites* GetInstance();
};

