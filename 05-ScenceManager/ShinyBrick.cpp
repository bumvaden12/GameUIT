#include "ShinyBrick.h"

SBrick::SBrick()
{
	isKillable = true;
}
void SBrick::Render()
{
	if(broke == false)
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void SBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}
void SBrick::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SBRICK_STATE_IDLE:
		break;
	case SBRICK_STATE_BREAK:
		explode = true;
		broke = true;
		x = 0;
		y = -10000;
		break;
	}
}
void SBrick::GetHit()
{
	explode = true;
	SetState(SBRICK_STATE_BREAK);

}