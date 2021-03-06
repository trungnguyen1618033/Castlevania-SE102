﻿	#include "Game.h"
#include <iostream>
#include <fstream>
#include "debug.h"
#include "PlayScene.h"
#include "IntroScene.h"
#include "TitleScene.h"

Game* Game::_instance = NULL;
GSound* Game::gameSound = NULL;


#define MAX_GAME_LINE 1024


#define GAME_FILE_SECTION_UNKNOWN -1
#define GAME_FILE_SECTION_SETTINGS 1
#define GAME_FILE_SECTION_SCENES 2


/*
Initialize DirectX, create a Direct3D device for rendering within the window, initial Sprite library for
rendering 2D images
- hInst: Application instance handle
- hWnd: Application window handle
*/
void Game::Init(HWND hWnd)
{
	LPDIRECT3D9 d3d = Direct3DCreate9(D3D_SDK_VERSION);

	this->hWnd = hWnd;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	RECT rect;
	GetClientRect(hWnd, &rect);

	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferHeight = rect.bottom + 1;
	d3dpp.BackBufferWidth = rect.right + 1;

	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddv);

	if (d3ddv == NULL)
	{
		DebugOut("[ERROR] CreateDevice failed\n");
		return;
	}

	d3ddv->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);

	// Initialize sprite helper from Direct3DX helper library
	D3DXCreateSprite(d3ddv, &spriteHandler);

	// Nguyen
	// Init sound
	Game::gameSound = new GSound();
	Game::gameSound->LoadSound(hWnd);

	DebugOut("[INFO] Init Game done\n");
}

void Game::InitKeyboard()
{
	HRESULT hr = DirectInput8Create(
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8, (VOID**)&di, NULL
	);

	if (hr != DI_OK)
	{
		DebugOut("[ERROR] DirectInput8Create failed\n");
		return;
	}

	hr = di->CreateDevice(GUID_SysKeyboard, &didv, NULL);

	if (hr != DI_OK)
	{
		DebugOut("[ERROR] CreateDevice failed\n");
		return;
	}

	// Set the data format to "keyboard format" - a predefined data format

	hr = didv->SetDataFormat(&c_dfDIKeyboard);

	hr = didv->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	// IMPORTANT STEP TO USE BUFFERED DEVICE DATA!
	//
	// DirectInput uses unbuffered I/O (buffer size = 0) by default.
	// If you want to read buffered data, you need to set a nonzero
	// buffer size.
	//
	// Set the buffer size to DINPUT_BUFFERSIZE (defined above) elements.
	//
	// The buffer size is a DWORD property associated with the device.

	DIPROPDWORD dipdw;

	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = KEYBOARD_BUFFER_SIZE; // Arbitary buffer size

	hr = didv->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);

	hr = didv->Acquire();
	if (hr != DI_OK)
	{
		DebugOut("[ERROR] DINPUT8::Acquire failed\n");
		return;
	}


	DebugOut("[INFO] Keyboard has been initialized successfully\n");
}



void Game::Draw(int accordingCam, int nx, float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha)
{

	// calculate position of object in real world
	D3DXVECTOR3 p(x - cameraPosition.x * accordingCam, y - cameraPosition.y * accordingCam, 0);

	RECT rect;
	rect.left = left;
	rect.top = top;
	rect.right = right;
	rect.bottom = bottom;

	// flip sprite, using nx parameter

	D3DXMATRIX oldTransform;
	D3DXMATRIX newTransform;

	spriteHandler->GetTransform(&oldTransform);

	D3DXVECTOR2 center = D3DXVECTOR2(p.x + (right - left) / 2, p.y + (bottom - top) / 2);
	D3DXVECTOR2 rotate = D3DXVECTOR2(nx > 0 ? -1 : 1, 1);

	// Xây dựng một ma trận 2D lưu thông tin biến đổi (scale, rotate)
	D3DXMatrixTransformation2D(&newTransform, &center, 0.0f, &rotate, NULL, 0.0f, NULL);

	// Cần nhân với ma trận cũ để tính ma trận biến đổi cuối cùng
	D3DXMATRIX finalTransform = newTransform * oldTransform;
	spriteHandler->SetTransform(&finalTransform);

	spriteHandler->Draw(texture, &rect, NULL, &p, D3DCOLOR_ARGB(alpha, 255, 255, 255));

	spriteHandler->SetTransform(&oldTransform);
}

int Game::IsKeyDown(int KeyCode)
{
	return (keyStates[KeyCode] & 0x80) > 0;
}

