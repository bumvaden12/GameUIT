#pragma once
#include "GameObject.h"
#include "Tail.h"
#include "fire.h"
#include "Koopas.h"
#define MARIO_WALKING_SPEED		0.08f 
#define MARIO_MAX_SPEED			0.15f
//0.1f
#define MARIO_TAILDROP_DISTANCE	15
#define MARIO_FLY_DISTANCE		15
#define MARIO_JUMP_ACCE			0.018f
#define MARIO_FLY_ACCE			0.01f
#define MARIO_MIN_JUMP_HEIGHT 0.4f
#define MARIO_MAX_JUMP_HEIGHT 50.0f
#define MARIO_JUMP_DEFLECT_SPEED 0.2f
#define MARIO_GRAVITY			0.0018f
#define MARIO_TAILDROP_SPEED	0.002f
#define MARIO_DIE_DEFLECT_SPEED	0.5f
#define MARIO_ACCE				0.01f
#define MARIO_STOP_ACCE			0.006f
#define MARIO_ANTI_MOVE			0.002f
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_JUMP			300
#define MARIO_STATE_DIE				400
#define MARIO_STATE_STOP			500
#define MARIO_STATE_DROP			600
#define MARIO_STATE_TAIL_ATTACK		700
#define MARIO_STATE_CROUCH			800
#define MARIO_STATE_UNCROUCH		900
#define MARIO_STATE_RUNNING_LEFT	1000
#define MARIO_STATE_RUNNING_RIGHT	1100
#define MARIO_STATE_FLYING			1200
#define MARIO_STATE_TAILDROP		1300
#define MARIO_STATE_FIRE_ATTACK		1400
#define MARIO_STATE_CARRYING				1500

#define MARIO_ANI_BIG_IDLE_RIGHT		0
#define MARIO_ANI_BIG_IDLE_LEFT			1
#define MARIO_ANI_SMALL_IDLE_RIGHT		2
#define MARIO_ANI_SMALL_IDLE_LEFT		3
#define MARIO_ANI_BIG_WALKING_RIGHT		4
#define MARIO_ANI_BIG_WALKING_LEFT		5
#define MARIO_ANI_SMALL_WALKING_RIGHT	6
#define MARIO_ANI_SMALL_WALKING_LEFT	7
#define MARIO_ANI_DIE					8
#define MARIO_ANI_SMALL_STOP_LEFT		9
#define	MARIO_ANI_SMALL_STOP_RIGHT		10
#define MARIO_ANI_BIG_STOP_LEFT			11
#define MARIO_ANI_BIG_STOP_RIGHT		12

#define MARIO_ANI_TAIL_IDLE_RIGHT		13
#define MARIO_ANI_TAIL_IDLE_LEFT		14
#define MARIO_ANI_TAIL_WALKING_RIGHT	15
#define MARIO_ANI_TAIL_WALKING_LEFT		16
#define MARIO_ANI_TAIL_STOP_LEFT		17
#define MARIO_ANI_TAIL_STOP_RIGHT		18
#define MARIO_ANI_TAIL_CROUCH_LEFT		19
#define MARIO_ANI_TAIL_CROUCH_RIGHT		20
#define MARIO_ANI_TAIL_ATTACK_LEFT		21
#define MARIO_ANI_TAIL_ATTACK_RIGHT		22
#define MARIO_ANI_TAIL_JUMP_LEFT		23
#define MARIO_ANI_TAIL_JUMP_RIGHT		24
#define	MARIO_ANI_TAIL_DROP_LEFT		25
#define	MARIO_ANI_TAIL_DROP_RIGHT		26
#define MARIO_ANI_TAIL_SLOW_DROP_LEFT	27
#define MARIO_ANI_TAIL_SLOW_DROP_RIGHT	28
#define	MARIO_ANI_TAIL_RUNNING_LEFT		29
#define	MARIO_ANI_TAIL_RUNNING_RIGHT	30
#define	MARIO_ANI_TAIL_FLYING_LEFT		31
#define MARIO_ANI_TAIL_FLYING_RIGHT		32

