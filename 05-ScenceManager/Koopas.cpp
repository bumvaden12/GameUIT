#include "Koopas.h"
#include "Brick.h"
#include "Mario.h"
#include "platform.h"

CKoopas::CKoopas()
{
	SetState(KOOPAS_STATE_WALKING);
	isKillable = true;
}

void CKoopas::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPAS_STATE_DIE)
	{
		bottom = y + KOOPAS_BBOX_HEIGHT_DIE - KOOPAS_BBOX_CHANGE ;
	}
	else if (state == KOOPAS_STATE_SPINNING)
	{
		bottom = y + KOOPAS_BBOX_HEIGHT_DIE - KOOPAS_BBOX_CHANGE;
	}
	else
		bottom = y + KOOPAS_BBOX_HEIGHT;
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Koopas can interact with the world and to each of them too!
	// 
	if(! beingcarried)
	vy+= KOOPAS_GRAVITY*dt;
	

	if (vx < 0 && x < 0) {
		x = 0; vx = -vx;
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
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

		if (nx != 0)
		{
			vx = -vx;

		}
		if (ny != 0)
		{
			vy = 0;
		}
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CBrick*>(e->obj))
			{
				CBrick* brick = dynamic_cast<CBrick*>(e->obj);
				if (e->nx != 0)
				{
					if (nx < 0)
					{
						nx = 1;
						/*vx = KOOPAS_SPINNING_SPEED * dt;*/
					}
					else
					{
						nx = -1;
					/*	vx = -KOOPAS_SPINNING_SPEED * dt;*/
					}
				}
			}
			if (dynamic_cast<platform*>(e->obj))
			{
				platform* Platform = dynamic_cast<platform*>(e->obj);
				if (e->nx != 0)
				{
					if (nx < 0)
					{
						nx = 1;
					/*	vx = KOOPAS_SPINNING_SPEED * dt;*/
					}
					else
					{
						nx = -1;
					/*	vx = -KOOPAS_SPINNING_SPEED * dt;*/
					}
				}
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CKoopas::Render()
{
	int ani = -1;
	if (state == KOOPAS_STATE_DIE) {
		if (flip)
			ani = KOOPAS_ANI_DIE_UP;
		else ani = KOOPAS_ANI_DIE;
	}
	else if (state == KOOPAS_STATE_SPINNING)
	{
		if (flip) ani = KOOPAS_ANI_SPINING_UP;
		else  ani = KOOPAS_ANI_SPINNING;
	}
	else if (vx > 0) ani = KOOPAS_ANI_WALKING_RIGHT;
	else if (vx <= 0) ani = KOOPAS_ANI_WALKING_LEFT;

	animation_set->at(ani)->Render(x, y);

	
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_DIE:
		/*y += KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_DIE +1;*/
		vx = 0;
		vy = 0;
		break;
	case KOOPAS_STATE_WALKING:
		if(nx <0)
		vx = -KOOPAS_WALKING_SPEED;
		else vx = KOOPAS_WALKING_SPEED;
		break;
	case KOOPAS_STATE_SPINNING:
		if(nx>0)
		vx = KOOPAS_SPINNING_SPEED;
		else vx = -KOOPAS_SPINNING_SPEED;
		break;
	}

}
void CKoopas::BeingCarry(LPGAMEOBJECT user)
{
	SetState(KOOPAS_STATE_DIE);
	if (user->nx > 0)
		SetPosition(user->x + 20, user->y+2);
	else SetPosition(user->x-3, user->y+2);
}
void CKoopas::GetHit()
{
	flip = true;
	state = KOOPAS_STATE_DIE;
	if (state == KOOPAS_STATE_DIE)
	{
		vy =	- 5 *dt;
	}
	else if (state != KOOPAS_STATE_DIE)
	{
		state = KOOPAS_STATE_DIE;
	}
}