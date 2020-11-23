#pragma once

#include "GameObject.h"

/*
	Object that triggers scene switching
*/
class Portal : public GameObject
{
	int scene_id;	// target scene to switch to
	int scene_back;

	int width;
	int height;
public:
	Portal(float l, float t, float r, float b, int scene_id, int scene_back);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	int GetSceneId() { return scene_id; }
	int GetSceneBack() { return scene_back; }
};