#include "TileMap.h"

#define TILE_WIDTH		32
#define TILE_HEIGHT		32

TileMap::TileMap(int ID, LPCWSTR filePath_tex, LPCWSTR filePath_data, int map_width, int map_height)
{
	this->ID = ID;
	this->filePath_tex = filePath_tex;
	this->filePath_data = filePath_data;

	sprites = Sprites::GetInstance();

	this->map_Width = map_width;
	this->map_Height = map_height;

	nums_row = map_Height / TILE_HEIGHT;
	nums_col = map_Width / TILE_WIDTH;

	LoadResources();
	Load_MapData();
}

void TileMap::LoadResources()
{
	Textures* texture = Textures::GetInstance();

	texture->Add(ID, filePath_tex, D3DCOLOR_XRGB(255, 255, 255));

	LPDIRECT3DTEXTURE9 texTileMap = texture->Get(ID);
	
	D3DSURFACE_DESC surfaceDesc;
	int level = 0;
	texTileMap->GetLevelDesc(level, &surfaceDesc);

	int nums_rowToRead = surfaceDesc.Height / TILE_HEIGHT;
	int nums_colToRead = surfaceDesc.Width / TILE_WIDTH;
	
	int id_sprite = 1;
	if(ID == 2 || ID == 1)
		id_sprite = 0;
		

	for (int i = 0; i < nums_rowToRead; i++)
	{
		for (int j = 0; j < nums_colToRead; j++)
		{
			sprites->Add(1000 * ID + id_sprite, TILE_WIDTH * j, TILE_HEIGHT * i, TILE_WIDTH * (j + 1), TILE_HEIGHT * (i + 1), texTileMap);
			id_sprite += 1;
		}
	}
}

void TileMap::Load_MapData()
{
	fstream fs;
	fs.open(filePath_data, ios::in);
	if (fs.fail())
	{
		DebugOut(L"[ERROR] TileMap::Load_MapData failed: ID=%d", ID);
		fs.close();
		return;
	}

	string line;

	while (!fs.eof())
	{
		getline(fs, line);
		//DebugOut(L"%d", line)
		vector<int> numInLine;
		stringstream ss(line);	
		int n;
		while (ss >> n) {
			numInLine.push_back(n);
		}

		map_Data.push_back(numInLine);
	}

	fs.close();
	//DebugOut(L"%d %d\n", map_Data.size(), map_Data[0].size());

}


void TileMap::Draw(D3DXVECTOR3 camPosition)
{
	int start_col_to_draw = (int)camPosition.x / 32;
	int end_col_to_draw = (int)(camPosition.x + SCREEN_WIDTH) / 32;

	for (int i = 0; i < nums_row; i++)
	{
		
		for (int j = start_col_to_draw; j <= end_col_to_draw; j++)
		{
			if (j >= nums_col + 1)
				return;
			float x = TILE_WIDTH * (j - start_col_to_draw) + camPosition.x - (int)camPosition.x % 32;
			float y = TILE_HEIGHT * i + 80;
			sprites->Get(1000 * ID + map_Data[i][j])->Draw(1, -1, x, y);
		}
	}

}

TileMaps* TileMaps::_instance = NULL;

void TileMaps::Add(int ID, LPCWSTR filePath_tex, LPCWSTR filePath_data, int map_width, int map_height)
{
	LPTILEMAP tilemap = new TileMap(ID, filePath_tex, filePath_data, map_width, map_height);
	tilemaps[ID] = tilemap;
}

TileMaps* TileMaps::GetInstance()
{
	if (_instance == NULL) _instance = new TileMaps();
	return _instance;
}