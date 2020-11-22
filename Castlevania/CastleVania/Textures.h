#pragma once

#include <unordered_map>
#include <d3d9.h>
#include <d3dx9.h>
#include "Debug.h"
#include "Game.h"

using namespace std;

/*
	Manage texture database
*/
class Textures
{
	static Textures* _instance;
	unordered_map<int, LPDIRECT3DTEXTURE9> textures;

public:
	void Add(int id, LPCSTR filePath, D3DCOLOR transparentColor);
	LPDIRECT3DTEXTURE9 Get(unsigned int i);

	void Clear();
	static Textures* GetInstance();
};

