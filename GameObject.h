#pragma once
#include "FontPool.h"
#include "math.h"
#include "VirtualObject.h"
#include <memory>
#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_
using namespace std;
enum errstate
{
	none = 1,
	freeze = 2,
	toxic = 4,
	vertigo = 8,
	heal = 16
};
class GameObject:public VirtualObject
{
public:
	GameObject(
		float x1, float y1, double speed1,
		int HP1, int MP1, int DEF1, int DMG1 = 0,
		bool reachable1 = true, double size1 = 2.0, bool alive1 = false,
		errstate errstate2 = none,SceneNumber scenenumber1=GameScene
		);
	~GameObject();	
    static GameObject* MakePosObject(float x, float y)
	{
		GameObject* a=new GameObject(x, y, 0, 0, 0, 0);
		return a;
	}

	bool BaseAction()
	{
		RestrictPos();
		ErrorState();
		AdjustHP();
		Invulnerable();
		SetImageAngle(GetRot());
		return true;
	}
	virtual bool BulletBoomEffect(){ return false; };
	virtual int GetCollisionRange(){ return 0; };
	virtual int GetAnimID(){ return 0; };
	virtual void action(){};
	virtual void born(){};
	virtual void death(){};
	virtual bool IsEnemy(){ return false; };
	virtual void AfterPlayDeadAnim(){};
	virtual void Underattack(GameObject *obj){};
	virtual bool IsShield(){ return false; };
	virtual bool IsBullet(){ return false; };
	virtual bool IsSkill(){ return false; };
	virtual bool IsBoss(){ return false; };
	virtual bool IsItem(){ return false; };
	virtual bool IsMine(){ return false; };
	virtual bool IsUnit(){ return false; }
	virtual bool NeedRenderFont(){ return true; };
	virtual bool NeedToDestroyWhenBossArrive(){ return true; }
	virtual bool AfterBlur(){ return false; };
	virtual bool CanChangeGlobalPos(){ return false; };
	void SetDashToCenter(){ dashtocenter = true; }
	void SetAlive(bool alive1);
	virtual int TellType(){ return OBJECT; };
	bool IsDelayrender()const{ return delayrender; };
	void SetDelayrender(bool newdelay){ delayrender = newdelay; };
	//真没想好怎么实现：
	//virtual void ReflectAngle(GameObject *obj){};
	//virtual void ReflectDmg(GameObject *obj){};
	void SetNeedChangeColor(bool need=true){ needchangecolor = need; }
	bool IsNeedChangeColor()const{ return needchangecolor; };
	virtual bool NeedChangeBlendMode(){ return false; };
	void SetChangeColor(int newCA, int newCR, int newCG, int newCB)
	{
		CA = newCA;
		CR = newCR;
		CG = newCG;
		CB = newCB;
	};
	DWORD GetChangeColor()
	{
		return ARGB(CA, CR, CG, CB);
	}
	