int Game::IsKeyPress(int KeyCode)
{
	return IsKeyDown(KeyCode) && !(keyEvents[KeyCode].dwData);
}

int Game::IsKeyRelease(int KeyCode)
{
	return !IsKeyDown(KeyCode) && (keyEvents[KeyCode].dwData);
}

void Game::ProcessKeyboard()
{
	HRESULT hr;

	// First, collect all key states
	hr = didv->GetDeviceState(sizeof(keyStates), keyStates);

	if (FAILED(hr))
	{
		// If the keyboard lost focus or was not acquired then try to get control back 
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			HRESULT h = didv->Acquire();
			if (h == DI_OK)
			{
				DebugOut("[INFO] Keyboard re-acquired\n");
			}
			else return;
		}
		else
		{
			//DebugOut("[ERROR] DINPUT::GetDeviceState failed\n");
			return;
		}
	}

	keyHandler->KeyState((BYTE*)&keyStates);

	// Collect all buffered events
	DWORD dwElements = KEYBOARD_BUFFER_SIZE;
	hr = didv->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), keyEvents, &dwElements, 0);

	if (FAILED(hr))
	{
		//DebugOut("[ERROR] DINPUT::GetDeviceState failed\n");
		return;
	}

	// Scan through all buffered events, check if the key is pressed or released
	for (DWORD i = 0; i < dwElements; i++)
	{
		int keyCode = keyEvents[i].dwOfs;
		int keyState = keyEvents[i].dwData;

		if ((keyState & 0x80) > 0)
			keyHandler->OnKeyDown(keyCode);
		else
			keyHandler->OnKeyUp(keyCode);
	}
}

Game* Game::GetInstance()
{
	if (_instance == NULL) _instance = new Game();
	return _instance;
}

void Game::SetCamPos(float x, float y)
{
	this->cameraPosition.x = x;
	this->cameraPosition.y = y;
}

void Game::SweptAABB(
	float ml, float mt, float mr, float mb,
	float dx, float dy,
	float sl, float st,
	float sr, float sb,
	float& t, float& nx, float& ny)
{
	float dx_entry, dx_exit, tx_entry, tx_exit;
	float dy_entry, dy_exit, ty_entry, ty_exit;

	float t_entry;
	float t_exit;

	t = -1.0f;		// no collision
	nx = ny = 0;


	//	Broad-phase test

	float bl = dx > 0 ? ml : ml + dx;
	float bt = dy > 0 ? mt : mt + dy;
	float br = dx > 0 ? mr + dx : mr;
	float bb = dy > 0 ? mb + dy : mb;

	if (br < sl || bl > sr || bt > sb || bb < st)
		return;

	// moving object is not moving
	if (dx == 0 && dy == 0)
		return;

	if (dx > 0)
	{
		dx_entry = sl - mr;
		dx_exit = sr - ml;
	}
	else if (dx < 0)
	{
		dx_entry = sr - ml;
		dx_exit = sl - mr;
	}

	if (dy > 0)
	{
		dy_entry = st - mb;
		dy_exit = sb - mt;
	}
	else if (dy < 0)
	{
		dy_entry = sb - mt;
		dy_exit = st - mb;
	}

	if (dx == 0)
	{
		tx_entry = -99999999999;
		tx_exit = 99999999999;
	}
	else
	{
		tx_entry = dx_entry / dx;
		tx_exit = dx_exit / dx;
	}

	if (dy == 0)
	{
		ty_entry = -99999999999;
		ty_exit = 99999999999;
	}
	else {
		ty_entry = dy_entry / dy;
		ty_exit = dy_exit / dy;
	}


	if ((tx_entry < 0.0f && ty_entry < 0.0f) || tx_entry > 1.0f || ty_entry > 1.0f) return;

	t_entry = max(tx_entry, ty_entry);
	t_exit = min(tx_exit, ty_exit);

	if (t_entry > t_exit) return;

	t = t_entry;

	if (tx_entry > ty_entry)
	{
		ny = 0.0f;
		dx > 0 ? nx = -1.0f : nx = 1.0f;
	}
	else
	{
		nx = 0.0f;
		dy > 0 ? ny = -1.0f : ny = 1.0f;
	}
}

Game::~Game()
{
	if (spriteHandler != NULL) spriteHandler->Release();
	if (backBuffer != NULL) backBuffer->Release();
	if (d3ddv != NULL) d3ddv->Release();
	if (d3d != NULL) d3d->Release();
	// Nguyen
	Game::gameSound->shutdownDirectSound();
}


