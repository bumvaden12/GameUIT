#pragma once
#include "GameObject.h"
#include "Utils.h"
#define TAIL_BBOX_HEIGHT 3
#define TAIL_BBOX_WIDTH 36
class CTail :public CGameObject
{
	
public : 
	bool attacking = false;
	CTail();
	~CTail();
	void Attack(LPGAMEOBJECT user);
	void SetPosition(float x, float y);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

};

