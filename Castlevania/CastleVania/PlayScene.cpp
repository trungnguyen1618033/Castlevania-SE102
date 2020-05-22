﻿#include "PlayScene.h"
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

#define OBJECT_TYPE_SIMON	0
#define OBJECT_TYPE_WHIP	1
#define OBJECT_TYPE_TORCH	2
#define OBJECT_TYPE_KNIFE	3
#define OBJECT_TYPE_ITEMS	4
#define OBJECT_TYPE_HEART	5
#define OBJECT_TYPE_MONEYBAG	6
#define OBJECT_TYPE_GROUND	7
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

	GameObject* obj = NULL;

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
		player->SetWhipAnimationSet(animation_sets->Get(ani_set_id));
		break;
	case OBJECT_TYPE_TORCH: {
		obj = new Torch();
		break; 
	}
	case OBJECT_TYPE_GROUND: obj = new Ground(); break;
	case OBJECT_TYPE_PORTAL:
		{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		obj = new Portal(x, y, r, b, scene_id);
	}
		break;
	case OBJECT_TYPE_KNIFE: {
		obj = new Weapon();
		weapon = (Weapon*)obj;
		obj->isEnable = false;
		break;
	}
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}
	// General object setup
	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
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
		}
	}

	f.close();


	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);

	tilemaps->Add(0, FILEPATH_TEX_SCENE, FILEPATH_DATA_SCENE, 768, 192);
	tilemaps->Add(1, FILEPATH_TEX_SCENE_2, FILEPATH_DATA_SCENE_2, 1536, 192);
	tilemaps->Add(2, FILEPATH_TEX_SCENE_3, FILEPATH_DATA_SCENE_3, 768, 192);

	

}

void PlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		if (objects[i]->isEnable == false)
			continue;
		else
			coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}


	// Update camera to follow simon
	float cx, cy;
	player->GetPosition(cx, cy);
	

	Game* game = Game::GetInstance();;

	if (cx > SCREEN_WIDTH / 2 && cx + SCREEN_WIDTH / 2 < tilemaps->Get(id)->GetMapWidth())
		game->SetCamPos(cx - SCREEN_WIDTH / 2, 0);

}

void PlayScene::Render()
{
	Game* game = Game::GetInstance();

	tilemaps->Get(id)->Draw(game->GetCameraPositon());
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->isEnable == false)
			continue;
		objects[i]->Render();
		/*objects[i]->RenderBoundingBox();*/
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

	if (simon->GetState() == DEATH && simon->animation_set->at(DEATH)->IsOver(200) == false)
		return;

	if (simon->GetState() == THROW && simon->animation_set->at(THROW)->IsOver(300) == false)
		return;

	if (game->IsKeyDown(DIK_RIGHT))
	{
		simon->nx = 1;
		simon->SetState(WALK);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		simon->nx = -1;
		simon->SetState(WALK);
	}
	else if (game->IsKeyDown(DIK_DOWN))
	{
		simon->SetState(DUCK);
	}
	else
	{
		simon->SetState(IDLE);
	}
}

void PlaySceneKeyHandler::OnKeyDown(int KeyCode)
{
	Simon* simon = ((PlayScene*)scene)->GetPlayer();
	Weapon* weapon = ((PlayScene*)scene)->GetKnife();
	switch (KeyCode)
	{
	case DIK_SPACE:
		if (simon->GetState() == JUMP || simon->GetState() == STANDING || simon->GetState() == DUCKING)
			return;
		simon->SetState(JUMP);
		break;
	case DIK_A:
		if (simon->GetState() == STANDING || simon->GetState() == DUCKING /*|| simon->GetState() == ASCENDING || simon->GetState() == DESCENDING*/)
			return;
		if (simon->GetState() == IDLE || simon->GetState() == JUMP)
		{
			simon->SetState(STANDING);
		}
		else if (simon->GetState() == DUCK)
		{
			simon->SetState(DUCKING);
		}
		break;
	case DIK_S:
		if (simon->hasKnife == false)
			return;
		if (simon->GetState() == IDLE || simon->GetState() == JUMP)
		{
			float sx, sy;
			simon->GetPosition(sx, sy);
			weapon->SetPosition(sx, sy + 4);
			weapon->SetOrientation(simon->GetOrientation());
			weapon->isEnable = true;
			simon->SetState(THROW);
		}
		break;
	case DIK_D:
		simon->SetState(DEATH);
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

