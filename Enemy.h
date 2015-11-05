#pragma once
#include "FontPool.h"
#include"GameObject.h"
#include "Item.h"
#include "EXPUnit.h"
#include "hge.h"
#include "Bullet.h"
#include "RenderPool.h"
#include "MPUnit.h"
#ifndef ENEMY_H_
#define ENEMY_H_
class Bullet;
class Enemy : public GameObject
{
public:
	Enemy(float x, float y, int enemytype, int innertype1 = -1) :GameObject(x, y, 0, 1, 0, 1)
	{
		HGE *hge = hgeCreate(HGE_VERSION);
		type = enemytype;
		skillcount = 0;
		skillcount2 = 0;
		skillcount3 = 0;
		IsRage = false;
		posobj = NULL;
		flag = false;
		if (SceneManager::GetSceneManager().GetCurrentMode() == WaveMode)
		{
			SetHP(10 + SceneManager::GetSceneManager().GetWave() * SceneManager::GetSceneManager().GetWave() * 2);
			SetDEF(sqrt(SceneManager::GetSceneManager().GetWave()) + SceneManager::GetSceneManager().GetWave());
			SetDMG(10+SceneManager::GetSceneManager().GetWave() * SceneManager::GetSceneManager().GetWave()*SceneManager::GetSceneManager().GetWave()*0.02 + SceneManager::GetSceneManager().GetWave());
			SetCrit(Hero::GetHero().GetBossKills() * 2);
			SetDodge(Hero::GetHero().GetBossKills() * 2);
			minexp = (Hero::GetHero().GetBossKills() + 1) * 2;
		}
		if (SceneManager::GetSceneManager().GetCurrentMode() == MazeEvent)
		{
			SetHP(15 + SceneManager::GetSceneManager().GetWave() * 10);
			SetDEF(sqrt(Hero::GetHero().GetLevel()) + SceneManager::GetSceneManager().GetWave());
			SetDMG(10+SceneManager::GetSceneManager().GetWave());
			SetCrit(SceneManager::GetSceneManager().GetWave() + 2);
			SetDodge(SceneManager::GetSceneManager().GetWave() + 2);
			minexp = (SceneManager::GetSceneManager().GetWave() + 5);
		}
		if (x < ACT_START_X || x > ACT_END_X || y > ACT_END_Y || y < ACT_START_Y)
			SetDashToCenter();
		switch (type)
		{
		case ENEMY:				  
		     collisionrange = 20;
			 SetSpeed(20);
		     score = 1;
		break;
		case SUPERSPEEDBALL:
			collisionrange = 17;
			SetSpeed(80);
			SetDMG(GetDMG(false) / 2+1);
			score = 2;
			minexp += 1;
			break;
		case TRICKBALL:
			collisionrange = 17;
			SetSpeed(40);
			SetHP(GetHP() / 2);
			SetMaxHP(GetHP());
			score = 5;
			minexp += 2;
			break;
		case RINGBULLETBALL:
			collisionrange = 0;
			SetSpeed(50 +hge->Random_Int(100,200));
			SetDMG(GetDMG(false)/2+20);
			SetRot(hge->Random_Float(0, 6.28));
			SetReachable(false);
			score = 0;
			minexp = 0;
			break;
		case STEALMANABALL:
			collisionrange = 15;
			SetSpeed(0);
			score = 3;
			minexp += 2;
		case GUARDBALL:
			SetSize(0.5);
			collisionrange = 0;
			SetSpeed(0);
			SetHP(20*(Hero::GetHero().GetBossKills()+1));
			SetDEF(SceneManager::GetSceneManager().GetWave() * 2);
			SetDMG(0);
			score = 3;
			SetExp(0);
			break;
		}

		if (innertype1 == -1)
		{
			if (hge->Random_Int(0, 100) < 10)
			{
				SetHP(GetHP()*(2+SceneManager::GetSceneManager().GetWave()));
				int inn = 0;
				switch (type)
				{
				case ENEMY:
					inn = hge->Random_Int(RUNE_LANSER, RUNE_BOOMER);
					switch (inn)
					{
					case RUNE_LANSER:
						SetHP(GetHP()*0.8);
						break;
					case RUNE_DRAGOON:
						SetHP(GetHP()*0.1);
						SetSpeed(GetSpeed() * 4);
						break;
					case RUNE_TANK:
						SetDMG(GetDMG()*0.25+1);
						break;
					case RUNE_BOOMER:
						SetHP(GetHP()*0.5);
						SetDMG(GetDMG() * 4);
						break;
					}
					SetMaxHP(GetHP());
					break;
				case SUPERSPEEDBALL:
					inn = hge->Random_Int(RUNE_SHOTGUN, RUNE_ROCKET);
					break;
				}
				SetInnerType(inn);
			}
		}
		if (type==SUPERSPEEDBALL)
			skillcount = hge->Random_Int(80, 120);
		if (type == TRICKBALL)
		{
			int inn = hge->Random_Int(RUNE_HEAL, RUNE_DEFUP);
			SetHP(GetHP() * 5);
			SetInnerType(inn);
			skillcount2 = hge->Random_Int(200, 350);
		}
		MAX_speed = GetSpeed();
	};
	~Enemy();
	//不允许有非纯虚函数了，有的话也访问不到。
	virtual bool BulletBoomEffect()
	{
		if (type == ENEMY)
			return true;
		else
			return false;
	};
	virtual void action();
	virtual void born();
	virtual void death();
	virtual void Underattack(GameObject *obj)
	{
		HGE *hge = hgeCreate(HGE_VERSION);
		switch (type)
		{
		case ENEMY:
			SetSpeed(GetSpeed() + hge->Random_Float(10, 20));
			break;
		case SUPERSPEEDBALL:
			break;
		}

	};
	virtual int TellType(){ return type; };
	virtual int GetCollisionRange(){ return collisionrange * GetSize(); };
	virtual bool IsEnemy(){ return true; };
	virtual bool IsBoss()
	{
			return false;
	};

