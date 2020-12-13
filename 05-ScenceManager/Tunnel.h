#pragma once
#include "GameObject.h"

#define TUNNEL_BBOX_WIDTH 32
#define TUNNEL_BBOX_HEIGHT_1 416


#define XTUNNEL_1 352
#define XTUNNEL_2 1794
#define XTUNNEL_3 1858	
#define XTUNNEL_4 2322	
class Tunnel : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
