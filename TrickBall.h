#pragma once
#include "FontPool.h"
#include "EXPUnit.h"
#include "Item.h"
#include "RenderPool.h"
#include "Bullet.h"
#ifndef TRICKBALL_H_
#define TRICKBALL_H_
class TrickBall :
	public GameObject
{
public:
	TrickBall
	(float x1, float y1, double speed1,
	int HP1, int MP1, int DEF1, int type1 = TRICKBALL) :GameObject(x1, y1, speed1, HP1, MP1, DEF1,0)
	{
		HGE *hge = hgeCreate(HGE_VERSION);
		type = type1;
		bomb = false;
		trigger = false;
		bullets = 0;
		bullettimer = hge->Random_Int(150,250);
		SetDMG((Hero::GetHero().GetLevel() + 1) * 5);
		if (hge->Random_Int(0, 30) < Hero::GetHero().GetLevel())
			innertype = hge->Random_Int(1, 2);
		else
			innertype = 0;
		switch (innertype)
		{
		case 1:
			SetChangeColor(255, 255, 255, 0);
			break;
		case 2:
			SetChangeColor(255, 0, 0, 255);
			break;
		}

	};
	~TrickBall();
	virtual void action();
	virtual void born(){ Swell(); };
	virtual void death()
	{ 
		switch (type)
		{
		case TRICKBALL:
			Contract();
			if (this->IsDead() && !this->IsPlayedDeadAnim())
			{
				SetPlayedDeadAnim();
				RenderPool::GetRenderPool().InstantRender(GetX(), GetY(), TRICKBALL, GetRot());
				if (!IsForcedToDie())Hero::GetHero().AddScore(1);
			}
			break;
		case SLOWBOMBBALL:
			if (!bomb && !IsAlive() && finishb)
			{
				bomb = true;
				Bullet *bul = new Bullet(GetX(), GetY(), BULLET3, 100);
				ObjectPool::GetObjPool().InsertObject(bul, true);
			}
			Contract();
			break;
		case BULLETBALL:
			Contract();
			if (this->IsDead() && !this->IsPlayedDeadAnim())
			{
				SetPlayedDeadAnim();
				RenderPool::GetRenderPool().InstantRender(GetX(), GetY(), BULLETBALL, GetRot());
				if (!IsForcedToDie())Hero::GetHero().AddScore(3);
			}
			break;
		}
		HGE *hge = hgeCreate(HGE_VERSION);
		for (int i = 0; i < hge->Random_Int(5, 9); i++)
		{
			EXPUnit *expunit = new EXPUnit(GetX(), GetY());
			ObjectPool::GetObjPool().InsertObject(expunit, true);
		}
		SFXManager::GetSFXManager().PlaySFX(ENEMYBOOMSFX1 + hge->Random_Int(0, 1));
	};
	virtual int TellType(){ return type; };
	virtual int GetCollisionRange(){ return 32; };
	virtual bool IsEnemy(){ return true; };

private:
	int type;
	bool bomb;
	int bullettimer;
	bool trigger;
	int innertype;
	int bullets;
};
#endif
