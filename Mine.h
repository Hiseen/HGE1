#pragma once
#include "RenderPool.h"
#include "main.h"
#include "Hero.h"
class Mine :
	public GameObject
{
public:
	Mine(float x, float y) :GameObject(0, 0, 0, Hero::GetHero().GetDMG() * 10, 0, 0, SceneManager::GetSceneManager().GetWave()*SceneManager::GetSceneManager().GetWave()*0.5+10)
	{
		HGE *hge = hgeCreate(HGE_VERSION);
		deathcount = 10;
		SetPos(x,y);
		data = 0;
		SetSoild(true);
		SetSpeed(0);
		SetSize(0);
		SetCanHaveErrorState(false);
		innertype = 1;
		if (GetX() > ACT_END_X || GetY() > ACT_END_Y || GetY() < ACT_START_Y || GetX() < ACT_START_X)
		{
			SetDelayrender(true);
		}else
			SetDelayrender(false);
		if (GetX() > ACT_END_X + 128 || GetY() > ACT_END_Y + 128 || GetY() < ACT_START_Y - 128 || GetX() < ACT_START_X - 128)
			Suicide(true);
	};
	virtual void action();
	virtual void born()
	{
		if (GetX() > ACT_END_X || GetY() > ACT_END_Y || GetY() < ACT_START_Y || GetX() < ACT_START_X)
		{
			SetDelayrender(true);
		}
		else
			SetDelayrender(false);
		if (GetX() > ACT_END_X + 128 || GetY() > ACT_END_Y + 128 || GetY() < ACT_START_Y - 128 || GetX() < ACT_START_X - 128)
			Suicide(true);
		SetSize(1);
		SetAlive(true);
		finishb = true;
		SetReachable(true);
	};
	virtual void death()
	{
		if (!IsForcedToDie())
		{
			if (!deathcount)
			{
				Contract();
				if (!IsPlayedDeadAnim())
				{
					if (innertype == 0)
					{
						Skills *skill = new Skills(this, BOOM, 90, true);
						skill->ReverseSkill();
						skill->ReversePenetrate();
						ObjectPool::GetObjPool().InsertObject(skill, true);
					}
					SetPlayedDeadAnim();
				}
			}
			else
				deathcount--;
		}
		else
		{
			Contract(true);
		}
	};
	virtual int TellType(){ return MINE; };
	virtual int GetCollisionRange(){ return 37; };
	virtual bool IsMine(){ return true; };
	virtual bool NeedRenderFont(){ return false; };
	int GetData()const{ return data; };
	void SetData(int data)
	{
		this->data = data;
	}
	~Mine();

private:
	int innertype;
	int deathcount;
	int data;
};

