#include "Tail.h"
#include "Utils.h"
#include "Mario.h"
CTail::CTail()
{
}
CTail::~CTail()
{

}
void CTail::Attack(LPGAMEOBJECT user)
{
	float obj_x, obj_y;
	user->GetPosition(obj_x, obj_y);
	if (user->nx == 1)
		SetPosition(obj_x, obj_y);
	else
		SetPosition(obj_x + MARIO_BIG_BBOX_WIDTH, obj_y);
	nx = user->nx;
}
void CTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	for (int i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);
		if (obj->isKillable)
			if (this->IsColidingAABB(obj) && animation_set->at(state)->IsRenderingLastFrame())
			{
				obj->GetHit();
			}
	}
}
void CTail::SetPosition(float x, float y)
{
	CGameObject::SetPosition(x, y);
}
void CTail::Render()
{
	RenderBoundingBox();
}

void CTail::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	top = y+8;
	left = x;
	right = left + TAIL_BBOX_WIDTH;
	bottom = top + TAIL_BBOX_HEIGHT;
}