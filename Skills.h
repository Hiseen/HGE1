#pragma once
#include "SKILL.h"
#include "Shield.h"
#include "RenderPool.h"
#include "SceneManager.h"
#ifndef SKILLS_H_
#define SKILLS_H_

class Bullet;
class Hero;
class Skills :
	public SKILL
{
public:
	//扣蓝一般在放技能之前。。。除了有些持续耗蓝的技能。。虽然也可以做在之前。。
	//技能可以产生技能！用this指针
	//nohost=nocost
	Skills(GameObject* host, int skilltype1, int last_time = 0, bool effectall = false);
	~Skills();
	virtual int GetAnimID(){ return skilltype; };
	virtual void born();
	virtual void action();
	virtual void effect(GameObject *vir);
	virtual void AfterOneEffect();
	virtual void AfterPlayDeadAnim(){ delay = 0; };
	virtual int GetCollisionRange(){ return GetRange(); };
	virtual bool IsMine()
	{
		if (skilltype == LENSPART)
			return true;
		else
			return false;
	}
	virtual void Underattack(GameObject *a)
	{
		
	}
	void ForceToStop()
	{
		//此函数用于强制停止有目标的移动：
		TX = GetX();
		TY = GetY();
	}
	virtual bool interaction(GameObject *vir);
	virtual bool Start()
	{
		return false;
	};
	virtual int TellType(){ return ANIM; };
	virtual void death();
	virtual bool EffectAll(){ return effectall; };
private:
	int skilltype;
	float TX, TY;
	double info;
	int info2;
	bool effective;
	int delay;
	bool followhost;
	bool effectall;
	GameObject *ctrltarget;
};

#endif