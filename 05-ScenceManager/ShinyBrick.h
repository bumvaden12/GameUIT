#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

#define SBRICK_STATE_IDLE 0
#define SBRICK_STATE_BREAK 100

class SBrick : public CGameObject
{
	void GetHit();
	bool broke = false;
	bool explode = false;
public: bool isWaitingForAni = false;
public:
	SBrick();
	void SetState(int state);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};