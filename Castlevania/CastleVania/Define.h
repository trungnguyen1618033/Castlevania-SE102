	#pragma once

	// main
	#define WINDOW_CLASS_NAME		L"Castlevania"
	#define MAIN_WINDOW_TITLE		L"Castlevania"

	#define BACKGROUND_COLOR		D3DCOLOR_XRGB(0,0,0)
	#define SCREEN_WIDTH			320
	#define SCREEN_HEIGHT			288

	#define MAX_FRAME_RATE			90

	// Input
	#define DIRECTINPUT_VERSION		0x0800
	#define KEYBOARD_BUFFER_SIZE	1024

	// special texture to draw object bounding box
	#define ID_TEX_BBOX				-100		

	#define ITEM_FALLING_SPEED		0.2f

	

	// TileMap
	#define FILEPATH_TEX_SCENE		L"scenes\\Entrance.BMP"
	#define FILEPATH_DATA_SCENE		L"scenes\\Entrance.txt"

	#define FILEPATH_TEX_SCENE_2	L"scenes\\Hall.BMP"
	#define FILEPATH_DATA_SCENE_2	L"scenes\\Hall.txt"

	#define FILEPATH_TEX_SCENE_3	L"scenes\\Tower1.BMP"
	#define FILEPATH_DATA_SCENE_3	L"scenes\\Tower1.txt"
	