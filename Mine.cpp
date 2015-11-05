#include "Mine.h"




Mine::~Mine()
{
}


void Mine::action()
{
	/*
	if (!Hero::GetHero().IsLocked() && ballcrash(GetX(), GetY(), GetCollisionRange(), Hero::GetHero().GetX(), Hero::GetHero().GetY()))
		Suicide();
	*/
	if (GetX() > ACT_END_X || GetY() > ACT_END_Y || GetY() < ACT_START_Y || GetX() < ACT_START_X)
	{
		SetDelayrender(true);
	}
	else
		SetDelayrender(false);
	if (GetX() > ACT_END_X + 128 || GetY() > ACT_END_Y + 128 || GetY() < ACT_START_Y - 128 || GetX() < ACT_START_X - 128)
		Suicide(true);

}