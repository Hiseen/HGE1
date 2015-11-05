#include "SKILLToken.h"
SKILLToken::~SKILLToken()
{
}



bool SKILLToken::Start()
{
	if (state == LOCKED)
	{
		Skills *newskill = new Skills(&Hero::GetHero(), skilltype);
		if (ObjectPool::GetObjPool().InsertObject(newskill, true))
		{
			//state = ACTIVE;
			//skilltype = 0;
			return true;
		}
		return false;
	}
}


int SKILLToken::GetAnimID()
{
	switch (state)
	{
	case LOCKED:
		return skilltype + 40;
	case BANNED:
		return TOKEN_BANNED;
	case ACTIVE:
		return TOKEN_ACTIVE;
	}
}

