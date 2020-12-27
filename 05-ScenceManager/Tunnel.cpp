#include "Tunnel.h"
Tunnel::Tunnel(float Height, int Sprite)
{
	this->Height = Height;
	this->SpriteTunnel = Sprite;
}
void Tunnel::Render()
{
	if (SpriteTunnel != -1)
		animation_set->at(SpriteTunnel)->Render(x, y);
	//RenderBoundingBox();
}

void Tunnel::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + TUNNEL_BBOX_WIDTH;
	b = y + Height;
}