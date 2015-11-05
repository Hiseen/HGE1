#include "EXPUnit.h"
#include "RenderPool.h"

EXPUnit::EXPUnit(float x, float y) :GameObject(x,y,0,1,0,0,0,false)
{
	HGE *hge = hgeCreate(HGE_VERSION);
	pos = MakePosObject(x + hge->Random_Int(-100, 100), y + hge->Random_Int(-100, 100));
	lifespan = 100;
	flag = false;
	speedup = 0;
	RenderPool::GetRenderPool().SpawnPS(this, EXPUNIT);
}


EXPUnit::~EXPUnit()
{
}


void EXPUnit::action()
{
	HGE *hge = hgeCreate(HGE_VERSION);
	
	if (pos)
	{
		if ((abs(GetX() - pos->GetX())) > 10 || (abs(GetY() - pos->GetY()) > 10))
		{
			SetSpeed(GetDistance(pos)*4);
			GoToAnotherObject(pos);
		}
		else
		{
			SetSpeed(0);
			pos = NULL;
		}
	}
	else
	{
		if (GetDistance(&Hero::GetHero()) < 100)
			flag = true;
		else
		lifespan--;
	}
	if (lifespan <= 0)
		Suicide(true);
	if (flag)
	{
		SetSpeed(500-GetDistance(&Hero::GetHero())*0.5);
		GoToAnotherObject(&Hero::GetHero());
		if ((abs(GetX() - Hero::GetHero().GetX())) <= 10 && (abs(GetY() - Hero::GetHero().GetY()) <= 10))
			Suicide();
	}
	
	//speedup+=hge->Random_Int(1,10);
	//if (!IsCoincide(&Hero::GetHero()))
//	{
//		SetSpeed(speedup + GetDistance(&Hero::GetHero()));
//		GoWithAnArc(&Hero::GetHero(), flag);
//	}
//	else
//		Suicide();


}


void EXPUnit::death()
{
	if (!IsForcedToDie() && !IsPlayedDeadAnim())
	{
		SAFE_DELETE(pos);
		SFXManager::GetSFXManager().PlaySFX(GETEXPSFX, true);
		Hero::GetHero().varyHP(1);
		SetPlayedDeadAnim();
	}
	Contract(true);
}

void EXPUnit::born()
{
	Swell2();
}