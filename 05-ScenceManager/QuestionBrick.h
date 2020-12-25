#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

#define QBRICK_ANI_FULL 0
#define QBRICK_ANI_EMPTY 1

class QBrick : public CGameObject
{
public:	bool start_moving_up = false;
public :bool start_moving_down = false;
	float base_y;
public:	bool disabled = false;
public:
	QBrick();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetHit();
};