#pragma once
#include "GameObject.h"
#include "Items.h"


#define SBRICK_STATE_IDLE 0
#define SBRICK_STATE_BREAK 100

#define BRICK_BBOX_WIDTH		16.0f
#define BRICK_VX_BREAK			0.001f
#define BRICK_VY_BREAK			0.00018f
#define BRICK_VY_START_BREAK	0.03f
class SBrick : public CGameObject
{
	void GetHit();
	
	bool explode = false;
	float dxBreak, dyBreak;
	float vxBreak, vyBreak;
	float xBreak, yBreak;
	bool TurnedOff = false;
	LPGAMEOBJECT items;
public: bool isWaitingForAni = false;
public:
	bool broke = false;
	SBrick(LPGAMEOBJECT item);
	void SetState(int state);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};