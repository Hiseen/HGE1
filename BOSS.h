#pragma once
#include "Hero.h"
#include "Enemy.h"
#include "Mine.h"
#include"Bullet.h"
#include "ObjectPool.h"
#include "hge.h"
#ifndef BOSS_H_
#define BOSS_H_

#define ATTACK 1
#define GUARD 2



class BOSS :
	public GameObject
{
public:
	BOSS(int ID) :GameObject(0,0,0,3500+(Hero::GetHero().GetLevel()*2000/5),0,0,0)
	{
		HGE *hge = hgeCreate(HGE_VERSION);
		int *a = new int[9]{HEXABULLETS, FANBULLETSEX, SPAWNMINIONS, BOUNCEANDBOUNCE, BOUNCEBULLETS, WIFIBULLETS, SHRINKBULLETS, RECTBULLETS,FIVEBULLETS};
		int countskills = Hero::GetHero().GetBossKills() + 2;
		attackskills = new int[countskills]; 
		int end = 9;
		for (int i = 0; i < countskills; i++)
		{
			random(a, end);
			attackskills[i] = a[end-1];
			end--;
		}
		delete[]a;
		a = NULL;
		int *b = new int[3]{SPAWNSHEILD, HEALFIELD,DODGEMAX};
		guardskills = new int[2];
		int end2 = 3;
		for (int i = 0; i < 2; i++)
		{
			random(b, end2);
			guardskills[i] = b[end2-1];
			end2--;
		}
		delete[]b;
		b = NULL;
		SetPos(hge->Random_Int(ACT_START_X, ACT_END_X), hge->Random_Int(ACT_START_Y, ACT_END_Y));
		SetSpeed(100);
		int chance;
		switch(SceneManager::GetSceneManager().GetCurrentMode())
		{
		case WaveMode:
			SetDMG(20 + Hero::GetHero().GetBossKills() * Hero::GetHero().GetBossKills() * 10);
			SetDEF(10 + sqrt(SceneManager::GetSceneManager().GetWave()) + 2 * SceneManager::GetSceneManager().GetWave());
			SetMP(8 + Hero::GetHero().GetBossKills() * 10);
			bosslevel = Hero::GetHero().GetLevel();
			chance = Hero::GetHero().GetBossKills() * Hero::GetHero().GetBossKills() + 5;
			if (chance > 60)
			{
				SetCrit(60);
				SetDodge(60);
			}
			else
			{
				SetCrit(chance);
				SetDodge(chance);
			}
			break;
		case MazeMode:
			SetDMG(20 + Hero::GetHero().GetLevel()*5);
			SetDEF(sqrt(SceneManager::GetSceneManager().GetWave()));
			SetMP(8 + Hero::GetHero().GetLevel() * 10);
			bosslevel = Hero::GetHero().GetLevel();
			chance = Hero::GetHero().GetLevel() + 10;
			SetCrit(chance);
			SetDodge(chance);
			break;
		}
		BossID = ID;
		tx = 0;
		ty = 0;
		recoverMP = 50;
		countmove = 0;
		MAX_HP = GetHP();
		MAX_MP = GetMP();
		HHP = Hero::GetHero().GetHP();
		MHP = GetHP();
		switchmodedelay = 0;
		chancetoshoot = 90;
		summondelay = 200;
		randomdelay = 50;
	};
	virtual void born(){ Swell(); };
	virtual bool NeedToDestroyWhenBossArrive(){ return false; };
	virtual void action();
	virtual bool varyHP(int dHP);
	virtual void death()
	{ 
		Contract();
		if (this->IsDead() && !this->IsPlayedDeadAnim())
		{
			delete[]attackskills;
			delete[]guardskills;
			HGE *hge = hgeCreate(HGE_VERSION);
			SetPlayedDeadAnim();
			RenderPool::GetRenderPool().InstantRender(GetX(), GetY(), BOSS1, GetRot());
			if (!IsForcedToDie())
			{
				//ObjectPool::GetObjPool().KillAllEnemies();
				Item *item = new Item(GetX(), GetY(), ITEM_RANDOM);
				ObjectPool::GetObjPool().InsertObject(item, true);
				Hero::GetHero().AddScore(Hero::GetHero().GetScore()*0.2,true);
				for (int i = 0; i < hge->Random_Int(20, 40); i++)
				{
					EXPUnit *exp = new EXPUnit(GetX(), GetY());
					ObjectPool::GetObjPool().InsertObject(exp, true);
				}
			}
			SFXManager::GetSFXManager().PlaySFX(GAMEBGM);
			SceneManager::GetSceneManager().WaveClear();
		}
	};
	virtual void Underattack(GameObject *obj);
	void Think();
	void BaseAttack();
	void UseSkills();
	void AttackMode()
	{
		if (Mode != ATTACK)
		{
			Mode = ATTACK;
			SetSpeed(GetSpeed()*0.5);
			switchmodedelay = 200;
		}
	};
	void GuardMode()
	{
		if (Mode != GUARD)
		{
			Mode = GUARD;
			SetSpeed(GetSpeed() * 2);
			switchmodedelay = 200;
		}
	};
	void Attack();
	void Guard();
	virtual int TellType(){ return BossID; };
	virtual bool IsBoss(){ return true; };
	virtual bool IsSkill(){ return false; };
	virtual bool IsItem(){ return false; }
	virtual int GetCollisionRange(){ return 20; };
	~BOSS();

private:
	int BossID;
	int tx, ty;
	int countmove;
	int MAX_HP;
	int HHP, MHP;
	int Mode;
	bool skilling;
	int bosslevel;
	int skilldelay;
	int randomdelay;
	void Summon();
	int switchmodedelay;
	int recoverMP;
	int chancetoshoot;
	int summondelay;
	int MAX_MP;
	int *attackskills;
	int *guardskills;
};

#endif