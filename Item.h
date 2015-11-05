#pragma once
#include "RenderPool.h"
#include "NoticeManager.h"
#include "Hero.h"
class RenderPool;
extern bool ballcrash(float center_x, float center_y, int r, float px, float py);
#ifndef ITEM_H_
#define ITEM_H_
class Item :
	public SKILL
{
public:
	Item(float x, float y, int number) :SKILL(0,0,0,0,50)
	{
		SetSoild(true);
		SetSpeed(0);
		count = 1;
		if (SceneManager::GetSceneManager().GetCurrentMode() == WaveMode)
		{
			SetDelayrender(true);
			SetCanHaveErrorState(false);
			HGE *hge = hgeCreate(HGE_VERSION);
			SetRot(hge->Random_Float(0, 6.28));
			number_of_item = number;
			if (number_of_item == ITEM_DOWN)
				itemHP = 5;
			else
				itemHP = 10;
			SetPos(x, y);
			item_last_time = 100;
			switch (number_of_item)
			{
			case ITEM_UP:
				item_last_time = 250;
				break;
			case ITEM_DOWN:
				item_last_time = 300;
				break;
			case ITEM_RANDOM:
				item_last_time = 250;
				break;
			case ITEM_HP:
				item_last_time = 300;
				break;
			case ITEM_MP:
				item_last_time = 300;
				break;
			}
		}
		else
		{
			SetPos(x, y);
			number_of_item = number;
			itemHP = 5;
			item_last_time = 100;
			SetCanHaveErrorState(false);
		}
		used = false;
	};
	~Item();
	virtual void born()
	{
		SetAlive(true);
		SetSize(1);
		if (!finishb)
		{
			switch (number_of_item)
			{
			case ITEM_UP:
				RenderPool::GetRenderPool().InstantRender(GetX(), GetY(), ITEMADDBEGIN, 0, 1, this);
				finishb = true;
				break;
			case ITEM_DOWN:
				RenderPool::GetRenderPool().InstantRender(GetX(), GetY(), ITEMMINUSBEGIN, 0, 1, this);
				finishb = true;
				break;
			case ITEM_RANDOM:
				RenderPool::GetRenderPool().InstantRender(GetX(), GetY(), ITEMUNKNOWNBEGIN, 0, 1, this);
				finishb = true;
				break;
			case ITEM_HP:
				RenderPool::GetRenderPool().InstantRender(GetX(), GetY(), ITEMHPBEGIN, 0, 1, this);
				finishb = true;
				break;
			case ITEM_MP:
				RenderPool::GetRenderPool().InstantRender(GetX(), GetY(), ITEMMPBEGIN, 0, 1, this);
				finishb = true;
				break;
			default:
				finishb = true;
				break;
			}
		}
	};
	virtual void action();
	virtual void death()
	{ 
		Contract();
		if (!used && IsDead() && !this->IsPlayedDeadAnim())
		{
			if (this->IsForcedToDie())
			{
				switch (number_of_item)
				{
				case ITEM_UP:
					RenderPool::GetRenderPool().InstantRender(GetX(), GetY(), ITEMADDBOOM);
					break;
				case ITEM_DOWN:
					RenderPool::GetRenderPool().InstantRender(GetX(), GetY(), ITEMMINUSBOOM);
					break;
				case ITEM_RANDOM:
					RenderPool::GetRenderPool().InstantRender(GetX(), GetY(), ITEMUNKNOWNBOOM);
					break;
				case ITEM_HP:
					RenderPool::GetRenderPool().InstantRender(GetX(), GetY(), ITEMHPBOOM);
					break;
				case ITEM_MP:
					RenderPool::GetRenderPool().InstantRender(GetX(), GetY(), ITEMMPBOOM);
					break;
				}
			}
			else
			{
				switch (number_of_item)
				{
				case ITEM_UP:
					RenderPool::GetRenderPool().InstantRender(GetX(), GetY(), ITEMADDBOOM);
					break;
				case ITEM_DOWN:
					RenderPool::GetRenderPool().InstantRender(GetX(), GetY(), ITEMMINUSDEAD);
					break;
				case ITEM_RANDOM:
					RenderPool::GetRenderPool().InstantRender(GetX(), GetY(), ITEMUNKNOWNDEAD);
					break;
				case ITEM_HP:
					RenderPool::GetRenderPool().InstantRender(GetX(), GetY(), ITEMHPDEAD);
					break;
				case ITEM_MP:
					RenderPool::GetRenderPool().InstantRender(GetX(), GetY(), ITEMMPDEAD);
					break;
				}
			}
			SetPlayedDeadAnim();
		}
	};
	virtual int TellType(){ return ANIM; };
	virtual bool IsSkill(){ return true; };
	virtual bool IsBoss(){ return false; };
	virtual bool IsItem(){ return true; };
	virtual bool IsEnemy()
	{
			return true;
	}
	virtual bool NeedRenderFont(){ return false; };
	virtual int GetAnimID(){ return number_of_item; };
	virtual void AfterPlayDeadAnim(){ count = 0; };
	virtual void effect(GameObject *obj){};
	virtual void Underattack(GameObject *obj)
	{
		BreakItem();
		while (GetHP() < 0)varyHP(100);
	}
	virtual bool Start(){ return false; };
    void effect();
	void BreakItem(){ itemHP--; };
private:
	int number_of_item;
	int item_last_time;
	bool used;
	int itemHP;
	int count;
};

#endif