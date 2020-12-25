#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "fire.h"
#define PLANT_BBOX_HEIGHT	33
#define PLANT_BBOX_WIDTH	16

#define PLANT_MOVING_DISTANCE 40

#define PLANT_STATE_IDLE_TOP_LEFT	0
#define PLANT_STATE_FIRE_TOP_LEFT	1
#define PLANT_STATE_IDLE_BOTTOM_LEFT	2
#define PLANT_STATE_FIRE_BOTTOM_LEFT	3
#define PLANT_STATE_IDLE_TOP_RIGHT	4
#define PLANT_STATE_FIRE_TOP_RIGHT	5
#define PLANT_STATE_IDLE_BOTTOM_RIGHT	6
#define PLANT_STATE_FIRE_BOTTOM_RIGHT	7

#define FIRE_PIRANHA_PLANT_DISTANCE_ATTACK_MARIO	80

#define FIRE_PIRANHA_PLANT_SPEED_VY		0.001f
#define FIRE_PIRANHA_PLANT_TIME_ATTACK	3000
#define FIRE_PIRANHA_PLANT_ANI_RIGHT_UP		0
#define FIRE_PIRANHA_PLANT_ANI_RIGHT_DOWN	1
#define FIRE_PIRANHA_PLANT_ANI_LEFT_UP		2
#define FIRE_PIRANHA_PLANT_ANI_LEFT_DOWN	3

#define FIRE_PLANT_WAIT 2500

#define MARIO_LEFT_TOP_TOP		1
#define MARIO_LEFT_TOP_BOT		2
#define MARIO_LEFT_BOT_TOP		3
#define MARIO_LEFT_BOT_BOT		4
#define MARIO_RIGHT_TOP_TOP		5
#define MARIO_RIGHT_TOP_BOT		6
#define MARIO_RIGHT_BOT_TOP		7
#define MARIO_RIGHT_BOT_BOT		8
class ShootingPlant : public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	int last_vx;
	int PosAttack;
	CMario* mario;
	fire_plant* fire;
	int base_y;
	bool basey;
	bool start_moving_up = false;
	bool start_moving_down = false;
	DWORD wait_plant_atk_start=0;
	int wait_plant_atk = 0;
public:
	void StartAttack() { wait_plant_atk = 1; wait_plant_atk_start = GetTickCount(); }
	ShootingPlant(CMario *mario, fire_plant *fireplant);
	void CheckPos(CMario* mario);
	virtual void SetState(int state);
};