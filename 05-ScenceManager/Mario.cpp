#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"
#include "Brick.h"
#include "Goomba.h"
#include "Koopas.h"
#include "platform.h"
#include "QuestionBrick.h"
#include "ShinyBrick.h"
#include "Tunnel.h"
#include "WingedGoomba.h"
#include "Portal.h"

CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_FIRE;
	untouchable = 0;
	attackflag = 0;
	SetState(MARIO_STATE_IDLE);
	mario_x = x + 10;
	start_x =50 ; 
	start_y =350 ;
	this->x = x; 
	this->y = y; 
	tail = new CTail();
	Fire1 = new fire();
	Fire2 = new fire();
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);
	if (level != MARIO_LEVEL_MINI)
	{
		Fire1->Update(dt, coObjects);
		Fire2->Update(dt, coObjects);
		// Simple fall down
		//vy += MARIO_GRAVITY*dt;
		if (state == MARIO_STATE_TAIL_ATTACK)
			tail->Update(dt, coObjects);
		// jump
		if (allow_fly == true && (fly_y - y < MARIO_FLY_DISTANCE))
		{
			vy = -MARIO_FLY_ACCE * dt;
			onground = false;
		}
		
		else if (allow_jump&& !allow_fly)
		{
			vy = -MARIO_JUMP_ACCE * dt;
			onground = false;
		}
		else
		{
			allow_fly = false;
		}
	
		if (istaildropping == true && (vy > 0) && (y - drop_y < MARIO_TAILDROP_DISTANCE))
		{
			vy = MARIO_TAILDROP_SPEED * dt;
		}
		else
		{
			if ( !allow_fly && !StartTeleport)
			{
				vy += MARIO_GRAVITY * dt;
				istaildropping = false;
				isdropping = true;
			}
			else if(StartTeleport)
				vy += MARIO_GRAVITY_TELEPORT * dt;
		
		}
		if (stand_y - y > MARIO_MAX_JUMP_HEIGHT)
		{
			allow_jump = false;
		}
		if (state == MARIO_STATE_UNCROUCH)
		{
			y -= MARIO_TAIL_CROUCH_BBOX_HEIGHT * 0.6;
		}
		if (vx == MARIO_MAX_SPEED || vx == -MARIO_MAX_SPEED)
			maxspeed = true;
		else maxspeed = false;
		if (is_tail_attacking & (animation_set->at(MARIO_ANI_TAIL_ATTACK_RIGHT)->IsRenderingLastFrame() || animation_set->at(MARIO_ANI_TAIL_ATTACK_LEFT)->IsRenderingLastFrame()))
		{
			AttackWithTail();
			is_tail_attacking = false;
		}

		CheckForAniEnd();
		if (isCarrying)
		{
			koopas_carry->BeingCarry(this);
			//koopas_carry->SetState(KOOPAS_STATE_DIE);
		}
		if (isKicking)
		{
			if (nx > 0)
				koopas_carry->nx = 1;
			else koopas_carry->nx = -1;
			koopas_carry->SetState(KOOPAS_STATE_SPINNING);
			isKicking = false;
			koopas_carry->beingcarried = false;
		}
		if (!CGame::GetInstance()->IsKeyDown(DIK_D))
		{
			isCarrying = false;
		}
	}

		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();

		// turn off collision when die 
		if (state != MARIO_STATE_DIE)
			CalcPotentialCollisions(coObjects, coEvents);

		// reset untouchable timer if untouchable time has passed
		if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
		{
			untouchable_start = 0;
			untouchable = 0;
		}
		// No collision occured, proceed normally
		if (coEvents.size() == 0)
		{
			x += dx;
			y += dy;
		}
		else
		{
			stand_y = y;
			float min_tx, min_ty, nx = 0, ny;
			float rdx = 0;
			float rdy = 0;
			last_vy = vy;
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
				if (ny < 0)
				{
					isdropping = false;
					istaildropping = false;
				}
			}

			//
			// Collision logic with other objects
			//
			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];
				if (dynamic_cast<Line*>(e->obj))
				{
					if (e->nx != 0)
					{
						x += dx;
					}
					if (e->ny < 0)
					{
						onground = true;
						allow_fly = false;
						jumpflag = false;
						isdropping = false;
						istaildropping = false;
					}
					if (e->ny > 0)
					{
						y += dy;
					}

				}
				if (dynamic_cast<Tunnel*>(e->obj))
				{
					Tunnel* tunnel = dynamic_cast<Tunnel*>(e->obj);
					if (e->ny < 0)
					{
						onground = true;
						allow_fly = false;
						jumpflag = false;
						isdropping = false;
						istaildropping = false;
					}
				}
				if (dynamic_cast<CBrick*>(e->obj))
				{
					CBrick* brick = dynamic_cast<CBrick*>(e->obj);
					if (e->ny < 0)
					{
						onground = true;
						allow_fly = false;
						jumpflag = false;
						isdropping = false;
						istaildropping = false;
					}
					if (e->ny > 0)
					{
						allow_jump = false;
					}
				}
				if (dynamic_cast<QBrick*>(e->obj))
				{
					QBrick* Qbrick = dynamic_cast<QBrick*>(e->obj);
					if (e->ny < 0)
					{
						onground = true;
						allow_fly = false;
						jumpflag = false;
						isdropping = false;
						istaildropping = false;
					}
					if (e->ny > 0)
					{
						allow_jump = false;
						if (Qbrick->start_moving_up == false && Qbrick->disabled == false)
						{
							Qbrick->start_moving_up = true;
							Qbrick->base_y = Qbrick->y;
						}
					}
				}
				if (dynamic_cast<SBrick*>(e->obj))
				{
					SBrick* Sbrick = dynamic_cast<SBrick*>(e->obj);
					if (e->ny < 0)
					{
						onground = true;
						allow_fly = false;
						jumpflag = false;
						isdropping = false;
						istaildropping = false;
					}

				}
				if (dynamic_cast<platform*>(e->obj))
				{
					platform* Platform = dynamic_cast<platform*>(e->obj);
					if (e->ny < 0)
					{
						onground = true;
						allow_fly = false;
						jumpflag = false;
						isdropping = false;
						istaildropping = false;
					}
					if (e->ny > 0)
					{
						allow_jump = false;
					}
				}
				if (dynamic_cast<Ground*>(e->obj))
				{
					Ground* Platform = dynamic_cast<Ground*>(e->obj);
					if (e->ny < 0)
					{
						onground = true;
						allow_fly = false;
						jumpflag = false;
						isdropping = false;
						istaildropping = false;
					}
					if (e->ny > 0)
					{
						allow_jump = false;
					}
				}
				if (dynamic_cast<WingedGoomba*>(e->obj)) // if e->obj is Goomba 
				{

					WingedGoomba* goomba = dynamic_cast<WingedGoomba*>(e->obj);
					// jump on top >> kill Goomba and deflect a bit 
					if (e->ny < 0)
					{
						if (goomba->level == WINGEDGOOMBA_LEVEL_WING)
						{
							goomba->level = WINGEDGOOMBA_LEVEL_NOWING;
							vy = -MARIO_JUMP_DEFLECT_SPEED;
						}
						else if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							goomba->SetState(GOOMBA_STATE_DIE);
							vy = -MARIO_JUMP_DEFLECT_SPEED;
						}
					}
					else
					{

						if (untouchable == 0)
						{
							if (goomba->GetState() != GOOMBA_STATE_DIE)
							{
								/*if (level == MARIO_LEVEL_TAIL && state == MARIO_STATE_TAIL_ATTACK)
								{
									goomba->SetState(GOOMBA_STATE_DIE);
								}*/
								if (level == MARIO_LEVEL_TAIL)
								{
									level = MARIO_LEVEL_BIG;
									StartUntouchable();
								}
								else if (level == MARIO_LEVEL_BIG)
								{
									level = MARIO_LEVEL_SMALL;
									StartUntouchable();
								}
								else if (level == MARIO_LEVEL_FIRE)
								{
									level = MARIO_LEVEL_BIG;
									StartUntouchable();
								}
								else
									SetState(MARIO_STATE_DIE);
							}
						}
					}
				}
				if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
				{

					CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
					// jump on top >> kill Goomba and deflect a bit 
					if (e->ny < 0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							goomba->SetState(GOOMBA_STATE_DIE);
							vy = -MARIO_JUMP_DEFLECT_SPEED;
						}
					}
					else if (e->nx != 0)
					{
						if (untouchable == 0)
						{
							if (goomba->GetState() != GOOMBA_STATE_DIE)
							{
								/*if (level == MARIO_LEVEL_TAIL && state == MARIO_STATE_TAIL_ATTACK)
								{
									goomba->SetState(GOOMBA_STATE_DIE);
								}*/
								if (level == MARIO_LEVEL_TAIL)
								{
									level = MARIO_LEVEL_BIG;
									StartUntouchable();
								}
								else if (level == MARIO_LEVEL_BIG)
								{
									level = MARIO_LEVEL_SMALL;
									StartUntouchable();
								}
								else if (level == MARIO_LEVEL_FIRE)
								{
									level = MARIO_LEVEL_SMALL;
									StartUntouchable();
								}
								else
									SetState(MARIO_STATE_DIE);
							}
						}
					}
				}
				/*if (dynamic_cast<ShootingPlant*>(e->obj))
				{
					ShootingPlant* plant = dynamic_cast<ShootingPlant*>(e->obj);
					if (untouchable == 0)
					{
						if (level == MARIO_LEVEL_TAIL)
						{
							level = MARIO_LEVEL_BIG;
							StartUntouchable();
						}
						else if (level == MARIO_LEVEL_BIG)
						{
							level = MARIO_LEVEL_SMALL;
							StartUntouchable();
						}
						else if (level == MARIO_LEVEL_FIRE)
						{
							level = MARIO_LEVEL_SMALL;
							StartUntouchable();
						}
						else
							SetState(MARIO_STATE_DIE);
					}
				}*/
				if (dynamic_cast<fire_plant*>(e->obj))
				{
					fire_plant* fire = dynamic_cast<fire_plant*>(e->obj);
					if (untouchable == 0)
					{
						if (level == MARIO_LEVEL_TAIL)
						{
							level = MARIO_LEVEL_BIG;
							StartUntouchable();
						}
						else if (level == MARIO_LEVEL_BIG)
						{
							level = MARIO_LEVEL_SMALL;
							StartUntouchable();
						}
						else if (level == MARIO_LEVEL_FIRE)
						{
							level = MARIO_LEVEL_SMALL;
							StartUntouchable();
						}
						else
							SetState(MARIO_STATE_DIE);
					}
				}
				if (dynamic_cast<CKoopas*>(e->obj)) // if e->obj is Goomba 
				{
					CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
					// jump on top >> kill Goomba and deflect a bit 
					if (e->ny < 0 && koopas->GetLevel() == KOOPAS_TYPE_KOOPA_PARATROOPA_GREEN)
					{
						koopas->SetLevel(KOOPAS_TYPE_KOOPA_TROOPA_GREEN);
					}
					else if (e->ny < 0 && koopas->GetState() == KOOPAS_STATE_WALKING)
					{
						if (koopas->GetState() != KOOPAS_STATE_DIE)
						{
							koopas->SetState(KOOPAS_STATE_DIE);
							vy = -MARIO_JUMP_DEFLECT_SPEED;
						}
					}
					
					else if (e->nx != 0 && CGame::GetInstance()->IsKeyDown(DIK_D) && koopas->GetState() == KOOPAS_STATE_DIE)
					{
						isCarrying = true;
						koopas_carry = koopas;
						koopas_carry->beingcarried = true;
						isKicking1 = true;
						if (nx < 0)
						{
							ResetAni(MARIO_ANI_FIRE_KICK_LEFT);
							isWaitingForAni = true;
						}
						else
						{
							ResetAni(MARIO_ANI_FIRE_KICK_RIGHT);
							isWaitingForAni = true;
						}
					}
					else if (e->ny < 0 && koopas->GetState() == KOOPAS_STATE_SPINNING)
					{
						koopas->vx = 0;
						vy = -MARIO_JUMP_DEFLECT_SPEED * 1.3;

						koopas->SetState(KOOPAS_STATE_DIE);
					}
					else if (koopas->GetState() == KOOPAS_STATE_DIE)
					{
						/*vy = -MARIO_JUMP_DEFLECT_SPEED * 2;*/
						if (nx > 0)
							koopas->nx = -1;
						else koopas->nx = 1;
						koopas->SetState(KOOPAS_STATE_SPINNING);
					}
					else if (e->nx != 0 && koopas->GetState() == KOOPAS_STATE_SPINNING)
					{
						if (untouchable == 0)
						{
							if (koopas->GetState() != KOOPAS_STATE_DIE)
							{
								if (level == MARIO_LEVEL_TAIL)
								{
									level = MARIO_LEVEL_BIG;
									StartUntouchable();
								}
								else if (level == MARIO_LEVEL_BIG)
								{
									level = MARIO_LEVEL_SMALL;
									StartUntouchable();
								}
								else if (level == MARIO_LEVEL_FIRE)
								{
									level = MARIO_LEVEL_SMALL;
									StartUntouchable();
								}
								else
									SetState(MARIO_STATE_DIE);
							}
						}
					}

					else if (koopas->GetState() == KOOPAS_STATE_WALKING)
					{
						if (untouchable == 0)
						{
							if (state == MARIO_STATE_TAIL_ATTACK)
							{
								koopas->GetHit();
							}
							if (koopas->GetState() != KOOPAS_STATE_DIE)
							{
								if (level == MARIO_LEVEL_TAIL)
								{
									level = MARIO_LEVEL_BIG;
									StartUntouchable();
								}
								else if (level == MARIO_LEVEL_BIG)
								{
									level = MARIO_LEVEL_SMALL;
									StartUntouchable();
								}
								else if (level == MARIO_LEVEL_FIRE)
								{
									level = MARIO_LEVEL_SMALL;
									StartUntouchable();
								}
								else
									SetState(MARIO_STATE_DIE);
							}
						}
					}
				}
				else if (dynamic_cast<Items*>(e->obj))
				{
					Items* item = dynamic_cast<Items*>(e->obj);
					// jump on top >> kill Goomba and deflect a bit 
					if (e->ny < 0)
					{
						item->SetState(ITEM_SWITCH_STATE_OFF);
					}
				}
				else if (dynamic_cast<CPortal*>(e->obj))
				{
				vy = last_vy;
				y += dy;
				}
			}
		}
		//DebugOut(L"onground %d\n", onground);
		//DebugOut(L"allowfly %d\n", allow_fly);
		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];


		vector<LPGAMEOBJECT> coEventsResultColl;
		coEventsResultColl.clear();
		if (state != MARIO_STATE_DIE && this != NULL)
			CalCollisions(coObjects, coEventsResultColl);
		if (state == MARIO_STATE_DIE && level == MARIO_LEVEL_SMALL && !CheckMarioInScreen())
		{
			CGame::GetInstance()->SwitchScene(SCENCE_START);				// scence Start
			return;
		}
		if (coEventsResultColl.size() != 0)
		{
			for (UINT i = 0; i < coEventsResultColl.size(); i++)
			{
				LPGAMEOBJECT e = coEventsResultColl[i];
				if (dynamic_cast<CPortal*>(e))
				{
					CPortal* p = dynamic_cast<CPortal*>(e);
					if (CGame::GetInstance()->Getcurrent_scene() == SCENCE_START && StartTeleport)
					{
						CGame::GetInstance()->SwitchScene(p->GetSceneId());
						StartTeleport = false;
						return;			// khong return thi coObjects duoi se co 1 vai Obj = NULL
					}
					if (CGame::GetInstance()->Getcurrent_scene() == SCENCE_WORD_MAP_1)
					{
						/*if (!IsWaitingTeleport)
							vy = 0;*/
						IsWaitingTeleport = true;
						if (StartTeleport) {
							y += 0.4;
						/*	this->vy = MARIO_START_TELEPORT_VY * dt;*/
						}
						if (this->y > p->y && StartTeleport) {
							this->x = 2336;
							this->y = 330;
							CGame::GetInstance()->SwitchScene(p->GetSceneId());
						}
						return;
					}
					else if (CGame::GetInstance()->Getcurrent_scene() == SCENCE_WORD_MAP_1_1)
					{
						/*if (!IsWaitingTeleport)
							vy = 0;*/
						IsWaitingTeleport = true;
						if (StartTeleport) {
							y -= 0.4;
							/*	this->vy = MARIO_START_TELEPORT_VY * dt;*/
						}
						if (this->y < p->y && StartTeleport) {
							this->x = 2336;
							this->y = 330;
							CGame::GetInstance()->SwitchScene(p->GetSceneId());
						}
						return;
					}
				}
				else if (dynamic_cast<fire_plant*>(e)) {
					if (untouchable == 0)
					{
						if (level > MARIO_LEVEL_BIG)
						{
							SetLevel(MARIO_LEVEL_BIG);
							this->y -= 1;		// khong bi rot xuong Coobj
							StartUntouchable();
						}
						else if (level > MARIO_LEVEL_SMALL)
						{
							level = MARIO_LEVEL_SMALL;
							StartUntouchable();
						}
						else
							SetState(MARIO_STATE_DIE);
					}
				}
			}
		}
	}
	bool CMario::CheckMarioInScreen()
	{
		float XLeftScreen = CGame::GetInstance()->GetCamPosX() - 48;
		float XRightScreen = CGame::GetInstance()->GetCamPosX() + CGame::GetInstance()->GetScreenWidth() + 48;
		float YTopScreen = CGame::GetInstance()->GetCamPosY() - 48;
		float YBotScreen = CGame::GetInstance()->GetCamPosY() + CGame::GetInstance()->GetScreenHeight() + 48;

		if (this->x < XLeftScreen || this->x > XRightScreen)
			return false;
		if (this->y < YTopScreen || this->y > YBotScreen)
			return false;
		return true;
	}
