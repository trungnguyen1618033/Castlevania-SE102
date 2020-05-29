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

#define OBJECT_TYPE_SIMON	0
#define OBJECT_TYPE_WHIP	1
#define OBJECT_TYPE_TORCH	2
#define OBJECT_TYPE_WEAPON	4
#define OBJECT_TYPE_ITEMS	3
#define OBJECT_TYPE_ZOMBIE	5
#define OBJECT_TYPE_LEOPAD	6
#define OBJECT_TYPE_GROUND	7
#define OBJECT_TYPE_STAIR	8
#define OBJECT_TYPE_BAT		9
#define OBJECT_TYPE_PORTAL	50

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
		objects.push_back(obj);
		break;
	case OBJECT_TYPE_WHIP:
		obj = new Whip();
		whip = (Whip*)obj;
		obj->SetAnimationSet(ani_set);
		objects.push_back(obj);
		break;
	case OBJECT_TYPE_WEAPON:
		obj = new Weapon();
		weapon = (Weapon*)obj;
		obj->SetPosition(x, y);
		obj->SetAnimationSet(ani_set);
		obj->SetEnable(false);
		objects.push_back(obj);
		break;
	case OBJECT_TYPE_TORCH: 
		obj = new Torch();
		obj->SetPosition(x, y);
		obj->SetAnimationSet(ani_set);
		s = atof(tokens[4].c_str());
		obj->SetState(s);
		objects.push_back(obj);
		listTorchs.push_back(obj);
		break;
	case OBJECT_TYPE_ZOMBIE: 
		obj = new Zombie();
		obj->SetEntryPosition(x, y);
		obj->SetAnimationSet(ani_set);
		obj->SetState(ZOMBIE_INACTIVE);
		//obj->SetEnable(false);
		objects.push_back(obj);
		listZombies.push_back(obj);
		break;
	case OBJECT_TYPE_LEOPAD: 
		obj = new Leopad();
		obj->SetEntryPosition(x, y);
		obj->SetPosition(x, y);
		obj->SetEnable(false);
		obj->SetAnimationSet(ani_set);
		obj->SetState(LEOPAD_IDLE);
		objects.push_back(obj);
		listLeopads.push_back(obj);
		break;
	case OBJECT_TYPE_BAT: 
		obj = new Bat();
		obj->SetEntryPosition(x, y);
		obj->SetPosition(x, y);
		obj->SetAnimationSet(ani_set);
		obj->SetState(BAT_INACTIVE);
		objects.push_back(obj);
		listBats.push_back(obj);
		break;
	case OBJECT_TYPE_GROUND: 
		obj = new Ground();
		obj->SetPosition(x, y);
		obj->SetAnimationSet(ani_set);
		objects.push_back(obj);
		listGrounds.push_back(obj);
		break;
	case OBJECT_TYPE_STAIR:
		obj = new Stair();
		obj->SetPosition(x, y);
		obj->SetAnimationSet(ani_set);
		s = atof(tokens[4].c_str());
		obj->SetState(s);
		objects.push_back(obj);
		listStairs.push_back(obj);
		break;
	case OBJECT_TYPE_PORTAL:
		r = atof(tokens[4].c_str());
		b = atof(tokens[5].c_str());
		scene_id = atoi(tokens[6].c_str());
		obj = new Portal(x, y, r, b, scene_id);
		objects.push_back(obj);
		listPortals.push_back(obj);
		break;
	case OBJECT_TYPE_ITEMS:
		obj = new Items();
		items = (Items*)obj;
		items->SetAnimationSet(ani_set);
		break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}
	

}

void PlayScene::_ParseSection_TILEMAP(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int mapID = atoi(tokens[0].c_str());
	wstring path_img = ToWSTR(tokens[1]);
	wstring path_text = ToWSTR(tokens[2]);
	int W = atoi(tokens[3].c_str());
	int H = atoi(tokens[4].c_str());

	tilemaps->Add(mapID, path_img.c_str(), path_text.c_str(), W, H);
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

}

void PlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 


	SetInactivation();

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 0; i < objects.size(); i++)
	{
			LPGAMEOBJECT object = objects[i];

		SetDropItems(object);

		vector<LPGAMEOBJECT> coObjects;

		if (dynamic_cast<Simon*>(object))
		{
			for (auto candle : listTorchs)
			{
				if (candle->IsEnable() == false)
					continue;

				coObjects.push_back(candle);
			}

			for (auto ground : listGrounds)
			{
				if (ground->IsEnable() == false)
					continue;

				coObjects.push_back(ground);
			}

			for (auto zombie : listZombies)
			{
				if (zombie->GetState() == ZOMBIE_INACTIVE)
					continue;

				coObjects.push_back(zombie);
			}

			for (auto leopard : listLeopads)
			{
				if (leopard->GetState() == LEOPAD_INACTIVE)
					continue;

				coObjects.push_back(leopard);
			}

			for (auto bat : listBats)
			{
				if (bat->GetState() == BAT_INACTIVE)
					continue;

				coObjects.push_back(bat);
			}
			for (auto portal : listPortals)
			{
				if (portal->IsEnable() == false)
					continue;

				coObjects.push_back(portal);
			}

			player->Update(dt, &coObjects);
			player->CheckCollisionWithItem(&listItems);
			player->CheckCollisionWithEnemyActiveArea(&listZombies);
			player->CheckCollisionWithEnemyActiveArea(&listLeopads);
			player->CheckCollisionWithEnemyActiveArea(&listBats);
		}
		else if (dynamic_cast<Items*>(object))
		{
			object->Update(dt, &listGrounds);
		}
		else if (dynamic_cast<Whip*>(object))
		{
			if (player->IsGotChainItem() == true)
			{
				player->SetGotChainItem(false);
				whip->Upgrade();
			}

			// lấy vị trí và phương của simon cho whip
			float simon_x, simon_y;
			player->GetPosition(simon_x, simon_y);

			whip->SetOrientation(player->GetOrientation());
			whip->SetWhipPosition(D3DXVECTOR3(simon_x, simon_y, 0), player->IsStand());

			// chỉ xét va chạm khi render tới sprite cuối cùng của simon (vung tay tới)
			if ((player->GetState() == STANDING || player->GetState() == DUCKING ||
				player->GetState() == ASCENDING || player->GetState() == DESCENDING) &&
				player->animation_set->at(player->GetState())->IsRenderingLastFrame() == true &&
				player->IsHitSubWeapons() == false)
			{
				// Lấy vector coobjects
				for (auto candle : listTorchs)
				{
					if (candle->IsEnable() == false)
						continue;

					coObjects.push_back(candle);
				}

				for (auto zombie : listZombies)
				{
					if (zombie->GetState() == ZOMBIE_INACTIVE)
						continue;

					coObjects.push_back(zombie);
				}

				for (auto leopard : listLeopads)
				{
					if (leopard->GetState() == LEOPAD_INACTIVE)
						continue;

					coObjects.push_back(leopard);
				}

				for (auto bat : listBats)
				{
					if (bat->GetState() == BAT_INACTIVE)
						continue;

					coObjects.push_back(bat);
				}
				whip->Update(dt, &coObjects);
			}
		}
		else if (dynamic_cast<Weapon*>(object))
		{
			coObjects.push_back(player);

			for (auto ground : listGrounds)
			{
				if (ground->IsEnable() == false)
					continue;

				coObjects.push_back(ground);
			}

			if (weapon->IsEnable() == false)
				continue;

			for (auto candle : listTorchs)
			{
				if (candle->IsEnable() == false)
					continue;

				coObjects.push_back(candle);
			}

			for (auto zombie : listZombies)
			{
				if (zombie->GetState() == ZOMBIE_INACTIVE)
					continue;

				coObjects.push_back(zombie);
			}

			for (auto leopard : listLeopads)
			{
				if (leopard->GetState() == LEOPAD_INACTIVE)
					continue;

				coObjects.push_back(leopard);
			}

			for (auto bat : listBats)
			{
				if (bat->GetState() == BAT_INACTIVE)
					continue;

				coObjects.push_back(bat);
			}
			weapon->Update(dt, &coObjects);
		}
		else if (dynamic_cast<Zombie*>(object))
		{
			if (object->GetState() != ZOMBIE_INACTIVE)
				object->Update(dt, &listGrounds);
		}
		else if (dynamic_cast<Leopad*>(object))
		{
			if (object->GetState() != LEOPAD_INACTIVE)
				object->Update(dt, &listGrounds);
		}
		else if (dynamic_cast<Bat*>(object))
		{
			if (object->GetState() != BAT_INACTIVE)
			{
				bat = dynamic_cast<Bat*>(object);

				if (bat->IsSettedPosition() == false)
				{
					bat->SetIsSettedPosition(true);
					float bx, by;

					by = player->y + 30;
					Game* game = Game::GetInstance();;

					if (bat->GetOrientation() == -1) bx = game->GetCameraPositon().x + SCREEN_WIDTH - BAT_BBOX_WIDTH;
					else bx = game->GetCameraPositon().x;

					bat->SetPosition(bx, by);
				}

				bat->Update(dt, NULL);
			}
		}
		else
		{
			object->Update(dt, &coObjects);
		}
	}

	UpdateCameraPosition();
}

