#include "ShinyBrick.h"


void SBrick::Render()
{
	if (!TurnedOff)
		if (!broke)
			animation_set->at(0)->Render(x, y);
		else
		{
			animation_set->at(1)->Render(xBreak + dxBreak, yBreak + dyBreak);
			animation_set->at(1)->Render(xBreak - dxBreak, yBreak + dyBreak);
			animation_set->at(1)->Render(xBreak + dxBreak / 1.5, yBreak + dyBreak / 1.5);
			animation_set->at(1)->Render(xBreak - dxBreak / 1.5, yBreak + dyBreak / 1.5);
		}
	//RenderBoundingBox();
}
void SBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	if (broke) {
		vyBreak += BRICK_VX_BREAK * dt;
		vxBreak += BRICK_VY_BREAK * dt;

		dyBreak += vyBreak * dt;
		dxBreak += vxBreak * dt;
		x = -200; y = -200; vy = 0;
	}
	else {
		vyBreak = -BRICK_VY_START_BREAK * dt;
		xBreak = this->x;
		yBreak = this->y;
	}
	if (TurnedOff)
		SetPosition(0, 0);
}
void SBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_WIDTH;
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
SBrick::SBrick(LPGAMEOBJECT item)
{
	isKillable = true;
	this->items = item;
}
void SBrick::GetHit()
{
	explode = true;
	SetState(SBRICK_STATE_BREAK);

}