void Game::Load(LPCWSTR gameFile)
{

	DebugOut("[INFO] Start loading game file : %s\n", gameFile);

	ifstream f;
	f.open(gameFile);
	char str[MAX_GAME_LINE];

	// current resource section flag
	int section = GAME_FILE_SECTION_UNKNOWN;

	while (f.getline(str, MAX_GAME_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SETTINGS]") { section = GAME_FILE_SECTION_SETTINGS; continue; }
		if (line == "[SCENES]") { section = GAME_FILE_SECTION_SCENES; continue; }

		//
		// data section
		//
		switch (section)
		{
		case GAME_FILE_SECTION_SETTINGS: _ParseSection_SETTINGS(line); break;
		case GAME_FILE_SECTION_SCENES: _ParseSection_SCENES(line); break;
		}
	}
	f.close();

	DebugOut("[INFO] Loading game file : %s has been loaded successfully\n", gameFile);

	SwitchScene(current_scene);
}

void Game::SwitchScene(int scene_id)
{
	
	Game* game = Game::GetInstance();
	int old_scene = current_scene;
	current_scene = scene_id;

	LPSCENE s = scenes[current_scene];

	//Lấy các thuộc tính của simon ở màn trước
	if (current_scene >= 0)
	{
		PlayScene* scene = (PlayScene*)scenes[current_scene];
		if (scene->IsDead())
		{
			Simon* simon = scene->GetPlayer();
			game->score = simon->GetScore();
			game->life = simon->GetLife();
			game->hp = 16;
			game->energy = simon->GetEnergy();
			game->subWeapon = simon->GetSubWeapon();

			Whip* whip = scene->GetWhip();
			game->stateWhip = whip->GetState();

			scene->SetDead(false);
		}
		else if (current_scene == 0)
		{
			game->score = 0;
			game->life = 3;
			game->hp = 16;
			game->energy = 99;
			game->subWeapon = -1;
			game->stateWhip = 0;
		}
		else
		{
			PlayScene* scene = (PlayScene*)scenes[current_scene - 1];
			Simon* simon = scene->GetPlayer();

			if (simon == NULL)
			{
				game->score = 0;
				game->life = 3;
				game->hp = 16;
				game->energy = 99;
				game->subWeapon = -1;
				game->stateWhip = 0;
			}
			else
			{
				game->score = simon->GetScore();
				game->life = simon->GetLife();
				game->hp = simon->GetHP();
				game->energy = simon->GetEnergy();
				game->subWeapon = simon->GetSubWeapon();
		
				Whip* whip = scene->GetWhip();
				game->stateWhip = whip->GetState();
			}
		}
		
	}

	// IMPORTANT: has to implement "unload" previous scene assets to avoid duplicate resources
	switch (old_scene)
	{
	case -2:
		Game::gameSound->stopSound(LVL1_VAMKILL);
		break;
	case -1:
		Game::gameSound->stopSound(LVL2_STALKER);
		break;
	case 0:
		Game::gameSound->stopSound(LVL1_VAMKILL);
		break;
	case 1:
		Game::gameSound->stopSound(LVL2_STALKER);
		break;
	case 2:
		Game::gameSound->stopSound(LVL2_STALKER);
		break;
	case 3:
		Game::gameSound->stopSound(LVL1_VAMKILL);
		break;
	case 4:
		Game::gameSound->stopSound(LVL1_VAMKILL);
		break;
	case 5:
		Game::gameSound->stopSound(BOSS_FIGHT);
		Game::gameSound->stopSound(LVL1_VAMKILL);
		break;
	}
	
	s->Unload();

	Textures::GetInstance()->Clear();
	Sprites::GetInstance()->Clear();
	Animations::GetInstance()->Clear();

	game->SetKeyHandler(s->GetKeyEventHandler());
	s->Load();

	switch (current_scene)
	{
	case -2:
		Game::gameSound->playSoundLoop(LVL1_VAMKILL);
		break;
	case -1:
		Game::gameSound->playSoundLoop(LVL2_STALKER);
		break;
	case 0:
		Game::gameSound->playSoundLoop(LVL1_VAMKILL);
		break;
	case 1:
		Game::gameSound->playSoundLoop(LVL2_STALKER);
		break;
	case 2:
		Game::gameSound->playSoundLoop(LVL2_STALKER);
		break;
	case 3:
		Game::gameSound->playSoundLoop(LVL1_VAMKILL);
		break;
	case 4:
		Game::gameSound->playSoundLoop(LVL1_VAMKILL);
		break;
	case 5:	
		Game::gameSound->playSoundLoop(LVL1_VAMKILL);
		break;
	}

	//Cập nhập các thuộc tính của simon từ màn trước đến màn tiếp theo
	if (current_scene >= 0)
	{
		PlayScene* scene = (PlayScene*)scenes[current_scene];
		Simon* simon = scene->GetPlayer();
		simon->SetScore(game->score);
		simon->SetLife(game->life);
		simon->SetHP(game->hp);
		simon->SetEnergy(game->energy);
		simon->SetSubWeapon(game->subWeapon);

		Whip* whip = scene->GetWhip();
		whip->SetState(game->stateWhip);
	}

	SetChangeScene(true);

}

