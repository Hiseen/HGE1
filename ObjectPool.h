#pragma once

#include<vector>
#include "SKILLToken.h"
#include "main.h"
#include "Shield.h"
#include "SceneManager.h"
class Item;
class SKILLToken;
#ifndef OBJECTPOOL_H_
#define OBJECTPOOL_H_
#define COUNT_HEROSKILL 7
#define MAZE_MAX    500
#define DIRECTION_UP 1
#define DIRECTION_DOWN 2
#define DIRECTION_LEFT 3
#define DIRECTION_RIGHT 4
#define MAZEPARTSIZE 128
extern void getRange(int *arr, int size, int *max, int *min);
extern bool Collision(Point *mPoint,const int count, const float angle5, float center_x, float center_y, float px, float py);
extern bool ballcrash(float center_x, float center_y, int r, float px, float py);
/*struct Node
{
	void *data = NULL;
	Node *next = NULL;
};
*/


enum Direction
{
	None,Left, Right, Up, Down
};

using namespace std;
class ObjectPool
{
public:
	friend class RenderPool;
	~ObjectPool();
	bool InsertObject(GameObject *vir,bool delay=false);
	void DelectObject();
	void UpdateObject();
	void CreatePool();
	void SkillEffect();
	void FlashObject();
	void CreateMaze();
	bool ChangeAllPos(Direction d, int delta);
	void InitTokens();
	void GetSkillToken(int skill=0);
	void UnLockToken();
	void LockedAllToken();
	void UseSkillToken();
	void ReviveTokens();
	void NextToken();
	void LastToken();
	void GenerateEnemy();
	int GetSelectedTokenX();
	int GetSelectedTokenY();
	int TestToken();
	void ExceptClean();
	void SetBWbools(const char* filename, int NumOfObject);
	/*
	bool GetSpawning()
	{
		for (int i = 0; i < pool.size();i++)
		if (pool[i]->TellType == BLACKHOLE)
			return true;
		return false;
	}
	*/
	int GetAmountOfObjects()
	{
		return pool.size();
	}
	float GetGlobalDelta()const{ return global*count; };
	Direction GetNowDirection()const { return direction; };
	SKILLToken *GetTokens(int tokennum);
	bool IsStageCleared()
	{
		for (int i = 0; i < pool.size(); i++)
		{
			if (pool[i]->IsEnemy() || pool[i]->IsBoss()|| pool[i]->IsUnit())
				return false;
		}
		if (countfordie == 0)
		{
			for (int i = 0; i < pool.size(); i++)
			if (pool[i]->IsMine())
			{
				pool[i]->Suicide(true);
				countfordie = 2;
				return false;
			}
			return true;
		}
		else
		{
			countfordie--;
			return false;
		}
	}
	void BreakMaze(int x, int y)
	{
		map[x][y] = 1;
	}
	bool Maze_IsWall(int x, int y)
	{
		if (!map[x][y])
			return true;
		else
			return false;
	}
	bool IsBossDied()
	{
		for (int i = 0; i < pool.size(); i++)
		if (pool[i]->IsBoss())
			return false;
		return true;
	};
	void KillAllEnemies()
	{
		for (int i = 0; i < pool.size();i++)
		if (!pool[i]->IsDead() && pool[i]->IsEnemy())
			pool[i]->Suicide(true);
	}


