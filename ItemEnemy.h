#pragma once
#include "Item.h"
#include "SFXManager.h"
class ItemEnemy :
	public GameObject
{
public:
	ItemEnemy(float x,float y);
	virtual void action();
	virtual void born();
	virtual void death();
	void SetType(int newtype)
	{
		type = newtype;
	}
	void SetEnemy(){ isenemy = true; };
	virtual int TellType(){ return ITEMENEMY1; };
	virtual int GetCollisionRange(){ return 0; };
	virtual bool IsEnemy(){ return isenemy; };
	virtual bool NeedRenderFont(){ return false; };
	virtual bool NeedToDestroyWhenBossArrive(){ return false; };
	~ItemEnemy();
private:
	bool effective;
	int type;
	bool isenemy;
	//分三种：
	//1.给血药
	//0.get技能
	//以下两种会按照时间变化 2->3->2..（以后再考虑）
	//2.get加（红）
	//3.get减（蓝）
	//可以考虑只有两种显示模式（1，2，3一种，0一种）
};

