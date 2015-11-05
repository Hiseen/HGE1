#pragma once
#include "GameObject.h"
class FloatingObject :
	public GameObject
{
public:
	FloatingObject(float x, float y,double size,int num=ENEMY) :GameObject(x, y, 0, 1, 0, 0, 0, true, size, 0, none, MenuScene)
	{
		//HGE *hge = hgeCreate(HGE_VERSION);
		//SetRot(hge->Random_Float(-0.2, 0.2));
		this->num = num;
		SetSpeed(40 * size);
		if (size<0.6 || size>1.5)
		{
			IsAfterBlur = false;
			SetZ(0.8);
		}
		else
		{
			IsAfterBlur = true;
			SetZ(0.7);
		}
	};
	virtual bool IsShield(){ return false; };
	virtual bool IsSkill(){ return false; };
	virtual bool IsBoss(){ return false; };
	virtual bool IsItem(){ return false; }
	virtual bool AfterBlur(){ return IsAfterBlur; };
	virtual int TellType(){ return num; };
	virtual void action();
	virtual void born()
	{
		SetAlive(true);
		finishb = true;
		finishd = false;
	};
	virtual void death()
	{
		Contract(true);
	};
	~FloatingObject();
private:
	bool IsAfterBlur;
	int num;
};

