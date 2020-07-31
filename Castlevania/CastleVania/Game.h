#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include "Define.h"
#include "Debug.h"
#include "KeyEventHandler.h"
#include <unordered_map>
#include "Scene.h"

class Game
{
	static Game * _instance;
	HWND hWnd;									// Window handle

	LPDIRECT3D9 d3d = NULL;						// Direct3D handle
	LPDIRECT3DDEVICE9 d3ddv = NULL;				// Direct3D device object

	LPDIRECT3DSURFACE9 backBuffer = NULL;		
	LPD3DXSPRITE spriteHandler = NULL;			// Sprite helper library to help us draw 2D image on the screen 

	LPDIRECTINPUT8       di;		// The DirectInput object         
	LPDIRECTINPUTDEVICE8 didv;		// The keyboard device 

	BYTE  keyStates[256];			// DirectInput keyboard state buffer 
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];		// Buffered keyboard data

	LPKEYEVENTHANDLER keyHandler;

	D3DXVECTOR3 cameraPosition;			// camera position for viewing

	unordered_map<int, LPSCENE> scenes;
	int current_scene;

	int screen_width;
	int screen_height;

	bool changeScene = false;

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_SCENES(string line);

public:
	int score;
	int life;
	int hp;
	int energy;
	int subWeapon;
	int stateWhip;


	void InitKeyboard();
	void SetKeyHandler(LPKEYEVENTHANDLER handler) { keyHandler = handler; }
	void Init(HWND hWnd);
	void Draw(int accordingCam, int nx, float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha = 255);

	int IsKeyDown(int KeyCode);
	int IsKeyPress(int KeyCode);
	int IsKeyRelease(int KeyCode);
	void ProcessKeyboard();

	void Load(LPCWSTR gameFile);
	LPSCENE GetCurrentScene() { return scenes[current_scene]; }
	void SwitchScene(int scene_id);

	bool GetChangeScene() { return changeScene; }
	void SetChangeScene(bool x) { this->changeScene = x; }

	static void SweptAABB(
		float ml,			// move left góc của đối tượng di chuyển
		float mt,			// move top
		float mr,			// move right 
		float mb,			// move bottom
		float dx,			// 
		float dy,			// 
		float sl,			// static left đối tượng đứng im
		float st, 
		float sr, 
		float sb,
		float &t,			//có va chạm hay ko 
		float &nx,			//
		float &ny);

	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }
	D3DXVECTOR3 GetCameraPositon() { return this->cameraPosition; }

	int GetScreenWidth() { return screen_width; }
	int GetScreenHeight() { return screen_height; }

	void SetCamPos(float x, float y);

	static Game * GetInstance();

	~Game();
};
