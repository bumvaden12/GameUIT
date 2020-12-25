#include "QuestionBrick.h"
#include "Utils.h"
QBrick::QBrick()
{
	isKillable = true;
}
void QBrick::Render()
{
	/*RenderBoundingBox();*/
	int ani = QBRICK_ANI_FULL;
	if (disabled == true)
		ani = QBRICK_ANI_EMPTY;
	animation_set->at(ani)->Render(x, y);
	
}

void QBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}

void QBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	if (start_moving_up == true)
	{
		vy = -0.005 * dt;
	}
	if (base_y - y > 7 && start_moving_up == true)
	{
		start_moving_up = false;
		start_moving_down = true;
	}
	if (start_moving_down == true)
		{
			vy = 0.005 * dt;
		}
	if (y  > base_y  && start_moving_down == true)
	{
		start_moving_down = false;
			vy = 0;
			y = base_y;
			disabled = true;
	}
	y += dy;

}
void QBrick::GetHit()
{
	disabled = true;
}