#include "Tree.h"

void Tree::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
}

void Tree::Render()
{
	animation_set->at(IdObj)->Render(x, y);
}

void Tree::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + 16;
	bottom = y + 16;
}

Tree::Tree(int IdObj)
{
	this->IdObj = IdObj;
}
