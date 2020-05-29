	#pragma once

	// main
	#define WINDOW_CLASS_NAME		L"Castlevania"
	#define MAIN_WINDOW_TITLE		L"Castlevania"

	#define BACKGROUND_COLOR		D3DCOLOR_XRGB(0,0,0)
	#define SCREEN_WIDTH			512
	#define SCREEN_HEIGHT			480

	#define MAX_FRAME_RATE			120

	// Input
	#define DIRECTINPUT_VERSION		0x0800
	#define KEYBOARD_BUFFER_SIZE	1024

	// special texture to draw object bounding box
	#define ID_TEX_BBOX				-100		

	#define ITEM_FALLING_SPEED		0.2f


	// Whip
	#define WHIP		0
	#define WHIP_1		1
	#define WHIP_2		2

	#define LONG_CHAIN_BBOX_WIDTH		80
	#define WHIP_BBOX_WIDTH				50
	#define WHIP_BBOX_HEIGHT			15


	#define KNIFE			0
	#define WHIPITEM		1
	#define HEART_BIG		2
	#define HEART_SMALL		3
	#define MONEYBAG		4
	#define	AXE				5
	#define BOOMERANG		6
	#define HOLY_WATER		7

	#define ITEM_TIME_DESTROYED			5000


	#define ZOMBIE_ACTIVE		0
	#define ZOMBIE_DESTROYED	1
	#define ZOMBIE_INACTIVE		2

	#define ZOMBIE_WALKING_SPEED		0.1f
	#define ZOMBIE_RESPAWN_TIME			5000
	#define ZOMBIE_GRAVITY				0.002f

	#define ZOMBIE_BBOX_WIDTH			30
	#define ZOMBIE_BBOX_HEIGHT			64

	#define ZOMBIE_ACTIVE_BBOX_WIDTH			260
	#define ZOMBIE_ACTIVE_BBOX_HEIGHT			150

	#define LEOPAD_ACTIVE			0    
	#define LEOPAD_DESTROYED		1
	#define LEOPAD_INACTIVE			2
	#define LEOPAD_IDLE				3
	#define LEOPAD_JUMP				4

	#define LEOPAD_RUNNING_SPEED_X		0.3f
	#define LEOPAD_RUNNING_SPEED_Y		0.2f
	#define LEOPAD_GRAVITY				0.001f
	#define LEOPAD_RESPAWN_TIME			20000

	#define LEOPAD_BBOX_WIDTH			10
	#define LEOPAD_BBOX_HEIGHT			32
	#define LEOPAD_ACTIVE_BBOX_WIDTH		100
	#define LEOPAD_ACTIVE_BBOX_HEIGHT	200

	#define BAT_ACTIVE		0
	#define BAT_DESTROYED	1
	#define BAT_INACTIVE		2
	#define BAT_BBOX_WIDTH			10
	#define BAT_BBOX_HEIGHT			32
	#define BAT_ACTIVE_BBOX_WIDTH			400 
	#define BAT_ACTIVE_BBOX_HEIGHT			200

	#define ACTIVE			0
	#define DESTROYED		1
	#define INACTIVE		2

	#define GROUND_BBOX_WIDTH 32
	#define GROUND_BBOX_HEIGHT 32