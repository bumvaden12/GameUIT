﻿#include "map.h"
TileMap::TileMap(int ID, LPCWSTR filePath_texture, LPCWSTR filePath_data, int num_row_on_texture, int num_col_on_textture, int num_row_on_tilemap, int num_col_on_tilemap, int tileset_width, int tileset_height)
{
	id = ID;

	this->filePath_texture = filePath_texture;
	this->filePath_data = filePath_data;

	this->num_row_on_texture = num_row_on_texture;
	this->num_col_on_textture = num_col_on_textture;
	this->num_row_on_tilemap = num_row_on_tilemap;
	this->num_col_on_tilemap = num_col_on_tilemap;
	this->tileset_width = tileset_width;
	this->tileset_height = tileset_height;

	LoadMap();
	Load();
}


void TileMap::LoadMap()
{
	CTextures* texture = CTextures::GetInstance();

	texture->Add(id, filePath_texture, D3DCOLOR_XRGB(255, 0, 255));

	LPDIRECT3DTEXTURE9 texTileMap = texture->Get(id);


	int id_sprite = 0;
	for (UINT i = 0; i < num_row_on_texture; i++)
	{
		for (UINT j = 0; j < num_col_on_textture; j++)
		{
			int id_SPRITE = id + id_sprite;
			sprites->Add(id_SPRITE, tileset_width * j, tileset_height * i, tileset_width * (j + 1), tileset_height * (i + 1), texTileMap);
			id_sprite = id_sprite + 1;
		}
	}
}

void TileMap::Load()
{

	ifstream fs(filePath_data, ios::in);

	if (fs.fail())
	{
		
		fs.close();
		return;
	}

	for (int i = 0; i < num_row_on_tilemap; i++)
	{
		for (int j = 0; j < num_col_on_tilemap; j++)
			fs >> tilemap[i][j];
	}
	fs.close();
}


void TileMap::Draw()
{
	int firstcol = (int)CGame::GetInstance()->GetCamPosX() / tileset_width;
	int lastcol = firstcol + (SCREEN_WIDTH / tileset_width);
	int firstrow = (int)CGame::GetInstance()->GetCamPosY() / tileset_height;
	int lastrow = firstcol + (SCREEN_WIDTH / tileset_height);
	for (UINT i = firstrow; i <=lastrow; i++)
	{
		for (UINT j = firstcol; j <= lastcol; j++)
		{
			float x = tileset_width * (j - firstcol) + CGame::GetInstance()->GetCamPosX() - (int)(CGame::GetInstance()->GetCamPosX()) % tileset_width;
			float y = tileset_height * (i - firstrow) + CGame::GetInstance()->GetCamPosY() - (int)(CGame::GetInstance()->GetCamPosY()) % tileset_height;
			sprites->Get(tilemap[i][j] + id)->Draw(x, y);
		}
	}
}

TileMap::~TileMap()
{
}

int TileMap::GetWidthTileMap()
{

	return (num_col_on_tilemap - 32) * tileset_width;


}
float TileMap::GetHeightMap()
{
	return num_row_on_tilemap * tileset_height;
}
float TileMap::GetWidthMap()
{
	return num_col_on_tilemap * tileset_width;
}

