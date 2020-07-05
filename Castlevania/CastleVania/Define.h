	#pragma once

	// main
	#define WINDOW_CLASS_NAME		L"Castlevania"
	#define MAIN_WINDOW_TITLE		L"Castlevania"

	#define BACKGROUND_COLOR		D3DCOLOR_XRGB(0,0,0)
	#define SCREEN_WIDTH			512
	#define SCREEN_HEIGHT			512

	#define MAX_FRAME_RATE			120

	// Input
	#define DIRECTINPUT_VERSION		0x0800
	#define KEYBOARD_BUFFER_SIZE	1024

	// special texture to draw object bounding box
	#define ID_TEX_BBOX				-100
	#define ID_TEX_BBOX_2			-200

	#define ITEM_FALLING_SPEED		0.2f


	#define ID_TEX_SIMON				0

	#define SIMON_BBOX_WIDTH			30
	#define SIMON_BBOX_HEIGHT			62

	#define SIMON_WALKING_SPEED			0.13f
	#define SIMON_JUMP_SPEED_Y			0.5f
	#define SIMON_GRAVITY				0.002f
	#define SIMON_GRAVITY_LOWER			0.001f

	#define IDLE		0
	#define WALK		1
	#define JUMP		2
	#define DUCK		3
	#define ASCEND		4
	#define DESCEND		5
	#define HURT		6
	#define STANDING	7
	#define DUCKING		8
	#define ASCENDING	9
	#define DESCENDING	10
	#define	UPGRADE		11
	#define DEAD		12

	#define SIMON_STAIR_SPEED_X			0.08f
	#define SIMON_STAIR_SPEED_Y			0.08f
	#define SIMON_DEFLECT_SPEED_X		0.13f
	#define SIMON_DEFLECT_SPEED_Y		0.3f


	#define SIMON_UNTOUCHABLE_TIME		2000
	#define SIMON_DEAD_TIME				3000
	// Whip
	#define WHIP		0
	#define WHIP_1		1
	#define WHIP_2		2

	#define LONG_CHAIN_BBOX_WIDTH		80
	#define WHIP_BBOX_WIDTH				50
	#define WHIP_BBOX_HEIGHT			15


	#define KNIFE			0
	#define WHIPITEM		1
	#define SMALL_HEART		2
	#define BIG_HEART		3
	#define MONEY_BAG_RED	4
	#define MONEY_BAG_BLUE	5
	#define MONEY_BAG_WHITE 6
	#define MONEY_BAG		7
	#define	AXE				8
	#define BOOMERANG		9
	#define HOLY_WATER		10
	#define DOUBLE_SHOT		11
	#define TRIPLE_SHOT		12
	

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

	#define BAT_ACTIVE			0
	#define BAT_DESTROYED		1
	#define BAT_INACTIVE		3
	#define BAT_IDLE			2

	#define BAT_BBOX_WIDTH			10
	#define BAT_BBOX_HEIGHT			32
	#define BAT_ACTIVE_BBOX_WIDTH			100 
	#define BAT_ACTIVE_BBOX_HEIGHT			100

	#define BAT_FLYING_SPEED_X	0.1f
	#define BAT_FLYING_SPEED_Y	0.15f
	#define BAT_SPEED_VARIATION 0.006f
	#define BAT_RESPAWN_TIME	5000

	#define ACTIVE			0
	#define DESTROYED		1
	#define INACTIVE		2

	#define GROUND_BBOX_WIDTH 32
	#define GROUND_BBOX_HEIGHT 32


	#define BOSS_ACTIVE			0
	#define BOSS_DESTROYED		1
	#define BOSS_INACTIVE		2

	#define BOSS_HP						16
	#define BOSS_DEFAULT_TIME_TO_FLY	1000
	#define BOSS_FAST_TIME_TO_FLY		750
	#define BOSS_STOP_TIME_WAITING		1500

	#define BOSS_BBOX_WIDTH						70
	#define BOSS_BBOX_HEIGHT					46
	#define BOSS_ACTIVE_BBOX_WIDTH				50
	#define BOSS_ACTIVE_BBOX_HEIGHT				400
	#define BOSS_RECT_RANDOMSPOT_BBOX_WIDTH		200
	#define BOSS_RECT_RANDOMSPOT_BBOX_HEIGHT	200

	#define ENEMY_DEFAULT_BBOX_WIDTH	32
	#define ENEMY_DEFAULT_BBOX_HEIGHT	32

	#define NORMAL				0
	#define BREAK				1
	// Wall pieces
	#define	WALLPIECES_GRAVITY			0.0005f
	#define WALLPIECES_TIME				2000

	#define HUNCHBACK_ACTIVE			0
	#define HUNCHBACK_DESTROYED			1
	#define HUNCHBACK_INACTIVE			4
	#define HUNCHBACK_IDLE				3
	#define HUNCHBACK_JUMP				2

	#define HUNCHBACK_RUNNING_SPEED_X	0.26f
	#define HUNCHBACK_RUNNING_SPEED_Y	0.18f
	#define HUNCHBACK_GRAVITY			0.001f



	#define GHOST_ACTIVE			0
	#define GHOST_DESTROYED			1
	#define GHOST_INACTIVE			2

	#define GHOST_FLYING_SPEED_X	0.12f
	#define GHOST_FLYING_SPEED_Y	0.012f
