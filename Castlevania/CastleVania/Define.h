	#pragma once

	// main
	#define WINDOW_CLASS_NAME		"Castlevania"
	#define MAIN_WINDOW_TITLE		"Castlevania"

	#define BACKGROUND_COLOR		D3DCOLOR_XRGB(0,0,0)
	#define SCREEN_WIDTH			512
	#define SCREEN_HEIGHT			480

	#define MAX_FRAME_RATE			120

	// Input
	#define DIRECTINPUT_VERSION		0x0800
	#define KEYBOARD_BUFFER_SIZE	1024

	// PlayScene
	#define SCENE_SECTION_UNKNOWN -1
	#define SCENE_SECTION_TEXTURES 2
	#define SCENE_SECTION_SPRITES 3
	#define SCENE_SECTION_ANIMATIONS 4
	#define SCENE_SECTION_ANIMATION_SETS	5
	#define SCENE_SECTION_OBJECTS	6
	#define SCENE_SECTION_TILEMAP	7
	#define SCENE_SECTION_GRID		8

	#define OBJECT_TYPE_SIMON		0
	#define OBJECT_TYPE_TORCH		2
	#define OBJECT_TYPE_GROUND		7
	#define OBJECT_TYPE_STAIR		8
	#define OBJECT_TYPE_BLOCK		15
	#define OBJECT_TYPE_BREAK		10
	#define OBJECT_TYPE_KNIGHT		16
	#define OBJECT_TYPE_BAT			17
	#define OBJECT_TYPE_GHOST		18
	#define OBJECT_TYPE_HUNCHBACK	19
	#define OBJECT_TYPE_SKELETON	20
	#define OBJECT_TYPE_RAVEN		21
	#define OBJECT_TYPE_ZOMBIE		22
	#define OBJECT_TYPE_BOSS		23
	#define OBJECT_TYPE_PORTAL		50
	#define OBJECT_TYPE_AREADEAD	60

	#define MAX_SCENE_LINE 1024

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

	
	#define SIMON_HP					16
	#define SIMON_ENERGY				99

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
	#define BEHIND		13
	#define IDLE_INVISIBLE			14
	#define WALK_INVISIBLE			15
	#define JUMP_INVISIBLE			16
	#define DUCK_INVISIBLE			17
	#define ASCEND_INVISIBLE		18
	#define DESCEND_INVISIBLE		19
	#define STANDING_INVISIBLE		20
	#define DUCKING_INVISIBLE		21
	#define ASCENDING_INVISIBLE		22
	#define DESCENDING_INVISIBLE	23

	#define SIMON_STAIR_SPEED_X			0.08f
	#define SIMON_STAIR_SPEED_Y			0.08f
	#define SIMON_HURT_SPEED_X		0.11f
	#define SIMON_HURT_SPEED_Y		0.25f


	#define SIMON_UNTOUCHABLE_TIME		2000
	#define SIMON_DEAD_TIME				3000
	#define SIMON_INVISIBLE_TIME		4000

	// Whip
	#define WHIP		0
	#define WHIP_1		1
	#define WHIP_2		2

	#define LONG_CHAIN_BBOX_WIDTH		80
	#define WHIP_BBOX_WIDTH				50
	#define WHIP_BBOX_HEIGHT			15

	// Ground
	#define GROUND_BBOX_WIDTH 32
	#define GROUND_BBOX_HEIGHT 32

	// Stair
	#define STAIR_BBOX_WIDTH			32
	#define STAIR_BBOX_HEIGHT			32

	// Torch 
	#define TORCH_BBOX_WIDTH		32
	#define TORCH_BBOX_HEIGHT		64

	//  ID Items
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
	#define STOP_WATCH		11
	#define DOUBLE_SHOT		12
	#define TRIPLE_SHOT		13
	#define MAGIC_CRYSTAL	14
	#define CROWN			15
	#define FOOD			16
	#define INVISIBLE_BOTLE	17
	#define CROSS			18

	// Items
	#define ITEM_FALLING_SPEED_X			0.15f
	#define ITEM_FALLING_SPEED_X_VARIATION	0.01f
	#define ITEM_FALLING_SPEED_Y			0.15f
	#define ITEM_SMALLHEART_FALLING_SPEED_Y	0.1f
	#define ITEM_TIME_DESTROYED				4000

	#define ITEM_TIME_DESTROYED			5000

	// Weapon
	#define WEAPON_KNIFE				0
	#define	WEAPON_AXE					1
	#define WEAPON_BOOMERANG			2
	#define WEAPON_HOLY_WATER			3
	#define	WEAPON_STOP_WATCH			4
	#define HOLY_WATER_BROKEN			5

	#define KNIFE_SPEED					0.2f
	#define KNIFE_BBOX_WIDTH			32
	#define KNIFE_BBOX_HEIGHT			32

	#define AXE_BBOX_WIDTH				30
	#define AXE_BBOX_HEIGHT				28

	#define AXE_SPEED_X			0.2f
	#define AXE_SPEED_Y			0.5f
	#define AXE_GRAVITY			0.001f

	#define BOOMERANG_SPEED		0.5f
	#define BOOMERANG_TURNBACK_SPEED		0.01f

	#define HOLY_WATER_GRAVITY	0.001f
	#define HOLY_WATER_SPEED_X	0.25f
	#define HOLY_WATER_SPEED_Y	0.1f
	#define HOLY_WATER_TIME_EFFECT	1000

	#define STOP_WATCH_TIME			2000
	#define CROSS_TIME				500



	// BreakWalk
	#define NORMAL				0
	#define BREAK				1
	// Wall pieces
	#define	WALLPIECES_GRAVITY			0.0005f
	#define WALLPIECES_TIME				2000
	#define	WALL_GRAVITY			0.0005f
	#define WALL_TIME				2000

	// BlockMove
	#define BLOCK_BBOX_WIDTH		64
	#define BLOCK_BBOX_HEIGHT		16
	#define SPEED_BLOCK				0.075f
	

	// Zombie
	#define ZOMBIE_ACTIVE			0
	#define ZOMBIE_DESTROYED		1
	#define ZOMBIE_INACTIVE			2
	#define ZOMBIE_WALKING_SPEED		0.1f
	#define ZOMBIE_RESPAWN_TIME			5000
	#define ZOMBIE_GRAVITY				0.002f
	#define ZOMBIE_BBOX_WIDTH			10
	#define ZOMBIE_BBOX_HEIGHT			60
	#define ZOMBIE_ACTIVE_BBOX_WIDTH			200
	#define ZOMBIE_ACTIVE_BBOX_HEIGHT			200


	// Bat
	#define BAT_ACTIVE			0
	#define BAT_DESTROYED		1
	#define BAT_INACTIVE		3
	#define BAT_IDLE			2
	#define BAT_BBOX_WIDTH			10
	#define BAT_BBOX_HEIGHT			32
	#define BAT_ACTIVE_BBOX_WIDTH			192 
	#define BAT_ACTIVE_BBOX_HEIGHT			112
	#define BAT_FLYING_SPEED_Y	0.1f
	#define BAT_MAX_Y			144
	#define BAT_FLYING_SPEED_X	0.1f
	#define BAT_FLYING_SPEED_Y	0.15f
	#define BAT_SPEED_VARIATION 0.006f
	#define BAT_RESPAWN_TIME	5000

	// Bone
	#define BONE_SPEED_X			0.16f
	#define BONE_SPEED_Y			0.40f
	#define BONE_GRAVITY			0.001f

	#define BONE_BBOX_WIDTH			32
	#define BONE_BBOX_HEIGHT		28

	// Enemys
	#define ACTIVE			0
	#define DESTROYED		1
	#define INACTIVE		2

	#define ENEMY_DEFAULT_BBOX_WIDTH	32
	#define ENEMY_DEFAULT_BBOX_HEIGHT	32

	// Knight

	#define KNIGHT_SPEED	0.03f

	#define KNIGHT_BBOX_WIDTH	32
	#define KNIGHT_BBOX_HEIGHT	64

	// Raven
	#define RAVEN_FLYING_SPEED_X 0.12f
	#define RAVEN_FLYING_SPEED_Y 0.1f
	#define RAVEN_BBOX_WIDTH			13
	#define RAVEN_BBOX_HEIGHT			32
	#define RAVEN_ACTIVE_BBOX_WIDTH		128
	#define RAVEN_ACTIVE_BBOX_HEIGHT	160

	// Skeleton
	#define SKELETON_JUMP_SPEED_Y		0.7f
	#define SKELETON_GRAVITY				0.0015f
	#define SKELETON_WALKING_SPEED_X		0.1f

	#define SKELETON_BBOX_WIDTH				10
	#define SKELETON_BBOX_HEIGHT			60
	#define RAVEN_ACTIVE_BBOX_WIDTH			256
	#define RAVEN_ACTIVE_BBOX_HEIGHT		110

	// Boss
	#define BOSS_ACTIVE			0
	#define BOSS_DESTROYED		1
	#define BOSS_INACTIVE		2

	#define BOSS_HP						16
	#define BOSS_DEFAULT_TIME_TO_FLY	1000
	#define BOSS_FAST_TIME_TO_FLY		1000
	#define BOSS_STOP_TIME_WAITING		1500

	#define BOSS_BBOX_WIDTH						70
	#define BOSS_BBOX_HEIGHT					46
	#define BOSS_ACTIVE_BBOX_WIDTH				50
	#define BOSS_ACTIVE_BBOX_HEIGHT				400
	#define BOSS_RECT_RANDOMSPOT_BBOX_WIDTH		200
	#define BOSS_RECT_RANDOMSPOT_BBOX_HEIGHT	200

	// HunchBack
	#define HUNCHBACK_ACTIVE			0
	#define HUNCHBACK_DESTROYED			1
	#define HUNCHBACK_INACTIVE			4
	#define HUNCHBACK_IDLE				2

	#define HUNCHBACK_RUNNING_SPEED_X	0.1f
	#define HUNCHBACK_RUNNING_SPEED_Y	0.26f
	#define HUNCHBACK_GRAVITY			0.001f

	#define HUNCHBACK_BBOX_WIDTH			32
	#define HUNCHBACK_BBOX_HEIGHT			32
	#define HUNCHBACK_ACTIVE_BBOX_WIDTH		160
	#define HUNCHBACK_ACTIVE_BBOX_HEIGHT	224

	// Ghost
	#define GHOST_ACTIVE			0
	#define GHOST_DESTROYED			1
	#define GHOST_INACTIVE			2

	#define GHOST_FLYING_SPEED_X	0.048f
	#define GHOST_FLYING_SPEED_Y	0.012f

	// Target
	#define TORCH		0
	#define KNIGHT		1
	#define BAT			2
	#define	GHOST		3
	#define	HUNCHBACK	4
	#define SKELETON	5
	#define BONE		6
	#define RAVEN		7
	#define ZOMBIE		8
	#define BOSS		9
	#define BREAKWALL	10

	// ====---SOUND--=======
	#define INTRODUCE1 0
	#define INTRODUCE2 1
	#define LVL1_VAMKILL 2
	#define LVL2_STALKER 3
	#define BOOMERANGS 4
	#define BRICKBREAK 5
	#define DOWNWATER 6
	#define EATCROSS 7
	#define EATCRYSTAL 8
	#define EATHEART 9
	#define EATMONEY 10
	#define EATPOTION 11
	#define EATWEAPON_CHICKEN 12
	#define HITSOFT 13
	#define HITSTEEL 14
	#define HOLYWATERBREAK 15
	#define INJURED 16
	#define SOUNDKNIFE 17
	#define MAINGATE 18
	#define PAUSE 19
	#define POTIONIMMORTAL 20
	#define SHOWSECRETITEM 21
	#define TETRENCAOXUONG 22
	#define TICKTICKTICK 23
	#define TIMENEAREND 24
	#define UPWATER 25
	#define WHIPATTACK_AXE 26
	#define BOSS_FIGHT 27

