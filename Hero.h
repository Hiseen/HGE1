#pragma once
#include "SKILL.h"
#include "NoticeManager.h"
#include "SFXManager.h"
#include "hge.h"

class ObjectPool;
#define MAX_LEVEL 20
#define ADD_ATK 1
#define ADD_DEX 2
#define ADD_INT 3
enum BulletStats
{
	no = 1,
	DtoDMG = 2,//越远攻击力越高
	DtoBOOM = 4,//死亡时爆炸
	DtoDMGdown = 8,//越远攻击力越低
	Follow = 16,//跟踪
	Frost = 32,//冻结
	Toxic = 64,//毒性
	SuckBlood = 128,//吸血
	SpeedUpDown = 256,//速度忽快忽慢
	Spin = 512,//围绕目标旋转
	Stun = 1024,//眩晕
};

#ifndef HERO_H_
#define HERO_H_
#define STAND 0
#define WALK 1
#define SPEED 400
#define RAGE 99
#define WEAKEN 98
#define MAXBULLETNUM 3
class Hero :
	public GameObject
{
public:
	static  Hero& GetHero()
	{
		static Hero myhero1;
		myhero1.AssureInitialized();
		return myhero1;
	}
	~Hero();
	virtual void action();
	virtual bool BulletBoomEffect(){ return false; };
	virtual void born(){
		if (!finishb && lives!=0){ 
			Swell(); SetPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2); while (GetHP() != GetMaxHP())varyHP(1);
			SetTarget(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
			BeTheGod(500);
		}
		
	};
	virtual void death()
	{
		DeadAndReborn();
	};
	virtual void Underattack(SKILL *skill)
	{

	};
	virtual bool varyHP(int dHP)
	{
		if (dHP < 0)
			BeTheGod(25);
		return GameObject::varyHP(dHP);
	}
	virtual int TellType(){ return HERO; };
	bool IsLocked()const
	{
		return locked;
	}
	int GetBullets()const
	{
		if (battlestate == RAGE)
			return 3;
		if (battlestate == WEAKEN)
			return 1;
		return bulletnum;
	};
	void BulletsAdjust(int bullet)
	{
		if (bullet <= MAXBULLETNUM && bullet>0)
	       bulletnum = bullet; 
	};
	int GetState()const{ return state; };
	void SetState(int state1){ state = state1; };
	void move(int targer_x, int target_y);
	void FlashRot()
	{
		GameObject* temp = MakePosObject(TargetX, TargetY);
		GetAngle(temp);
		delete temp;
		temp = NULL;
	}
	void UseSkill();
	void Lock(){ locked = true; }; 
	void Unlock(){ locked = false; };
	void SetTarget(float x, float y, bool isDash=false);
	void Dash();
	int GetScore()const{ return score; };
	int GetBattleState()const{ return battlestate; };
	void SetBattleState(int battle){ battlestate = battle; };
	int GetKills()const{ return kills; };
	int GetBossKills()const{ return bosskill; };
	int GetCombo()const{ return combo; };
	int GetMaxCombo()const{ return maxcombo; };
	int GetModeScore()const{ return modescore; };
	int GetAttackLevel()const{ return atk; };
	void ResetBulletStats()
	{
		bstats = no;
	}
	void SetBulletSpeed(int newspeed)
	{
		if (newspeed > 10)
			bulletspeed = newspeed;
		else
			bulletspeed = 10;
	};
	int GetBulletSpeed()const{ return bulletspeed; };
	void SetModeScore(int basescore){ modescore = basescore; };
	void BeTheGod(int last_time)
	{
		peerless_time = last_time;
		peerless = true;
	}
	void AddScore(int addnum,bool isboss=false)
	{
		score += addnum;
		if (isboss)
			bosskill++;
		else
			kills++;
		combo++;
		if (maxcombo < combo)maxcombo = combo;
	};
	void AbsorbEXP(int newexp)
	{
		EXP += newexp;
		expadded += newexp;
	}
	int GetEXP()const{ return EXP; };
	int GetMaxEXP()const{ return MAX_EXP; };
	int GetMaxMP()const{ return MAX_MP; };
	void SetMaxMP(int newmp)
	{
		if (newmp > 0)
			MAX_MP = newmp;
		else
			MAX_MP = 0;
	};
	int GetLevel()const{ return LV; };
	int GetExtraDMG()const{ return extraDMG; };
	void SetExtraDMG(int newextradmg){extraDMG = newextradmg; };
	void ItemAdd(){ itemget++; };
	int GetItem()const{ return itemget; };
	bool AddBulletStats(BulletStats newstat)
	{
		if (bstats & newstat)
			return false;
		int newstats = (int)bstats | (int)newstat;
		bstats = (BulletStats)newstats;
		return true;
	}
	BulletStats GetBulletStats()const{ return bstats; };
	bool LevelUp();
	bool AddStats(int type);
	void ResetHero();
	virtual double GetSpeed()const
	{
		return GameObject::GetSpeed()*GetSpeedRate();
	};
	bool GameOver()
	{
		if (lives > 0)return false;
		else
		{
			SetNeedChangeColor(false);
			ReleseAllErrorStates();
			if (!IsLocked())
			{
				NoticeManager::GetNoticeManager().InsertNotice("GAME OVER", "It is the end");
				Lock();
			}
			return true;
		}
	};
	void AddDx(float delta)
	{
		
		dx += delta;
	}
	void AddDy(float delta)
	{
		dy += delta;
	}
	void ReverseDx()
	{
		dx = -dx;
	}
	void ReverseDy()
	{
		dy = -dy;
	}
	double GetCharge()const{ return SowrdCharge; };
	int GetMaxCharge()const{ return MAX_SC; };
	int GetShootCD()const{ return ShootCD; };
	int GetAllEXP()const{ return expadded; };
	int GetRecoverMP(){ return recoverMP; };
	void SetLives(int newlives)
	{ 
		if (newlives>0)
		lives = newlives;
	}
	int GetLives()const{ return lives; };
	bool SetShootCD(int newCD)
	{ 
		if (ShootCD == 5)
			return false;
		if (newCD>=5)
		   ShootCD = newCD; 
		    return true;
	};
	void DeadAndReborn()
	{
		if (finishb && IsDead() && lives > 0)
		{
			lives--;
				finishb = false;
		}
	};
	int ToAttack()const
	{
		return Attack+adddmg;
	};
	float GetTX()const
	{
		return TargetX;
	}
	float GetTY()const
	{
		return TargetY;
	}

	bool IsVulnerable(GameObject *obj,bool needcoincide=true)
	{
		if (needcoincide)
		if (!IsDead() && !IsLocked() && !IsGod() && obj->GetHP() > 0 && IsCoincide(obj))
		{
			combo = 0;
			return true; 
		}
		else return false;
		else
		if (!IsDead() && !IsLocked() && !IsGod() && obj->GetHP() > 0)
		{
			combo = 0;
			return true;
		}
		else return false;
	}
	int GetKnockBack()const{ return knockback; };
	void SetKnockBack(int newknockback)
	{
		knockback = newknockback;
	}
	int GetMagicLevel()const{ return magic; };
	void AddRecoverMP(int add)
	{
		recoverMP += add;
	}
	bool IsGod()const{ return peerless; };
	virtual int GetCollisionRange(){ return 15; };
	void SetLevelUpRate(float newrate)
	{
		lvuprate = newrate;
	}
	float GetLevelUpRate()const{ return lvuprate; };
	//void Setdt(float dt1){ dt = dt1; };
	//float Getdt()const{return dt;};
private:
	Hero() :GameObject(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SPEED, 100, 10, 10, 5, true, 1, true)
	{
		dx = 0;
		dy = 0;
		LV = 0;
		MAX_EXP = 100;
		EXP = 0;
		MAX_DEF = 10;
		MAX_MP = 10;
		SowrdCharge = 0.0;
		MAX_SC = 500;		
		state = STAND;
		speedup = false;
		TargetX = GetX(); 
		TargetY = GetY();
		locked = false;
		Attack = 10;
		lives = 1;
		Isreborned = false;
		adddmg = 0;
		score = 0;
		peerless = false;
		peerless_time = 0;
		ShootCD = 15;
		battlestate = 0;
		bulletnum = 1;
		RageCountDown = 250;
		countaddMp = 100;
		bosskill = 0;
		kills = 0;
		modescore = 0;
		maxcombo = 0;
		itemget = 0;
		combo = 0;
		expadded = 0;
		bstats = no;
		bulletspeed = 600;
		extraDMG = 0;
		m_bHasInitialized = true;
		recoverMP = 0;
		lvuprate = 1;
		atk = 0;
		magic = 0;
		dex = 0;
		knockback = 0;
	};
	Hero(const Hero &);
	Hero & operator=(const Hero &);
	void AssureInitialized()
	{
		while (!m_bHasInitialized)
			Sleep(0);
	};
	//static Hero Hero1;
	//SKILL *SkillToken;
	int expadded;
	int itemget;
	int modescore;
	int maxcombo;
	int combo;
	int kills;
	int bosskill;
	bool speedup;
	int MAX_DEF;
	int MAX_MP;
	int EXP;
	int MAX_EXP;
	double SowrdCharge;
	int MAX_SC;
	float TargetX, TargetY;
	float TargetX2, TargetY2;
	bool    m_bHasInitialized;
	int state;
	bool locked;
	int Attack;
	int lives;
	bool Isreborned;
	int adddmg;
	int score;
	bool peerless;
	int peerless_time;
	int ShootCD;
	int battlestate;
	int bulletnum;
	int RageCountDown;
	int LV;
	int countaddMp;
	int bulletspeed;
	int extraDMG;
	BulletStats bstats;
	int recoverMP;
	float lvuprate;
	float dx, dy;
	int atk, dex, magic;
	int knockback;
	void GodMode()
	{
		static bool flag = false;
		if (peerless)
		{
			if (peerless_time % 5 == 0)
			{
				flag = !flag;
				SetNeedChangeColor(flag);
			}
			peerless_time--;
		}
		else
			peerless_time = 0;
		if (peerless_time == 0)
		{
			peerless = false;
			SetNeedChangeColor(false);
		}
	}
	void RageCount()
	{
		if (battlestate == RAGE ||  battlestate==WEAKEN)
		if (ReturnValid(RageCountDown, 250, 250))
		{
			battlestate = 0;
		}
	}
};
//static Hero Hero::Hero1 = Hero::GetHero();



#endif
//Daisy Exclamation Give To Have The For I'm Of Your Crazy
//Daisy Excalmation CommA Answer All Me Love For You
//av517033