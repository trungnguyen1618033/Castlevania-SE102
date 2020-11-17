#include "PlayScene.h"
#include <iostream>
#include <fstream>

using namespace std;

PlayScene::PlayScene(int id, LPCWSTR filePath) :Scene(id, filePath)
{
	key_handler = new PlaySceneKeyHandler(this);
}

/*
	Parse a line in section [OBJECTS]
*/
void PlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());
	
	int ani_set_id = atoi(tokens[3].c_str());
	

	AnimationSets* animation_sets = AnimationSets::GetInstance();

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	float r;
	float b;
	int scene_id;
	int s, idItem;

	switch (object_type)
	{
	case OBJECT_TYPE_SIMON:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] SIMON object was created before! ");
			return;
		}
		player = Simon::GetInstance();
		player->SetPosition(x, y);
		player->SetAnimationSet(ani_set);
		player->SetState(IDLE);
		break;
	case OBJECT_TYPE_TORCH:
	{
		Torch* torch = new Torch();
		torch->SetPosition(x, y);
		torch->SetAnimationSet(ani_set);
		s = atof(tokens[4].c_str());
		torch->SetState(s);
		unit = new Unit(grid, torch, x, y);
		break;
	}
	case OBJECT_TYPE_GROUND:
	{
		Ground* ground = new Ground();
		ground->SetPosition(x, y);
		ground->SetAnimationSet(ani_set);
		unit = new Unit(grid, ground, x, y);
		break;
	}
	case OBJECT_TYPE_AREADEAD:
	{
		AreaDead* area = new AreaDead();
		area->SetPosition(x, y);
		area->SetAnimationSet(ani_set);
		unit = new Unit(grid, area, x, y);
		break;
	}
	case OBJECT_TYPE_STAIR:
	{
		Stair* stair = new Stair();
		stair->SetPosition(x, y);
		stair->SetAnimationSet(ani_set);
		s = atof(tokens[4].c_str());
		stair->SetState(s);
		unit = new Unit(grid, stair, x, y);
		break;
	}
	case OBJECT_TYPE_PORTAL:
	{
		r = atof(tokens[4].c_str());
		b = atof(tokens[5].c_str());
		scene_id = atoi(tokens[6].c_str());
		portal = new Portal(x, y, r, b, scene_id);
		unit = new Unit(grid, portal, x, y);
		break;
	}
	case OBJECT_TYPE_BLOCK:
	{
		BlockMove* block = new BlockMove();
		block->SetPosition(x, y);
		block->SetAnimationSet(ani_set);
		unit = new Unit(grid, block, x, y);
		break;
	}
	case OBJECT_TYPE_BREAK:
	{
		idItem = atof(tokens[4].c_str());
		BreakWall* wall = new BreakWall();
		wall->SetPosition(x, y);
		wall->SetAnimationSet(ani_set);
		wall->SetIDItem(idItem);
		unit = new Unit(grid, wall, x, y);
		break;
	}
	case OBJECT_TYPE_KNIGHT:
	{
		r = atof(tokens[4].c_str());
		Knight* knight = new Knight();
		knight->SetEntryPosition(x, y);
		knight->SetPosition(x, y);
		knight->SetAnimationSet(ani_set);
		knight->SetState(KNIGHT_INACTIVE);
		knight->SetLeft(x);
		knight->SetRight(r);
		unit = new Unit(grid, knight, x, y);
		break;
	}
	case OBJECT_TYPE_BAT:
	{
		Bat* bat = new Bat();
		bat->SetEntryPosition(x, y);
		bat->SetPosition(x, y);
		bat->SetAnimationSet(ani_set);
		bat->SetState(BAT_INACTIVE);
		unit = new Unit(grid, bat, x, y);
		break;
	}
	case OBJECT_TYPE_GHOST:
	{
		Ghost* ghost = new Ghost();
		ghost->SetEntryPosition(x, y);
		ghost->SetAnimationSet(ani_set);
		ghost->SetState(true);
		ghost->SetState(GHOST_INACTIVE);
		unit = new Unit(grid, ghost, x, y);
		break;
	}
	case OBJECT_TYPE_HUNCHBACK:
	{
		HunchBack* hunchback = new HunchBack();
		hunchback->SetEntryPosition(x, y);
		hunchback->SetPosition(x, y);
		hunchback->SetAnimationSet(ani_set);
		hunchback->SetState(HUNCHBACK_INACTIVE);
		unit = new Unit(grid, hunchback, x, y);
		break;
	}
	case OBJECT_TYPE_SKELETON:
	{
		Skeleton* skeleton = new Skeleton();
		skeleton->SetEntryPosition(x, y);
		skeleton->SetAnimationSet(ani_set);
		skeleton->SetState(SKELETON_INACTIVE);
		unit = new Unit(grid, skeleton, x, y);
		break;
	}
	case OBJECT_TYPE_RAVEN:
	{
		Raven* raven = new Raven();
		raven->SetEntryPosition(x, y);
		raven->SetAnimationSet(ani_set);
		raven->SetState(RAVEN_INACTIVE);
		unit = new Unit(grid, raven, x, y);
		break;
	}
	case OBJECT_TYPE_ZOMBIE:
	{
		Zombie* zombie = new Zombie();
		zombie->SetEntryPosition(x, y);
		zombie->SetAnimationSet(ani_set);
		zombie->SetState(ZOMBIE_INACTIVE);
		unit = new Unit(grid, zombie, x, y);
		break;
	}
	case OBJECT_TYPE_BOSS:
		boss = new Boss();
		boss->SetEntryPosition(x, y);
		boss->SetAnimationSet(ani_set);
		boss->SetState(INACTIVE);
		boss->SetEnable(true);
		unit = new Unit(grid, boss, x, y);
		break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

}

void PlayScene::_ParseSection_TILEMAP(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 7) return; // skip invalid lines

	int mapID = atoi(tokens[0].c_str());
	wstring path_img = ToWSTR(tokens[1]);
	wstring path_text = ToWSTR(tokens[2]);
	int W = atoi(tokens[3].c_str());
	int H = atoi(tokens[4].c_str());
	int x = atoi(tokens[5].c_str());
	int y = atoi(tokens[6].c_str());

	tilemaps->Add(mapID, path_img.c_str(), path_text.c_str(), W, H);
	
	//DebugOut(L"ok_______\n");
	Game::GetInstance()->SetCamPos(x, y);
	
}

