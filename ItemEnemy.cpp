#include "ItemEnemy.h"


ItemEnemy::ItemEnemy(float x, float y) :GameObject(x,y,0,Hero::GetHero().GetDMG()*5,0,0,0)
{
	HGE *hge = hgeCreate(HGE_VERSION);
	type = 0;
	SetSpeed(150);
	SetSoild(true);
	GameObject *a = MakePosObject((ACT_END_X - ACT_START_X) / 2, hge->Random_Int(ACT_START_Y + 10, ACT_END_Y - 10));
	GetAngle(a);
	delete a;
	a = NULL;
	effective = false;
	isenemy = false;
	SetCanHaveErrorState(false);
}


ItemEnemy::~ItemEnemy()
{
}

void ItemEnemy::born()
{
	Swell();
}

void ItemEnemy::action()
{
	SetImageAngle(GetRot());
	if (!effective)
	{
		if (GetX() > ACT_START_X && GetY() > ACT_START_Y && GetX() < ACT_END_X && GetY() < ACT_END_Y)
			effective = true;
	}
	else
	{
		if (GetX() < ACT_START_X || GetY() < ACT_START_Y || GetX() > ACT_END_X || GetY() > ACT_END_Y)
			Suicide(true);
	}
	HGE *hge = hgeCreate(HGE_VERSION);
	GoWithAnAngle(hge->Timer_GetDelta(), GetRot() - 1.57);
	if (ballcrash(GetX(), GetY(), 15, Hero::GetHero().GetX(), Hero::GetHero().GetY()))
	{
		if (hge->Random_Int(0, 1))
		{
			//øÒ±©“Ù–ß£∫
			Hero::GetHero().SetBattleState(RAGE);
			Hero::GetHero().varyHP(Hero::GetHero().GetMaxHP()*0.05);
		}
		else
		{
			//weaken“Ù–ß£∫
			Hero::GetHero().SetBattleState(WEAKEN);
			Hero::GetHero().varyMP(5 + Hero::GetHero().GetBossKills());
			Hero::GetHero().SetExtraDMG(Hero::GetHero().GetExtraDMG() + Hero::GetHero().GetBossKills() * 3);
			Hero::GetHero().SetDEF(Hero::GetHero().GetDEF() + +Hero::GetHero().GetBossKills() * 3);
		}
		Suicide();
	}
}

void ItemEnemy::death()
{
	Contract();
	if (!IsForcedToDie() && !IsPlayedDeadAnim())
	{
		SetPlayedDeadAnim();
	}
}