void PlayScene::Render()
{
	Game* game = Game::GetInstance();

	tilemaps->Get(id)->Draw(game->GetCameraPositon());

	for (auto torch : listTorchs)
	{
		if (torch->IsEnable() == false)
			continue;

		torch->Render();
		torch->RenderBoundingBox();
	}

	for (auto items : listItems)
	{
		if (items->IsEnable() == false)
			continue;

		items->Render();
		items->RenderBoundingBox();
	}

	for (auto zombie : listZombies)
	{
		if (zombie->GetState() == ZOMBIE_INACTIVE)
			continue;

		zombie->Render();
		zombie->RenderBoundingBox();
	}

	for (auto leopard : listLeopads)
	{
		if (leopard->GetState() == LEOPAD_INACTIVE)
			continue;

		leopard->Render();
		leopard->RenderBoundingBox();
	}

	for (auto bat : listBats)
	{
		if (bat->GetState() == BAT_INACTIVE)
			continue;

		bat->Render();
		bat->RenderBoundingBox();
	}

	player->Render();
	player->RenderBoundingBox();

	if (weapon->IsEnable() == true)
	{
		weapon->Render();
		weapon->RenderBoundingBox();
	}

	if ((player->GetState() == STANDING || player->GetState() == DUCKING ||
		player->GetState() == ASCENDING || player->GetState() == DESCENDING)
		&& player->IsHitSubWeapons() == false)
	{
		whip->Render(player->animation_set->at(player->GetState())->GetCurrentFrame());
		whip->RenderBoundingBox();
	}

	for (auto portal : listPortals)
	{
		if (portal->IsEnable() == false)
			continue;

		portal->Render();
	}
}

/*
	Unload current scene
*/
void PlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;
	listTorchs.clear();
	listStairs.clear();
	listGrounds.clear();
	listItems.clear();
	listZombies.clear();
	listLeopads.clear();
	listBats.clear();
	listPortals.clear();
	
}

void PlayScene::SetDropItems(LPGAMEOBJECT object)
{
	if (object->IsEnable() == false && dynamic_cast<Torch*>(object))
	{
		if (object->IsDroppedItem() == false)
		{
			object->SetIsDroppedItem(true);
			items = new Items();
			items->SetEnable(true);
			items->SetPosition(object->x, object->y);
			listItems.push_back(items);
			objects.push_back(items);
		}
	}
}

