#include "Items.h"
#include "Utils.h"
#include "QuestionBrick.h"
#include "ShinyBrick.h"
#include "platform.h"
#include "Effect.h"
void Items::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (IdItem == ITEM_SWITCH)
	{
		left = x;
		top = y;
	}
	else {
		left = x + ITEM_BBOX_L;
		top = y + ITEM_BBOX_T;
	}
	if (!Active)
	{
		right = x + ITEM_BBOX_R;
		bottom = y + ITEM_BBOX_B;
		if (IdItem == ITEM_MONEY_IDLE)
		{
			left = x - TAIL_BBOX_WIDTH;
			bottom = y + ITEM_BBOX_MONEY_IDLE;
		}
	}
	else {
		right = x + ITEM_BBOX_R_ACTIVE;
		bottom = y + ITEM_BBOX_B_ACTIVE;
		if (IdItem == ITEM_SWITCH)
		{
			right = x + ITEM_BBOX_SWITCH;
			bottom = y + ITEM_BBOX_SWITCH;
			if (state == ITEM_SWITCH_STATE_OFF)
				bottom = y + ITEM_BBOX_SWITCH_B;
		}
		else if (IdItem == ITEM_MONEY_ROTATOR) {
			right = x + ITEM_BBOX_MONEY_IDLE - ITEM_BBOX_L;
			bottom = y + ITEM_BBOX_MONEY_IDLE;
		}
		else if (IdItem == ITEM_MUSHROOM_GREEN) {
			left = x;
			top = y;
			right = x + ITEM_BBOX_MUSHROOM;
			bottom = y + ITEM_BBOX_MUSHROOM;
		}
	}
}