#define MARIO_ANI_FIRE_IDLE_LEFT		33
#define MARIO_ANI_FIRE_IDLE_RIGHT		34
#define	MARIO_ANI_FIRE_WALKING_LEFT		35
#define	MARIO_ANI_FIRE_WALKING_RIGHT	36
#define	MARIO_ANI_FIRE_RUNNING_LEFT		37
#define	MARIO_ANI_FIRE_RUNNING_RIGHT	38
#define	MARIO_ANI_FIRE_STOP_LEFT		39
#define MARIO_ANI_FIRE_STOP_RIGHT		40
#define	MARIO_ANI_FIRE_JUMP_LEFT		41
#define MARIO_ANI_FIRE_JUMP_RIGHT		42
#define MARIO_ANI_FIRE_DROP_LEFT		43
#define	MARIO_ANI_FIRE_DROP_RIGHT		44
#define	MARIO_ANI_FIRE_CROUCH_LEFT		45
#define	MARIO_ANI_FIRE_CROUCH_RIGHT		46

#define MARIO_ANI_BIG_CARRY_LEFT		47
#define MARIO_ANI_BIG_CARRY_RIGHT		48
#define MARIO_ANI_FIRE_CARRY_LEFT		49
#define MARIO_ANI_FIRE_CARRY_RIGHT		50

#define MARIO_ANI_FIRE_CARRY_MOVING_LEFT	51
#define MARIO_ANI_FIRE_CARRY_MOVING_RIGHT	52
#define MARIO_ANI_TAIL_CARRY_MOVING_LEFT	53
#define MARIO_ANI_TAIL_CARRY_MOVING_RIGHT	54
#define MARIO_ANI_BIG_CARRY_MOVING_LEFT		55
#define MARIO_ANI_BIG_CARRY_MOVING_RIGHT	56
#define MARIO_ANI_TAIL_CARRY_LEFT			57
#define MARIO_ANI_TAIL_CARRY_RIGHT			58

#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_TAIL	3
#define MARIO_LEVEL_FIRE	4

#define MARIO_BIG_BBOX_WIDTH  15
#define MARIO_BIG_BBOX_HEIGHT 27

#define MARIO_TAIL_BBOX_WIDTH	21
#define MARIO_TAIL_BBOX_HEIGHT	28
#define	MARIO_TAIL_CROUCH_BBOX_WIDTH	22
#define MARIO_TAIL_CROUCH_BBOX_HEIGHT	18
#define MARIO_TAIL_ATTACK_BBOX_WIDTH	22
#define MARIO_TAIL_ATTACK_BBOX_HEIGHT 28



#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 15

#define	MARIO_FIRE_CROUCH_BBOX_WIDTH	14
#define MARIO_FIRE_CROUCH_BBOX_HEIGHT	18

#define MARIO_UNTOUCHABLE_TIME	5000
#define MARIO_ATTACKING_TIME	1000


class CMario : public CGameObject
{
	CTail* tail;
	fire* Fire1,*Fire2;
	CKoopas* koopas_carry;
	float mario_x;
	float mario_y;
	float last_vx = 0;
	float stand_y = 0;
public:bool isCarrying = false;
public:bool isKicking = false;
public: bool isWaitingForAni = false;
public:	float drop_y = 0;
	 float fly_y = 0;
	float sum_jump_acce = 0;
public:	bool allow_jump = false;
	bool allow_fly = false;
	bool jumpflag = false;
	bool isflying = false;
	bool istaildropping = false;
public:bool onground = false;
	bool crouch = false;
	bool isdropping = false;
	bool maxspeed = false;
	int level;
	int untouchable;
	int attackflag;
	bool isFireAttacking = false;
	DWORD untouchable_start;

	float start_x;			// initial position of Mario at scene
	float start_y; 
public: 
	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void Reset();
	void Reset2();
	void Reset3();
	void AttackWithTail();
	void AttackWithFire(fire* Fire);
	void CheckForAniEnd();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};