void PlayScene::_ParseSection_GRID(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 4) return; // skip invalid lines

	int W = atoi(tokens[0].c_str());
	int H = atoi(tokens[1].c_str());
	int x = atoi(tokens[2].c_str());
	int y = atoi(tokens[3].c_str());

	grid = new Grid(W, H, x, y);

}

void PlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);
	
	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[TILEMAP]") { 
			section = SCENE_SECTION_TILEMAP; continue; 
		}
		if (line == "[TEXTURES]") { 
			section = SCENE_SECTION_TEXTURES; continue; 
		}
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line == "[GRID]") {
			section = SCENE_SECTION_GRID; continue;
		}
		if (line[0] == '[') {
			section = SCENE_SECTION_UNKNOWN; continue; 
		}

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_TILEMAP: _ParseSection_TILEMAP(line); break;
		case SCENE_SECTION_GRID: _ParseSection_GRID(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);

	if (id == 0)
		player->SetOrientation(1);
	if(id == 2)
	{
		player->SetOrientation(1);
		player->isFalling = false;
		player->SetState(ASCEND);
		player->isAutoWalk = false;
	}
	if(id == 4)
	{
		player->isFalling = false;
		player->SetOrientation(-1);
		player->SetState(ASCEND);
	}
	if (id == 3)
	{
	/*	player->SetPosition(320, 304);*/
		player->SetOrientation(-1);
	}

	whip = new Whip();
	for (int i = 1; i <= 3; i++)
	{
		weapon = new Weapon();
		weapon->SetEnable(false);
		weaponlist.push_back(weapon);
	}
	isBossFighting = false;

}

void PlayScene::Update(DWORD dt)
{
	// Pause Game
	if (isGamePause == true)
		return;

	// Game over
	if (player->isGotMagicCrystalItem == true)
	{
		DoneGame();

		if (isGameOver == true)
			return;
	}


	// Lấy danh sách object từ grid 
	GetObjectFromGrid();

	// Cross effect
	CrossEffect();

	// Cập nhật bộ đếm thời gian
	UpdateTimeCounter();

	// double shot
	DoubleShotEffect();

	// triple shot
	TripleShotEffect();

	SetDropItems();

	// Vị trí xuất hiện của quái
	SetEnemiesSpawnPositon();

	// Update
	Simon_Update(dt);
	Whip_Update(dt);
	for (int i = 0; i < 3; i++)
		Weapon_Update(dt, i);

	for (UINT i = 0; i < listObjects.size(); i++)
	{
		LPGAMEOBJECT object = listObjects[i];
		vector<LPGAMEOBJECT> coObjects;

		GetColliableObjects(object, coObjects);
		object->Update(dt, &coObjects, isUsingStopWatch);

		if (dynamic_cast<Ghost*>(object))
		{
			Ghost* ghost = dynamic_cast<Ghost*>(object);
			float sx, sy;
			player->GetPosition(sx, sy);
			ghost->SetSimonPosition(sx, sy);
		}
		else if (dynamic_cast<HunchBack*>(object))
		{
			HunchBack* hunchback = dynamic_cast<HunchBack*>(object);
			float sx, sy;
			player->GetPosition(sx, sy);
			hunchback->SetSimonPosition(sx, sy);
		}
		else if (dynamic_cast<Skeleton*>(object))
		{
			Skeleton* skeleton = dynamic_cast<Skeleton*>(object);
			float sx, sy;
			player->GetPosition(sx, sy);
			skeleton->SetSimonPosition(sx, sy);
			if (skeleton->CanHit() == true)
			{
				skeleton->SetState(SKELETON_HIT);

				skeleton->Hit(grid);
			}
		}
		else if (dynamic_cast<Raven*>(object))
		{
			Raven* raven = dynamic_cast<Raven*>(object);
			float sx, sy;
			player->GetPosition(sx, sy);
			raven->SetSimonPosition(sx, sy);
		}
		else if (dynamic_cast<Boss*>(object))
		{
			// Lấy vị trí simon cho Boss
			float sx, sy;
			player->GetPosition(sx, sy);
			boss->SetSimonPosition(sx, sy);
		}
	}


	// Không cho Simon đi ra khỏi vùng đánh boss
	Game* game = Game::GetInstance();
	if (isBossFighting == true && player->x < game->GetCameraPositon().x)
		player->x = game->GetCameraPositon().x;

	if (isUsingStopWatch == true)
		game->stopMove = true;
	else
		game->stopMove = false;
	if (isGamePause == true)
		game->isPause = true;
	else
		game->isPause = false;
	
	// Xoá các object đi ra khỏi vùng viewport
	SetInactivation();

	// update camera
	UpdateCameraPosition();

	// update grid
	UpdateGrid();
}

void PlayScene::Render()
{
	Game* game = Game::GetInstance();

	if (isGameOver == true)
		return;

	if (game->GetChangeScene() == true)
		return;
	
	tilemaps->Get(id)->Draw(game->GetCameraPositon(), isCrossEffect);

	for (auto obj : listStaticObjectsToRender)
	{
		if (obj->IsEnable() == false)
			continue;

		obj->Render();
		//obj->RenderBoundingBox();
	}
	for (auto obj : listMovingObjectsToRender)
	{
		if (obj->IsEnable() == false)
			continue;
			

		obj->Render();
		obj->RenderBoundingBox();
	}

	for (auto obj : listItems)
	{
		if (obj->IsEnable() == false)
			continue;
		obj->Render();
		//obj->RenderBoundingBox();
	}

	player->Render();
	//player->RenderBoundingBox();

	for (int i = 0; i < 3; i++)
	{
		weaponlist[i]->Render();
		//weaponlist[i]->RenderBoundingBox();
	}

	if (player->isHitWeapons == false)
	{
		if (player->IsHit() == true)
			whip->Render(player->animation_set->at(player->GetState())->GetCurrentFrame());
		else
			whip->Render(-1);

		//whip->RenderBoundingBox();
	}
	
	//portal->RenderBoundingBox();

}

