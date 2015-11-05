#include "SuperBomb.h"





SuperBomb::~SuperBomb()
{
}


void SuperBomb::Start()
{
	HGE *hge = hgeCreate(HGE_VERSION);
	float x, y;
	hge->Input_GetMousePos(&x, &y);
	SetPos(Hero::GetHero().GetX(), Hero::GetHero().GetY());
	GameObject *a = GameObject::MakePosObject(x, y);
	SetRot(atan2f(GetY() - a->GetY(), GetX() - a->GetX()));
	TX = x;
	TY = y;
	boomed = false;
	boom_time = 50;
	SetSpeed(200);
	ResetNeedStop();
	SetRange(1);
}

void SuperBomb::action()
{
	SetImageAngle(GetRot());
	if (GetSpeed() != 0)
	{
		HGE *hge = hgeCreate(HGE_VERSION);
		GameObject *obj = MakePosObject(TX, TY);
		if (!IsCoincide(obj))
		{
			GoToAnotherObject(obj, hge->Timer_GetDelta());
		}
		else
		{
			SetRange(200);
			NeedStop();
			if (!boomed)
			{
				RenderPool::GetRenderPool().InstantRender(GetX(), GetY(), SUPERBOMB);
				boomed = true;
			}
			if (boom_time != 0)
			{
				boom_time--;
				UnLock();
			}
			else
			{
				Lock();
				Suicide();
			}
		}
	}
}

void SuperBomb::effect(GameObject *obj)
{
	HGE *hge = hgeCreate(HGE_VERSION);
	if (!obj->IsAttacked())
	{
		int damage = -GetATK();
		obj->DmgDispose(damage);
		obj->varyHP(damage);
		FontPool::GetFontPool().InsertFont(obj->GetX(), obj->GetY(),damage);
	}
	
}