void PlayScene::SetInactivation()
{
	Game* game = Game::GetInstance();
	D3DXVECTOR2 entryViewPort = game->GetCameraPositon();

	for (auto zombie : listZombies)
	{
		Zombie* zb = dynamic_cast<Zombie*>(zombie);

		if (zb->GetState() == ZOMBIE_ACTIVE)
		{
			float zx, zy;
			zb->GetPosition(zx, zy);

			if (zx + ZOMBIE_BBOX_WIDTH < entryViewPort.x || zx > entryViewPort.x + SCREEN_WIDTH)
			{
				zb->SetState(ZOMBIE_INACTIVE);
			}
		}
	}

	for (auto leopard : listLeopads)
	{
		Leopad* bl = dynamic_cast<Leopad*>(leopard);

		if (bl->GetState() == LEOPAD_ACTIVE)
		{
			float lx, ly;
			bl->GetPosition(lx, ly);

			if (lx + LEOPAD_BBOX_WIDTH < entryViewPort.x || lx > entryViewPort.x + SCREEN_WIDTH)
			{
				bl->SetState(LEOPAD_INACTIVE);
			}
		}
	}

	for (auto bat : listBats)
	{
		Bat* vb = dynamic_cast<Bat*>(bat);

		if (vb->GetState() == BAT_ACTIVE && vb->IsSettedPosition() == true)
		{
			float bx, by;
			vb->GetPosition(bx, by);

			if (bx + BAT_BBOX_WIDTH < entryViewPort.x || bx > entryViewPort.x + SCREEN_WIDTH)
			{
				vb->SetState(BAT_INACTIVE);
			}
		}
	}
	if (weapon->IsEnable() == true)
	{
		float wx, wy;
		weapon->GetPosition(wx, wy);

		if (wx < entryViewPort.x || wx > entryViewPort.x + SCREEN_WIDTH ||
			wy > entryViewPort.y + SCREEN_HEIGHT)
		{
			weapon->SetEnable(false);
		}
	}
}

void PlayScene::UpdateCameraPosition()
{
	if (player->x > SCREEN_WIDTH / 2 &&
		player->x + SCREEN_WIDTH / 2 < tilemaps->Get(id)->GetMapWidth())
	{
		int min_col = 0;
		int max_col = tilemaps->Get(id)->GetMapWidth() / 32;
	
		if (player->x >= min_col * 32 + (SCREEN_WIDTH / 2 - 16) &&
			player->x <= max_col * 32 - (SCREEN_WIDTH / 2 - 16))
		{
			Game* game = Game::GetInstance();
			game->SetCamPos(player->x - SCREEN_WIDTH / 2, 0);
		}
	}
}




