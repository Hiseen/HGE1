#include "Bullet.h"




Bullet::~Bullet()
{
}


void Bullet::action()
{
	SetImageAngle(GetRot());
	if (IsDelayrender())
		count--;
	if (count == 0)
		SetDelayrender(false);
	if (stoptime>0)
		stoptime--;
	else
	{
		if (GetX()<0 || GetY()<0 || GetX()>SCREEN_WIDTH || GetY()>SCREEN_HEIGHT)
		{
			if (bounce > 0)
			{
				bounce--;
				lifespan = 0;
				if (GetY() > 0)
					SetRot(-GetRot());
				else
					SetRot(3.14 - GetRot());
				if (GetY() > SCREEN_HEIGHT)
					SetRot(3.14 + GetRot());
			}
			else
			if (!(bulletstats & Spin))
				Suicide();
		}

		HGE *hge = hgeCreate(HGE_VERSION);
		switch (bulletnum)
		{
		case BULLET1:
			if (bulletstats & DtoDMG)
			{
				SetDMG(GetDMG(false) + 1);
				SetSize(basesize + (float)(GetDMG(false) - Hero::GetHero().GetDMG(false)) / Hero::GetHero().GetDMG(false));
			}
			if (bulletstats & DtoDMGdown)
			{
				SetDMG(GetDMG(false) - (1 + GetDMG(false) / 50));
				SetSize(basesize + (float)(GetDMG(false) - Hero::GetHero().GetDMG(false)) / Hero::GetHero().GetDMG(false));
			}
			if (bulletstats & Follow)
			{
				if (!maintarget)
				{
					if (ObjectPool::GetObjPool().IsObjectAvild())
					{
						maintarget = ObjectPool::GetObjPool().GetRandomObject(false, true);
						if (abs(GetAngle(maintarget, true) - GetRot()) > 0.5)
						{
							delete maintarget;
							maintarget = NULL;
						}
					}
				}
				else
				{
					if (maintarget->IsDead())
					if (ObjectPool::GetObjPool().IsObjectAvild())
						maintarget = ObjectPool::GetObjPool().GetRandomObject(false, true);
					else
					{
						delete maintarget;
						maintarget = NULL;
						Suicide();
						break;
					}
					GoToAnotherObject(maintarget);
				}
			}
			GoWithAnAngle(hge->Timer_GetDelta(), GetRot() - 1.57);
			break;
		case BULLET2:
			if (bulletstats & Follow)
			{
				if (maintarget)
				{
					if (maintarget->IsDead())
						maintarget = MakePosObject(maintarget->GetX(), maintarget->GetY());
					GoWithAnArc(maintarget, arc);
					if (IsCoincide(maintarget, true))
					{
						Suicide();
						delete maintarget;
						maintarget = NULL;
					}
				}
			}
			GoWithAnAngle(hge->Timer_GetDelta(), GetRot() - 1.57);
			break;
		case BULLET3:

			break;
		case BULLET4:
			GoWithAnAngle(hge->Timer_GetDelta(), GetRot() - 1.57);
			if (bulletstats & DtoDMG)
			{
				if (lifespan<100)
				lifespan++;
				SetSpeed(GetSpeed()+100/(101-lifespan));
			}
			if (bulletstats & Follow)
			{
				if (GetDistance(&Hero::GetHero()) < 100)
				{
					SetSpeed(GetSpeed() + 0.5);
					int newbulletstats = (int)bulletstats ^ (int)Follow;
					bulletstats = (BulletStats)newbulletstats;
				}
				else
				{
					if (ox < 2)
					     ox++;
					else
					{
						GetAngle(&Hero::GetHero());
						ox = 0;
					}
				}
			}
			break;
		case BULLET5:
			if (!maintarget)
			{
				GoWithAnAngle(hge->Timer_GetDelta(), GetRot() - 1.57);
				//	ox = hge->Random_Int(0, SCREEN_WIDTH);
				//	oy = hge->Random_Int(0, SCREEN_HEIGHT);
				if (ObjectPool::GetObjPool().IsObjectAvild())
				{
					maintarget = ObjectPool::GetObjPool().GetRandomObject(false, true);
					//maintarget->BeTargeted();
				}
			}
			else
			{
				if (maintarget->IsDead())
				if (ObjectPool::GetObjPool().IsObjectAvild())
					maintarget = ObjectPool::GetObjPool().GetRandomObject(false, true);
				else
				{
					delete maintarget;
					maintarget = NULL;
					break;
				}
				GoWithAnAngle(hge->Timer_GetDelta(), GetRot() - 1.57);
				if (GetAngle(maintarget, true) > GetRot())
					SetRot(GetRot() + 0.1);
				if (GetAngle(maintarget, true) < GetRot())
					SetRot(GetRot() - 0.1);
			}
		}
		if (bulletstats & SpeedUpDown)
		{
			if (!reversespeed)
			if (GetSpeed() <= tempspeed * 1.4)
				SetSpeed(GetSpeed()*1.2);
			else
				reversespeed = true;
			else
			if (GetSpeed() >= 10)
				SetSpeed(GetSpeed()*0.3);
			else
				reversespeed = false;
		}
		if (bulletstats & Spin)
		{
			if (GetHost())
			{
				if (GetHost()->GetCollisionRange())
					SpinWithRadius(GetHost(), GetHost()->GetCollisionRange() + 15);
				else
					SpinWithRadius(GetHost(), 30);
			}
			else
			{
				int newbulletstats = (int)bulletstats ^ (int)Spin;
				bulletstats = (BulletStats)newbulletstats;
			}
		}
		if (GetSize() <= 0 || GetSpeed() <= 0)
			Suicide();
	}
}