/*
	Unload current scene
*/
void PlayScene::Unload()
{
	for (int i = 0; i < listObjects.size(); i++)
		delete listObjects[i];

	listUnits.clear();
	listObjects.clear();
	player = NULL;
	listStaticObjectsToRender.clear();
	listStairs.clear();
	listMovingObjectsToRender.clear();
	listItems.clear();
	portal = NULL;
	whip = NULL;
	weaponlist.clear();

}

void PlayScene::SetDropItems()
{
	for (UINT i = 0; i < listObjects.size(); i++)
	{
		LPGAMEOBJECT object = listObjects[i];
		int idItem = -1;
		float x, y;

		if (object->IsDroppedItem() == true)
			continue;
		if (dynamic_cast<Torch*>(object) && object->GetState() == EFFECTEXPLODE)
		{
			idItem = GetRandomItem();
			DebugOut(L"idItem: %d\n", idItem);
			object->GetPosition(x, y);
			object->SetIsDroppedItem(true);
		}
		else if (dynamic_cast<BreakWall*>(object)
			&& object->GetState() == BREAK && object->IsDroppedItem() == false)
		{
			object->SetIsDroppedItem(true);
			idItem = object->idItem;
			x = Game::GetInstance()->GetCameraPositon().x + SCREEN_WIDTH / 2;
			y = Game::GetInstance()->GetCameraPositon().y + SCREEN_HEIGHT / 2;
		}
		else if ((dynamic_cast<Zombie*>(object) && object->GetState() == ZOMBIE_DESTROYED) ||
			(dynamic_cast<Bat*>(object) && object->GetState() == BAT_DESTROYED) ||
			(dynamic_cast<Raven*>(object) && object->GetState() == RAVEN_DESTROYED))
		{
			idItem = GetRandomItem();
			object->GetPosition(x, y);
			object->SetIsDroppedItem(true);
		}
		else if (dynamic_cast<Boss*>(object) && object->GetState() == BOSS_DESTROYED)
		{
			if (boss->IsDroppedItem() == false)
			{
				boss->SetEnable(false);
				boss->SetIsDroppedItem(true);

				idItem = MAGIC_CRYSTAL;
				x = Game::GetInstance()->GetCameraPositon().x + SCREEN_WIDTH / 2;
				y = Game::GetInstance()->GetCameraPositon().y + SCREEN_HEIGHT / 2;
			}
		}
		if (idItem != -1)
		{
			// Tạo một item theo id
			auto items = new Items();
			items->SetEnable(true);
			items->SetPosition(x, y);
			items->SetState(idItem);

			listItems.push_back(items);
			unit = new Unit(grid, items, x, y);
		}
	}
}

int PlayScene::GetRandomItem()
{
	/*srand(time(NULL));
	int idItem = rand() % 15;
	return idItem;*/

	std::map<int, int> randomRange = {
		{STOP_WATCH,	1},
		{KNIFE,		4},
		{AXE,			6},
		{HOLY_WATER,	8},
		{BOOMERANG,		10},
		{SMALL_HEART,	60},
		{BIG_HEART,	70},
		{CROSS,			72},
		{INVISIBLE_BOTLE, 74},
		{WHIPITEM,			80},
		{MONEY_BAG_RED,	85},
		{MONEY_BAG_BLUE, 89},
		{MONEY_BAG_WHITE,	92},
		{MONEY_BAG,94},
		{DOUBLE_SHOT,	96},
		{TRIPLE_SHOT,	98},
		{FOOD,		100} };

	bool canDropItem = (rand() % 100) < 80 ? true : false; // tỉ lệ rớt item là 80/100

	if (canDropItem == false)
		return -1;

	int randomValue = rand() % 100;

	for (auto i = randomRange.begin(); i != randomRange.end(); i++)
	{
		if (randomValue < (*i).second)
			return (*i).first;
	}
}

void PlayScene::SetInactivation()
{
	Game* game = Game::GetInstance();
	D3DXVECTOR2 entryViewPort = game->GetCameraPositon();

	for (auto object : listObjects)
	{
		if (IsInViewport(object) == false)
		{
			if (dynamic_cast<Knight*>(object) && object->GetState() == KNIGHT_ACTIVE)
			{
				auto knight = dynamic_cast<Knight*>(object);
				knight->SetState(KNIGHT_INACTIVE);
			}
			else if (dynamic_cast<Ghost*>(object) && object->GetState() == GHOST_ACTIVE)
			{
				auto ghost = dynamic_cast<Ghost*>(object);
				ghost->SetState(GHOST_INACTIVE);
			}
			else if (dynamic_cast<HunchBack*>(object) && object->GetState() == HUNCHBACK_ACTIVE)
			{
				auto hunchback = dynamic_cast<HunchBack*>(object);
				hunchback->SetState(HUNCHBACK_INACTIVE);
			}
			else if (dynamic_cast<Raven*>(object) && object->GetState() == RAVEN_ACTIVE)
			{
				auto raven = dynamic_cast<Raven*>(object);
				raven->SetState(RAVEN_INACTIVE);
			}
			else if (dynamic_cast<Skeleton*>(object) && object->GetState() == SKELETON_ACTIVE)
			{
				auto skeleton = dynamic_cast<Skeleton*>(object);
				skeleton->SetState(SKELETON_INACTIVE);
			}
			else if (dynamic_cast<Zombie*>(object) && object->GetState() == ZOMBIE_ACTIVE)
			{
				auto zombie = dynamic_cast<Zombie*>(object);
				zombie->SetState(ZOMBIE_INACTIVE);
			}
			else if (dynamic_cast<Bone*>(object) && object->IsEnable() == true)
				object->SetEnable(false);
			else if (dynamic_cast<Items*>(object) && object->IsEnable() == true)
				object->SetEnable(false);

		}
	}

	for (int i = 0; i < 3; i++)
	{
		if (weaponlist[i]->IsEnable() == true)
		{
			if (IsInViewport(weaponlist[i]) == false)
				weaponlist[i]->SetEnable(false);
		}
	}
}


bool PlayScene::IsInViewport(LPGAMEOBJECT object)
{
	Game* game = Game::GetInstance();
	D3DXVECTOR2 camPosition = game->GetCameraPositon();

	float obj_x, obj_y;
	object->GetPosition(obj_x, obj_y);

	return obj_x >= camPosition.x && obj_x < camPosition.x + SCREEN_WIDTH
		&& obj_y >= camPosition.y && obj_y < camPosition.y + SCREEN_HEIGHT;
}

