#include "TitleScene.h"
#include <iostream>
#include <fstream>

TitleScene::TitleScene(int id, LPCWSTR filePath) : Scene(id, filePath)
{
	key_handler = new TitleSceneKeyHandler(this);
}

void TitleScene::SwitchScene()
{
}

void TitleScene::Load()
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
		}
	}

	f.close();

	DebugOut("[INFO] Done loading scene resources %s\n", sceneFilePath);

	view = Textures::GetInstance()->Get(1);
	bird = Animations::GetInstance()->Get(101);

}

void TitleScene::Update(DWORD dt)
{
}

void TitleScene::Render()
{
	Game::GetInstance()->Draw(0, 0, 0, 0, view, 0, 0, 514, 450);
	bird->Render(0, 0, 370, 210);

	if (bird->GetCurrentFrame() == 15)
		bird->SetCurrentFrame(12);
}

void TitleScene::Unload()
{
}


void TitleSceneKeyHandler::KeyState(BYTE* states)
{
}

void TitleSceneKeyHandler::OnKeyDown(int KeyCode)
{
	DebugOut("[INFO TitleSceneKey] KeyDown: %d\n", KeyCode);
	Game::GetInstance()->SwitchScene(-1);
	/*TitleScene* s = dynamic_cast<TitleScene*>(scene);
	s->SwitchScene();*/
}

void TitleSceneKeyHandler::OnKeyUp(int KeyCode)
{
}
