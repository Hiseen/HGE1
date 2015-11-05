#pragma once
#include "Enemy.h"
#include "ObjectPool.h"
class BlackHole :
	public GameObject
{
public:
	BlackHole(float x,float y,int life1,int type,int direction=0);
	virtual void action();
	virtual void born();
	virtual void death();
	virtual int TellType(){ return ANIM; };
	virtual int GetAnimID()
	{ 
		return mytype;
	};
	virtual bool IsEnemy(){ return true; };
	~BlackHole();
private:
	int life;
	int ready;
	int spawndelay;
	int spawntype;
	int count;
	int direction;
	int mytype;
	bool eternal;
	bool canletheroin;
	bool spawnmine;
};

