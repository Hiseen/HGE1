#pragma once
#include "FontPool.h"
#include "RenderPool.h"
#include "hge.h"
#include "SKILL.h"
class SuperBomb :
	public SKILL
{
public:
	SuperBomb() :SKILL(90, 3, 0, 0,1,false,true)
	{
		TX = 0;
		TY = 0;
		boomed = false;
		boom_time = 100;
	};
	~SuperBomb();
	virtual void action();
	virtual void born(){ Swell(); };
	virtual void effect(GameObject *obj);
	virtual void Start();
	virtual int TellType(){ return ANIM; };
	virtual int GetAnimID(){ return SUPERBOMB; };
private:
	float TX, TY;
	bool boomed;
	int boom_time;
};

