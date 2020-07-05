#include "PlayScene.h"
#include <iostream>
#include <fstream>

using namespace std;

PlayScene::PlayScene(int id, LPCWSTR filePath) :Scene(id, filePath)
{
	key_handler = new PlaySceneKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_TILEMAP	7

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

#define MAX_SCENE_LINE 1024

void PlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	Textures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void PlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = Textures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	Sprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void PlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new Animation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	Animations::GetInstance()->Add(ani_id, ani);
}

void PlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new AnimationSet();

	Animations* animations = Animations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	AnimationSets::GetInstance()->Add(ani_set_id, s);
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
	GameObject* obj = NULL;
	float r;
	float b;
	int scene_id;
	int s;

	switch (object_type)
	{
	case OBJECT_TYPE_SIMON:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] SIMON object was created before! ");
			return;
		}
		obj = new Simon();
		player = (Simon*)obj;
		obj->SetPosition(x, y);
		obj->SetAnimationSet(ani_set);
		//listObjects.push_back(obj);
		break;
	case OBJECT_TYPE_TORCH: 
		obj = new Torch();
		obj->SetPosition(x, y);
		obj->SetAnimationSet(ani_set);
		s = atof(tokens[4].c_str());
		obj->SetState(s);
		listObjects.push_back(obj);
		listStaticObjectsToRender.push_back(obj);
		break;
	case OBJECT_TYPE_GROUND: 
		obj = new Ground();
		obj->SetPosition(x, y);
		obj->SetAnimationSet(ani_set);
		listObjects.push_back(obj);
		break;
	case OBJECT_TYPE_STAIR:
		obj = new Stair();
		obj->SetPosition(x, y);
		obj->SetAnimationSet(ani_set);
		s = atof(tokens[4].c_str());
		obj->SetState(s);
		listObjects.push_back(obj);
		listStairs.push_back(obj);
		break;
	case OBJECT_TYPE_PORTAL:
		r = atof(tokens[4].c_str());
		b = atof(tokens[5].c_str());
		scene_id = atoi(tokens[6].c_str());
		obj = new Portal(x, y, r, b, scene_id);
		portal = (Portal*)obj;
		listObjects.push_back(obj);
		break;
	case OBJECT_TYPE_BLOCK:
		obj = new BlockMove();
		block = (BlockMove*)obj;
		obj->SetPosition(x, y);
		obj->SetAnimationSet(ani_set);
		listObjects.push_back(obj);
		listStaticObjectsToRender.push_back(obj);
		break;
	case OBJECT_TYPE_BREAK:
		obj = new BreakWall();
		obj->SetPosition(x, y);
		obj->SetAnimationSet(ani_set);
		listObjects.push_back(obj);
		listStaticObjectsToRender.push_back(obj);
		break;
	case OBJECT_TYPE_KNIGHT:
	{
		Knight* knight = new Knight();
		knight->SetEntryPosition(x, y);
		knight->SetAnimationSet(ani_set);
		knight->SetState(true);
		r = atof(tokens[4].c_str());
		knight->SetLeft(x);
		knight->SetRight(r);
		listObjects.push_back(knight);
		listMovingObjectsToRender.push_back(knight);
		break;
	}
	case OBJECT_TYPE_BAT:
	{
		Bat* bat = new Bat();
		bat->SetEntryPosition(x, y);
		bat->SetPosition(x, y);
		bat->SetAnimationSet(ani_set);
		bat->SetState(true);
		listObjects.push_back(bat);
		listMovingObjectsToRender.push_back(bat);
		break;
	}
	case OBJECT_TYPE_GHOST:
	{
		Ghost* ghost = new Ghost();
		ghost->SetEntryPosition(x, y);
		ghost->SetAnimationSet(ani_set);
		ghost->SetState(true);
		ghost->SetState(GHOST_INACTIVE);
		listObjects.push_back(ghost);
		listMovingObjectsToRender.push_back(ghost);
		break;
	}
	case OBJECT_TYPE_HUNCHBACK:
	{
		HunchBack* hunchback = new HunchBack();
		hunchback->SetEntryPosition(x, y);
		hunchback->SetAnimationSet(ani_set);
		hunchback->SetState(true);
		listObjects.push_back(hunchback);
		listMovingObjectsToRender.push_back(hunchback);
		break;
	}
	case OBJECT_TYPE_SKELETON:
	{
		Skeleton* skeleton = new Skeleton();
		skeleton->SetEntryPosition(x, y);
		skeleton->SetAnimationSet(ani_set);
		skeleton->SetState(true);
		listObjects.push_back(skeleton);
		listMovingObjectsToRender.push_back(skeleton);
		break;
	}
	case OBJECT_TYPE_RAVEN:
	{
		Raven* raven = new Raven();
		raven->SetEntryPosition(x, y);
		raven->SetAnimationSet(ani_set);
		raven->SetState(true);
		listObjects.push_back(raven);
		listMovingObjectsToRender.push_back(raven);
		break;
	}
	case OBJECT_TYPE_ZOMBIE:
	{
		Zombie* zombie = new Zombie();
		zombie->SetEntryPosition(x, y);
		zombie->SetAnimationSet(ani_set);
		zombie->SetState(true);
		listObjects.push_back(zombie);
		listMovingObjectsToRender.push_back(zombie);
		break;
	}
	case OBJECT_TYPE_BOSS:
		boss = new Boss();
		boss->SetEntryPosition(x, y);
		boss->SetAnimationSet(ani_set);
		boss->SetState(INACTIVE);
		boss->SetEnable(true);
		listObjects.push_back(boss);
		listMovingObjectsToRender.push_back(boss);
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

	/*Game* game = Game::GetInstance();*/
	
	DebugOut(L"ok_______\n");
	Game::GetInstance()->SetCamPos(x, y);
	/*game->SetCamPos(x, y);

	game->GetCameraPositon();*/
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
		if (line == "[TILEMAP]") { section = SCENE_SECTION_TILEMAP; continue; }
		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
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
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

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
		}
	}

	f.close();


	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);

	whip = new Whip();
	for (int i = 1; i <= 3; i++)
	{
		weapon = new Weapon();
		weapon->SetEnable(false);
		weaponlist.push_back(weapon);
	}

}

void PlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

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
		object->Update(dt, &coObjects);

		if (dynamic_cast<Boss*>(object))
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


	// Xoá các object đi ra khỏi vùng viewport
	SetInactivation();

	// update camera
	UpdateCameraPosition();
}

void PlayScene::Render()
{
	Game* game = Game::GetInstance();
	tilemaps->Get(id)->Draw(game->GetCameraPositon());

	for (auto obj : listStaticObjectsToRender)
	{
		if (obj->IsEnable() == false)
			continue;

		obj->Render();
		//obj->RenderBoundingBox();
	}
	for (auto obj : listStairs)
	{
		obj->RenderBoundingBox();
	}
	/*for (auto obj : listObjects)
	{
		if (obj->IsEnable() == false)
			continue;

		obj->RenderBoundingBox();
	}*/

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
		obj->RenderBoundingBox();
	}

	player->Render();
	//player->RenderBoundingBox();

	/*if (weapon->IsEnable() == true)
	{
		weapon->Render();
		weapon->RenderBoundingBox();
	}*/

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
	

	if(block != NULL)
		block->Render();
	portal->RenderBoundingBox();

}

/*
	Unload current scene
*/
void PlayScene::Unload()
{
	for (int i = 0; i < listObjects.size(); i++)
		delete listObjects[i];

	listObjects.clear();
	player = NULL;
	listStaticObjectsToRender.clear();
	listStairs.clear();
	listMovingObjectsToRender.clear();
	listItems.clear();
	portal = NULL;
	
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
		if (dynamic_cast<Torch*>(object) && object->GetState() == EFFECTEXPLODE )
		{
			if (object->animation_set->at(EFFECTEXPLODE)->IsOver(300) == true)
			{
				object->SetIsDroppedItem(true);
				items = new Items();
				items->SetEnable(true);
				items->SetPosition(object->x, object->y);
				listItems.push_back(items);
				listObjects.push_back(items);
			}
		}
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
			if (dynamic_cast<Zombie*>(object) && object->GetState() == ZOMBIE_ACTIVE)
			{
				auto zombie = dynamic_cast<Zombie*>(object);
				zombie->SetState(ZOMBIE_INACTIVE);
			}
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

	if (isBossFighting == true)	// Boss fight -> not moving camera
		return;

	if (game->GetChangeScene() == true)
		return;
	/*if (boss->GetState() == BOSS_ACTIVE)
	{
		isBossFighting = true;
		return;
	}*/

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

void PlayScene::Simon_Update(DWORD dt)
{
	vector<LPGAMEOBJECT> coObjects;

	GetColliableObjects(player, coObjects);

	player->Update(dt, &coObjects);
	player->CheckCollisionWithItem(&listItems);
	player->CheckCollisionWithEnemyActiveArea(&listObjects);
}

void PlayScene::Whip_Update(DWORD dt)
{
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
			else if ((dynamic_cast<Zombie*>(obj))
				&& obj->GetState() == ACTIVE)
				coObjects.push_back(obj);
			else if (dynamic_cast<Boss*>(obj) && obj->GetState() == BOSS_ACTIVE)
				coObjects.push_back(obj);
		}
	}
	else if (dynamic_cast<Whip*>(curObj))
	{
		for (auto obj : listObjects)
		{
			if (dynamic_cast<Torch*>(obj) || dynamic_cast<BreakWall*>(obj))
				coObjects.push_back(obj);
			else if ((dynamic_cast<Zombie*>(obj))
				&& obj->GetState() == ACTIVE)
				coObjects.push_back(obj);
			else if (dynamic_cast<Boss*>(obj) && obj->GetState() == BOSS_ACTIVE)
				coObjects.push_back(obj);
		}
	}
	else if (dynamic_cast<Simon*>(curObj))
	{
		for (auto obj : listObjects)
		{
			if (dynamic_cast<Portal*>(obj) || dynamic_cast<Ground*>(obj) || dynamic_cast<BlockMove*>(obj))
				coObjects.push_back(obj);
			else if(dynamic_cast<BreakWall*>(obj) && obj->GetState() == NORMAL)
				coObjects.push_back(obj);
		}
	}
}

