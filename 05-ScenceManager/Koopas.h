#pragma once

#include "GameObject.h"

#define KOOPAS_WALKING_SPEED 0.03f
#define KOOPAS_SPINNING_SPEED 0.1f
#define KOOPAS_GRAVITY			0.0018f

#define KOOPAS_BBOX_WIDTH 16
#define KOOPAS_BBOX_HEIGHT 26	
#define KOOPAS_BBOX_HEIGHT_DIE 16
#define KOOPAS_BBOX_CHANGE 2

#define KOOPAS_STATE_WALKING 100
#define KOOPAS_STATE_DIE 200
#define KOOPAS_STATE_SPINNING	300

#define KOOPAS_ANI_WALKING_LEFT 0
#define KOOPAS_ANI_WALKING_RIGHT 1
#define KOOPAS_ANI_DIE 2
#define KOOPAS_ANI_SPINNING	3
#define KOOPAS_ANI_DIE_UP 4
#define KOOPAS_ANI_SPINING_UP 5
class CKoopas : public CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	int last_vx;
	bool flip = false;
	
public:
	bool beingcarried = false;
	void GetHit();
	CKoopas();
	virtual void SetState(int state);
	void BeingCarry(LPGAMEOBJECT user);
};