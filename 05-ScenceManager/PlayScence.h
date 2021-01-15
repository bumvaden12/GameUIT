#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "Koopas.h"
#include "map.h"
#include "platform.h"
#include "QuestionBrick.h"
#include "ShinyBrick.h"
#include "StatusBar.h"

class CPlayScene: public CScene
{
protected: 
	CMario *player;					// A play scene has to have player, right? 
	TileMap* tileMap;
	int SpriteEffectStart;
	LPGAMEOBJECT ItemSwitch;
	StatusBar* statusBar;
	/*vector<fire*> fires;*/
	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> Itemobjects;
	int mapWidth, mapHeight;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_EFFECT(string line);
	void _ParseSection_STATUS_BAR(string line);
	void _ParseSection_TILEMAP_DATA(string line);
	
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Unload();
	virtual void Update(DWORD dt);
	virtual void Render();

	CMario * GetPlayer() { return player; } 

	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public: 
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};

