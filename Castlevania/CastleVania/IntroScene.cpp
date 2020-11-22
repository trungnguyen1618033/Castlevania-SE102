#include "IntroScene.h"
#include <iostream>
#include <fstream>

#define OBJECT_TYPE_INTROSCEEN		0
#define OBJECT_TYPE_CLOUD			1
#define OBJECT_TYPE_INTROBAT		2


void IntroScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut("--> %s\n",ToWSTR(line).c_str());

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
			DebugOut("[ERROR] SIMON object was created before! ");
			return;
		}
		player = Simon::GetInstance();
		player->SetPosition(x, y);
		player->SetAnimationSet(ani_set);
		player->SetOrientation(-1);
		player->SetState(WALK);
		player->vx = -0.05;
		player->AutoWalk(-230, BEHIND, -1);
		break;
	case OBJECT_TYPE_CLOUD:
		cloud = new Cloud;
		cloud->SetPosition(x, y);
		cloud->SetAnimationSet(ani_set);
		break;
	case OBJECT_TYPE_INTROBAT:
	{
		auto* bat = new BatIntro();
		x += rand() % 300;
		y += rand() % 100;
		bat->SetPosition(x, y);
		bat->SetAnimationSet(ani_set);
		bats.push_back(bat);
		break;
	}
	default:
		DebugOut("[ERR] Invalid object type: %d\n", object_type);
		return;
	}

}

IntroScene::IntroScene(int id, LPCWSTR filePath) : Scene(id, filePath)
{
	key_handler = new IntroSceneKeyHandler(this);

}

IntroScene::~IntroScene()
{
	for (int i = 0; i < 3; i++)
		delete bats[i];

	bats.clear();

	delete cloud;
}

void IntroScene::Load()
{
	DebugOut("[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines

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
		}
	}

	f.close();

	DebugOut("[INFO] Done loading scene resources %s\n", sceneFilePath);

	view = Textures::GetInstance()->Get(1);
}

void IntroScene::Update(DWORD dt)
{
	player->Update(dt);
	cloud->Update(dt);
	for (auto bat : bats)
		bat->Update(dt);
}

void IntroScene::Render()
{
	Game::GetInstance()->Draw(0, 0, 0, 0, view, 0, 0, 512, 448);

	player->Render();
	cloud->Render();
	for (auto bat : bats)
		bat->Render();
}

void IntroScene::Unload()
{
	player = NULL;
}

void IntroSceneKeyHandler::KeyState(BYTE* states)
{
	Game* game = Game::GetInstance();
	Simon* simon = ((IntroScene*)scene)->GetPlayer();
	if (simon->GetState() == BEHIND && simon->animation_set->at(BEHIND)->IsOver(3000) == true)
		Game::GetInstance()->SwitchScene(0);
}

void IntroSceneKeyHandler::OnKeyDown(int KeyCode)
{
}

void IntroSceneKeyHandler::OnKeyUp(int KeyCode)
{
}