void Bullet::effect(GameObject *vir)
{
	    HGE *hge = hgeCreate(HGE_VERSION);
		int damage=0;
		if (bulletnum != BULLET2)
			damage = -GetDMG();
		else
		{
			if (vir->IsBoss())
				damage = -GetDMG() / 2;
			else
				damage = -GetDMG();
		}
	    vir->DmgDispose(damage,this->GetBreakArmor());
	 	vir->varyHP(damage);
		if (knockback!=0 && vir->BulletBoomEffect())
		vir->BoomEffect(this, hge->Timer_GetDelta(), knockback);
		if (vir->NeedRenderFont())
		FontPool::GetFontPool().InsertFont(this, damage, vir, Damage);
		if (bulletstats & Frost)
		{
			if(hge->Random_Int(0,4)==2 && vir->SetErrorState(freeze, -0.8*vir->GetSpeed(), 100))
			RenderPool::GetRenderPool().InstantRender(vir->GetX(), vir->GetY(), FREEZE,0, 1, vir, 24);
		}
		if (bulletstats & Toxic)
		{
			if (hge->Random_Int(0, 7) == 4 && vir->SetErrorState(toxic, GetDMG() / 10, 100))
			RenderPool::GetRenderPool().InstantRender(vir->GetX(), vir->GetY(),  TOXIC,0, 1, vir, 24);
		}
		if (bulletstats & Stun)
		{
			if (hge->Random_Int(1,10)==5 && vir->SetErrorState(vertigo, GetDMG() / 10, 50))
				RenderPool::GetRenderPool().InstantRender(vir->GetX(), vir->GetY(), VERTIGO, 0, 1, vir, 48);
		}
		if (bulletstats & SuckBlood && GetHost())
		{
			GetHost()->varyHP(Hero::GetHero().GetLevel()/5+1);
		}
			Suicide();
		SFXManager::GetSFXManager().PlaySFX(HITSFX1+hge->Random_Int(0,1));
}

bool Bullet::interaction(GameObject *vir)
{
	if (vir->IsMine())
	{
		Suicide();
		return false;
	}
	if (vir->IsShield())
	{
		dynamic_cast<Shield*>(vir)->MinusDurability();
		if (!GetIsPenetrate())
		{
			if (dynamic_cast<Shield*>(vir)->GetShieldType() == SkillReflect)
			{
				float delta = vir->GetRot()-3.14 - GetRot();
					SetRot(vir->GetRot()+delta);
				if (bulletstats & Follow)
				{
					int newbulletstats = (int)bulletstats ^ (int)Follow;
					bulletstats = (BulletStats)newbulletstats;
				}
				ReverseSkill();
				return false;
			}
			else
			{
				Suicide();
				return true;
			}
			return true;
		}
		return true;
	}
	return true;
}