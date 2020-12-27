#pragma once

#include <unordered_map>

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>


#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "Scence.h"

using namespace std;
#define SCENCE_START			0
#define SCENCE_WORD_MAP_1		1
#define SCENCE_WORD_MAP_1_1		4

#define KEYBOARD_BUFFER_SIZE 1024
#define TILE_WIDTH 16
#define TILE_HEIGHT 16
class CGame
{
	static CGame * __instance;
	HWND hWnd;									// Window handle

	LPDIRECT3D9 d3d = NULL;						// Direct3D handle
	LPDIRECT3DDEVICE9 d3ddv = NULL;				// Direct3D device object

	LPDIRECT3DSURFACE9 backBuffer = NULL;		
	LPD3DXSPRITE spriteHandler = NULL;			// Sprite helper library to help us draw 2D image on the screen 

	LPDIRECTINPUT8       di;		// The DirectInput object         
	LPDIRECTINPUTDEVICE8 didv;		// The keyboard device 

	BYTE  keyStates[256];			// DirectInput keyboard state buffer 
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];		// Buffered keyboard data

	LPKEYEVENTHANDLER keyHandler;

	float cam_x = 0.0f;
	float cam_y = 0.0f;

	int screen_width;
	int screen_height; 

	unordered_map<int, LPSCENE> scenes;
	int current_scene; 

	int Score = 0;
	int Money = 0;
	int Life = 4;
	int Time = 900;

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_SCENES(string line);

public:
	void InitKeyboard();
	void SetKeyHandler(LPKEYEVENTHANDLER handler) { keyHandler = handler; }
	void Init(HWND hWnd);
	void Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha = 255);

	int IsKeyDown(int KeyCode);
	void ProcessKeyboard();
	int GetScore() { return Score; };
	void SetScore(int Score) { this->Score = Score; }
	int GetLife() { return Life; };
	void SetLife(int Life) { this->Life = Life; }
	int GetTime() { return Time; };
	void SetTime(int Life) { this->Time = Time; }
	int GetMoney() { return Money; }
	void SetMoney(int money) { this->Money = money; }
	int Getcurrent_scene() { return current_scene; }
	void Load(LPCWSTR gameFile);
	LPSCENE GetCurrentScene() { return scenes[current_scene]; }
	void SwitchScene(int scene_id);

	int GetScreenWidth() { return screen_width; }
	int GetScreenHeight() { return screen_height; }
	float GetCamPosX() { return cam_x; }
	float GetCamPosY() { return cam_y; }
	static bool IsOverlapping(
		float ml,			// move left 
		float mt,
		float mr,
		float mb,
		float sl,			// static left
		float st,
		float sr,
		float sb);


	static void SweptAABB(
		float ml,			// move left 
		float mt,			// move top
		float mr,			// move right 
		float mb,			// move bottom
		float dx,			// 
		float dy,			// 
		float sl,			// static left
		float st, 
		float sr, 
		float sb,
		float &t, 
		float &nx, 
		float &ny);

	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }

	void SetCamPos(float x, float y) { cam_x = x; cam_y = y; }
	void SetCamPosX(float x) { cam_x = x; }
	void SetCamPosY(float y) { cam_y = y; }

	static CGame * GetInstance();

	~CGame();
};


