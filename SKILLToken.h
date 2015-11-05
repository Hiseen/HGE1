#pragma once
#include "Skills.h"
class ObjectPool;
class Hero;
class Skills;
class FontPool;
#ifndef SKILLTOKEN_H_
#define SKILLTOKEN_H_
#define BANNED 0 //禁用
#define ACTIVE 1 //可接收技能
#define LOCKED 2 //已锁技能
//没啥用：
#define USED 3 //开始用
//skilltoken类用于存储skill的种类，右键后调用start函数创建skill再插入对象池中
class SKILLToken :
	public SKILL
{
public:
	SKILLToken() :SKILL(0, 0, 0, 0, 1)
	{
		skilltype = 0;
		state = BANNED;
		force_to_ban = false;
		SetSize(1.0);
		SetAlive(true);
		finishb = true;
		tempstate = 0;
		SetImageAngle(GetRot());
	};
	~SKILLToken();
	//
	void Reset()
	{
		tempstate = 0;
		skilltype = 0;
		state = BANNED;
		force_to_ban = false;
	}
	virtual void action(){};
	void Ban()
	{
		if (state != BANNED)
		{
			tempstate = state;
			state = BANNED;
			force_to_ban = true;
		}
	};
	void StartUp(){ state = ACTIVE; };
	void Release()
	{
		state = ACTIVE;
		skilltype = 0;
	}
	void Re_Active()
	{
		if (force_to_ban)
		{
			state = tempstate;
			tempstate = 0;
			force_to_ban = false;
		}
	};
	int GetState()const
	{
		if (!force_to_ban)
			return state;
		else
		{
			if (tempstate)
				return LOCKED;
			else
				return ACTIVE;
		}
	};
	void LockWithSkill(int skilltype1)
	{
		skilltype = skilltype1;
		if (skilltype1 != 0)
			state = LOCKED;
		else
			state = ACTIVE;
	};
	virtual void born()
	{
		
	};
	//undead:
	virtual void death(){};
	virtual void Underattack(){};
	//
	int GetSkillType()const{ return skilltype; };
	virtual int TellType(){ return ANIM; };
	virtual bool IsSkill(){ return true; };
	virtual bool IsBoss(){ return false; };
	virtual bool IsItem(){ return false; };
	virtual bool AfterBlur(){ return true; };
	virtual void effect(GameObject *vir){};
	virtual bool Start();
	virtual int GetAnimID();

private:
	int state;
	int skilltype;
	int force_to_ban;
	int tempstate;
};

#endif