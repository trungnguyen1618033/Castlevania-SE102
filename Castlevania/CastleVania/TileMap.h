#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "Textures.h"
#include "Sprites.h"
#include "Debug.h"

class TileMap
{
	int map_Width;			
	int map_Height;			

	int nums_row;			
	int nums_col;		

	LPCWSTR filePath_tex;
	LPCWSTR filePath_data;
	int ID;					

public:
	vector<vector<int>> map_Data;

	Sprites* sprites;		

	TileMap(int ID, LPCWSTR filePath_tex, LPCWSTR filePath_data, int map_width, int map_height);

	void LoadResources();	
	void Load_MapData();	
	void Draw(D3DXVECTOR3 camPosition, bool isCrossEffect = false);

	int GetMapWidth() { return map_Width; }

};

typedef TileMap* LPTILEMAP;

class TileMaps
{
	static TileMaps* _instance;
	unordered_map<int, LPTILEMAP> tilemaps;

public:
	void Add(int ID, LPCWSTR filePath_tex, LPCWSTR filePath_data, int map_width, int map_height);
	LPTILEMAP Get(int ID) { return tilemaps[ID]; }

	static TileMaps* GetInstance();
};

