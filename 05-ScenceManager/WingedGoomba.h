#pragma once
#include "GameObject.h"

#define GOOMBA_WALKING_SPEED 0.05f;

#define WINGEDGOOMBA_BBOX_WIDTH 20
#define WINGEDGOOMBA_BBOX_HEIGHT 24
#define WINGEDGOOMBA_2_BBOX_HEIGHT 19

#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 16
#define GOOMBA_BBOX_HEIGHT_DIE 9

#define WINGEDGOOMBA_LEVEL_WING	0
#define WINGEDGOOMBA_LEVEL_NOWING	1

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200

#define WINGEDGOOMBA_ANI_WALK_WING 0
#define WINGEDGOOMBA_ANI_FLY_WING 1
#define WINGEDGOOMBA_ANI_WALK 2
#define WINGEDGOOMBA_ANI_DIE 3
class WingedGoomba : public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void GetHit();
	int last_vx;
	float base_y;
public:int level;
	bool onground;
	int jumpcount = 0;
public:
	WingedGoomba();
	virtual void SetState(int state);
};