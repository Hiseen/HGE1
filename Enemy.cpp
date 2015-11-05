#include "Enemy.h"



Enemy::~Enemy()
{
}

void Enemy::action()
{
	HGE *hge = hgeCreate(HGE_VERSION);
	if (BaseAction() &&!(GetErrorState() & vertigo))
	{
		switch (type)
		{
		case ENEMY:
				switch (GetInnerType())
				{
				case RUNE_LANSER:
					if (!flag)
					{
						if (GetDistance(&Hero::GetHero()) > 250)
						{
							GoToAnotherObject(&Hero::GetHero());
						}
						else
						{
							if (skillcount <= 0)
							{
								posobj = MakePosObject(Hero::GetHero().GetX(), Hero::GetHero().GetY());
								SetSpeed(GetSpeed() * 5);
								skillcount = 100;
								flag = true;
							}
							else
								GoToAnotherObject(&Hero::GetHero());
						}
					}
					else
					{
						if (posobj)
						{
							if (!IsCoincide(posobj))
								GoToAnotherObject(posobj);
							else
							{
								flag = false;
								delete posobj;
								posobj = NULL;
								SetSpeed(MAX_speed);
							}
						}
					}
					if (skillcount>0)
						skillcount--;
					break;
				case RUNE_DRAGOON:
					if (GetDistance(&Hero::GetHero()) > 200)
					{
						GoToAnotherObject(&Hero::GetHero());
						skillcount = hge->Random_Int(0, 1);
					}
					else
						GoWithAnArc(&Hero::GetHero(), skillcount);
					break;
				case RUNE_TANK:
					if (GetDistance(&Hero::GetHero())<150)
						GoToAnotherObject(&Hero::GetHero());
					else
					{
						double rot = Hero::GetHero().GetRot()+hge->Random_Float(-0.2,0.2);
						posobj = MakePosObject(Hero::GetHero().GetX() + 80 * cos(rot-1.57), Hero::GetHero().GetY() + 80 * sin(rot-1.57));
						GoToAnotherObject(posobj);
						delete posobj;
						GetAngle(&Hero::GetHero());
					}
					if (skillcount >= 50)
					{
						varyHP((GetMaxHP()-GetHP())*0.01+1);
						skillcount = 0;
					}
					else
						skillcount++;
					break;
				case RUNE_BOOMER:
					if (GetDistance(&Hero::GetHero()) < 100)
						Suicide();
					else
						GoToAnotherObject(&Hero::GetHero());
					break;
				case RUNE_BLACKHOLE:
					if (GetDistance(&Hero::GetHero()) < 100)
					{
						Hero::GetHero().SetErrorState(freeze, -0.8, 50);
					}
					GoToAnotherObject(&Hero::GetHero());
						break;
				default:
					GoWithAnAngle(hge->Timer_GetDelta(),GetRot()-M_PI/2);
					break;
			     }
			if (Hero::GetHero().IsVulnerable(this))
			{
				int damage = -GetDMG(false);
				Hero::GetHero().DmgDispose(damage, this->GetBreakArmor());
				Hero::GetHero().varyHP(damage);
				FontPool::GetFontPool().InsertFont(Hero::GetHero().GetX(), Hero::GetHero().GetY(), damage, Damage);
			}
			break;
		case SUPERSPEEDBALL:
			if (skillcount > 0)
			{
				skillcount--;
				GoWithAnAngle(hge->Timer_GetDelta(), GetRot() - M_PI/2);
			}
			else
			{
				switch (GetInnerType())
				{
				case RUNE_SHOTGUN:
					for (int i = 0; i < 15; i++)
					{
						Bullet *bullet = new Bullet(this, BULLET4);
						bullet->SetRot(GetRot() - 0.30 + 0.04*i);
						ObjectPool::GetObjPool().InsertObject(bullet, true);
					}
					delete posobj;
					posobj = NULL;
					skillcount = hge->Random_Int(80,120);
					break;
				case RUNE_UZI:
					if (skillcount2 <= 400)
					{
						GetAngle(&Hero::GetHero());
						skillcount2++;
						if (skillcount2 % 10 == 0)
						{
							Bullet *bullet = new Bullet(this, BULLET4);
							ObjectPool::GetObjPool().InsertObject(bullet, true);
						}
					}
					else
					{
						delete posobj;
						posobj = NULL;
						skillcount = hge->Random_Int(80, 120);
						skillcount2 = 0;
					}
					break;
				case RUNE_ROCKET:
					for (int i = 0; i < 10; i++)
					{
						Bullet *bullet = new Bullet(this, BULLET4);
						bullet->SetBulletStats(DtoDMG);
						bullet->SetSpeed(1);
						bullet->SetRot(GetRot() -1.5+0.3*i);
						ObjectPool::GetObjPool().InsertObject(bullet, true);
					}
						delete posobj;
						posobj = NULL;
						skillcount = hge->Random_Int(80, 120);
						skillcount2 = 0;
					break;
				case RUNE_MAGICIAN:
				{
				GetAngle(&Hero::GetHero()); 
				Bullet *bullet = new Bullet(this, BULLET4);
				bullet->SetRot(GetRot());
				bullet->SetBulletStats(Follow);
				ObjectPool::GetObjPool().InsertObject(bullet, true);
				delete posobj;
				posobj = NULL;
				skillcount = hge->Random_Int(80, 120);
				}
					break;
				default:
				{
							   Bullet *bullet = new Bullet(this, BULLET4);
							   bullet->SetRot(GetRot() + hge->Random_Float(-0.5, 0.5));
							   ObjectPool::GetObjPool().InsertObject(bullet, true);
							   delete posobj;
							   posobj = NULL;
							   skillcount2 = 0;
							   skillcount = hge->Random_Int(80, 120);
				}
					break;
				}
			}
			break;
		case TRICKBALL:
		{
						  if (skillcount2 > 0)
						  {
							  skillcount2--;
							  GoWithAnAngle(hge->Timer_GetDelta(), GetRot() - 1.57);
						  }
						  else
						  {
							  if (skillcount3>=400)
								  GoWithAnAngle(hge->Timer_GetDelta(), GetRot() - 1.57);
							  else
							  {
								  skillcount3++;
								  if (skillcount <= 50)
								  {
									  skillcount++;
									  SetRot(GetRot() + M_PI / 200);
								  }
								  else
								  {
									  for (int i = 0; i < 4; i++)
									  {
										  Bullet *bullet = new Bullet(this, BULLET4);
										  bullet->SetRot(GetRot() + M_PI / 4 + M_PI / 2 * i);
										  bullet->SetSpeed(200);
										  ObjectPool::GetObjPool().InsertObject(bullet, true);
									  }
									  skillcount = 0;
								  }
							  }
						  }
			}
			break;
		case RINGBULLETBALL:
			//GoWithAnAngle(hge->Timer_GetDelta(), GetRot()-1.57);
			//if (GetSpeed() > 20)
				//SetSpeed(GetSpeed() - 1);
			//if (GetX()<ACT_START_X || GetY()<ACT_START_Y || GetX()>ACT_END_X || GetY()>ACT_END_Y)
		//	{
			//	if (GetY()>ACT_START_Y)
				//	SetRot(-GetRot());
				//else
					//SetRot(3.14 - GetRot());
				//if (GetY()>ACT_END_Y)
					//SetRot(3.14 + GetRot());
			//}
			if (skillcount < 100)
				skillcount++;
			else
			    Suicide(true);
			if (!Hero::GetHero().IsGod() && GetDistance(&Hero::GetHero()) < 37)
				Suicide();
			break;
		case STEALMANABALL:
			break;
		case GUARDBALL:
			if (ObjectPool::GetObjPool().IsBossDied())
				Suicide(true);
			else
			{
				SetSize(0.5);
				if (skillcount2 != (int)(500-300*((float)Hero::GetHero().GetLevel()/MAX_LEVEL)))
				{
					skillcount2++;
					SpinWithRadius(ObjectPool::GetObjPool().GetCurrentBoss(), ObjectPool::GetObjPool().GetCurrentBoss()->GetCollisionRange() + 10);
				}
				else
				{
					ObjectPool::GetObjPool().GetCurrentBoss()->varyHP(GetHP() * 4);
					Suicide(true);
				}
			}
			break;
		}
	}
}
void Enemy::born()
{
	switch (type)
	{
	case GUARDBALL:
		SetAlive(true);
		finishb = true;
		SetSize(0.5);
		break;
	default:
		if (!GetInnerType())
		Swell();
		else
		{
			SetSize(1.5);
			SetAlive(true);
			finishb = true;
		}
		break;
	}
}

