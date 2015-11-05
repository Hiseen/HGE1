#pragma once
#include"FontPool.h"
#include "SFXManager.h"
#include "RenderPool.h"
#include "SKILL.h"
#include "Hero.h"
#include "hge.h"
class ObjectPool;
#ifndef BULLET_H_
#define BULLET_H_
class Bullet :
	public SKILL
{
public:
	Bullet(Hero *obj,int number_of_bullet, int boomrange = 0, int bulletnums=0) :SKILL(obj->GetDMG(false)+obj->GetExtraDMG(), 0, 0, 0, boomrange)
	{
		stoptime = 0;
		knockback = Hero::GetHero().GetKnockBack();
		SetBreakArmor(obj->GetBreakArmor());
		bulletstats = obj->GetBulletStats();
		ox = 0;
		oy = 0;
		HGE *hge = hgeCreate(HGE_VERSION);
		bulletnum = number_of_bullet;
		switch (Hero::GetHero().GetBullets())
		{	
		case 1:
			SetPos(obj->GetX() + 50 * cos(obj->GetRot() - 1.57), obj->GetY() + 50 * sin(obj->GetRot() - 1.57));
			break;
		case 2:
			SetPos(obj->GetX() + 50 * cos(obj->GetRot() - 1.57 + 0.2 - bulletnums*0.4), obj->GetY() + 50 * sin(obj->GetRot() - 1.57 + 0.2 - bulletnums*0.4));
			break;
		case 3:
			SetPos(obj->GetX() + 50 * cos(obj->GetRot() - 1.57), obj->GetY() + 50 * sin(obj->GetRot() - 1.57));
			SetRot(obj->GetRot()+0.2-0.2*bulletnums);
			bounce = 0;
			break;
		}
		SetSpeed(obj->GetBulletSpeed());
		if (Hero::GetHero().GetBattleState() == RAGE)
		{
			bounce = 1;
		}
		else if (Hero::GetHero().GetBullets()<=2)
		{
			SetRot(obj->GetRot());
			bounce = 0;
		}
		SetCrit(obj->GetCrit());
		SetCritDMG(obj->GetCritDMG());
		maintarget = NULL;
		lifespan = 0;
		SetImageAngle(GetRot());
		count = 5;
		SetHost(&Hero::GetHero());
		basesize = 1+(float)Hero::GetHero().GetAttackLevel()/20;
		SetSize(basesize);
		if (GetSize() <= 0)
		{
			Suicide();
		}
		
	};
	Bullet(GameObject *obj, int number_of_bullet, int boomrange = 0) :SKILL(obj->GetDMG(false), 0, 0, 0, boomrange,true)
	{
		basesize=1;
		stoptime = 0;
		knockback = 2;
		SetDelayrender(true);
		SetBreakArmor(obj->GetBreakArmor());
		bulletstats = no;
		HGE *hge = hgeCreate(HGE_VERSION);
		bulletnum = number_of_bullet;
		SetPos(obj->GetX(), obj->GetY());
		SetCrit(obj->GetCrit());
		SetCritDMG(obj->GetCritDMG());
		SetSpeed(obj->GetSpeed() * 2);
		SetRot(obj->GetRot());
		arc = hge->Random_Int(0,1);
		ox = 0;
		oy = 0;
		lifespan = 0;
		bounce = 0;
		count = 1;
		SetHost(obj);
		if (boomrange == 0)
		{
			SetPointNum(4);
			MakePoint(0, -6, 0);
			MakePoint(1, 0, 7);
			MakePoint(2, 6, 0);
			MakePoint(3, 0, -21);
			SetRange(0);
		}
	};


	Bullet(int x,int y, int number_of_bullet, int boomrange = 0) :SKILL(100, 0, 0, 0, boomrange,true)
	{
		basesize = 1;
		stoptime = 0;
		bulletstats = no;
		SetPos(x, y);
		bulletnum = number_of_bullet;
		SetSpeed(1);
		lifespan = 0;
		bounce = 0;
		count = 5;
	};
	virtual void born()
	{ 
		reversespeed = false;
		SetSize(basesize);
		SetAlive(true);
		finishb = true;
		tempspeed = GetSpeed();
		if (IsThisSkillEnemy())
			bulletnum= BULLET4;
		else
		{
			if (GetHost() && GetHost()->TellType() == HERO)
				bulletnum = BULLET1;
			else
			{
				RenderPool::GetRenderPool().SpawnPS(this, BULLET2);
				bulletnum = BULLET2;
			}
		}
		int R=0,G=0,B = 0;
		if (bulletstats & Frost)
		{
			R |= 46;
			G |= 187;
			B |= 245;
		}
		if (bulletstats & Toxic)
		{
			R |= 131;
			G |= 112;
			B |= 233;
		}
		if (R != 0 || G != 0 || B != 0)
		{
			SetNeedChangeColor();
			SetChangeColor(255, R, G, B);
		}
	};
	virtual void death()
	{
		if (!this->IsPlayedDeadAnim())
		{
			if (bulletstats & DtoBOOM)
			{
				Skills *newskill = new Skills(this,BOOM,50);
				newskill->SetDMG(newskill->GetDMG() / 2);
				ObjectPool::GetObjPool().InsertObject(newskill,true);
			}
			SetPlayedDeadAnim();
			if (bulletnum==BULLET2)
				RenderPool::GetRenderPool().InstantRender(GetX(), GetY(), BULLET1, GetRot(), GetSize());
			else
			RenderPool::GetRenderPool().InstantRender(GetX(), GetY(), bulletnum,GetRot(),GetSize());
		}
		Contract(true);
	};
	virtual void action();
	virtual void effect(GameObject *vir);
	virtual int TellType(){ return ANIM; };
	virtual int GetRange()const{ return SKILL::GetRange()*GetSize(); };
	virtual int GetAnimID()
	{ 
		return bulletnum;
	};
	virtual bool IsBullet(){ return true; };
	virtual bool interaction(GameObject *vir);
	void SetKnockBack(int knock)
	{
		knockback = knock;
	}
	void SetBounce(int times)
	{
		bounce = times;
	};
	void SetStopTime(int time)
	{
		stoptime = time;
	}
	void SetBulletStats(BulletStats stats)
	{
		int newstats = (int)bulletstats | (int)stats;
		bulletstats = (BulletStats)newstats;
	};
	~Bullet();

private:
	int bulletnum;
	int lifespan;
	GameObject *maintarget;
	BulletStats bulletstats;
	int ox, oy;
	int bounce;
	int count;
	int tempspeed;
	bool reversespeed;
	bool arc;
	int knockback;
	int stoptime;
	float basesize;
};

#endif