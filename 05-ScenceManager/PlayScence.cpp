#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Tunnel.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
	//tileMap = new TileMap();
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_TILEMAP_DATA	7

#define OBJECT_TYPE_MARIO	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_GOOMBA	2
#define OBJECT_TYPE_KOOPAS	3
#define OBJECT_TYPE_INVIBRICK	4
#define OBJECT_TYPE_FIRE	5
#define OBJECT_TYPE_PLATFORM	6
#define OBJECT_TYPE_QBRICK 7
#define OBJECT_TYPE_SBRICK 8
#define OBJECT_TYPE_PORTAL	50
#define OBJECT_TYPE_TUNNEL	9

#define MAX_SCENE_LINE 1024
#define ONGROUND_Y		433


void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return; 
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}
void CPlayScene::_ParseSection_TILEMAP_DATA(string line)
{
	vector<string> tokens = split(line);
	if (tokens.size() < 9) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	wstring filePath_texture = ToWSTR(tokens[1]);
	wstring filePath_data = ToWSTR(tokens[2]);
	int num_row_on_texture = atoi(tokens[3].c_str());
	int num_col_on_textture = atoi(tokens[4].c_str());
	int num_row_on_Map = atoi(tokens[5].c_str());
	int num_col_on_Map = atoi(tokens[6].c_str());
	int tileset_width = atoi(tokens[7].c_str());
	int tileset_height = atoi(tokens[8].c_str());
	tileMap = new TileMap(ID, filePath_texture.c_str(), filePath_data.c_str(), num_row_on_texture, num_col_on_textture, num_row_on_Map, num_col_on_Map, tileset_width, tileset_height);
	mapWidth = num_col_on_Map * tileset_width;
	mapHeight = num_row_on_Map * tileset_height;
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations *animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());
		
		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();

	CGameObject *obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player!=NULL) 
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x,y); 
		player = (CMario*)obj;  

		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(); break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(); break;
	case OBJECT_TYPE_KOOPAS: obj = new CKoopas(); break;
	case OBJECT_TYPE_INVIBRICK: obj = new InviBrick(); break;
	case OBJECT_TYPE_FIRE:	obj = new fire(); break;
	case OBJECT_TYPE_PLATFORM: obj = new platform(); break;
	case OBJECT_TYPE_QBRICK: obj = new QBrick(); break;
	case OBJECT_TYPE_SBRICK: obj = new SBrick(); break;
	case OBJECT_TYPE_TUNNEL: obj = new Tunnel(); break;
	case OBJECT_TYPE_PORTAL:
		{	
			float r = atof(tokens[4].c_str());
			float b = atof(tokens[5].c_str());
			int scene_id = atoi(tokens[6].c_str());
			obj = new CPortal(x, y, r, b, scene_id);
		}
		break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") { 
			section = SCENE_SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") { 
			section = SCENE_SECTION_ANIMATIONS; continue; }
		if (line == "[ANIMATION_SETS]") { 
			section = SCENE_SECTION_ANIMATION_SETS; continue; }
		if (line == "[OBJECTS]") { 
			section = SCENE_SECTION_OBJECTS; continue; }
		if (line == "[TILE_MAP_DATA]") { section = SCENE_SECTION_TILEMAP_DATA; continue; }
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	


		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
			case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
			case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
			case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
			case SCENE_SECTION_TILEMAP_DATA: _ParseSection_TILEMAP_DATA(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way
	//if (player->isFireAttacking&&player->onground)
	//{
	//	fire* Fire = new fire();
	//	Fire->Attack(player);
	//	if (fires.size() < FIRE_AMOUNT)
	//		fires.push_back(Fire);
	//	DebugOut(L"fire 1 \n");
	//}
	//if (fires.size() == FIRE_AMOUNT)player->isFireAttacking = false;
	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return; 

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);
	CGame *game = CGame::GetInstance();
	if (cy <= ONGROUND_Y - game->GetScreenHeight()*0.98)game->SetCamPosY(cy- game->GetScreenHeight() *0.02);
	else game->SetCamPosY(ONGROUND_Y- game->GetScreenHeight());
	if (cx <= game->GetScreenWidth() / 2)
		game->SetCamPosX(0.0f);
	else if(cx >= mapWidth - game->GetScreenWidth()/2 -20) game->SetCamPosX(mapWidth - game->GetScreenWidth()-20);
	else game->SetCamPosX(cx - game->GetScreenWidth() / 2);

}

void CPlayScene::Render()
{
	tileMap->Draw();
	/*for (int i = 0; i < fires.size(); i++)
		fires[i]->Render();*/
	for (int i = 1; i < objects.size(); i++)
		objects[i]->Render();
	objects[0]->Render();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CMario *mario = ((CPlayScene*)scence)->GetPlayer();
	
	switch (KeyCode)
	{
	case DIK_1: 
		mario->Reset();
		break;
	case DIK_2:
		mario->Reset2();
		break;
	case DIK_3:
		mario->Reset3();
		break;
	case DIK_SPACE :
		DebugOut(L"key space %d \n");
		if (mario->maxspeed && !mario->onground&& mario->level == MARIO_LEVEL_TAIL)
		{
			mario->fly_y = mario->y;
			mario->SetState(MARIO_STATE_FLYING);
		}
		else if (mario->onground)
		{
			mario->SetState(MARIO_STATE_JUMP);
		}
		if (mario->level == MARIO_LEVEL_TAIL && mario->onground == false)
		{
			mario->drop_y = mario->y;
			mario->SetState(MARIO_STATE_TAILDROP);
		}
		break;
	case DIK_D:
		if (mario->level == MARIO_LEVEL_TAIL && !mario->isWaitingForAni)
			mario->SetState(MARIO_STATE_TAIL_ATTACK);
		else if (mario->level == MARIO_LEVEL_FIRE)
		{
			mario->SetState(MARIO_STATE_FIRE_ATTACK);
		}
		break;
	}
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	switch (KeyCode)
	{
	
	case DIK_SPACE:
		mario->SetState(MARIO_STATE_DROP);
		break;
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_UNCROUCH);
		break;
	case DIK_D:
		if (mario->isCarrying)
		{
			mario->isKicking = true;
		}

	}
}

void CPlayScenceKeyHandler::KeyState(BYTE *states)
{
	CGame *game = CGame::GetInstance();
	CMario *mario = ((CPlayScene*)scence)->GetPlayer();
		/* disable control key when Mario die */
		if (mario->GetState() == MARIO_STATE_DIE) return;
		if (mario->isWaitingForAni)
			return;
		if ((mario->vx > 0 && game->IsKeyDown(DIK_LEFT)) || (mario->vx < 0 && game->IsKeyDown(DIK_RIGHT)))
			{
				mario->SetState(MARIO_STATE_STOP);
			}
		else if (game->IsKeyDown(DIK_DOWN))
			mario->SetState(MARIO_STATE_CROUCH);
		else if (game->IsKeyDown(DIK_LSHIFT) && game->IsKeyDown(DIK_LEFT))
			mario->SetState(MARIO_STATE_RUNNING_LEFT);
		else if (game->IsKeyDown(DIK_LSHIFT) && game->IsKeyDown(DIK_RIGHT))
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);
		else if (game->IsKeyDown(DIK_RIGHT))
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
		else if (game->IsKeyDown(DIK_LEFT))
			mario->SetState(MARIO_STATE_WALKING_LEFT);
		else
			mario->SetState(MARIO_STATE_IDLE);
	
}