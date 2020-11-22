#include "Animations.h"

Animation::Animation(int defaultTime)
{
	this->defaultTime = defaultTime;
	lastFrameTime = -1;
	currentFrame = -1;
}

void Animation::Add(int spriteID, DWORD time)
{
	if (time == 0) time = this->defaultTime;

	LPSPRITE sprite = Sprites::GetInstance()->Get(spriteID);
	LPANIMATION_FRAME frame = new AnimationFrame(sprite, time);
	frames.push_back(frame);
}

void Animation::Render(int accordingCam, int nx, float x, float y, int alpha)
{
	DWORD now = GetTickCount();

	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		
		if (now - lastFrameTime >= t) {
			currentFrame++;
			lastFrameTime = now;

			if (currentFrame >= frames.size())
			{
				currentFrame = 0;
			}
		}
		
	}

	frames[currentFrame]->GetSprite()->Draw(accordingCam, nx, x, y, alpha);
	//DebugOut("[INFO] State: %d\n", ani);
}

void Animation::RenderByID(int currentID, int nx, float x, float y, int alpha)
{
	if (frames.size() <= 5) // normal whip, short chain
	{
		frames[currentID]->GetSprite()->Draw(1, nx, x, y, alpha);
	}
	else  // == 12, long chain
	{
		int rd = rand() % 4;

		frames[currentID * 4 + rd]->GetSprite()->Draw(1, nx, x, y, alpha);
	}
}


Animations* Animations::_instance = NULL;

Animations* Animations::GetInstance()
{
	if (_instance == NULL) _instance = new Animations();
	return _instance;
}

void Animations::Clear()
{
	for (auto x : animations)
	{
		LPANIMATION ani = x.second;
		delete ani;
	}

	animations.clear();
}


AnimationSets* AnimationSets::__instance = NULL;

AnimationSets* AnimationSets::GetInstance()
{
	if (__instance == NULL) __instance = new AnimationSets();
	return __instance;
}
	
LPANIMATION_SET AnimationSets::Get(unsigned int id)
{
	LPANIMATION_SET ani_set = animation_sets[id];
	if (ani_set == NULL)
		DebugOut("[ERROR] Failed to find animation set id: %d\n", id);

	return ani_set;
}


void AnimationSets::Add(int id, LPANIMATION_SET ani_set)
{
	animation_sets[id] = ani_set;
}