void CMario::Render()
{
	int ani = -1;
	if (state == MARIO_STATE_DIE)
		ani = MARIO_ANI_DIE;
	else
		if (level == MARIO_LEVEL_BIG)
		{
			if (isCarrying && (state == MARIO_STATE_WALKING_RIGHT || state == MARIO_STATE_RUNNING_RIGHT) && nx > 0)ani = MARIO_ANI_BIG_CARRY_MOVING_RIGHT;
			else if (isCarrying && (state == MARIO_STATE_WALKING_LEFT || state == MARIO_STATE_RUNNING_LEFT) && nx < 0)ani = MARIO_ANI_BIG_CARRY_MOVING_LEFT;
			else if (isCarrying && state == MARIO_STATE_IDLE && nx > 0)ani = MARIO_ANI_BIG_CARRY_RIGHT;
			else if (isCarrying && state == MARIO_STATE_IDLE && nx < 0)ani = MARIO_ANI_BIG_CARRY_LEFT;
			else if (state == MARIO_STATE_STOP && vx < 0) ani = MARIO_ANI_BIG_STOP_LEFT;
			else if (state == MARIO_STATE_STOP && vx > 0) ani = MARIO_ANI_BIG_STOP_RIGHT;
			else if (vx > 0)
				ani = MARIO_ANI_BIG_WALKING_RIGHT;
			else if (vx < 0) ani = MARIO_ANI_BIG_WALKING_LEFT;
			else
			{
				if (nx > 0) ani = MARIO_ANI_BIG_IDLE_RIGHT;
				else ani = MARIO_ANI_BIG_IDLE_LEFT;
			}

		}
		else if (level == MARIO_LEVEL_FIRE)
		{
			if (isCarrying && state == MARIO_STATE_IDLE && nx > 0)ani = MARIO_ANI_FIRE_CARRY_RIGHT;
			else if (isCarrying && state == MARIO_STATE_IDLE && nx < 0)ani = MARIO_ANI_FIRE_CARRY_LEFT;
			else if (isCarrying &&( state == MARIO_STATE_WALKING_RIGHT||state == MARIO_STATE_RUNNING_RIGHT) && nx > 0)ani = MARIO_ANI_FIRE_CARRY_MOVING_RIGHT;
			else if (isCarrying &&( state == MARIO_STATE_WALKING_LEFT || state == MARIO_STATE_RUNNING_LEFT) && nx < 0)ani = MARIO_ANI_FIRE_CARRY_MOVING_LEFT;
			else if (isKicking1 && nx < 1)
				ani = MARIO_ANI_FIRE_KICK_LEFT;
			else if (isKicking1 && nx > 1)
				ani = MARIO_ANI_FIRE_KICK_RIGHT;
			else if (state == MARIO_STATE_FIRE_ATTACK && nx > 0)ani = MARIO_ANI_FIRE_CARRY_RIGHT;
			else if (state == MARIO_STATE_FIRE_ATTACK && nx < 0)ani = MARIO_ANI_FIRE_CARRY_LEFT;
			else if (maxspeed && nx < 0)ani = MARIO_ANI_FIRE_RUNNING_LEFT;
			else if (maxspeed && nx > 0)ani = MARIO_ANI_FIRE_RUNNING_RIGHT;
			else if (allow_jump && nx > 0)ani = MARIO_ANI_FIRE_JUMP_RIGHT;
			else if (allow_jump && nx < 0)ani = MARIO_ANI_FIRE_JUMP_LEFT;
			else if (isdropping && !onground && nx < 0)ani = MARIO_ANI_FIRE_DROP_LEFT;
			else if (isdropping && !onground && nx > 0)ani = MARIO_ANI_FIRE_DROP_RIGHT;
			else if (state == MARIO_STATE_STOP && nx < 0) ani = MARIO_ANI_FIRE_STOP_LEFT;
			else if (state == MARIO_STATE_STOP && nx > 0) ani = MARIO_ANI_FIRE_STOP_RIGHT;
			else if (!IsWaitingTeleport && state == MARIO_STATE_CROUCH && nx < 0) ani = MARIO_ANI_FIRE_CROUCH_LEFT;
			else if (!IsWaitingTeleport && state == MARIO_STATE_CROUCH && nx > 0) ani = MARIO_ANI_FIRE_CROUCH_RIGHT;
			else if (vx > 0)
				ani = MARIO_ANI_FIRE_WALKING_RIGHT;
			else if (vx < 0) ani = MARIO_ANI_FIRE_WALKING_LEFT;
			else
			{
				if (nx > 0) ani = MARIO_ANI_FIRE_IDLE_RIGHT;
				else ani = MARIO_ANI_FIRE_IDLE_LEFT;
			}
		}
		else if (level == MARIO_LEVEL_SMALL)
		{
			if (state == MARIO_STATE_STOP && vx < 0) ani = MARIO_ANI_SMALL_STOP_LEFT;
			else if (state == MARIO_STATE_STOP && vx > 0) ani = MARIO_ANI_SMALL_STOP_RIGHT;
			else if (vx > 0)
				ani = MARIO_ANI_SMALL_WALKING_RIGHT;
			else if (vx < 0) ani = MARIO_ANI_SMALL_WALKING_LEFT;
			else
			{
				if (nx > 0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
				else ani = MARIO_ANI_SMALL_IDLE_LEFT;
			}

		}
		else if (level == MARIO_LEVEL_TAIL)
		{
			if (state == MARIO_STATE_TAIL_ATTACK && nx < 0)
			{
				ani = MARIO_ANI_TAIL_ATTACK_LEFT;
				
			}
			else if (state == MARIO_STATE_TAIL_ATTACK && nx > 0)
			{

				ani = MARIO_ANI_TAIL_ATTACK_RIGHT;
				
			}
			else if (isCarrying && (state == MARIO_STATE_WALKING_RIGHT || state == MARIO_STATE_RUNNING_RIGHT) && nx > 0)ani = MARIO_ANI_TAIL_CARRY_MOVING_LEFT;
			else if (isCarrying && (state == MARIO_STATE_WALKING_LEFT || state == MARIO_STATE_RUNNING_LEFT) && nx < 0)ani = MARIO_ANI_TAIL_CARRY_MOVING_RIGHT;
			else if (isCarrying && state == MARIO_STATE_IDLE && nx > 0)ani = MARIO_ANI_TAIL_CARRY_RIGHT;
			else if (isCarrying && state == MARIO_STATE_IDLE && nx < 0)ani = MARIO_ANI_TAIL_CARRY_LEFT;
			else if (maxspeed && !onground && nx < 0)
				ani = MARIO_ANI_TAIL_FLYING_LEFT;
			else if (maxspeed && !onground && nx > 0)
				ani = MARIO_ANI_TAIL_FLYING_RIGHT;
			else if (maxspeed && nx < 0)ani = MARIO_ANI_TAIL_RUNNING_LEFT;
			else if (maxspeed && nx > 0)ani =MARIO_ANI_TAIL_RUNNING_RIGHT;
			else if (istaildropping && nx < 0)ani = MARIO_ANI_TAIL_SLOW_DROP_LEFT;
			else if (istaildropping && nx > 0)ani = MARIO_ANI_TAIL_SLOW_DROP_RIGHT;
			else if (allow_jump && !allow_fly && nx > 0)
				ani = MARIO_ANI_TAIL_JUMP_RIGHT;
			else if (allow_jump && !allow_fly && nx < 0)ani = MARIO_ANI_TAIL_JUMP_LEFT;
			else if (isdropping&&!onground && nx < 0)ani = MARIO_ANI_TAIL_DROP_LEFT;
			else if (isdropping &&!onground &&nx > 0)ani = MARIO_ANI_TAIL_DROP_RIGHT;
			else if (state == MARIO_STATE_STOP && nx < 0) ani = MARIO_ANI_TAIL_STOP_LEFT;
			else if (state == MARIO_STATE_STOP && nx > 0) ani = MARIO_ANI_TAIL_STOP_RIGHT;
			else if (!IsWaitingTeleport && state == MARIO_STATE_CROUCH && nx < 0) ani = MARIO_ANI_TAIL_CROUCH_LEFT;
			else if (!IsWaitingTeleport && state == MARIO_STATE_CROUCH && nx > 0) ani = MARIO_ANI_TAIL_CROUCH_RIGHT;
			else if (vx > 0)
				ani = MARIO_ANI_TAIL_WALKING_RIGHT;
			else if (vx < 0) ani = MARIO_ANI_TAIL_WALKING_LEFT;
			else
			{
				if (nx > 0) ani = MARIO_ANI_TAIL_IDLE_RIGHT;
				else ani = MARIO_ANI_TAIL_IDLE_LEFT;
			}
		}
		else if (level == MARIO_LEVEL_MINI) 
		{
			ani = MARIO_MINI_ANI;
		}
	int alpha = 255;
	if (untouchable) alpha = 128;
	if(level == MARIO_LEVEL_MINI)
		animation_set->at(ani)->Render(x, y, alpha);
	else if (level == MARIO_LEVEL_TAIL && nx > 0)
	{
		animation_set->at(ani)->Render(x + 3 , y, alpha);
	}
	else if (level == MARIO_LEVEL_TAIL && nx < 0 && state == MARIO_STATE_TAIL_ATTACK)
	{
		animation_set->at(ani)->Render(x + 3, y, alpha);
	}
	else animation_set->at(ani)->Render(x+10, y, alpha);
	//DebugOut(L"")
	if (level != MARIO_LEVEL_MINI)
	{
		Fire1->Render();
		Fire2->Render();
		tail->Render(); 
	}
	//RenderBoundingBox();
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);
	if (level == MARIO_LEVEL_MINI)
	{
		switch (state)
		{
		case MARIO_MINI_STATE_UP:
			vy = -MARIO_MINI_SPEED;
			vx = 0;
			break;
		case MARIO_MINI_STATE_DOWN:
			vy = MARIO_MINI_SPEED;
			vx = 0;
			break;
		case MARIO_MINI_STATE_LEFT:
			vy = 0;
			vx = -MARIO_MINI_SPEED;
			break;
		case MARIO_MINI_STATE_RIGHT:
			vy = 0;
			vx = MARIO_MINI_SPEED;
			break;
		case MARIO_MINI_STATE_TELEPORT:
			StartTeleport = true;
			//this->y += 1.0f;
			break;
		}
	}
	else
	{
		switch (state)
		{
		case MARIO_STATE_WALKING_RIGHT:
			vx += MARIO_ACCE;
			if (vx > MARIO_WALKING_SPEED)
				vx = MARIO_WALKING_SPEED;
			last_vx = vx;
			nx = 1;
			break;
		case MARIO_STATE_WALKING_LEFT:
			vx -= MARIO_ACCE;
			if (vx < -MARIO_WALKING_SPEED)
				vx = -MARIO_WALKING_SPEED;
			last_vx = vx;
			nx = -1;
			break;
		case MARIO_STATE_RUNNING_LEFT:
			if(!maxspeed)vx -= MARIO_ACCE;
			if (vx < -MARIO_MAX_SPEED)
				vx = -MARIO_MAX_SPEED;
			last_vx = vx;
			nx = -1;
			break;
		case MARIO_STATE_RUNNING_RIGHT:
			if (!maxspeed)vx += MARIO_ACCE;
			if (vx > MARIO_MAX_SPEED)
				vx = MARIO_MAX_SPEED;
			last_vx = vx;
			nx = 1;
			break;
		case MARIO_STATE_JUMP:
			if (onground)
				stand_y = y;
			allow_jump = true;
			jumpflag = true;
			onground = false;
			break;
		case MARIO_STATE_FLYING:
			allow_fly = true;
			if (nx < 0)
			{
				ResetAni(MARIO_ANI_TAIL_FLYING_LEFT);
				isWaitingForAni = true;
			}
			else if (nx > 0)
			{
				ResetAni(MARIO_ANI_TAIL_FLYING_RIGHT);
				isWaitingForAni = true;
			}
			break;
		case MARIO_STATE_DROP:
			allow_jump = false;
			jumpflag = false;
			allow_fly = false;
			break;
		case MARIO_STATE_TAILDROP:
			istaildropping = true;
		case MARIO_STATE_STOP:
			if (vx < 0)
			{
				vx += MARIO_STOP_ACCE;
				if (vx >= 0)vx = 0;
				last_vx = vx + MARIO_ANTI_MOVE;
			}
			else if (vx > 0)
			{
				vx -= MARIO_STOP_ACCE;
				if (vx <= 0)vx = 0;
				last_vx = vx - MARIO_ANTI_MOVE;
			}
			else vx = 0;
			break;

		case MARIO_STATE_IDLE:
			if (vx < 0)
			{
				vx += MARIO_STOP_ACCE;
				if (vx >= 0)vx = 0;
				last_vx = vx + MARIO_ANTI_MOVE;
			}
			else if (vx > 0)
			{
				vx -= MARIO_STOP_ACCE;
				if (vx <= 0)vx = 0;
				last_vx = vx - MARIO_ANTI_MOVE;
			}
			else vx = 0;
			break;
		case MARIO_STATE_DIE:
			vy = -MARIO_DIE_DEFLECT_SPEED;
			break;
		case MARIO_STATE_TAIL_ATTACK:
			if (nx < 0)
			{
				ResetAni(MARIO_ANI_TAIL_ATTACK_LEFT);
				isWaitingForAni = true;
			}
			else if (nx > 0)
			{
				ResetAni(MARIO_ANI_TAIL_ATTACK_RIGHT);
				isWaitingForAni = true;
			}
			is_tail_attacking = true;
			break;
		case MARIO_STATE_FIRE_ATTACK:
			/*	if (nx < 0)
				{
					ResetAni(MARIO_ANI_TAIL_ATTACK_LEFT);
					isWaitingForAni = true;
				}
				else if (nx > 0)
				{
					ResetAni(MARIO_ANI_TAIL_ATTACK_RIGHT);
					isWaitingForAni = true;
				}*/
			if (nx > 0)
			{
				ResetAni(MARIO_ANI_BIG_CARRY_RIGHT);
				isWaitingForAni = true;
			}
			else if (nx < 0)
			{
				ResetAni(MARIO_ANI_BIG_CARRY_RIGHT);
				isWaitingForAni = true;
			}
			if (Fire1->isFiring && Fire2->allow_fire == false)
			{
				AttackWithFire(Fire2);
				Fire2->isFiring = true;
			}
			if (Fire1->allow_fire == false)
			{
				AttackWithFire(Fire1);
				Fire1->isFiring = true;
			}
			/*	isFireAttacking = true;*/
			break;
		case MARIO_STATE_CROUCH:
			if (!IsWaitingTeleport)
			{
				if (onground)
				{
					crouch = true;
					vx = 0;
				}
			}
			else
			if (IsWaitingTeleport)
				StartTeleport = true;
			break;
		case MARIO_STATE_UNCROUCH:
			break;
		case MARIO_STATE_CARRYING:
			break;
		}
	}
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (level == MARIO_LEVEL_MINI)left = x;
	else
	left = x+10;
	top = y; 

	if (level==MARIO_LEVEL_BIG)
	{
		right = x+10 + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
	}
	else if (level == MARIO_LEVEL_TAIL)
	{
		if (state == MARIO_STATE_CROUCH)
		{
			right = (x+10 + MARIO_TAIL_CROUCH_BBOX_WIDTH);
			bottom = y + MARIO_TAIL_CROUCH_BBOX_HEIGHT;
		}
		else
		{
			right = x+10 + MARIO_BIG_BBOX_WIDTH;
			bottom = y + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else if (level == MARIO_LEVEL_FIRE)
	{
		if (state == MARIO_STATE_CROUCH && !IsWaitingTeleport)
		{
			right = (x+	10 + MARIO_FIRE_CROUCH_BBOX_WIDTH);
			bottom = y + MARIO_FIRE_CROUCH_BBOX_HEIGHT;
		}
		else
		{
			right = x+	10 + MARIO_BIG_BBOX_WIDTH;
			bottom = y + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else if (level == MARIO_LEVEL_MINI)
	{
		right = x + 15;
		bottom = y + 15;
	}
	else
	{
		right = x+10 + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
}

/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	SetLevel(MARIO_LEVEL_FIRE);
	SetState(MARIO_STATE_IDLE);
	SetPosition(x, y - 10);
	SetSpeed(0, 0);
}
void CMario::Reset2()
{
	SetLevel(MARIO_LEVEL_TAIL);
	SetState(MARIO_STATE_IDLE);
	SetPosition(x, y - 10);
	SetSpeed(0, 0);
}
void CMario::Reset3()
{
	SetLevel(MARIO_LEVEL_BIG);
	SetState(MARIO_STATE_IDLE);
	SetPosition(x, y-10);
	SetSpeed(0, 0);
}

void CMario::AttackWithTail()
{
	tail->attacking = true;
	tail->Attack(this);

}
void CMario::AttackWithFire(fire* Fire)
{
	Fire->Attack(this);
}
void CMario::CheckForAniEnd()
{
	if (level == MARIO_LEVEL_TAIL )
	{
		if (isWaitingForAni)
		{
			if (nx == 1)
			{
				if (animation_set->at(MARIO_ANI_TAIL_ATTACK_RIGHT)->IsOver())
				{
					isWaitingForAni = false;

				}
			
			}
			else if (nx == -1)
			{
				if (animation_set->at(MARIO_ANI_TAIL_ATTACK_LEFT)->IsOver())
					isWaitingForAni = false;
				
			}
		}
	}
	 if (level == MARIO_LEVEL_TAIL && allow_fly)
	{
		if (isWaitingForAni)
		{
			if (nx == 1)
			{
			
				 if (animation_set->at(MARIO_ANI_TAIL_FLYING_RIGHT)->IsOver())
				{
					isWaitingForAni = false;

				}
			}
			else if (nx == -1)
			{
				
				 if (animation_set->at(MARIO_ANI_TAIL_FLYING_LEFT)->IsOver())
				{
					isWaitingForAni = false;

				}
			}
		}
	}
	if (level == MARIO_LEVEL_FIRE)
	{
		if (isWaitingForAni)
		{
			if (nx == 1)
			{
				/*if (animation_set->at(MARIO_ANI_FIRE_CARRY_RIGHT)->IsOver())
				{
					isWaitingForAni = false;

				}*/
				if (animation_set->at(MARIO_ANI_FIRE_KICK_RIGHT)->IsOver())
				{
					isWaitingForAni = false;
				}
				isKicking1 = false;
			}
			else if (nx == -1)
			{
				/*if (animation_set->at(MARIO_ANI_FIRE_CARRY_LEFT)->IsOver())
					isWaitingForAni = false;*/
				if (animation_set->at(MARIO_ANI_FIRE_KICK_LEFT)->IsOver())
				{
					isWaitingForAni = false;
				}
				isKicking1 = false;
			}
		}
	}
}