	void SuicideOneType(int Type);
	bool IsObjectAvild(bool targeted=true,bool cangetboss=false)
	{
		if (targeted)
		{
			if (cangetboss)
			for (int i = 0; i < pool.size(); i++)
			{
				if (!pool[i]->IsSkill() && !pool[i]->IsShield() && pool[i]->IsReachable() && !pool[i]->IsDead() && !pool[i]->IsTargeted() && !pool[i]->IsMine())
					return true;
			}
			else
			for (int i = 0; i < pool.size(); i++)
			{
				if (!pool[i]->IsBoss() && !pool[i]->IsSkill() && pool[i]->IsReachable() && !pool[i]->IsShield() && !pool[i]->IsDead() && !pool[i]->IsTargeted() && !pool[i]->IsMine())
					return true;
			}
		}
		else
		{
			if (cangetboss)
			for (int i = 0; i < pool.size(); i++)
			{
				if (!pool[i]->IsSkill() && !pool[i]->IsShield() && pool[i]->IsReachable() && !pool[i]->IsDead() && !pool[i]->IsMine())
					return true;
			}
			else
			for (int i = 0; i < pool.size(); i++)
			{
				if (!pool[i]->IsBoss() && !pool[i]->IsSkill() && pool[i]->IsReachable() && !pool[i]->IsShield() && !pool[i]->IsDead() && !pool[i]->IsMine())
					return true;
			}
		}
			return false;
	};
	GameObject* GetRandomObject(bool targeted=true, bool cangetboss=false)
	{
		HGE *hge = hgeCreate(HGE_VERSION);
		int i = hge->Random_Int(0, pool.size()-1);
		if (targeted)
		{
			if (cangetboss)
			while (pool[i]->IsItem() || pool[i]->IsSkill() || !pool[i]->IsReachable() || pool[i]->IsDead() || pool[i]->IsShield() || pool[i]->IsTargeted() || pool[i]->IsMine())
				i = hge->Random_Int(0, pool.size() - 1);
			else
			while (pool[i]->IsBoss() || pool[i]->IsItem() || pool[i]->IsSkill() || !pool[i]->IsReachable() || !pool[i]->IsDead() || pool[i]->IsShield() || pool[i]->IsTargeted() || pool[i]->IsMine())
				i = hge->Random_Int(0, pool.size() - 1);
		}
		else
		{
			if (cangetboss)
			while (pool[i]->IsItem() || pool[i]->IsSkill() || !pool[i]->IsReachable() || pool[i]->IsDead() || pool[i]->IsShield() || pool[i]->IsMine())
				i = hge->Random_Int(0, pool.size() - 1);
			else
			while (pool[i]->IsBoss() || pool[i]->IsItem() || pool[i]->IsSkill() || !pool[i]->IsReachable() || pool[i]->IsDead() || pool[i]->IsShield() || pool[i]->IsMine())
				i = hge->Random_Int(0, pool.size() - 1);
		}
		return pool[i];


	};
	void SetMazeSave(bool Maze)
	{ 
	
		MazeSave = Maze; 
	};
	void ResetMazePos(int x, int y)
	{
		startx = x, starty = y;
		movex = 0;
		movey = 0;
	}
	bool IsMazeSave()const{ return MazeSave; };
	void SaveMazePart(int x, int y, int eventname);
	void SetNeedCleanPadPool(bool need)
	{
		NeedCleanPadPool = need;
	}
	bool GetNeedCleanPadPool()const{ return NeedCleanPadPool; };
	vector<GameObject*> GetPool()
	{
		return pool;
	}
	GameObject* GetCurrentBoss()
	{ for (int i = 0; i < pool.size();i++ )
	if (pool[i]->IsBoss())return pool[i];
	};
	bool BossArrive();
	void ResetTokens();
	void MazeEvent(int &x, int &y, int &eventname);
	int CountSkill()
	{
		int c = 0;
		for (int i = 0; i < pool.size();i++)
		if (pool[i]->IsSkill())
			c++;
		return c;
	}
	static ObjectPool& GetObjPool()
	{
		static ObjectPool ObjPool;
		ObjPool.AssureInitialized();
		return ObjPool;
	}
	void AssureInitialized()
	{
		while (!m_bHasInitialized)
			Sleep(0);
	};
	void ResetAll()
	{
		for (int i = 0; i < instantpool.size(); i++)
			InsertObject(instantpool[i]);
		//instantpool.clear();
		for (int i = 0; i < pool.size(); i++)
			pool[i]->avlid = false;
		ObjectPool::DelectObject();
		//pool.clear();
	};
	bool MazeMove();
	void SetEventDirection(int newdirection)
	{
		EventDirection = newdirection;
	}
	int GetEventDirection()const{ return EventDirection; };
	void ReverseDx(){ dx = -dx; };
	void ReverseDy(){ dy = -dy; };
	void ResetDxDy()
	{
		dx = 0;
		dy = 0;
	}
	void AddDx(float add)
	{
		if (!collisionwall && abs(dx)<10)
		dx = add;
	}
	void AddDy(float add)
	{
		if (!collisionwall && abs(dy)<10)
	    dy = add;
	}
	void Exit()
	{
		for (int i = 0; i < pool.size(); i++)
			pool[i]->avlid = false;
		ObjectPool::DelectObject();
		for (int i = 0; i < BWpics.size(); i++)
		{
			for (int j = 0; j < BWpics[i].width; j++)
				free(BWpics[i].bools[j]);
			free(BWpics[i].bools);
		}
		for (int i = 0; i < heroskills.size(); i++)
		{
			delete heroskills[i];
			heroskills[i] = NULL;
		}
		heroskills.clear();
		instantpool.clear();
		pool.clear();
		BWpics.clear();
	}

private:
	ObjectPool();
	ObjectPool(const ObjectPool &);
	ObjectPool & operator=(const ObjectPool &);
	vector<GameObject*> pool; 
	vector<GameObject*> instantpool;
	vector<SKILLToken*> heroskills;
	vector<BWpic> BWpics;
	int focusskill;
	int testcount;
	int countfordie;
	int EventDirection;
	float global;
	bool NeedCleanPadPool;
	bool MazeSave;
	int countskilleffect;
	Direction direction;
	bool reverse;
	int count;
	int map[502][502];
	bool m_bHasInitialized;
	int startx, starty;
	int dx, dy;
	int movex, movey;
	float movexx, moveyy;
	bool collisionwall;
	bool NoNeedToSave;
	//static ObjectPool *ObjPool;
	int getw(int x, int y);
	int search(int x, int y);
	void Make_Maze(int x, int y);
	bool MazeCollision(int x, int y,int &eventname,int &direction);
	bool IsDead(GameObject* vir)
	{
		if (!vir->avlid)
			return true;
		else
			return false;
	}
	void ObjectRun(GameObject* vir)
	{
		if (!vir->IsDead())
		{
			if (!vir->finishb)vir->born();
			else
				vir->action();
		}
		else
			vir->death();
	}
	int GetSkill(GameObject *vir)
	{
		return vir->IsSkill();
	}

