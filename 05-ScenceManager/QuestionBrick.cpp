#include "QuestionBrick.h"
#include "Utils.h"

void QBrick::Render()
{
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
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
		vy = -0.1 * dt;
	}
	if (start_moving_down == true)
	{
		vy = 0.1 * dt;
	}
	if (base_y - y > 5)
	{
		start_moving_up = false;
		start_moving_down = true;
	}
	if (y >= base_y)
	{
		start_moving_down = false;
		vy = 0;
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
	DebugOut(L"siZE %d \n", coEvents.size());
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0)
		{
			vx = 0;

		}
		if (ny != 0)
		{
			if (ny < 0)
			{
				base_y = y;
				start_moving_up = true;
			}

		}
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}
