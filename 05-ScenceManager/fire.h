#pragma once
#include "GameObject.h"
#include "Utils.h"
#define FIRE_FOWARD_SPEED	0.09f
#define FIRE_JUMP_SPEED		0.05f
#define FIRE_JUMP_LIMIT		10.0f
#define FIRE_STATE_WAITING	7100
#define FIRE_STATE_BOUNCING	7200
#define FIRE_STATE_EXPLODE	7300
#define FIRE_ANI_LEFT					0
#define FIRE_ANI_RIGHT					1
#define FIRE_ANI_EXPLODE				2

#define FIRE_BBOX_WIDTH		9
#define FIRE_BBOX_HEIGHT		9

#define FIRE_PLANT_VX 0.0025
#define FIRE_PLANT_VY_TOP 0.0025
#define FIRE_PLANT_VY_BOTTOM	0.0005
class fire:public CGameObject
{
public:
	fire(float x = 0.0f, float y = 0.0f);;
	~fire();
	bool isWaitingForAniFire = false;
	bool allow_fire = false;
public: bool isFiring = false;
	bool allow_bouncing= false;
	float bounce_y;
	bool fire_onground = false;
	void Attack(LPGAMEOBJECT user);
	void SetPosition(float x, float y);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	void SetState(int state);
	void CheckStateEnd();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

};
class fire_plant :public CGameObject
{
public:
	fire_plant(float x = 0.0f, float y = 0.0f);;
	~fire_plant();
	bool isWaitingForAniFire = false;
	bool allow_fire = false;
	int fire_pos;
public: bool isFiring = false;
	  bool allow_bouncing = false;
	  float bounce_y;
	  bool fire_onground = false;
	  void Attack(LPGAMEOBJECT user, int fire_pos);
	  void SetPosition(float x, float y);
	  void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	  void Render();
	  void SetState(int state);
	  void CheckStateEnd();
	  void GetBoundingBox(float& left, float& top, float& right, float& bottom);

};