void Items::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!SetPosStart)
	{
		this->X_Start = this->x;
		this->Y_Start = this->y;
		SetPosStart = true;
	}
	if (effect != NULL)
		effect->Update(dt);
	if (IdItem == ITEM_MONEY_IDLE && !OfBrick)
		Active = true;
	CGameObject::Update(dt);
	//x += dx;
	//y += dy;
	
	if (Active)
	{
		if (IdItem == ITEM_TREE_LEAF)
		{
			vy += ITEM_GRAVITY * dt;
			vy = 2 * ITEM_GRAVITY * dt;
			if ((this->x - this->X_Start) > ITEM_TREE_LEAF_X_DISTANCE_X_START)
				vx -= ITEM_TREE_LEAF_VX * dt;
			else
				vx += ITEM_TREE_LEAF_VX * dt;
		}
		else if (IdItem == ITEM_MONEY) {
			vy += ITEM_GRAVITY * dt;
			if (y - Y_Start > 0 && vy != 0)
			{
			/*	int CurentScore = CGame::GetInstance()->GetScore();
				CGame::GetInstance()->SetScore(CurentScore + 100);
				int CurentMoney = CGame::GetInstance()->GetMoney();
				CGame::GetInstance()->SetMoney(CurentMoney + 1);*/
				AniEffect = SpriteEffectStart + EFFECT_100;
				effect = new Effect(this->x, this->y - BRICK_BBOX_HEIGHT, AniEffect);
				x = -100; y = -100.0f; vy = 0; vx = 0;
				Active = false;
			}
		}
		else if (IdItem == ITEM_MUSHROOM_GREEN) {
			//this->vx = -ITEM_MUSHROOM_SPEEED_VX * dt;
			//vy += ITEM_GRAVITY/5 * dt;
		}
		else if (IdItem == ITEM_SWITCH)
		{
			if (Y_Start - y > ITEM_SWITCH_YSTART_DISTANCE_Y)
				vy = 0;
			if (state == ITEM_SWITCH_STATE_OFF) {
				for (int i = 0; i < coObjects->size(); i++) {
					if (dynamic_cast<SBrick*>(coObjects->at(i)))
					{
						//Active = false;
						SBrick* brick = dynamic_cast<SBrick*>(coObjects->at(i));
						brick->broke = true;
					}
				}
			}
		}
	}
	else {
		vy = 0; vx = 0;
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResultPro;
	coEvents.clear();
	if (Active && IdItem == ITEM_MUSHROOM_GREEN)
		CalcPotentialCollisions(coObjects, coEvents);
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else {
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0, rdy = 0;
		FilterCollision(coEvents, coEventsResultPro, min_tx, min_ty, nx, ny, rdx, rdy);
		x += min_tx * dx + nx * 0.3f;
		y += min_ty * dy + ny * 0.3f;

		if (nx != 0)
			vx = 0;
		if (ny != 0)
			vy = 0;
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	vector<LPGAMEOBJECT> coEventsResult;
	coEventsResult.clear();
	CalCollisions(coObjects, coEventsResult);
	int sizeCo = coEventsResult.size();
	if (!CollTail)
		if (sizeCo == 0 && !MarioGetMoney && IdItem == ITEM_MONEY_IDLE)
			Active = true;
	if (sizeCo == 0 && IdItem == ITEM_MUSHROOM_GREEN)
	{
		vx = -ITEM_MUSHROOM_VX * dt;
		vy += ITEM_GRAVITY / 6 * dt;
	}
	if (sizeCo != 0)
	{
		for (UINT i = 0; i < sizeCo; i++)
		{
			LPGAMEOBJECT e = coEventsResult[i];
			if (dynamic_cast<CMario*>(e)) // if e->obj is Goomba 
			{
				CMario* mario = dynamic_cast<CMario*>(e);
				if (IdItem == ITEM_TREE_LEAF)
				{
					if (mario->GetLevel() == MARIO_LEVEL_TAIL && vy > 0)
					{
						AniEffect = SpriteEffectStart + EFFECT_1000;
						effect = new Effect(this->x, this->y - BRICK_BBOX_HEIGHT, AniEffect);
						/*int CurentScore = CGame::GetInstance()->GetScore();
						CGame::GetInstance()->SetScore(CurentScore + 1000);
						int CurentMoney = CGame::GetInstance()->GetMoney();
						CGame::GetInstance()->SetMoney(CurentMoney + 1);*/
						x = -100; y = -100; vy = 0; vx = 0;
						Active = false;
					}
					if (mario->GetLevel() == MARIO_LEVEL_TAIL && sizeCo == 1)
					{
						x = -100; y = -100; vy = 0; vx = 0;
						Active = false;
					}
					if (mario->GetLevel() == MARIO_LEVEL_BIG)
						mario->SetLevel(MARIO_LEVEL_TAIL);
				}
				else if (IdItem == ITEM_MONEY_IDLE)
				{
					
					if (Active)
					{
					/*	int CurentScore = CGame::GetInstance()->GetScore();
						CGame::GetInstance()->SetScore(CurentScore + 100);
						int CurentMoney = CGame::GetInstance()->GetMoney();
						CGame::GetInstance()->SetMoney(CurentMoney + 1);*/
						x = -100; y = -100; vy = 0; vx = 0;
						Active = false;
					}
					MarioGetMoney = true;
				}
				else if (IdItem == ITEM_MONEY_ROTATOR) {
					Active = false;
					/*int CurentScore = CGame::GetInstance()->GetScore();
					CGame::GetInstance()->SetScore(CurentScore + 100);
					int CurentMoney = CGame::GetInstance()->GetMoney();
					CGame::GetInstance()->SetMoney(CurentMoney + 1);*/
					x = -100; y = -100.0f; vy = 0; vx = 0;
				}
				else if (IdItem == ITEM_MUSHROOM_GREEN && vx != 0) {
					AniEffect = SpriteEffectStart + EFFECT_1_UP;
					effect = new Effect(this->x, this->y, AniEffect);
				/*	int CurentLife = CGame::GetInstance()->GetLife();
					CGame::GetInstance()->SetLife(CurentLife + 1);*/
					this->Active = false;
					x = -100; y = -100.0f; vy = 0; vx = 0;
				}
			}
			else if (dynamic_cast<QBrick*>(e))
			{
				QBrick* QuestionBrick = dynamic_cast<QBrick*>(e);
				if (QuestionBrick->disabled)
				{
					if (IdItem == ITEM_TREE_LEAF || IdItem == ITEM_MONEY)
						vy = -ITEM_DEFLECT_SPEED * dt;
					Active = true;
				}
				if (QuestionBrick->disabled)
				{
					if (IdItem == ITEM_SWITCH)
						vy = -ITEM_SWITCH_VY * dt;
					if (IdItem == ITEM_MUSHROOM_GREEN && Active)
					{
						vx = 0; vy = -ITEM_MUSHROOM_VY_UP * dt;
					}
					Active = true;
				}
			}

			else if (dynamic_cast<SBrick*>(e)) {
				Active = false;
				OfBrick = true;
			}
		}
	}
}
void Items::Render()
{
	if (effect != NULL)
		effect->Render();
	int ani = -1;
	if (this->Active)
		if (IdItem == ITEM_TREE_LEAF)
			ani = ITEM_ANI_TREE_LEAF;
		else if (IdItem == ITEM_MONEY || IdItem == ITEM_MONEY_ROTATOR)
			ani = ITEM_ANI_MONEY;
		else if (IdItem == ITEM_MONEY_IDLE)
			ani = ITEM_ANI_MONEY_IDLE;
		else if (IdItem == ITEM_MUSHROOM_GREEN)
			ani = ITEM_ANI_MUSHROOM_GREEN;
		else if (IdItem == ITEM_SWITCH)
		{
			if (state != ITEM_SWITCH_STATE_OFF)
				ani = ITEM_ANI_SWITCH_ON;
			else {
				ani = ITEM_ANI_SWITCH_OFF;
			}
		}
	if (ani != -1)
		animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

Items::Items(int IdItem, int SpriteEffectStart)
{
	this->IdItem = IdItem;
	if (IdItem == ITEM_MONEY_ROTATOR)
		Active = true;
	this->SpriteEffectStart = SpriteEffectStart;
}

void Items::SetState(int state)
{
	switch (state)
	{
	case ITEM_STATE_LISTEN:
		vx = 0;
		vy = 0;
		break;
	case ITEM_SWITCH_STATE_OFF:
		if (this->state != ITEM_SWITCH_STATE_OFF)
			y += ITEM_BBOX_SWITCH - ITEM_BBOX_SWITCH_B;
		break;
	}
	CGameObject::SetState(state);
}
void Items::GetHit()
{
		Active = false;
		CollTail = true;
}