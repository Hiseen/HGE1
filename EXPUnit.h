#pragma once
#include "Hero.h"
#include "SFXManager.h"
class EXPUnit :
	public GameObject
{
public:
	EXPUnit(float x,float y);
	virtual void action();
	virtual void born();
	virtual void death();
	virtual void Underattack(GameObject *obj){};
	virtual int TellType(){ return EXPUNIT; };
	virtual int GetCollisionRange(){ return 0; };
	virtual bool IsEnemy(){ return false; };
	virtual bool IsUnit(){ return true; }
	~EXPUnit();
private:
	GameObject *pos;
	//int lifespan;
	int speedup;
	bool flag;
	int lifespan;
};

