#include "fire.h"
#include "Utils.h"
#include "Mario.h"
#include "Goomba.h"
#include "Koopas.h"
#include "InviBrick.h"
#include "Brick.h"
#include "Game.h"
#include "platform.h"
fire::fire(float x, float y):CGameObject()
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	animation_set = animation_sets->Get(5);
	this->x = x;
	this->y = y;
	SetState(FIRE_STATE_WAITING);
}
fire::~fire()
{

}
void fire::Attack(LPGAMEOBJECT user)
{
	float obj_x, obj_y;
	user->GetPosition(obj_x, obj_y);
	if (user->nx == 1)
	{
		SetPosition(obj_x + MARIO_BIG_BBOX_WIDTH, obj_y + 5);
	}
	else {
		SetPosition(obj_x + 8, obj_y+3);
	}
	SetState(FIRE_STATE_BOUNCING);
	nx = user->nx;	
	vx = 0;
	vy = 0;

}
void fire::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 
	if (allow_fire)
	{
		if (fire_onground)allow_bouncing = true;
		if (vy > FIRE_JUMP_SPEED)
			vy = FIRE_JUMP_SPEED;
		else if (vy < -FIRE_JUMP_SPEED)
			vy = -FIRE_JUMP_SPEED;
		if (nx == 1)
			vx = FIRE_FOWARD_SPEED;
		else vx = -FIRE_FOWARD_SPEED;
		if (allow_bouncing == true )
		{
			vy = -FIRE_JUMP_SPEED;
			fire_onground = false;
		}
		else 		vy = FIRE_JUMP_SPEED;
		if (bounce_y - y > FIRE_JUMP_LIMIT)
		{
			allow_bouncing = false;
		}

		if (this->x > CGame::GetInstance()->GetCamPosX() + CGame::GetInstance()->GetScreenWidth())
		{
			SetState(FIRE_STATE_WAITING);
		}
		else if (this->x < CGame::GetInstance()->GetCamPosX() - CGame::GetInstance()->GetScreenWidth() / 3)
		{
			SetState(FIRE_STATE_WAITING);
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

			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

			x += min_tx * dx + nx * 0.4f;
			y += min_ty * dy + ny * 0.4f;

			if (nx != 0) vx = 0;
			if (ny != 0)
			{
				vy = 0;
				if (ny < 0)
				{
					fire_onground = true;
					bounce_y = y;
				}
			}

			//
			// Collision logic with other objects
			//
			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];
				if (dynamic_cast<InviBrick*>(e->obj))
				{
					if (e->nx != 0)
					{
						x += dx;
					}
					if (e->ny < 0)
					{

					}
					if (e->ny == 1)
					{
						y += dy;
					}

				}
				if (dynamic_cast<CBrick*>(e->obj))
				{
					CBrick* brick = dynamic_cast<CBrick*>(e->obj);
					if (e->ny < 0)
					{
					}
					if (e->ny > 0)
					{
					}
					if (e->nx != 0)
					{
						state = FIRE_STATE_WAITING;
						allow_fire = false;
					}
				}
				if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
				{
					CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						allow_fire = false;
						SetState(FIRE_STATE_EXPLODE);
					}
					else
					{
						x += dx;
						y += dy;
					}
				}
				if (dynamic_cast<platform*>(e->obj))
				{
					if (e->nx != 0)
					{
						state = FIRE_STATE_WAITING;
					}
					if (e->ny < 0)
					{

					}
					if (e->ny == 1)
					{
						y += dy;
					}
				}
				///////
				else if (dynamic_cast<CKoopas*>(e->obj)) // if e->obj is Koopas
				{
					CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
					// jump on top >> kill Goomba and deflect a bit 

					if (koopas->GetState() != KOOPAS_STATE_DIE)
					{
						koopas->SetState(KOOPAS_STATE_DIE);
						allow_fire = false;
						SetState(FIRE_STATE_EXPLODE);
					}
					else
					{
						x += dx;
						y += dy;
					}
				}
			}
		}
	}
	else
	{
	vx = 0;
	vy = 0;
 }
 CheckStateEnd();
}
void fire::SetPosition(float x, float y)
{
	CGameObject::SetPosition(x, y);
}
void fire::Render()
{
	if (state != FIRE_STATE_WAITING)
	{
		int ani = -1;
		if (state == FIRE_STATE_EXPLODE)
		{
			ani = FIRE_ANI_EXPLODE;

		}
		else if (state == FIRE_STATE_BOUNCING && nx > 0)
			ani = FIRE_ANI_RIGHT;
		else if (state == FIRE_STATE_BOUNCING && nx < 0)
			ani = FIRE_ANI_LEFT;
		animation_set->at(ani)->Render(x, y);
	}
}

void fire::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	top = y;
	left = x;
	right = left + FIRE_BBOX_WIDTH;
	bottom = top + FIRE_BBOX_HEIGHT;
}
void fire::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case FIRE_STATE_WAITING:
		allow_fire = FALSE;
		isFiring = false;
		break;
	case FIRE_STATE_EXPLODE:
		ResetAni(FIRE_ANI_EXPLODE);
		isWaitingForAniFire = true;
		break;
	case FIRE_STATE_BOUNCING:
		allow_fire = TRUE;
		isFiring = true;
	
	}
}
void fire::CheckStateEnd()
{
	if (isWaitingForAniFire)
	{
		if (animation_set->at(FIRE_ANI_EXPLODE)->IsOver())
		{
			isWaitingForAniFire = false;
			SetState(FIRE_STATE_WAITING);
		}
	}
}