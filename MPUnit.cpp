#include "MPUnit.h"
#include "RenderPool.h"

MPUnit::MPUnit(float x,float y) :GameObject(x,y,0,1,0,0,0,false)
{
	HGE *hge = hgeCreate(HGE_VERSION);
	fangxiang = hge->Random_Int(0, 1);
	speedup = 0;
	RenderPool::GetRenderPool().SpawnPS(this, MPUNIT);
}


MPUnit::~MPUnit()
{
}

void MPUnit::born()
{
	Swell2();
}

void MPUnit::action()
{
	HGE *hge = hgeCreate(HGE_VERSION);
	speedup += hge->Random_Int(1, 10);
	if (!IsCoincide(&Hero::GetHero()))
	{
		SetSpeed(speedup + GetDistance(&Hero::GetHero()));
		GoWithAnArc(&Hero::GetHero(), fangxiang);
	}
	else
		Suicide();
}

void MPUnit::death()
{
	Contract(true);
	Hero::GetHero().varyMP(1);
}