#pragma once
#include "GameObject.h"
#ifndef BAR_H_
#define BAR_H_
class Bar: public GameObject
{
public:
	Bar(int barnumber1) :GameObject(0, 0, 0, 1, 0, 0,0, false)
	{
		maxbar = 0;
		numnow = 0;
		barnumber = barnumber1;
	};
	virtual void born()
	{
		SetSize(1.0);
		SetAlive(true);
		finishb = true;
	}
	virtual void action()
	{
		SetVarImg(1, numnow / maxbar);
	};
	virtual void Underattack(){};
	virtual int TellType(){ return barnumber; };
	virtual bool IsSkill(){ return false; };
	virtual bool AfterBlur(){ return true; };
	virtual bool NeedToDestroyWhenBossArrive(){ return false; };
	inline void Link(int num, int max)
	{
		if (num <= max)
		{
			if (numnow < num)
				numnow += (num - numnow) / 10;
			if (numnow > num)
				numnow -= (numnow - num) / 10;
			maxbar = max;
		}
		else
		{
			numnow = max;
			maxbar = max;
		}
		if (max == 0)
		{
			numnow = 0;
			maxbar = 100;
		}
	};
	~Bar();

private:
	float numnow;
	float maxbar;
	int barnumber;
};

#endif