	int GetErrStateTime(errstate err);
     bool IsCoincide(GameObject *obj,bool force_to_10=false);
	double  GetAngle(GameObject *obj,bool return1=false);
	void DmgDispose(int &damage,int DEFbreak);
	void GoToAnotherObject(GameObject *obj);
	void AwayFromOneObject(GameObject *obj);
	void GoWithAnAngle(float dt,double angle);
	void GoWithAnArc( GameObject *end,bool fangxiang);
	void SpinAroundObject(GameObject *obj);
	void BoomEffect(GameObject *obj, float dt,int effect);
	int GetMaxHP()const{ return MAX_HP; };
	float GetX()const;
	float GetY()const;
	int GetDEF()const;
	void SetDEF(int newDEF);
	void SetPos(float x1, float y1);
	int GetDMG(bool hascrit=true)const
	{
		if (hascrit)
		{
			HGE *hge = hgeCreate(HGE_VERSION);
			if (hge->Random_Int(0, 100) <= Crit)
				return DMG*CritDMG;
			else
				return DMG;
		}
		else return DMG;
	};
	void SetDMG(int newdmg){ DMG = newdmg; };
	bool IsAlive()const{ return alive; };
	void SetReachable(bool a)
	{
		reachable = a;
	};
	void SetMaxHP(int newmaxhp)
	{
		if (newmaxhp > 1)
			MAX_HP = newmaxhp;
		else
			MAX_HP = 1;
	};
	double GetSize()const; 
	double GetRot()const;
	virtual double GetSpeed()const;
	float GetSpeedRate()const{ return speedrate; };
	double GetImageAngle()const{ return angleforimage; };
	void SetImageAngle(double newangle)
	{ 
		if(!IsSoild())
			angleforimage = newangle; 
	};
	void BeTargeted(){ Targeted = true; };
	bool IsTargeted()const{ return Targeted; };
	void CancelTargeted(){ Targeted = false; };
	void SetSpeed(double speed1);
	double GetDistance(GameObject *obj);
	void SetReverseDMG(bool newreverseDMG){ reverseDMG = newreverseDMG; };
	bool GetReverseDMG()const{ return reverseDMG; };
	virtual bool varyHP(int dHP);
	void varyMP(int dMP);
	bool IsReachable()const{ return reachable; };
	float GetZ()const{ return Z; };
	void SetZ(float newz){ Z = newz; };
	void Invulnerable()
	{
		if (IsAttacked())
		{
			invulnerable_time--;
			SetNeedChangeColor();
			SetChangeColor(255, 255, 0, 0);
		}
		if (invulnerable_time <= 0)
		{
			FlashAttacked();
			SetNeedChangeColor(false);
			invulnerable_time = 3;
		}
	}
	//两个一组：
	void UpDownSpeed()
	{
		/*
		static int a = 0;
		if (!a)a = speed;
		static bool b = true;
		if (b)speed-=a/50;
		else
			speed+=a/50;
		if (speed <= 0)b = false;
		if (speed >= a)b = true;
		*/
	}
	void ErrorState();
	void Attacked(){ attacked = true; };
	bool SetErrorState(errstate err, double var, int last_time = 1);
	bool IsAttacked(){ return attacked; };
	void FlashAttacked(){ attacked = false; };
	bool IsForcedToDie()const{ return force_to_die; };
	void Suicide(bool forced=false)
	{
		if (finishb)
			HP = -1;
		if (forced)
		{
			force_to_die = true;
			HP = -1;
		}
	};
	void SkillRevive(){ HP = 1; };
	int GetHP()const{ return HP; };
	int GetMP()const{ return MP; };
	void SetMP(int newMP){ MP = newMP; };
	void AdjustHP()
	{
		if (HP > MAX_HP)HP = MAX_HP;
		if (HP < 0)HP = 0;
		if (MP < 0)MP = 0;
	}
	bool RestrictPos()
	{
		//出屏幕外快速飞行进屏幕
		HGE *hge = hgeCreate(HGE_VERSION);
		if (dashtocenter)
		{
			if (x > ACT_START_X && x < ACT_END_X && y < ACT_END_Y && y > ACT_START_Y)
				dashtocenter = false;
		}
		else
		{
			if (x < ACT_START_X-100 || x > ACT_END_X+100 || y > ACT_END_Y+100 || y < ACT_START_Y-100)
			Suicide(true);
		}
		return true;
	}
	void RestrictPos2()
	{
		if (x > ACT_END_X)x = ACT_END_X;
		if (x < ACT_START_X)x = ACT_START_X;
		if (y > ACT_END_Y)y = ACT_END_Y;
		if (y < ACT_START_Y)y = ACT_START_Y;
	}
	void SetHP(int newhp)
	{
		if (!finishb)
		{
			HP = newhp;
			MAX_HP = HP;
		}
	}
	int GetBreakArmor()const{ return PenetrateArmor; };
	void SetBreakArmor(int DEFbreak){ PenetrateArmor = DEFbreak; };
	void ResetBreakArmor(){ PenetrateArmor = 0; };
     bool IsDead()const
	{
		if (HP <= 0)return true;
		else return false;
	};
	void SetRot(float rot1)
	{
		angle = rot1;
	}
	void Swell(bool faster=false);
	void Contract(bool instant=false); 
	void SetSize(double size1){ size = size1; };
	void SetSoild(bool soild)
	{
		DoNotSpin = soild;
	};
	bool IsSoild()const
	{
		return DoNotSpin;
	};
	bool IsPlayedDeadAnim()const{ return deadanimover; };
	void SetPlayedDeadAnim(){ deadanimover = true; };
	//图形效果：
	void SpinWithRadius(GameObject *target,int radius)
	{
		float delta = (float)1/(radius*2);
		SetRot(GetRot() + delta);
		double angle = GetRot() + 1.57;
		SetPos(target->x + (cos(angle) - sin(angle))*radius, target->y+(cos(angle) + sin(angle))*radius);
	};
	void Recoil();
	float GetImgH()const{ return varimgh; };
	float GetImgW()const{ return varimgw; };
	void Swell2();
	void SetVarImg(float varh, float varw)
	{
		varimgh = varh;
		varimgw = varw;
	};