void PlayScene::UpdateCameraPosition()
{
	Game* game = Game::GetInstance();

	if (isBossFighting == true)	// ko cho camera di chuyển khi đánh với bos
		return;

	if (game->GetChangeScene() == true)
		return;

	if (boss != NULL && boss->GetState() == BOSS_ACTIVE)
	{
		isBossFighting = true;
		return;
	}

	if (id == 1)
	{
		game->SetCamPos(0, 0);
		return;
	}
	if (player->x > SCREEN_WIDTH / 2 &&
		player->x + SCREEN_WIDTH / 2 < tilemaps->Get(id)->GetMapWidth())
	{
		int min_col = 0;
		int max_col = tilemaps->Get(id)->GetMapWidth() / 32;

		if (player->x >= min_col * 32 + (SCREEN_WIDTH / 2 - 16) &&
			player->x <= max_col * 32 - (SCREEN_WIDTH / 2 - 16))
		{

			game->SetCamPos(player->x - SCREEN_WIDTH / 2, 0);
		}
	}
}

void PlayScene::UpdateTimeCounter()
{
	// Stop Watch
	if (isUsingStopWatch == true && GetTickCount() - stopWatchCounter > STOP_WATCH_TIME)
	{
		isUsingStopWatch = false;
		stopWatchCounter = 0;
	}
	// Cross
	if (isCrossEffect == true && GetTickCount() - crossTimeCounter > CROSS_TIME)
	{
		isCrossEffect = false;
		crossTimeCounter = 0;
	}

	// Simon dead
	if (isSimonDead == true && GetTickCount() - simonDeadTimeCounter > SIMON_DEAD_TIME)
	{
		isSimonDead = false;
		simonDeadTimeCounter = 0;
		if(player->GetLife() == 0)
			isGameOver = true;
		else
			ResetGame(id);
	}

	// Double shot
	if (isDoubleShot == true && GetTickCount() - doubleShotTimeCounter > 10000)
	{
		isDoubleShot = false;
		doubleShotTimeCounter = 0;
	}

	// Triple shot
	if (isTripleShot == true && GetTickCount() - tripleShotTimeCounter > 10000)
	{
		isTripleShot = false;
		tripleShotTimeCounter = 0;
	}
}

void PlayScene::Simon_Update(DWORD dt)
{


	if (player->GetState() == DEAD)
	{
		if (isSimonDead == false)
		{
			isDead = true;
			isSimonDead = true;
			simonDeadTimeCounter = GetTickCount();
		}

		return;
	}

	vector<LPGAMEOBJECT> coObjects;

	GetColliableObjects(player, coObjects);

	player->Update(dt, &coObjects);
	player->CheckCollisionWithItem(&listItems);
	player->CheckCollisionWithEnemyActiveArea(&listObjects);
}

void PlayScene::Whip_Update(DWORD dt)
{
	if (whip->GetScoreReceived() != 0)
	{
		player->AddScore(whip->GetScoreReceived());
		whip->SetScoreReceived(0);
	}

	if (player->isGotChainItem == true) // update trạng thái của whip
	{
		player->isGotChainItem = false;
		whip->Upgrade();
	}

	// lấy vị trí và phương của simon cho whip
	float simon_x, simon_y;
	player->GetPosition(simon_x, simon_y);
	bool isSimonStand = true;

	if (player->GetState() == DUCK || player->GetState() == DUCKING)
		isSimonStand = false;

	whip->SetOrientation(player->GetOrientation());
	whip->SetWhipPosition(D3DXVECTOR3(simon_x, simon_y, 0), isSimonStand);

	// chỉ xét va chạm khi render tới sprite cuối cùng của simon (vung tay tới)
	if (player->IsHit() && player->animation_set->at(player->GetState())->IsRenderingLastFrame() == true && player->isHitWeapons == false)
	{
		vector<LPGAMEOBJECT> coObjects;

		GetColliableObjects(whip, coObjects);

		whip->Update(dt, &coObjects);
	}
	else
		whip->SetTargetTypeHit(-1);

}

void PlayScene::Weapon_Update(DWORD dt, int index)
{
	if (player->GetSubWeapon() == WEAPON_STOP_WATCH)
		return;

	if (weaponlist[index]->GetScoreReceived() != 0)
	{
		player->AddScore(weaponlist[index]->GetScoreReceived());
		weaponlist[index]->SetScoreReceived(0);
	}

	if (weaponlist[index]->IsEnable() == false)
	{
		weaponlist[index]->SetTargetTypeHit(-1);
		return;
	}

	vector<LPGAMEOBJECT> coObjects;
	GetColliableObjects(weaponlist[index], coObjects);

	weaponlist[index]->Update(dt, &coObjects);
}