void PlayScene::SetEnemiesSpawnPositon()
{
	Game* game = Game::GetInstance();
	for (auto obj : listObjects)
	{
		if (dynamic_cast<Zombie*>(obj))
		{
			Zombie* zombie = dynamic_cast<Zombie*>(obj);

			if (zombie->GetState() != ZOMBIE_INACTIVE && zombie->isSettedPosition == false)
			{
				zombie->isSettedPosition = true;

				float simon_x, simon_y;
				player->GetPosition(simon_x, simon_y);

				int nx = zombie->GetEntryPosition().x < simon_x ? 1 : -1;
				zombie->SetOrientation(nx);

				// Cần random một khoảng nhỏ để tránh việc các zombie spawn cùng lúc, tại cùng một vị trí
				int randomDistance = rand() % 30 + 10;

				float x, y;
				y = zombie->GetEntryPosition().y;
				
				if (nx == -1)
					x = game->GetCameraPositon().x + SCREEN_WIDTH - (ENEMY_DEFAULT_BBOX_WIDTH + randomDistance);
				else
					x = game->GetCameraPositon().x + (ENEMY_DEFAULT_BBOX_WIDTH + randomDistance);

				zombie->SetPosition(x, y);
				zombie->SetState(ZOMBIE_ACTIVE);
			}
		}
		else if (dynamic_cast<Knight*>(obj))
		{
			Knight* knight = dynamic_cast<Knight*>(obj);

			if (knight->GetState() == KNIGHT_INACTIVE)
			{
				if (knight->IsAbleToActivate() == true && IsInViewport(knight) == true)
				{
					knight->SetState(KNIGHT_ACTIVE);
				}
			}
		}
		else if (dynamic_cast<Bat*>(obj))
		{
			Bat* bat = dynamic_cast<Bat*>(obj);

			if (bat->IsAbleToActivate() == true && IsInViewport(bat) == true)
			{
				bat->SetState(BAT_IDLE);
			}
		}
		else if (dynamic_cast<HunchBack*>(obj))
		{
			HunchBack* hunchback = dynamic_cast<HunchBack*>(obj);

			if (hunchback->IsAbleToActivate() == true && IsInViewport(hunchback) == true)
			{
				hunchback->SetState(HUNCHBACK_IDLE);
			}
		}
		else if (dynamic_cast<Skeleton*>(obj))
		{
			Skeleton* skeleton = dynamic_cast<Skeleton*>(obj);

			if (skeleton->GetState() == SKELETON_INACTIVE)
			{
				if (skeleton->IsAbleToActivate() == true && IsInViewport(skeleton) == true
					&& abs(player->x - skeleton->GetEntryPosition().x) > 200)
				{
					int nx = skeleton->GetEntryPosition().x < player->x ? 1 : -1;
					skeleton->SetOrientation(-nx);

					skeleton->SetState(SKELETON_ACTIVE);
				}
			}
		}
		else if (dynamic_cast<Raven*>(obj))
		{
			Raven* raven = dynamic_cast<Raven*>(obj);

			if (raven->IsAbleToActivate() == true && IsInViewport(raven) == true)
			{
				int nx = raven->GetEntryPosition().x < player->x ? 1 : -1;
				raven->SetOrientation(nx);
				raven->SetState(RAVEN_IDLE);
			}
		}
		if (dynamic_cast<Ghost*>(obj))
		{
			Ghost* ghost = dynamic_cast<Ghost*>(obj);

			if (ghost->GetState() != GHOST_INACTIVE && ghost->isSettedPosition == false)
			{
				ghost->isSettedPosition = true;

				float simon_x, simon_y;
				player->GetPosition(simon_x, simon_y);

				int nx = ghost->GetEntryPosition().x < simon_x ? 1 : -1;
				ghost->SetOrientation(nx);

				float x, y;
				y = ghost->GetEntryPosition().y;

				if (nx == -1)
					x = game->GetCameraPositon().x + SCREEN_WIDTH - (ENEMY_DEFAULT_BBOX_WIDTH );
				else
					x = game->GetCameraPositon().x + (ENEMY_DEFAULT_BBOX_WIDTH );

				ghost->SetPosition(x, y);
				ghost->SetState(GHOST_ACTIVE);
			}
		}
	}
}




