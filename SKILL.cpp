#include "SKILL.h"


SKILL::SKILL(int ATK1, int CD1, int LAST_TIME1, int cost1, int range1, bool Isenemy) : GameObject(0, 0, 0, 1, 0, 0, false)
{
	SetDMG(ATK1);
	CD = CD1;
	LAST_TIME = LAST_TIME1;
	cost = cost1;
	reATK = GetDMG();
	recost = cost;
	reCD = CD;
	range = range1;
	Box = NULL;
	pointnum = 0;
	host = nullptr;
	IsEnemy = Isenemy;
	IsPenetrate = false;
}


SKILL::~SKILL()
{
	if (Box)
	{
		delete[]Box;
		Box = NULL;
	}
}





int SKILL::GetCD()const
{
	return CD;
}

int SKILL::GetLAST_TIME()const
{
	return LAST_TIME;
}

int SKILL::GetCost()const
{
	return cost;
}



void SKILL::SetCD(int nowCD)
{
	CD = nowCD;
}
void SKILL::Setcost(int nowcost)
{
	cost = nowcost;
}

void SKILL::RESET(bool bCD, bool bATK, bool bcost)
{
	if (bCD)
		SetCD(reCD);
	if (bATK)
		SetDMG(reATK);
	if (bcost)
		Setcost(recost);
}



Point* SKILL::GetBoundingBox()
{
	return Box;
}