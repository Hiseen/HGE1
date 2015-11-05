#pragma once
#include "GameObject.h"
enum ShieldType
{
	//抵挡伤害：
	Tank,
	//抵挡技能：
	Guard,
	//技能反射：
	SkillReflect,
	//伤害反射：
	DmgReflect
};
class Shield :
	public GameObject
{
public:
	Shield(GameObject *host1, ShieldType type1, int shieldnum1, int shieldHP, bool Isinfinite = false) :GameObject(host1->GetX(), host1->GetY(), 0, 1, 0, 0)
	{
		HGE *hge = hgeCreate(HGE_VERSION);
		SetRot(hge->Random_Float(0, 6.28));
		host = host1;
		type = type1;
		SetSpeed(0);
		shieldnum = shieldnum1;
		infinate = Isinfinite;
		last_HP = host->GetMaxHP()*4;
		SetCanHaveErrorState(false);
		if (shieldnum == SHIELD2)
			SetSoild(true);
		switch (type)
		{
		case Tank:
			SetMaxHP(last_HP);
			SetHP(GetMaxHP());
			break;
		case Guard:
			SetDEF(99999);
			SetDodge(100);
			break;
		case SkillReflect:
			SetDEF(99999);
			SetDodge(100);
			break;
		case DmgReflect:
			SetDEF(99999);
			SetDodge(100);
			break;
		}
		last_times = shieldHP;
	}
	~Shield();
	virtual void action();
	virtual void born()
	{
		Swell2();
		if (host)
		{
			SetPos(host->GetX(), host->GetY());
			if (host->IsDead())
			{
				host = NULL;
				Suicide();
			}
		}
	};
	virtual void death()
	{
		Contract();
		if (!IsPlayedDeadAnim())
		{
			SetPlayedDeadAnim();
			if (host)
			host->SetHasShield(false);
		}
	};
	void MinusDurability()
	{
		if (infinate)
		if (type == Guard || type==SkillReflect || type==DmgReflect)
		last_times--;
	};
	ShieldType GetShieldType()const{ return type; };
	virtual void Underattack(GameObject *obj);
	virtual bool IsShield(){ return true; };
	virtual bool NeedRenderFont(){ return false; };
	virtual bool IsSkill(){ return false; };
	virtual bool IsBoss(){ return false; };
	virtual bool IsItem(){ return false; }
	virtual bool AfterBlur(){ return true; };
	virtual int GetCollisionRange()
	{
     if(shieldnum==SHIELD2)
		return 60;
	else
		return 0;
	};
	virtual bool BulletBoomEffect(){ return false; };
	virtual int TellType(){ return shieldnum; };

	GameObject *GetHostOfShield()const{ return host; };
private:
	ShieldType type;
	int shieldnum;
	bool infinate;
	//持续时间：
	int last_time;
	//持续次数：
	int last_times;
	//持续血量：
	int last_HP;
	GameObject *host;
};

