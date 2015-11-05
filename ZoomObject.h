#pragma once
#include "GameObject.h"
#include "SceneManager.h"
class ZoomObject :
	public GameObject
{
public:
	ZoomObject(int num) :GameObject(0, 0, 0, 1, 0, 0, 0, true, 0, 0, none, MenuScene)
	{
		this->num = num;
		switch (num)
		{
		case TITLE:
			alpha = 1;
			SetPos(424.5, 106.0);
			SetSize(7);
			SetChangeColor(0, 255, 255, 255);
			SetNeedChangeColor();
			SetZ(0.7);
			break;
		default:
			break;
		}
	
	};
	~ZoomObject();
	virtual bool AfterBlur(){ return true; };
	virtual int TellType(){ return num; };
	virtual void action();
	virtual bool CanChangeGlobalPos(){ return true; };
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
private:
	int num;
	double alpha;
};

/*	
if (titlealpha < 255)
			{
				titlealpha *= 1.16;
				titlesize *= 0.96;
			}
			else
			{
				titlesize = 1;
				titlealpha = 255;
				}
*/