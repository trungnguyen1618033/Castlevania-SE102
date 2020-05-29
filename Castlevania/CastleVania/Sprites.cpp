#include "Sprites.h"


Sprite::Sprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->texture = tex;
}

void Sprite::Draw(int accordingCam, int nx, float x, float y, int alpha)
{
	Game* game = Game::GetInstance();
	game->Draw(accordingCam, nx, x, y, texture, left, top, right, bottom, alpha);
}


Sprites* Sprites::_instance = NULL;

void Sprites::Add(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	LPSPRITE sprite = new Sprite(id, left, top, right, bottom, tex);
	sprites[id] = sprite;
}

Sprites* Sprites::GetInstance()
{
	if (_instance == NULL) _instance = new Sprites();
	return _instance;
}


void Sprites::Clear()
{
	for (auto x : sprites)
	{
		LPSPRITE s = x.second;
		delete s;
	}

	sprites.clear();
}

