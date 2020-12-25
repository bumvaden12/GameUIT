#pragma once
#include "GameObject.h"
#define SPEED_VY			0.001f
#define SPEED_VY_MAX		1.0f

#define EFFECT_100			0
#define EFFECT_200			1
#define EFFECT_400			2
#define EFFECT_800			3
#define EFFECT_1000			4
#define EFFECT_2000			5
#define EFFECT_4000			6
#define EFFECT_8000			7
#define EFFECT_1_UP			8
class Effect
{
public:
	int AniEffect;
	float PosX, PosY, Vy = 0;
	bool StopDraw;
	void Update(DWORD dt);
	void Render();
	Effect(float PosX, float PosY, int Ani);
};
