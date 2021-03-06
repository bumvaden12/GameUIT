#include "platform.h"
void platform::Render()
{
	//RenderBoundingBox();
}

void platform::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	switch ((int)x)
	{
	case XGROUND_1:
		r = x + GROUND_BBOX_WIDTH_1;
		break;
	case XGROUND_2:
		r = x + GROUND_BBOX_WIDTH_2;
		break;
	case XGROUND_3:
		r = x + GROUND_BBOX_WIDTH_3;
		break;
	case XGROUND_4:
		r = x + GROUND_BBOX_WIDTH_4;
		break;
	case XGROUND_5:
		r = x + GROUND_BBOX_WIDTH_5;
		break;
	case XGROUND_6:
		r = x + GROUND_BBOX_WIDTH_6;
		break;
	default:
		r = x;
		break;
	}
	b = y + GROUND_BBOX_HEIGHT;
}

Ground::Ground(float Width)
{
	this->Width = Width;
}
void Ground::Render()
{
	RenderBoundingBox();
}

void Ground::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + Width;
	b = y + GROUND_BBOX_HEIGHT;
}

Line::Line(float Width)
{
	this->Width = Width;
}
void Line::Render()
{
	RenderBoundingBox();
}

void Line::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + Width;
	b = y + LINE_BBOX_HEIGHT;
	X_Left = l;
	X_Right = r;
}