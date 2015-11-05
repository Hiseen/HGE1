#pragma once
#include "SKILL.h"
class Impact :
	public SKILL
{
public:
	Impact() :SKILL(100, 3, 0, 1){};
	virtual void effect(GameObject *vir){};
	virtual void Start(){};
	virtual int GetAnimID(){ return IMPACT; };
	~Impact();
};

