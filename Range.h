#pragma once
#include "GameObject.h"
#ifndef RANGE_H_
#define RANGE_H_
class Range :public GameObject
{
public:
	Range() :GameObject(0, 0, 0, 0, 0, 0,0, false)
	{
		changesize = 0;
	};
	~Range();
	virtual void born(){
		SetSize(0.0);
		SetAlive(true);
		finishb = true;
	};
	virtual void action()
	{
		SetSize(changesize);
	};
	virtual void death(){};
	virtual int TellType(){ return 0; };
	void GetInfo(GameObject linkwho,double mutisize);
private:
	double changesize;
};
#endif