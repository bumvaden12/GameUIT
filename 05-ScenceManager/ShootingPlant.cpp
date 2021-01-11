#include "ShootingPlant.h"
#include "Goomba.h"
#include "Koopas.h"
#include"Brick.h"
ShootingPlant::ShootingPlant(CMario *mario,fire_plant * fire)
{
	this->mario = mario;
	this->fire = fire;
	
	start_moving_down = true;
}

void ShootingPlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + PLANT_BBOX_WIDTH;
	bottom = y + PLANT_BBOX_HEIGHT;
}

void ShootingPlant::CheckPos(CMario* mario)
{
	if (mario->x < this->x)	// LEFT
	{
		if (mario->y < this->y)	//TOP
		{
			if (vy == 0)
				SetState(FIRE_PIRANHA_PLANT_ANI_LEFT_UP);
			if (this->x - mario->x > FIRE_PIRANHA_PLANT_DISTANCE_ATTACK_MARIO)
				PosAttack = MARIO_LEFT_TOP_TOP;
			else
				PosAttack = MARIO_LEFT_TOP_BOT;
		}
		else
		{
			if (vy == 0)
				SetState(FIRE_PIRANHA_PLANT_ANI_LEFT_DOWN);
			if (this->x - mario->x > FIRE_PIRANHA_PLANT_DISTANCE_ATTACK_MARIO)
				PosAttack = MARIO_LEFT_BOT_TOP;
			else
				PosAttack = MARIO_LEFT_BOT_BOT;
		}
	}
	else
	{
		if (mario->y < this->y)
		{
			if (vy == 0)
				SetState(FIRE_PIRANHA_PLANT_ANI_RIGHT_UP);
			if (mario->x - this->x > FIRE_PIRANHA_PLANT_DISTANCE_ATTACK_MARIO)
				PosAttack = MARIO_RIGHT_TOP_TOP;
			else
				PosAttack = MARIO_RIGHT_TOP_BOT;
		}
		else
		{
			if (vy == 0)
				SetState(FIRE_PIRANHA_PLANT_ANI_RIGHT_DOWN);
			if (mario->x - this->x > FIRE_PIRANHA_PLANT_DISTANCE_ATTACK_MARIO)
				PosAttack = MARIO_RIGHT_BOT_TOP;
			else
				PosAttack = MARIO_RIGHT_BOT_BOT;
		}
	}
}


void ShootingPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
	if (!basey)
	{
		base_y = y;
	basey = true;
}
	CGameObject::Update(dt, coObjects);
	CheckPos(mario);
	if (PosAttack <= 4)nx = -1;
	else nx = 1;
	if (start_moving_up == true)
	{
		vy = -0.001 * dt;
	}
	if (y <= base_y && start_moving_up == true)
	{
		start_moving_up = false;
		StartAttack();
		vy = 0;
		
	}
	if (GetTickCount() - wait_plant_atk_start > FIRE_PLANT_WAIT && wait_plant_atk==1)
	{
		wait_plant_atk_start = 0;
		wait_plant_atk = 0;
		fire->Attack(this, PosAttack);
		start_moving_down = true;
	}
	if (start_moving_down == true)
	{
		vy = 0.001 * dt;	
	}
	if (y > base_y  + PLANT_MOVING_DISTANCE && start_moving_down == true)
	{
		start_moving_down = false;
		start_moving_up = true;
	}
	y += dy;
	fire->Update(dt, coObjects);
}

void ShootingPlant::Render()
{
	int ani=-1;
	switch (PosAttack)
	{
	case MARIO_LEFT_TOP_TOP	:	
		ani = PLANT_STATE_FIRE_TOP_LEFT;
		break;
	case MARIO_LEFT_TOP_BOT	:
		ani = PLANT_STATE_FIRE_TOP_LEFT;
		break;
	case MARIO_LEFT_BOT_TOP:
		ani = PLANT_STATE_FIRE_BOTTOM_LEFT;
		break;
	case MARIO_LEFT_BOT_BOT:
		ani = PLANT_STATE_FIRE_BOTTOM_LEFT;
		break;
	case MARIO_RIGHT_TOP_TOP:
		ani = PLANT_STATE_FIRE_TOP_RIGHT;
		break;
	case MARIO_RIGHT_TOP_BOT:
		ani = PLANT_STATE_FIRE_TOP_RIGHT;
		break;
	case MARIO_RIGHT_BOT_TOP:
		ani = PLANT_STATE_FIRE_BOTTOM_RIGHT;
		break;
	case MARIO_RIGHT_BOT_BOT:
		ani = PLANT_STATE_FIRE_BOTTOM_RIGHT;
		break;
	}

	fire->Render();
	if(ani!=-1)
 	animation_set->at(ani)->Render(x, y);

	//RenderBoundi	ngBox();
}

void ShootingPlant::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{

	}
}