void PlayScene::GetColliableObjects(LPGAMEOBJECT curObj, vector<LPGAMEOBJECT>& coObjects)
{
	if (dynamic_cast<Items*>(curObj))
	{
		for (auto obj : listObjects)
		{
			if (dynamic_cast<Ground*>(obj))
				coObjects.push_back(obj);
		}
	}
	else if (dynamic_cast<Zombie*>(curObj))
	{
		for (auto obj : listObjects)
		{
			if (dynamic_cast<Ground*>(obj))
				coObjects.push_back(obj);
		}
	}
	else if (dynamic_cast<Knight*>(curObj))
	{
		for (auto obj : listObjects)
		{
			if (dynamic_cast<Ground*>(obj))
				coObjects.push_back(obj);
		}
	}
	else if (dynamic_cast<HunchBack*>(curObj))
	{
		for (auto obj : listObjects)
		{
			if (dynamic_cast<Ground*>(obj))
				coObjects.push_back(obj);
		}
	}
	else if (dynamic_cast<Skeleton*>(curObj))
	{
		for (auto obj : listObjects)
		{
			if (dynamic_cast<Ground*>(obj))
				coObjects.push_back(obj);
		}
	}
	else if (dynamic_cast<Weapon*>(curObj))
	{
		Weapon* weapon = dynamic_cast<Weapon*>(curObj);

		coObjects.push_back(player); // dùng để xét va chạm của Simon với boomerang

		if (isBossFighting == true && boss->GetState() == BOSS_ACTIVE && weapon->GetTargetTypeHit() != OBJECT_TYPE_BOSS)
			coObjects.push_back(boss);

		for (auto obj : listObjects)
		{
			if (dynamic_cast<Torch*>(obj) || dynamic_cast<Ground*>(obj) || dynamic_cast<BreakWall*>(obj))
				coObjects.push_back(obj);
			else if ((dynamic_cast<Knight*>(obj) || dynamic_cast<Bat*>(obj) || dynamic_cast<Ghost*>(obj) || dynamic_cast<HunchBack*>(obj) || dynamic_cast<Skeleton*>(obj) ||
				dynamic_cast<Raven*>(obj) || dynamic_cast<Zombie*>(obj) || dynamic_cast<Boss*>(obj)) && obj->GetState() == ACTIVE)
				coObjects.push_back(obj);
		}
	}
	else if (dynamic_cast<Whip*>(curObj))
	{
		for (auto obj : listObjects)
		{
			if (dynamic_cast<Torch*>(obj))
				coObjects.push_back(obj);
			if (dynamic_cast<BreakWall*>(obj))
				coObjects.push_back(obj);
			else if ((dynamic_cast<Knight*>(obj) || dynamic_cast<Bat*>(obj) || dynamic_cast<Ghost*>(obj) || dynamic_cast<HunchBack*>(obj) || dynamic_cast<Skeleton*>(obj) ||
				dynamic_cast<Raven*>(obj) || dynamic_cast<Zombie*>(obj) || dynamic_cast<Boss*>(obj)) && obj->GetState() == ACTIVE)
				coObjects.push_back(obj);
		}
	}
	else if (dynamic_cast<Simon*>(curObj))
	{
		for (auto obj : listObjects)
		{
			if (dynamic_cast<Portal*>(obj) || dynamic_cast<Ground*>(obj) || dynamic_cast<BlockMove*>(obj) || dynamic_cast<AreaDead*>(obj))
				coObjects.push_back(obj);
			else if(dynamic_cast<BreakWall*>(obj) && obj->GetState() == NORMAL)
				coObjects.push_back(obj);
			else if (player->isAutoWalk == false) // nếu simon auto-walk sẽ không xét va chạm với enemy
			{
				if (dynamic_cast<Bone*>(obj) && obj->IsEnable() == true)
					coObjects.push_back(obj);
				else if ((dynamic_cast<Knight*>(obj) || dynamic_cast<Bat*>(obj) || dynamic_cast<Ghost*>(obj) || dynamic_cast<HunchBack*>(obj) || dynamic_cast<Skeleton*>(obj) ||
					dynamic_cast<Raven*>(obj) || dynamic_cast<Zombie*>(obj) || dynamic_cast<Boss*>(obj)) && obj->GetState() == ACTIVE)
					coObjects.push_back(obj);
			}
		}
	}
}

void PlayScene::GetObjectFromGrid()
{
	listUnits.clear();
	listObjects.clear();
	listStairs.clear();
	listStaticObjectsToRender.clear();
	listMovingObjectsToRender.clear();

	grid->Get(Game::GetInstance()->GetCameraPositon(), listUnits);
	//DebugOut(L"%d \n", listUnits.size());

	for (UINT i = 0; i < listUnits.size(); i++)
	{
		LPGAMEOBJECT obj = listUnits[i]->GetObj();
		listObjects.push_back(obj);

		if (dynamic_cast<Ground*>(obj))
			continue;
		else if (dynamic_cast<Stair*>(obj))
			listStairs.push_back(obj);
		else if (dynamic_cast<Torch*>(obj) || dynamic_cast<BreakWall*>(obj) || dynamic_cast<AreaDead*>(obj))
			listStaticObjectsToRender.push_back(obj);
		else
			listMovingObjectsToRender.push_back(obj);
	}
}

void PlayScene::UpdateGrid()
{
	for (int i = 0; i < listUnits.size(); i++)
	{
		LPGAMEOBJECT obj = listUnits[i]->GetObj();

		if (obj->IsEnable() == false)
			continue;

		float newPos_x, newPos_y;
		obj->GetPosition(newPos_x, newPos_y);
		listUnits[i]->Move(newPos_x, newPos_y);
	}
}

void PlayScene::SetEnemiesSpawnPositon()
{
	// Không spawn enemies khi hiệu ứng cross còn diễn ra
	if (GetTickCount() - crossTimeCounter < SIMON_INVISIBLE_TIME)
		return;

	Game* game = Game::GetInstance();
	for (auto obj : listObjects)
	{
		 if (dynamic_cast<Knight*>(obj))
		{
			Knight* knight = dynamic_cast<Knight*>(obj);

			if (knight->GetState() == KNIGHT_INACTIVE && knight->IsEnable() == true)
			{
				if (IsInViewport(knight) == true)
				{
					knight->SetState(KNIGHT_ACTIVE);
				}
			}
		}
		else if (dynamic_cast<Bat*>(obj))
		{
			Bat* bat = dynamic_cast<Bat*>(obj);

			if(bat->GetState() == BAT_INACTIVE && bat->IsEnable() == true)
			if (IsInViewport(bat) == true)
			{
				bat->SetState(BAT_IDLE);
			}
		}
		if (dynamic_cast<Ghost*>(obj))
		{
			Ghost* ghost = dynamic_cast<Ghost*>(obj);

			if (ghost->GetState() != GHOST_INACTIVE && ghost->isSettedPosition == false)
			{
				if (ghost->IsAbleToActivate() == true)
				{
					ghost->isSettedPosition = true;
					int nx = ghost->GetEntryPosition().x < player->x ? 1 : -1;
					ghost->SetOrientation(nx);
					ghost->SetState(GHOST_ACTIVE);
				}

				/*ghost->isSettedPosition = true;

				float simon_x, simon_y;
				player->GetPosition(simon_x, simon_y);

				int nx = ghost->GetEntryPosition().x < simon_x ? 1 : -1;
				ghost->SetOrientation(nx);

				ghost->SetState(GHOST_ACTIVE);*/
			}
		}
		else if (dynamic_cast<HunchBack*>(obj))
		{
			HunchBack* hunchback = dynamic_cast<HunchBack*>(obj);

			if (hunchback->GetState() == HUNCHBACK_INACTIVE && hunchback->isSettedPosition == false)
			{
				if (hunchback->IsAbleToActivate() == true)
				{
					int nx = hunchback->GetEntryPosition().x < player->x ? 1 : -1;
					hunchback->SetOrientation(nx);
					hunchback->SetState(HUNCHBACK_IDLE);
				}
			}
		}
		else if (dynamic_cast<Skeleton*>(obj))
		{
			Skeleton* skeleton = dynamic_cast<Skeleton*>(obj);

			if (skeleton->GetState() != SKELETON_INACTIVE && skeleton->isSettedPosition == false)
			{
				skeleton->isSettedPosition = true;

				float simon_x, simon_y;
				player->GetPosition(simon_x, simon_y);

				int nx = skeleton->GetEntryPosition().x < simon_x ? 1 : -1;
				skeleton->SetOrientation(nx);

				skeleton->SetState(SKELETON_JUMP);
			}
		}
		else if (dynamic_cast<Raven*>(obj))
		{
			Raven* raven = dynamic_cast<Raven*>(obj);

			if (raven->GetState() == RAVEN_INACTIVE)
			{
				if (raven->IsAbleToActivate() == true && IsInViewport(raven) == true)
				{
					int nx = raven->GetEntryPosition().x < player->x ? 1 : -1;
					raven->SetOrientation(nx);
					raven->SetState(HUNCHBACK_IDLE);
				}
			}
		}
	}
}

