#pragma once
#include "SceneManager.h"
#include "SFXManager.h"
enum ButtomState
{
	MouseOff,MouseOver,MouseDown
};

class Button :
	public GameObject
{
public:
	Button(float x, float y, SceneNumber scenenumber,int buttonnum1,float buttonwidth,float buttonheight);
	~Button();
	virtual void action();
	virtual void born()
	{
		SetSize(1.0);
		SetAlive(true);
		finishb = true;
	};
	virtual void death()
	{
		Contract();
	};
	virtual void Underattack(){};
	virtual bool CanChangeGlobalPos(){ return true; };
	virtual bool NeedChangeBlendMode(){ return true; };
	virtual int TellType()
	{
		switch (state)
		{
		case MouseOff:
			return buttonnum;
		case MouseOver:
			return buttonnum + 1;
		case MouseDown:
			return buttonnum + 2;
		}
	};
	virtual bool IsSkill(){	return false; };
	virtual bool IsBoss(){ return false; };
	virtual bool IsItem(){ return false; };
	virtual bool AfterBlur()
	{
		     return true; 
	};
private:
	ButtomState state;
	int buttonnum;
	float width, height;
	int count;
	bool buttondown;
	bool colorreverse;
	bool finishchangecolor;
	bool finishmove;
	int countdelay;
	int countalpha;
	bool playsound;
	bool flag;
};

