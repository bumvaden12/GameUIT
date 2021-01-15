#pragma once
#include "Sprites.h"
#include "Mario.h"
#include "GameObject.h"
#include <string>
#define SPEED_X_TIER_1 0.01f
#define SPEED_X_TIER_2 0.02f
#define SPEED_X_TIER_3 0.04f
#define SPEED_X_TIER_4 0.06f
#define SPEED_X_TIER_5 0.08f
#define SPEED_X_TIER_6 0.10f
#define SPEED_X_TIER_7 0.12f
#define SPEED_X_TIER_8 0.15f

#define X_DISTANCE_PMETER		52
#define Y_DISTANCE_PMETER		7
#define X_WORD					37
#define Y_WORD					7
#define X_SCORE					52
#define Y_SCORE					15
#define X_LIFE					37
#define Y_LIFE					15
#define X_TIME					124
#define Y_TIME					15
#define X_MONEY					132
#define Y_MONEY					7
#define X_CARD					193
class StatusBar
{
	LPSPRITE number;
	float posX;
	float posY;
	int TotalScore;
	int TotalMoney;
	int WordMap;
	int Life;
	int Time;
	int Second;
	int SpriteStatusBar, SpriteCardBar, SpriteNumber0, SpritePowerState;
	void DrawNumber(float x, float y, string stringnumb, int maxsize);
	void DrawPMeter();
public:
	void Update(DWORD dt, float camX, float camY);
	void Render();
	CMario* Mario;
	StatusBar(CMario* mario, int SpriteStatusBar, int SpriteCardBar, int SpriteNumber0, int SpritePowerState);
	void PlusScore(int Score);
	//~StatusBar();
};

