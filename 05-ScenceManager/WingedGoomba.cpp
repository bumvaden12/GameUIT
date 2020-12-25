#include "WingedGoomba.h"
#include "Koopas.h"
#include "Brick.h"
#include "Tunnel.h"
#include "platform.h"
#include "Utils.h"
WingedGoomba::WingedGoomba()
{
	SetState(GOOMBA_STATE_WALKING);
	isKillable = true;
	level = WINGEDGOOMBA_LEVEL_WING;
}

void WingedGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + WINGEDGOOMBA_BBOX_WIDTH;

	if (state == GOOMBA_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
	else if (level == WINGEDGOOMBA_LEVEL_WING)
	{
		if (onground)bottom = y + WINGEDGOOMBA_2_BBOX_HEIGHT;
		else bottom = y + WINGEDGOOMBA_BBOX_HEIGHT;
	}
	else
		bottom = y + GOOMBA_BBOX_HEIGHT;
}

void WingedGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);


	vy += KOOPAS_GRAVITY * dt;
	if (level == WINGEDGOOMBA_LEVEL_NOWING)
	{
		if (state != GOOMBA_STATE_DIE)
		{
			if (vx < 0 && x < 0) {
				x = 0; vx = -vx;
			}

			if (vx > 0 && x > 290) {
				x = 290; vx = -vx;
			}

		}
		else
		{
			vx = 0;
		}
	}
	if (level == WINGEDGOOMBA_LEVEL_WING)
	{
		if (onground == true)
		{
			if (jumpcount < 3)
			{
				vy = -0.015 * dt;
			}
			else
			{ 
			vy = -0.025 * dt;
			jumpcount = 0;
			}
			onground = false;
			jumpcount++;
		}
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	if(state != GOOMBA_STATE_DIE)
	CalcPotentialCollisions(coObjects, coEvents);
	

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

		if (nx != 0) vx = last_vx;
		if (ny != 0)
		{
			vy = 0;
		}
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CBrick*>(e->obj))
			{
				if (e->nx != 0)
				{
					if (nx < 0)
					{
						nx = 1;	
					}
					else
					{
						nx = -1;
					}
				}
				if (e->ny < 0)
				{
					onground = true;
					base_y = y;
				}

			}
			if (dynamic_cast<platform*>(e->obj))
			{
				if (e->nx != 0)
				{
					if (nx < 0)
					{
						nx = 1;
					}
					else
					{
						nx = -1;
					}

				}
				if (e->ny < 0)
				{
					onground = true;
					base_y = y;
				}
			}
			if (dynamic_cast<Tunnel*>(e->obj))
			{
				if (e->nx != 0)
				{
					if (nx < 0)
					{
						nx = 1;
					}
					else
					{
						nx = -1;
					}
				}
				if (e->ny < 0)
				{
					onground = true;
					base_y = y;
				}
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void WingedGoomba::Render()
{
	int ani;
	if (level == WINGEDGOOMBA_LEVEL_WING)
	{
		if (y < base_y - 14)ani = WINGEDGOOMBA_ANI_FLY_WING;
		else ani = WINGEDGOOMBA_ANI_WALK_WING;
	}
	else if (state == GOOMBA_STATE_DIE)
		ani = WINGEDGOOMBA_ANI_DIE;
	else ani = WINGEDGOOMBA_ANI_WALK;
	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void WingedGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_DIE:
		y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case GOOMBA_STATE_WALKING:
		vx = -GOOMBA_WALKING_SPEED;
	}
}
void WingedGoomba::GetHit()
{
	state = GOOMBA_STATE_DIE;
}