	BWpic GetBWpic(int number)
	{
		for (int i = 0; i < BWpics.size();i++)
		if (BWpics[i].number == number)return BWpics[i];
		BWpic pic1;
		pic1.bools = nullptr;
		return pic1;
	}

	void ShieldEffect(Shield *shield, SKILL *skill)
	{
		int typenum = shield->TellType();
		if (!skill->IsThisSkillEnemy())
		{
			if (shield->GetHostOfShield() && shield->GetHostOfShield()->TellType() != HERO)
			{
				if (shield->IsReachable() && CollisionBW(GetBWpic(typenum), skill->GetX(), skill->GetY(), shield->GetRot(), shield->GetX(), shield->GetY(),shield->GetSize()))
					skill->interaction(shield);
			}
		}
		else
		{
			if (shield->GetHostOfShield() && shield->GetHostOfShield()->TellType() == HERO)
			{
				if (shield->IsReachable() && CollisionBW(GetBWpic(typenum), skill->GetX(), skill->GetY(), shield->GetRot(), shield->GetX(), shield->GetY(),shield->GetSize()))
					skill->interaction(shield);
			}
		}
	}

	void UseSkillForHero(Hero *hero,SKILL *skill)
	{
		if (skill->GetRange() == 0)
		{
			{

				if (hero->IsVulnerable(skill, false) && 
				(ballcrash(skill->GetX(), skill->GetY(), hero->GetCollisionRange(), hero->GetX(), hero->GetY())
				||Collision(skill->GetBoundingBox(),skill->GetPointNum(), skill->GetRot(), skill->GetX(), skill->GetY(), hero->GetX(), hero->GetY()) 
		        ))
				{
					skill->effect(hero);
					hero->Underattack(skill);
				}
			}
		}
		else
		{
			if (hero->IsVulnerable(skill, false) &&
				ballcrash(skill->GetX(), skill->GetY(), skill->GetRange(), hero->GetX(), hero->GetY()))
				{
				skill->effect(hero);
				hero->Underattack(skill);
				}
		}
	}


	void UseSkill(GameObject *vir, SKILL *skill)
	{
			if (skill->GetRange() == 0)
			{
					if (vir->IsReachable() && Collision(skill->GetBoundingBox(),
						skill->GetPointNum()//如果要做多边形碰撞盒的话要在每个技能中获取点的个数
						, skill->GetRot(), skill->GetX(), skill->GetY(), vir->GetX(), vir->GetY()))
					{
						//如果有盾则不会触发effect效果：
						// int a = skill->GetAnimID();
						if (skill->interaction(vir) && !skill->IsDead())
						{
							skill->effect(vir);
							vir->Attacked();
							vir->Underattack(skill->GetHost());
						}
					}
			}
			else
			{
					if (vir->IsReachable())
					if (skill->GetRange() > vir->GetCollisionRange())
					{

						if (ballcrash(skill->GetX(), skill->GetY(), skill->GetRange(), vir->GetX(), vir->GetY()) && skill->interaction(vir) && !skill->IsDead())
						{
							skill->effect(vir);
							vir->Attacked();
							vir->Underattack(skill->GetHost());
						}
					}
					else
					{
						if (ballcrash(vir->GetX(), vir->GetY(), vir->GetCollisionRange(), skill->GetX(), skill->GetY()) && skill->interaction(vir) && !skill->IsDead())
						{
							skill->effect(vir);
							vir->Attacked();
							vir->Underattack(skill->GetHost());
						}
					}
			}
		}
};

#endif