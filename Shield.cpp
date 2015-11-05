#include "Shield.h"





Shield::~Shield()
{
}

void Shield::action()
{
	if (host->IsDead())
	{
		host = NULL;
		Suicide();
	}
	if (host)
	{
		HGE *hge = hgeCreate(HGE_VERSION);
		BaseAction();
		SetRot(GetRot() + 0.005);
		SetPos(host->GetX(), host->GetY());
		SetSize(host->GetSize());
		if (!infinate)
			last_times--;
		if (last_times <= 0)
		{
			Suicide();
		}
	}
	else
		Suicide();
}



void Shield::Underattack(GameObject *obj)
{
	
}
