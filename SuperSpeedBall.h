#pragma once
#include"FontPool.h"
#include "Item.h"
#include "RenderPool.h"
#include "EXPUnit.h"
#include "Bullet.h"
#ifndef SUPERSPEEDBALL_H_
#define SUPERSPEEDBALL_H_
class SuperSpeedBall :
	public GameObject
{
public:
	SuperSpeedBall(float x1, float y1, double speed1,
		int HP1, int MP1, int DEF1) :GameObject(x1, y1, speed1, HP1, MP1, DEF1)
	{
		HGE *hge = hgeCreate(HGE_VERSION);
		skillcount = 150;
		IsRage = false;
		posobj = NULL;
		SetDMG(Hero::GetHero().GetLevel());
		if (hge->Random_Int(0, 30) < Hero::GetHero().GetLevel())
			innertype = hge->Random_Int(1, 2);
		else
			innertype = 0;
		switch (innertype)
		{
		case 1:
			SetChangeColor(255, 120, 255, 0);
			break;
		case 2:
			SetChangeColor(255, 0,100, 255);
			break;
		}
	};

	virtual void action();
	virtual void born(){ Swell(); };
	virtual void death(){
		Contract(); 
		if (this->IsDead() && !this->IsPlayedDeadAnim())
		{
			SetPlayedDeadAnim();
			RenderPool::GetRenderPool().InstantRender(GetX(), GetY(), SUPERSPEEDBALL, GetRot());
			if (!IsForcedToDie())Hero::GetHero().AddScore(2);
			HGE *hge = hgeCreate(HGE_VERSION);
			for (int i = 0; i < hge->Random_Int(3, 7); i++)
			{
				EXPUnit *exp = new EXPUnit(GetX(), GetY());
				ObjectPool::GetObjPool().InsertObject(exp, true);
			}
			SFXManager::GetSFXManager().PlaySFX(ENEMYBOOMSFX1 + hge->Random_Int(0, 1));
		}
	};
	virtual void Underattack(){};
	virtual int TellType(){ return SUPERSPEEDBALL; };
	virtual int GetCollisionRange(){ return 32; };
	virtual bool IsEnemy(){ return true; };
	~SuperSpeedBall();

private:
	int skillcount;
	bool IsRage;
	GameObject *posobj;
	int innertype;
};

#endif