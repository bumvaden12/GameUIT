#include "Tunnel.h"
void Tunnel::Render()
{
	//RenderBoundingBox();
}

void Tunnel::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	switch ((int)x)
	{
	case XTUNNEL_1:
		b =  TUNNEL_BBOX_HEIGHT_1;
		break;
	case XTUNNEL_2:
		b = TUNNEL_BBOX_HEIGHT_1;
		break;
	case XTUNNEL_3:
		b = TUNNEL_BBOX_HEIGHT_1;
		break;
	case XTUNNEL_4:
		b = TUNNEL_BBOX_HEIGHT_1;
		break;
	default:
		b = y;
		break;
	}
	r = x + TUNNEL_BBOX_WIDTH;
}