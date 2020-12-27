#pragma once
#include "GameObject.h"

#define TUNNEL_BBOX_WIDTH		32

#define X_TUNNEL_1			352	
#define X_TUNNEL_2			1792
#define X_TUNNEL_3			1856
#define X_TUNNEL_4			2256
#define X_TUNNEL_5			2255
#define X_TUNNEL_6			2320

#define TUNNEL_BBOX_HIGHT_1	48
#define TUNNEL_BBOX_HIGHT_2	32
#define TUNNEL_BBOX_HIGHT_3	48
#define TUNNEL_BBOX_HIGHT_4	192
#define TUNNEL_BBOX_HIGHT_5	32
#define TUNNEL_BBOX_HIGHT_6	32
class Tunnel : public CGameObject
{
	float Height;
	int SpriteTunnel = -1;
public:
	Tunnel(float Height, int Sprite);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