void PlaySceneKeyHandler::KeyState(BYTE* state)
{
	Game* game = Game::GetInstance();
	Simon* simon = ((PlayScene*)scene)->GetPlayer();

	if (simon->GetState() == JUMP && simon->IsTouchGround() == false)
		return;

	if (simon->GetState() == STANDING && simon->animation_set->at(STANDING)->IsOver(300) == false)
		return;

	if (simon->GetState() == DUCKING && simon->animation_set->at(DUCKING)->IsOver(300) == false)
		return;

	if (simon->GetState() == ASCENDING && simon->animation_set->at(ASCENDING)->IsOver(300) == false)
		return;

	if (simon->GetState() == DESCENDING && simon->animation_set->at(DESCENDING)->IsOver(300) == false)
		return;

	if (simon->GetState() == HURT && simon->animation_set->at(HURT)->IsOver(200) == false)
		return;

	if (simon->GetState() == UPGRADE && simon->animation_set->at(UPGRADE)->IsOver(300) == false)
		return;
	if (simon->GetState() == HIT && simon->animation_set->at(HIT)->IsOver(300) == false)
		return;

	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (StairCollisionsDetection() == true && simon->IsStandOnStair() == true)
		{
			if (simon->GetStairDirection() == 1) // cầu thang trái dưới - phải trên
			{
				Simon_Stair_Up();
			}
			else {
				Simon_Stair_Down();
			}

			return;
		}
		simon->nx = 1;
		simon->SetState(WALK);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (StairCollisionsDetection() == true && simon->IsStandOnStair() == true)
		{
			if (simon->GetStairDirection() == 1) // cầu thang trái dưới - phải trên
			{
				Simon_Stair_Down();
			}
			else {
				Simon_Stair_Up();
			}

			return;
		}
		simon->nx = -1;
		simon->SetState(WALK);
	}
	else if (game->IsKeyDown(DIK_DOWN))
	{
		if (StairCollisionsDetection() == true)
		{
			Simon_Stair_Down();
			return;
		}
		if (simon->IsTouchGround() == true)
			simon->SetState(DUCK);
		else simon->SetState(IDLE);
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
		simon->SetHitSubWeapons(false);

		if (StairCollisionsDetection() == true)
		{
			if (Simon_Stair_Stand() == true)
			{
				return;
			}
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
		simon->SetSubWeapon(0);
		break;
	case DIK_2:
		simon->SetSubWeapon(1);
		break;
	case DIK_3:
		simon->SetSubWeapon(2);
		break;
	case DIK_4:
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
	if (simon->GetState() == JUMP || simon->GetState() == STANDING || simon->GetState() == DUCKING)
		return;
	simon->SetState(JUMP);
}

void PlaySceneKeyHandler::Simon_Hit()
{
	Simon* simon = ((PlayScene*)scene)->GetPlayer();
	if (simon->GetState() == STANDING || simon->GetState() == DUCKING)
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
	Weapon* weapon = ((PlayScene*)scene)->GetWeapon();
	if (simon->GetState() == STANDING || simon->GetState() == DUCKING)
		return;

	if (simon->GetSubWeapon() == -1 || simon->GetEnergy() == 0)
		simon->SetState(HIT);

	if (simon->GetState() == IDLE || simon->GetState() == JUMP || simon->GetState() == DUCK || simon->GetState() == ASCEND || simon->GetState() == DESCEND)
	{
		float sx, sy;

		simon->GetPosition(sx, sy);

		if (simon->GetState() == DUCK) sy += 25.0f;
		else sy += 10.0f;

		if (simon->GetOrientation() < 0) sx += 30.0f;

		weapon->SetPosition(sx, sy);
		weapon->SetOrientation(simon->GetOrientation());

		weapon->SetState(simon->GetSubWeapon());
		weapon->SetEnable(true);

		simon->LoseEnergy(1);
		simon->SetState(HIT);
	}
}

void PlaySceneKeyHandler::Simon_Stair_Down()
{
	Simon* simon = ((PlayScene*)scene)->GetPlayer();
	int prevState = simon->GetState();
	int stairDirection = simon->GetStairDirection();

	if (simon->IsMovingDown() == false)
	{
		simon->SetState(IDLE);
		return;
	}

	// Auto-walk của Simon đi đến đúng đầu cầu thang rồi mới bước lên
	if (simon->IsStandOnStair() == false)
	{
		float stair_x, simon_x, temp_y;

		simon->GetStairCollided()->GetPosition(stair_x, temp_y);
		simon->GetPosition(simon_x, temp_y);

		if (stairDirection == -1) stair_x -= 28.0f;

		if (stair_x < simon_x) simon->SetOrientation(-1);
		else if (stair_x > simon_x) simon->SetOrientation(1);
		else return;

		simon->SetState(WALK);
		simon->vy = 0;
		/*simon->AutoWalk(stair_x - simon_x, STAIR_DOWN, -stairDirection);*/
		simon->SetStandOnStair(true);

		return;
	}
	else
	{
		simon->SetOrientation(-simon->GetStairDirection());
		simon->SetState(DESCEND);
	}

	return;
}

void PlaySceneKeyHandler::Simon_Stair_Up()
{
	Simon* simon = ((PlayScene*)scene)->GetPlayer();
	int prevState = simon->GetState();
	int stairDirection = simon->GetStairDirection();

	if (simon->IsMovingUp() == false)
	{
		if (prevState == ASCEND || prevState == DESCEND)
		{
			float sx, sy;
			simon->GetPosition(sx, sy);
			int nx = simon->GetStairDirection();
			simon->SetOrientation(nx);
			simon->SetState(ASCEND);
			simon->SetPosition(sx + nx * 5.0f, sy - 5.0f);
		}

		return;
	}

	// Auto-walk của Simon đi đến đúng chân cầu thang rồi mới bước lên
	if (simon->IsStandOnStair() == false)
	{
		float stair_x, simon_x, temp_y;

		simon->GetStairCollided()->GetPosition(stair_x, temp_y);
		simon->GetPosition(simon_x, temp_y);

		if (stairDirection == 1) stair_x -= 31.0f;
		else stair_x += 5.0f;

		if (stair_x < simon_x) simon->SetOrientation(-1);
		else if (stair_x > simon_x)  simon->SetOrientation(1);
		else return;

		simon->SetState(WALK);
		simon->vy = 0;
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
	return false;
}

bool PlaySceneKeyHandler::StairCollisionsDetection()
{
	Simon* simon = ((PlayScene*)scene)->GetPlayer();
	vector<LPGAMEOBJECT>* listStairs = ((PlayScene*)scene)->GetListStairs();

	return simon->CheckCollisionWithStair(listStairs);
}