void PlayScene::CrossEffect()
{
	if (player->isGotCrossItem == true)
	{
		player->isGotCrossItem = false;
		isCrossEffect = true;
		crossTimeCounter = GetTickCount();

		for (UINT i = 0; i < listObjects.size(); i++)
		{
			// Cross chỉ tác dụng với các object nằm trong viewport
			if (IsInViewport(listObjects[i]) == false)
				continue;

			if (dynamic_cast<Knight*>(listObjects[i]) && listObjects[i]->GetState() == ACTIVE)
			{
				auto knight = dynamic_cast<Knight*>(listObjects[i]);
				knight->SetState(KNIGHT_DESTROYED);
			}
			else if (dynamic_cast<Bat*>(listObjects[i]) && listObjects[i]->GetState() == ACTIVE)
			{
				auto bat = dynamic_cast<Bat*>(listObjects[i]);
				bat->SetState(BAT_DESTROYED);
			}
			else if (dynamic_cast<Ghost*>(listObjects[i]) && listObjects[i]->GetState() == ACTIVE)
			{
				auto ghost = dynamic_cast<Ghost*>(listObjects[i]);
				ghost->SetState(GHOST_DESTROYED);
			}
			else if (dynamic_cast<HunchBack*>(listObjects[i]) && listObjects[i]->GetState() == ACTIVE)
			{
				auto hunchBack = dynamic_cast<HunchBack*>(listObjects[i]);
				hunchBack->SetState(HUNCHBACK_DESTROYED);
			}
			else if (dynamic_cast<Raven*>(listObjects[i]) && listObjects[i]->GetState() == ACTIVE)
			{
				auto raven = dynamic_cast<Raven*>(listObjects[i]);
				raven->SetState(RAVEN_DESTROYED);
			}
			else if (dynamic_cast<Skeleton*>(listObjects[i]) && listObjects[i]->GetState() == ACTIVE)
			{
				auto skeleton = dynamic_cast<Skeleton*>(listObjects[i]);
				skeleton->SetState(SKELETON_DESTROYED);
			}
			else if (dynamic_cast<Zombie*>(listObjects[i]) && listObjects[i]->GetState() == ACTIVE)
			{
				auto zombie = dynamic_cast<Zombie*>(listObjects[i]);
				zombie->SetState(ZOMBIE_DESTROYED);
			}
		}
	}
}

void PlayScene::DoubleShotEffect()
{
	if (player->isGotDoubleShotItem == true)
	{
		player->isGotDoubleShotItem = false;
		isDoubleShot = true;
		doubleShotTimeCounter = GetTickCount();
	}
}

void PlayScene::TripleShotEffect()
{
	if (player->isGotTripleShotItem == true)
	{
		player->isGotTripleShotItem = false;
		isTripleShot = true;
		tripleShotTimeCounter = GetTickCount();
	}
}

void PlayScene::ResetGame(int i)
{
	isGameReset = true;
	isGamePause = false;
	isSimonDead = false;
	isGameOver = false;
	

	whip->SetState(0);

	boss = new Boss();
	boss->SetState(BOSS_INACTIVE);

	Game::GetInstance()->SwitchScene(i);

}

void PlayScene::DoneGame()
{
	// 3 giai đoạn:
	// + Cộng HP
	// + Cộng điểm theo thời gian còn lại
	// + Cộng điểm theo energy còn lại
	if (player->GetHP() < 16)
	{
		if (GetTickCount() - delayTimeHP > 50)		// delay tăng máu
		{
			delayTimeHP = GetTickCount();
			player->AddHP(1);
		}

		return;
	}

	if (isNeedToAddScoreTime == -1)
	{
		isNeedToAddScoreTime = 0;
		return;
	}
	else if (isNeedToAddScoreTime == 0)
	{
		return;
	}

	if (player->GetEnergy() > 0)
	{
		player->LoseEnergy(1);
		player->AddScore(100);
		return;
	}

	if (isGameOver == false && delayTimeGameOver == 0)
	{
		delayTimeGameOver = GetTickCount();
	}
	else if (GetTickCount() - delayTimeGameOver > 3000)
	{
		isGameOver = true;
	}
}




