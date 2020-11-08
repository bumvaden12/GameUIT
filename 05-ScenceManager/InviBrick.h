#pragma once
#include "GameObject.h"

#define INVIBRICK_BBOX_WIDTH  16
#define INVIBRICK_BBOX_HEIGHT 2

class InviBrick : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};