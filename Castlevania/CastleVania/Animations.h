#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>

#include "Sprites.h"

/*	
	Sprite Animation Frame
*/
class AnimationFrame
{
	LPSPRITE sprite;
	DWORD time;				// time to render a sprite

public:
	AnimationFrame(LPSPRITE sprite, int time) { this->sprite = sprite; this->time = time; }
	LPSPRITE GetSprite() { return this->sprite; }
	DWORD GetTime() { return this->time; }
};

typedef AnimationFrame* LPANIMATION_FRAME;

class Animation
{
	DWORD lastFrameTime;
	int defaultTime;
	int currentFrame;
	vector<LPANIMATION_FRAME> frames;
	bool isOverAnimation = false;
public:
	Animation(int defaultTime = 100);
	bool IsOver() { return isOverAnimation; }
	void Reset() { isOverAnimation = false; }

	void Add(int spriteID, DWORD time = 0);
	void Render(int nx, float x, float y, int alpha = 255);
};

typedef Animation* LPANIMATION;

/*
	Manage all animations
*/
class Animations
{
	static Animations* _instance;
	unordered_map<int, LPANIMATION> animations;

public:
	void Add(int id, LPANIMATION ani) { animations[id] = ani; }
	LPANIMATION Get(int id) { return animations[id]; }
	void Clear();

	static Animations* GetInstance();
};;

typedef vector<LPANIMATION> AnimationSet;

typedef AnimationSet* LPANIMATION_SET;

/*
	Manage animation set database
*/
class AnimationSets
{
	static AnimationSets* __instance;

	unordered_map<int, LPANIMATION_SET> animation_sets;

public:
	
	void Add(int id, LPANIMATION_SET ani);
	LPANIMATION_SET Get(unsigned int id);


	static AnimationSets* GetInstance();
};