void PlaySceneKeyHandler::KeyState(BYTE* state)
{
	Game* game = Game::GetInstance();
	Simon* simon = ((PlayScene*)scene)->GetPlayer();

	if (simon->isAutoWalk == true)
		return ;

	if (isNeedToWaitingAnimation == true)
	{
		if ((simon->GetState() == JUMP || simon->GetState() == IDLE) && simon->isTouchGround == false)
			return;
		if (simon->GetState() == DEAD)
			return;
		if (simon->GetState() == ASCEND && simon->animation_set->at(ASCEND)->IsOver(200) == false)
			return;

		if (simon->GetState() == DESCEND && simon->animation_set->at(DESCEND)->IsOver(200) == false)
			return;

		if (simon->GetState() == STANDING && simon->animation_set->at(STANDING)->IsOver(300) == false)
			return;

		if (simon->GetState() == DUCKING && simon->animation_set->at(DUCKING)->IsOver(300) == false)
			return;

		if (simon->GetState() == ASCENDING && simon->animation_set->at(ASCENDING)->IsOver(300) == false)
			return;

		if (simon->GetState() == DESCENDING && simon->animation_set->at(DESCENDING)->IsOver(300) == false)
			return;

		if (simon->GetState() == HURT && simon->animation_set->at(HURT)->IsOver(600) == false)
			return;

		if (simon->GetState() == UPGRADE && simon->animation_set->at(UPGRADE)->IsOver(450) == false)
			return;
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

	// Xét trạng thái phím
	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (StairCollisionsDetection() == true && simon->isStandOnStair == true)
		{
			if (simon->stairDirection == 1)// cầu thang trái dưới - phải trên
			{
				Simon_Stair_Up();
			}
			else 
			{
				Simon_Stair_Down();
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
				Simon_Stair_Down();
			}
			else 
			{
				Simon_Stair_Up();
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
			Simon_Stair_Down();
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
			Simon_Stair_Up();
			return;
		}
		simon->SetState(IDLE);
	}
	else
	{
		simon->isHitWeapons = false;

		if (StairCollisionsDetection() == true)
		{
			if (Simon_Stair_Stand() == true)
				return;
		}

		simon->SetState(IDLE);
	}
}

void PlaySceneKeyHandler::OnKeyDown(int KeyCode)
{
	Simon* simon = ((PlayScene*)scene)->GetPlayer();
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
		//DebugOut(L"hello");
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
	case DIK_SPACE:
		Simon_Jump();
		break;
	case DIK_A:
		Simon_Hit();
		break;
	case DIK_S:
		Simon_Hit_Weapon();
		break;
	case DIK_H:
		simon->SetState(HURT);
		break;
	default:
		break;
	}
}

void PlaySceneKeyHandler::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}

void PlaySceneKeyHandler::Simon_Jump()
{
	Simon* simon = ((PlayScene*)scene)->GetPlayer();
	if (simon->isTouchGround == false || simon->IsHit() == true)
		return;
	simon->SetState(JUMP);
}

void PlaySceneKeyHandler::Simon_Hit()
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

void PlaySceneKeyHandler::Simon_Hit_Weapon()
{
	Simon* simon = ((PlayScene*)scene)->GetPlayer();

	vector<Weapon*>* weaponlist = ((PlayScene*)scene)->GetWeaponList();
	//Weapon* weapon;
	Weapon* weapon = ((PlayScene*)scene)->GetWeapon();

	if (simon->isFalling == true)
		return;

	if (simon->GetSubWeapon() == -1) // không có vũ khí 
		return;

	if (weaponlist->at(0)->IsEnable() == false)
		weapon = weaponlist->at(0);
	else if (weaponlist->at(1)->IsEnable() == false)
		weapon = weaponlist->at(1);
	else if (weaponlist->at(2)->IsEnable() == false)
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

		simon->isHitWeapons = true;
		Simon_Hit();
	}
}

void PlaySceneKeyHandler::Simon_Stair_Down()
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

void PlaySceneKeyHandler::Simon_Stair_Up()
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
			stair_x -= 31.0f;
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

bool PlaySceneKeyHandler::Simon_Stair_Stand()
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
		simon->animation_set->at(DESCEND)->Reset();
		return true;
	}
	return false;
}

bool PlaySceneKeyHandler::StairCollisionsDetection()
{
	Simon* simon = ((PlayScene*)scene)->GetPlayer();
	vector<LPGAMEOBJECT>* listStairs = ((PlayScene*)scene)->GetListStairs();

	return simon->CheckCollisionWithStair(listStairs);
}