void PlaySceneKeyHandler::KeyState(BYTE* state)
{
	Game* game = Game::GetInstance();
	Simon* simon = ((PlayScene*)scene)->GetPlayer();

	if (CanProcessKeyboard() == false)
		return;

	// nếu simon đang nhảy và chưa chạm đất
	if ((simon->GetState() == JUMP || simon->GetState() == IDLE) && simon->isTouchGround == false)
		return;

	// Xét trạng thái phím
	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (StairCollisionsDetection() == true && simon->isStandOnStair == true)
		{
			if (simon->stairDirection == 1)// cầu thang trái dưới - phải trên
			{
				SimonStairUp();
			}
			else 
			{
				SimonStairDown();
			}
			return;
		}
		simon->SetOrientation(1);
		simon->SetState(WALK);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (StairCollisionsDetection() == true && simon->isStandOnStair == true)
		{
			if (simon->GetStairDirection() == 1)// cầu thang trái dưới - phải trên
			{
				SimonStairDown();
			}
			else 
			{
				SimonStairUp();
			}
			return;
		}
		simon->SetOrientation(-1);
		simon->SetState(WALK);
	}
	else if (game->IsKeyDown(DIK_DOWN))
	{
		if (StairCollisionsDetection() == true)
		{
			SimonStairDown();
			return;
		}
		if (simon->isTouchGround == false || simon->isFalling == true)
		{
			simon->SetState(IDLE);
			return;
		}	
		simon->SetState(DUCK);
	}
	else if (game->IsKeyDown(DIK_UP))
	{
		if (StairCollisionsDetection() == true)
		{
			SimonStairUp();
			return;
		}
		simon->SetState(IDLE);
	}
	else
	{
		simon->isHitWeapons = false;

		if (StairCollisionsDetection() == true)
		{
			if (SimonStairStand() == true)
				return;
		}
		simon->SetState(IDLE);
	}
}

void PlaySceneKeyHandler::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	PlayScene* s = (PlayScene*)scene;
	
	if (KeyCode == DIK_P)						// Pause game
	{
		s->isGamePause = !(s->isGamePause);
		return;
	}

	if (s->isGameOver == true)
	{
		if (KeyCode == DIK_UP || KeyCode == DIK_DOWN)
			s->choiceGameOver = (s->choiceGameOver + 1) % 2;
		else if (KeyCode == DIK_A)
		{
			if (s->choiceGameOver == 0)		// Continue game
			{
				s->GetPlayer()->isGotMagicCrystalItem = false;
				s->GetPlayer()->SetLife(3);
				s->ResetGame(0);
			}
			else
			{
				HWND hWnd = GetActiveWindow();		// End game
				DestroyWindow(hWnd);
			}
		}

		return;
	}

	if (CanProcessKeyboard() == false)
		return;

	Simon* simon = s->GetPlayer();
	switch (KeyCode)
	{
	case DIK_1:
		Game::GetInstance()->SwitchScene(0);
		break;
	case DIK_2:
		Game::GetInstance()->SwitchScene(1);
		break;
	case DIK_3:
		Game::GetInstance()->SwitchScene(2);
		break;
	case DIK_4:
		Game::GetInstance()->SwitchScene(3);
		break;
	case DIK_5:
		Game::GetInstance()->SwitchScene(4);
		break;
	case DIK_6:
		Game::GetInstance()->SwitchScene(5);
		break;
	case DIK_7:
		simon->SetSubWeapon(0);
		break;
	case DIK_8:
		simon->SetSubWeapon(1);
		break;
	case DIK_9:
		simon->SetSubWeapon(2);
		break;
	case DIK_0:
		simon->SetSubWeapon(3);
		break;
	case DIK_Q:
		simon->SetSubWeapon(4);
		break;
	case DIK_W:
		simon->isGotDoubleShotItem = true;
		break;
	case DIK_E:
		simon->isGotTripleShotItem = true;
		break;
	case DIK_R:
		simon->isGotCrossItem = true;
		break;
	case DIK_T:
		simon->StartInvisible();
		break;
	case DIK_SPACE:
		SimonJump();
		break;
	case DIK_A:
		SimonHit();
		break;
	case DIK_S:
		SimonHitWeapon();
		break;
	case DIK_H:
		simon->SetState(DEAD);
		break;
	default:
		break;
	}
}

void PlaySceneKeyHandler::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}

void PlaySceneKeyHandler::SimonJump()
{
	Simon* simon = ((PlayScene*)scene)->GetPlayer();
	if (simon->isTouchGround == false || simon->IsHit() == true || SimonStairStand() == true)
		return;
	simon->SetState(JUMP);
}

void PlaySceneKeyHandler::SimonHit()
{
	Simon* simon = ((PlayScene*)scene)->GetPlayer();
	if (simon->IsHit() == true)
		return;

	if (simon->isFalling == true)
		return;

	if (simon->GetState() == IDLE || simon->GetState() == JUMP)
	{
		simon->SetState(STANDING);
	}
	else if (simon->GetState() == DUCK)
	{
		simon->SetState(DUCKING);
	}
	else if (simon->GetState() == ASCEND)
	{
		simon->SetState(ASCENDING);
	}
	else if (simon->GetState() == DESCEND)
	{
		simon->SetState(DESCENDING);
	}
}

void PlaySceneKeyHandler::SimonHitWeapon()
{
	PlayScene* playscene = (PlayScene*)scene;

	Simon* simon = playscene->GetPlayer();

	vector<Weapon*>* weaponlist = playscene->GetWeaponList();

	Weapon* weapon;

	if (simon->isFalling == true)
		return;

	if (simon->GetSubWeapon() == -1 || simon->GetEnergy() == 0) // không có vũ khí hoặc enery = 0
		return;

	if (simon->GetSubWeapon() == 4)
	{
		if (simon->GetEnergy() < 5)
			return;
		if (((PlayScene*)scene)->IsUsingStopWatch() == true) // đang sử dụng stop watch
			return;
		weaponlist->at(0)->SetEnable(false);
	}

	if (weaponlist->at(0)->IsEnable() == false)
		weapon = weaponlist->at(0);
	else if (weaponlist->at(1)->IsEnable() == false && (playscene->IsDoubleShot() || playscene->IsTripleShot()))
		weapon = weaponlist->at(1);
	else if (weaponlist->at(2)->IsEnable() == false && playscene->IsTripleShot())
		weapon = weaponlist->at(2);
	else 
		return;

	if (simon->GetState() == IDLE || simon->GetState() == JUMP || simon->GetState() == DUCK || simon->GetState() == ASCEND || simon->GetState() == DESCEND)
	{
		float sx, sy;

		simon->GetPosition(sx, sy);

		if (simon->GetState() == DUCK) 
			sy += 25.0f; // vị trí tay simon
		else 
			sy += 10.0f;
		if (simon->GetOrientation() < 0) 
			sx += 30.0f;

		weapon->SetPosition(sx, sy);
		weapon->SetOrientation(simon->GetOrientation());

		weapon->SetState(simon->GetSubWeapon());
		weapon->SetEnable(true);

		if (weapon->GetState() == WEAPON_STOP_WATCH)
		{
			simon->LoseEnergy(5);
			((PlayScene*)scene)->StartStopWatch();
		}
		else
		{
			simon->LoseEnergy(1);
			simon->isHitWeapons = true;
			SimonHit();
		}
	}
}