	/*
	void GoToAnotherObject(GameObject *obj, float dt,bool astar=false)
	{
		if (astar)
		{
			double angle = GetAngle(obj, true);
			double speed = GetSpeed();
			float x = GetX(), y = GetY();
			if (GetX() >= obj->GetX() && GetY() > obj->GetY()){ x -= speed*dt*cos(angle + 1.57); y -= speed*dt*sin(angle + 1.57); }
			if (GetX() < obj->GetX() && GetY() >= obj->GetY()){ x += speed*dt*sin(angle); y -= speed*dt*cos(angle); }
			if (GetX() > obj->GetX() && GetY() <= obj->GetY()){ x -= speed*dt*sin(angle - 3.14); y += speed*dt*cos(angle - 3.14); }
			if (GetX() <= obj->GetX() && GetY() < obj->GetY()){ x += speed*dt*cos(angle - 1.57); y += speed*dt*sin(angle - 1.57); }
			int s1=0, s2=0, s3=0, s4=0;
			for (int i = 0; i < ObjectPool::GetObjPool().GetPool().size(); i++)
			if (ObjectPool::GetObjPool().GetPool()[i]->IsMine() && GetDistance(ObjectPool::GetObjPool().GetPool()[i])<100)
			{
				if (ObjectPool::GetObjPool().GetPool()[i]->GetX()>GetX() && ObjectPool::GetObjPool().GetPool()[i]->GetY()>GetY())
					s2 += GetDistance(ObjectPool::GetObjPool().GetPool()[i]);
				if (ObjectPool::GetObjPool().GetPool()[i]->GetX()<GetX() && ObjectPool::GetObjPool().GetPool()[i]->GetY()>GetY())
					s3 += GetDistance(ObjectPool::GetObjPool().GetPool()[i]);
				if (ObjectPool::GetObjPool().GetPool()[i]->GetX() > GetX() && ObjectPool::GetObjPool().GetPool()[i]->GetY() < GetY())
					s1 += GetDistance(ObjectPool::GetObjPool().GetPool()[i]);
				if (ObjectPool::GetObjPool().GetPool()[i]->GetX() < GetX() && ObjectPool::GetObjPool().GetPool()[i]->GetY() < GetY())
					s4 += GetDistance(ObjectPool::GetObjPool().GetPool()[i]);
			}
			if (s1 != 0 || s2 != 0 || s3 != 0 || s4 != 0)
			{
				int max1 = max(max(s1, s2), max(s3, s4));
				double r = GetDistance(GameObject::MakePosObject(x, y));
				if (max1 == s1)
				{
					if (GetX()<x || GetY()>y)
					{
						angle += 6.28 - 0.785;
					}
				}
				if (max1 == s2)
				{
					if (GetX() < x || GetY()<y)
					{
						angle += 0.785;
					}
				}
				if (max1 == s3)
				{
					if (GetX()>x || GetY()<y)
					{
						angle += 1.57;
					}
				}
				if (max1 == s4)
				{
					if (GetX()>x || GetY() > y)
					{
						angle += 6.28 - 1.57;
					}
				}
				x = GetX() + (cos(angle) - sin(angle))*r;
				y = GetY() + (cos(angle) + sin(angle))*r;
				SetPos(x, y);
			}
			else
				GameObject::GoToAnotherObject(obj, dt);
		}
		else
			GameObject::GoToAnotherObject(obj, dt);
	}
	*/
	void SetExp(int newexp)
	{
		minexp = newexp;
	}
	int GetExp()const{ return minexp; };
private:
	double MAX_speed;
	bool flag;
	int type;
	int collisionrange;
	int skillcount;
	int skillcount2;
	int skillcount3;
	bool IsRage;
	GameObject *posobj;
	int score;
	int minexp;
};

#endif