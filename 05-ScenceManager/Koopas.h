#pragma once

#include "GameObject.h"

#define KOOPAS_SPINNING_SPEED 0.1f
#define KOOPAS_GRAVITY			0.0018f

#define KOOPAS_WALKING_SPEED				0.04f
#define KOOPAS_PARATROOPA_WALKING_SPEED		0.01f
#define KOOPAS_ROTATORY_SPEED	0.2f
#define KOOPAS_DIE_DEFLECT_SPEED 0.02f
#define KOOPAS_DIE				0.1f
#define KOOPAS_BBOX_WIDTH 16
#define KOOPAS_BBOX_HEIGHT  26
#define KOOPAS_BBOX_HEIGHT_SHELL 16

#define KOOPAS_BBOX_WIDTH 16
#define KOOPAS_BBOX_HEIGHT 26	
#define KOOPAS_BBOX_HEIGHT_DIE 16
#define KOOPAS_BBOX_CHANGE 2

#define KOOPAS_STATE_WALKING 100
#define KOOPAS_STATE_DIE 200
#define KOOPAS_STATE_SPINNING	300
#define KOOPAS_STATE_PREPARE_WAKE_UP	400

#define KOOPAS_ANI_WALKING_LEFT 0
#define KOOPAS_ANI_WALKING_RIGHT 1
#define KOOPAS_ANI_DIE 2
#define KOOPAS_ANI_SPINNING	3
#define KOOPAS_ANI_DIE_UP 4
#define KOOPAS_ANI_SPINING_UP 5
#define KOOPA_PARATROOPA_GREEN_ANI_WING_RIGHT				6
#define KOOPA_PARATROOPA_GREEN_ANI_JUMP_RIGHT				7
#define KOOPA_PARATROOPA_GREEN_ANI_WING_LEFT				8
#define KOOPA_PARATROOPA_GREEN_ANI_JUMP_LEFT				9
#define KOOPAS_ANI_SHELL_TAIL_ATTACK_PREPARE_WAKE_UP_GREEN	10

#define KOOPAS_TIME_WAKE_UP								2000
#define KOOPAS_TIME_SLEEP								5000
#define KOOPAS_TYPE_KOOPA_TROOPA_GREEN					1
#define KOOPAS_TYPE_KOOPA_PARATROOPA_GREEN				2
class CKoopas : public CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	int last_vx;
	bool flip = false;
	bool ShakingLeft;
	int Sleep, PrepareWakeUp;
	DWORD Sleep_start, PrepareWakeUp_start;
public:
	bool beingcarried = false;
	void GetHit();
	int TypeKoopas;
	CKoopas(int TypeKoopas);
	virtual void SetState(int state);
	void BeingCarry(LPGAMEOBJECT user);
	void StartSleep() { Sleep = 1; Sleep_start = GetTickCount(); }
	void StartPrepareWakeUp() { PrepareWakeUp = 1; PrepareWakeUp_start = GetTickCount(); }
};