void PlaySceneKeyHandler::SimonStairDown()
{
	Simon* simon = ((PlayScene*)scene)->GetPlayer();

	int stairDirection = simon->stairDirection;

	if (simon->canMoveDownStair == false)
	{
		if (simon->isStandOnStair == true)
			simon->SetState(IDLE);
		else
			simon->SetState(DUCK);
		return;
	}

	// Auto-walk của Simon đi đến đúng đầu cầu thang rồi mới bước lên
	if (simon->IsStandOnStair() == false)
	{
		float stair_x, simon_x, temp_y;

		simon->GetStairCollided()->GetPosition(stair_x, temp_y);
		simon->GetPosition(simon_x, temp_y);

		if (stairDirection == -1) 
			stair_x -= 26.0f;

		if (stair_x < simon_x) 
			simon->SetOrientation(-1);
		else if (stair_x > simon_x) 
			simon->SetOrientation(1);
		else 
			return;

		simon->SetState(WALK);
		simon->vy = 0;
		simon->AutoWalk(stair_x - simon_x, DESCEND, -stairDirection);
		simon->isStandOnStair = true;
		return;
	}
	else
	{
		simon->SetOrientation(-simon->stairDirection);
		simon->SetState(DESCEND);
	}
	return;
}

void PlaySceneKeyHandler::SimonStairUp()
{
	Simon* simon = ((PlayScene*)scene)->GetPlayer();
	int stairDirection = simon->GetStairDirection();

	if (simon->canMoveUpStair == false)
	{
		if (simon->isStandOnStair == true)
		{
			int nx = simon->stairDirection;
			simon->SetOrientation(nx);
			simon->SetState(ASCEND);
			simon->AutoWalk(14 * nx, IDLE, nx);
		}
		return;
	}

	// Auto-walk của Simon đi đến đúng chân cầu thang rồi mới bước lên
	if (simon->isStandOnStair == false)
	{
		float stair_x, simon_x, temp_y;

		simon->GetStairCollided()->GetPosition(stair_x, temp_y);
		simon->GetPosition(simon_x, temp_y);

		if (stairDirection == 1) 
			stair_x -= 32.0f;
		else 
			stair_x += 5.0f;

		if (stair_x < simon_x) 
			simon->SetOrientation(-1);
		else if (stair_x > simon_x) 
			simon->SetOrientation(1);
		else 
			return;

		simon->SetState(WALK);
		simon->vy = 0;
		simon->AutoWalk(stair_x - simon_x, ASCEND, stairDirection);
		simon->SetStandOnStair(true);
		return;
	}
	else
	{
		simon->SetOrientation(stairDirection);
		simon->SetState(ASCEND);
	}
	return;
}

bool PlaySceneKeyHandler::SimonStairStand()
{
	Simon* simon = ((PlayScene*)scene)->GetPlayer();

	if (simon->GetState() == ASCEND || simon->GetState() == DESCEND ||simon->GetState() == ASCENDING || simon->GetState() == DESCENDING)
	{
		if (simon->GetState() == ASCENDING)
		{
			simon->SetState(ASCEND);
			isNeedToWaitingAnimation = false;
		}
		else if (simon->GetState() == DESCENDING)
		{
			simon->SetState(DESCEND);
			isNeedToWaitingAnimation = false;
		}
		simon->StandOnStair();
		simon->animation_set->at(ASCEND)->Reset();
		simon->animation_set->at(ASCEND_INVISIBLE)->Reset();
		simon->animation_set->at(DESCEND)->Reset();
		simon->animation_set->at(DESCEND_INVISIBLE)->Reset();

		return true;
	}
	return false;
}

bool PlaySceneKeyHandler::AnimationDelay()
{
	Game* game = Game::GetInstance();
	Simon* simon = ((PlayScene*)scene)->GetPlayer();

	if (isNeedToWaitingAnimation == true)
	{
		if (simon->GetState() == ASCEND && simon->animation_set->at(ASCEND)->IsOver(200) == false)
			return true;

		if (simon->GetState() == DESCEND && simon->animation_set->at(DESCEND)->IsOver(200) == false)
			return true;

		if (simon->GetState() == STANDING && simon->animation_set->at(STANDING)->IsOver(300) == false)
			return true;

		if (simon->GetState() == DUCKING && simon->animation_set->at(DUCKING)->IsOver(300) == false)
			return true;

		if (simon->GetState() == ASCENDING && simon->animation_set->at(ASCENDING)->IsOver(300) == false)
			return true;

		if (simon->GetState() == DESCENDING && simon->animation_set->at(DESCENDING)->IsOver(300) == false)
			return true;

		if (simon->GetState() == HURT && simon->animation_set->at(HURT)->IsOver(600) == false)
			return true;

		if (simon->GetState() == UPGRADE && simon->animation_set->at(UPGRADE)->IsOver(450) == false)
			return true;
	}
	else
	{
		// Đặt lại biến chờ render animation
		isNeedToWaitingAnimation = true;

		//Để tránh việc ở frame tiếp theo rơi vào trạng thái chờ render animation 
		//(vì animation == 200ms, một frame ~ 30ms nên sẽ phải bị chờ dù cho có biến = false),
		// do đó cần reset lại animation start time về 0
		simon->animation_set->at(ASCEND)->SetAniStartTime(0);
		simon->animation_set->at(DESCEND)->SetAniStartTime(0);
	}

	return false;
}

bool PlaySceneKeyHandler::CanProcessKeyboard()
{
	Game* game = Game::GetInstance();
	Simon* simon = ((PlayScene*)scene)->GetPlayer();

	if (((PlayScene*)scene)->isGamePause == true)
		return false;

	if (simon->GetState() == DEAD)
		return false;

	if (simon->isAutoWalk == true)
		return false;

	if (AnimationDelay() == true)
		return false;

	return true;
}

bool PlaySceneKeyHandler::StairCollisionsDetection()
{
	Simon* simon = ((PlayScene*)scene)->GetPlayer();
	vector<LPGAMEOBJECT>* listStairs = ((PlayScene*)scene)->GetListStairs();

	return simon->CheckCollisionWithStair(listStairs);
}