void Enemy::death()
{
	HGE *hge = hgeCreate(HGE_VERSION);
	if (!this->IsPlayedDeadAnim() && !IsForcedToDie())
	{

		if (GetInnerType() == RUNE_BOOMER)
		{
			Skills *skill = new Skills(this, BOOM, 100, true);
			ObjectPool::GetObjPool().InsertObject(skill, true);
		}
		else
		if (type!=RINGBULLETBALL)
		{
			Skills *skill = new Skills(this, BOOM, 50, true);
			skill->SetDMG(hge->Random_Int(1, 5));
			ObjectPool::GetObjPool().InsertObject(skill, true);
		}
		else
		{
			Skills *skill = new Skills(this, BOOM, 80, true);
			ObjectPool::GetObjPool().InsertObject(skill, true);
		}
		SetPlayedDeadAnim();
		RenderPool::GetRenderPool().InstantRender(GetX(), GetY(), type,GetRot(),GetSize());
		SFXManager::GetSFXManager().PlaySFX(ENEMYBOOMSFX1 + hge->Random_Int(0, 1));
		Hero::GetHero().AddScore(score);
		if (minexp != 0)
		{
			for (int i = 0; i<hge->Random_Int(1,5); i++)
			{
				EXPUnit *exp = new EXPUnit(GetX(), GetY());
				ObjectPool::GetObjPool().InsertObject(exp, true);
			}
			Hero::GetHero().AbsorbEXP(minexp);
		}
	}
	Contract();
}

