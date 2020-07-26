#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>
#include <algorithm>

#include "Debug.h"
#include "Game.h"
#include "Textures.h"
#include "Sprites.h"
#include "Animations.h"


using namespace std;


class GameObject;
typedef GameObject* LPGAMEOBJECT;

struct CollisionEvent;
typedef CollisionEvent* LPCOLLISIONEVENT;
struct CollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;

	CollisionEvent(float t, float nx, float ny, LPGAMEOBJECT obj = NULL)
	{
		this->t = t;
		this->nx = nx;
		this->ny = ny;
		this->obj = obj;
	}

	static bool compare(const LPCOLLISIONEVENT& a, LPCOLLISIONEVENT& b)
	{
		return a->t < b->t;
	}
};

class GameObject
{
public:
	float x;		// coordinate
	float y;

	float dx;		// distance
	float dy;

	float vx;		// velocity
	float vy;

	int nx;			// orientation (left / right)

	int state;		// state of object

	DWORD dt;

	LPANIMATION_SET animation_set;

	bool isEnable;


	bool isDroppedItem = false;

	GameObject();

	void SetPosition(float x, float y) { this->x = x; this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx; this->vy = vy; }
	void SetState(int state) { this->state = state; }

	void GetPosition(float& x, float& y) { x = this->x; y = this->y; }
	void GetSpeed(float& vx, float& vy) { vx = this->vx; vy = this->vy; }
	int GetState() { return this->state; }
	int GetOrientation() { return this->nx; }

	void RenderBoundingBox();

	virtual void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }

	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);

	// kiểm tra va chạm của 2 đối tượng (ex: whip and torch)
	bool AABB(
		float left_a, float top_a, float right_a, float bottom_a,
		float left_b, float top_b, float right_b, float bottom_b
	);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT>& coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult,
		float& min_tx,
		float& min_ty,
		float& nx,
		float& ny);


	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL, bool stopMoving = false);
	virtual void Render() = 0;

	void SetOrientation(int nx) { this->nx = nx; }

	void SetEnable(bool enable) { this->isEnable = enable; }
	bool IsEnable() { return this->isEnable; }

	virtual void GetActiveBoundingBox(float& left, float& top, float& right, float& bottom) {};
	bool IsDroppedItem() { return this->isDroppedItem; }
	void SetIsDroppedItem(bool x) { this->isDroppedItem = x; }
};