	//Scene 
	SceneNumber GetSceneNumber()const
	{
		return scenenumber;
	};
	errstate GetErrorState()const
	{
		return errstate1;
	};
	void ReleseAllErrorStates()
	{
		errlast_time_freeze = 1;
		errlast_time_toxic = 1;
		errlast_time_vertigo = 1;
		errlast_time_heal = 1;
		ErrorState();
	}
	void SetInnerType(int inn)
	{
		innertype = inn;
	}
	int GetInnerType()const
	{
		return innertype;
	}
	void SetCanHaveErrorState(bool can)
	{
		canhaveerrorstate = can;
	}

	void SetCritDMG(float newcritdmg)
	{
		if (newcritdmg > 1)
			CritDMG = newcritdmg;
		else
			CritDMG = 1;
	}
	void SetCrit(int newcrit)
	{
		if (newcrit > 100)
			Crit = 100;
		else
		{
			if (newcrit > 0)
				Crit = newcrit;
			else
				Crit = 0;
		}
	}
	int GetCrit()const{ return Crit; };
	float GetCritDMG()const{ return CritDMG; };
	void SetLockState(bool islocked)
	{
		this->islocked = islocked;
	};
	bool GetIsLocked()const{ return islocked; };
	void SetHasShield(bool has){ hasshield = has; };
	bool GetHasShield()const{ return hasshield; };
	void SetDodge(int newdodge)
	{
		if (newdodge > 100)
			Dodge = 100;
		else
		{
			if (newdodge > 0)
				Dodge = newdodge;
			else
				Dodge = 0;
		}
	}
	int GetDodge()const{ return Dodge; };
private:
	float x, y;
    double speed;
	double size;
    double angle;
	int MAX_HP;
	int HP;
	int MP;
	int DMG;
	bool alive;
	errstate errstate1;
	bool reachable;
	int DEF;
	bool attacked;
	float varimgh;
	float varimgw;
	bool DoNotSpin;
	bool Targeted;
	SceneNumber scenenumber;
	double angleforimage;
	bool deadanimover;
	//伤害吸收：
	bool reverseDMG;
	//非正常死亡：
	bool force_to_die;
	int CA, CR, CG, CB;
	bool needchangecolor;
	int invulnerable_time;
	double errvar_freeze;
	int errlast_time_freeze;
	int errvar_toxic;
	int errlast_time_toxic;
	double  back_speed;
	int errvar_vertigo;
	int errlast_time_vertigo;
	int errvar_heal;
	int errlast_time_heal; 
	float Z;
	bool delayrender;
	bool canhaveerrorstate;
	bool dashtocenter;
	float CritDMG;
	int Crit;
	int Dodge;
	bool islocked;
	bool hasshield;
	int PenetrateArmor;
	float speedrate;
	int innertype;
};

#endif