void Game::BackScene(int scene_id)
{
	Game* game = Game::GetInstance();
	int old_scene = current_scene;
	current_scene = scene_id;
	game->backScene = true;

	LPSCENE s = scenes[current_scene];

	//Lấy các thuộc tính của simon ở màn hiện tại
	PlayScene* scene = (PlayScene*)scenes[old_scene];
	Simon* simon = scene->GetPlayer();

	game->score = simon->GetScore();
	game->life = simon->GetLife();
	game->hp = simon->GetHP();
	game->energy = simon->GetEnergy();
	game->subWeapon = simon->GetSubWeapon();

	Whip* whip = scene->GetWhip();
	game->stateWhip = whip->GetState();

	// IMPORTANT: has to implement "unload" previous scene assets to avoid duplicate resources
	switch (old_scene)
	{
	case 0:
		Game::gameSound->stopSound(LVL1_VAMKILL);
		break;
	case 1:
		Game::gameSound->stopSound(LVL2_STALKER);
		break;
	case 2:
		Game::gameSound->stopSound(LVL2_STALKER);
		break;
	case 3:
		Game::gameSound->stopSound(LVL1_VAMKILL);
		break;
	case 4:
		Game::gameSound->stopSound(LVL1_VAMKILL);
		break;
	case 5:
		Game::gameSound->stopSound(BOSS_FIGHT);
		Game::gameSound->stopSound(LVL1_VAMKILL);
		break;
	}

	s->Unload();

	Textures::GetInstance()->Clear();
	Sprites::GetInstance()->Clear();
	Animations::GetInstance()->Clear();

	game->SetKeyHandler(s->GetKeyEventHandler());
	s->Load();

	switch (current_scene)
	{
	case 0:
		Game::gameSound->playSoundLoop(LVL1_VAMKILL);
		break;
	case 1:
		Game::gameSound->playSoundLoop(LVL2_STALKER);
		break;
	case 2:
		Game::gameSound->playSoundLoop(LVL2_STALKER);
		break;
	case 3:
		Game::gameSound->playSoundLoop(LVL1_VAMKILL);
		break;
	case 4:
		Game::gameSound->playSoundLoop(LVL1_VAMKILL);
		break;
	case 5:
		Game::gameSound->playSoundLoop(LVL1_VAMKILL);
		break;
	}

	//Cập nhập các thuộc tính của simon từ màn trước đến màn tiếp theo
	PlayScene* temp = (PlayScene*)s;
	simon = temp->GetPlayer();
	simon->SetScore(game->score);
	simon->SetLife(game->life);
	simon->SetHP(game->hp);
	simon->SetEnergy(game->energy);
	simon->SetSubWeapon(game->subWeapon);

	whip = temp->GetWhip();
	whip->SetState(game->stateWhip);

	SetChangeScene(true);
	if (Game::GetInstance()->backScene == true)
		Game::GetInstance()->backScene = false;
}

void Game::_ParseSection_SETTINGS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;
	if (tokens[0] == "start")
		current_scene = atoi(tokens[1].c_str());
	else
		DebugOut("[ERROR] Unknown game setting %s\n", ToWSTR(tokens[0]).c_str());
}

void Game::_ParseSection_SCENES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;
	int id = atoi(tokens[0].c_str());
	LPCWSTR path = ToLPCWSTR(tokens[1]);
	LPSCENE scene;
	switch (id)
	{
	case -2:
		scene = new TitleScene(id, path);
		break;
	case -1:
		scene = new IntroScene(id, path);
		break;
	default:
		scene = new PlayScene(id, path);
		break;
	}
	scenes[id] = scene;
	
}