#include "Hero.h"
#include "ObjectPool.h"


Hero::~Hero()
{
}

void Hero::action()
{
	GodMode();
	RageCount();
	RestrictPos2();
	ErrorState();
	HGE *hge = hgeCreate(HGE_VERSION);
	GameObject *temp = GameObject::MakePosObject(TargetX, TargetY);
	/*
	if (state == DASH)
	{
		Dash();
		GoToAnotherObject(temp,hge->Timer_GetDelta());
		if (IsCoincide(temp))
		{
			state = STAND;
			locked = false;
		}
	}
	*/
	if (!locked)
	{
		if (state == WALK)
		if (SowrdCharge < GetMaxCharge())
			SowrdCharge += 1;
		else
			SowrdCharge = GetMaxCharge();
		state = STAND;
	}
	    if (recoverMP && GetMP() < MAX_MP)
		{
			if (ReturnValid(countaddMp, recoverMP, recoverMP))
			{
				varyMP(1);
			}	
	    }
	/*
	if (shakescreen)
	{
		shakestage++;
		switch (shakestage)
		{
		case 1:
			hge->Gfx_SetTransform(0, 0, 100, 0, 0, 1, 1);
			break;
		case 2:
			hge->Gfx_SetTransform(0, 0, 100, 100, 0, 1, 1);
			break;
		case 3:
			hge->Gfx_SetTransform(0, 0, 0, 100, 0, 1, 1);
			break;
		case 4:
			hge->Gfx_SetTransform(0, 0, 0, 0, 0, 1, 1);
			shakescreen = false;
			shakestage = 0;
			break;
		}
	}
	*/

	SetPos(GetX() + dx, GetY() + dy);
	dx *= 0.8;
	dy *= 0.8;
	SetImageAngle(GetRot());
	AdjustHP();
	if (GetMP() > MAX_MP)SetMP(MAX_MP);
	delete temp;
	temp = NULL;
}

void Hero::SetTarget(float x, float y,bool isDash)
{
	
	if (isDash)
	{
		//state = DASH;
		TargetX2 = x;
		TargetY2 = y;
	}
	else
	{
		TargetX = x;
		TargetY = y;
	}
}

void Hero::Dash()
{
	HGE *hge = hgeCreate(HGE_VERSION);
	GameObject *temp = GameObject::MakePosObject(TargetX2, TargetY2);
	if (locked == false)
	{

		SowrdCharge -= GetDistance(temp);
		if ((int)SowrdCharge > 0) 
		{
			locked = true;
			adddmg = GetDistance(temp) / 20;
			TargetX = TargetX2;
			TargetY = TargetY2;
		}
		else
		{
			SowrdCharge += GetDistance(temp);
			return;
		}
	}
	if (locked)
	{	
		SetSpeed(SPEED + (GetDistance(temp) * 8));
	}
}

bool Hero::LevelUp()
{
	if (EXP >= MAX_EXP)
	{
		if (LV < MAX_LEVEL)
		{
			EXP = 0;
			LV++;
			if (LV % 4 == 0)
			{
				ObjectPool::GetObjPool().UnLockToken();
			}
			MAX_EXP += 20 * LV;
			SetDMG(GetDMG() + 5);
			SFXManager::GetSFXManager().PlaySFX(LVUPSFX);
			return true;
		}
		else
			EXP = MAX_EXP-1;
	}
	return false;
}


void Hero::ResetHero()
{
	SetPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	LV = 0;
	MAX_EXP = 20;
	EXP = 0;
	MAX_DEF = 10;
	MAX_MP = 10;
	SowrdCharge = 0.0;
	MAX_SC = 500;
	state = STAND;
	speedup = false;
	TargetX = GetX();
	TargetY = GetY();
	locked = false;
	Attack = 10;
	lives = 1;
	Isreborned = false;
	adddmg = 0;
	score = 0;
	peerless = false;
	peerless_time = 0;
	ShootCD = 15;
	battlestate = 0;
	bulletnum = 1;
	RageCountDown = 250;
	countaddMp = 200;
	bosskill = 0;
	kills = 0;
	modescore = 0;
	maxcombo = 0;
	itemget = 0;
	combo = 0;
	expadded = 0;
	bstats = no;
	varyHP(100);
	SetMaxHP(100);
	SetDEF(0);
	SetSpeed(SPEED);
	SetDMG(5);
	SetAlive(true);
	BeTheGod(250);
    bulletspeed = 800;
	SetExtraDMG(0);
	SetDelayrender(false);
	recoverMP = 175;
	SetMP(10);
	SetCrit(1);
	SetCritDMG(2);
	ResetBreakArmor();
	SetDodge(1);
	lvuprate = 1;
	magic = 0;
	atk = 0;
	dex = 0;
	knockback = 0;
	ObjectPool::GetObjPool().UnLockToken();
	ObjectPool::GetObjPool().GetSkillToken(LENSBULLET);
	SFXManager::GetSFXManager().PlaySFX(GETSKILLSFX);
}


bool Hero::AddStats(int type)
{
	switch (type)
	{
	case ADD_ATK:
		atk++;
		if (atk > 15)
		{
			atk = 15;
			return false;
		}
		if (atk % 5 == 0)
		{
			SetKnockBack(GetKnockBack() + 5);
			SetExtraDMG(GetExtraDMG() + GetDMG()*0.2);
			BulletsAdjust(GetBullets() + 1);
		}
		SetBulletSpeed(GetBulletSpeed() - atk * 3);
		SetDMG(GetDMG() + atk * 3);
		SetMaxHP(GetMaxHP() + atk * 10);
		break;
	case ADD_DEX:
		dex++;
		if (dex > 15)
		{
			dex = 15;
			return false;
		}
		if (dex % 5 == 0)
		{
			SetCritDMG(GetCritDMG() + 0.5);
			ShootCD -= 3;
		}
		SetBulletSpeed(GetBulletSpeed() + dex * 3);
		SetCrit(GetCrit() + 3+dex/5);
		SetDodge(GetDodge() + 2+dex/5);
		break;
	case ADD_INT:
		magic++;
		if (magic > 15)
		{
			magic = 15;
			return false;
		}
		if (magic % 5 == 0)
		{
			SetSpeed(GetSpeed() + 100);
			recoverMP = 175 - 10 * magic;
		}
		SetMaxMP(GetMaxMP() + 2 * magic);
		break;
	}
	return true;
}