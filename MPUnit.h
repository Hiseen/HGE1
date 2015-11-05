#pragma once
#include "Hero.h"
class MPUnit :
	public GameObject
{
public:
	MPUnit(float x,float y);
	virtual void action();
	virtual void born();
	virtual void death();
	virtual void Underattack(GameObject *obj){};
	virtual int TellType(){ return MPUNIT; };
	virtual int GetCollisionRange(){ return 0; };
	virtual bool IsEnemy(){ return false; };
	virtual bool IsUnit(){ return true; }
	~MPUnit();
private:
	bool fangxiang;
	int speedup;
};

