#include "InviBrick.h"

void InviBrick::Render()
{
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void InviBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + INVIBRICK_BBOX_WIDTH;
	b = y + INVIBRICK_BBOX_HEIGHT;
}