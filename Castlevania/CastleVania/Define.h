	#pragma once

	// main
	#define WINDOW_CLASS_NAME		L"Castlevania"
	#define MAIN_WINDOW_TITLE		L"Castlevania"

	#define BACKGROUND_COLOR		D3DCOLOR_XRGB(255,255,255)
	#define SCREEN_WIDTH			320
	#define SCREEN_HEIGHT			224

	#define MAX_FRAME_RATE			90

	// Input
	#define DIRECTINPUT_VERSION		0x0800
	#define KEYBOARD_BUFFER_SIZE	1024

	// special texture to draw object bounding box
	#define ID_TEX_BBOX				-100		

	// Simon	
	#define ID_TEX_SIMON				0
	#define FILEPATH_TEX_SIMON			L"Textures\\Simon.png"

	#define SIMON_BBOX_WIDTH			32
	#define SIMON_BBOX_HEIGHT			32

	#define SIMON_WALKING_SPEED			0.1f
	#define SIMON_JUMP_SPEED_Y			0.8f
	#define SIMON_GRAVITY				0.03f

	
	#define IDLE		0
	#define WALK		1
	#define DUCK		2
	#define JUMP		3
	#define ASCEND		4
	#define DESCEND		5
	#define HURT		6
	#define DEATH		7
	#define STANDING	8
	#define DUCKING		9
	#define ASCENDING	10
	#define DESCENDING	11


	// TORCH
	#define ID_TEX_TORCH				1
	#define FILEPATH_TEX_TORCH			L"Textures\\Item.png"

	// Whip
	#define ID_TEX_WHIP					2
	#define FILEPATH_TEX_WHIP			L"Textures\\Whip.png"
	#define NORMAL_WHIP	0

	// TileMap
	#define FILEPATH_TEX_SCENE		L"Textures\\Entrance.BMP"
	#define FILEPATH_DATA_SCENE		L"Textures\\Entrance.txt"