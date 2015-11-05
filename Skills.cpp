#include "Skills.h"
#include "Bullet.h"
#include "Mine.h"

Skills::~Skills()
{
}


Skills::Skills(GameObject* host, int skilltype1, int last_time, bool effectall) :SKILL(host->GetDMG(false), 0, 0, 0, 1)
{
	this->effectall = effectall;
	SetCrit(host->GetCrit());
	SetCritDMG(host->GetCritDMG());
	SetBreakArmor(host->GetBreakArmor());
	info2 = 0;
	followhost = false;
	ctrltarget = NULL;
	SetSize(1.0);
	info = 0;
	HGE *hge = hgeCreate(HGE_VERSION);
	TX = 0;
	TY = 0;
	delay = 0;
	effective = false;
	skilltype = skilltype1;
	if (!host->IsDead())
		SetHost(host);
	else
		SetHost(NULL);
	SetDelayrender(true);
	float x, y;
	switch (skilltype)
	{
	case BOOM:
		Setcost(0);
		SetSpeed(0);
		SetPos(host->GetX(), host->GetY());
		if (!last_time)
			SetRange(150);
		else
		{
			SetRange(last_time);
			SetSize((float)last_time / (float)150);
		}
		ReversePenetrate();
		break;
	case FIREBALL:
		Setcost(100);
		SetSpeed(200);
		hge->Input_GetMousePos(&x, &y);
		SetRange(1);
		info = host->GetX();
		info2 = host->GetY();
		SetPos(host->GetX(), host->GetY());
		if (host->TellType() == HERO)
			SetDMG(GetDMG(false)*(Hero::GetHero().GetMagicLevel() + 5));
		else
			SetDMG(GetDMG(false) * 10);
		TX =x;
		TY =y;
		SetSize(0.4);
		break;
	case CHAIN:
		Setcost(8);
		SetSpeed(100);
		SetRange(20);
		SetPos(host->GetX(), host->GetY());
		SetRot(host->GetRot());
		break;
	case CHAINPART:
		Setcost(0);
		SetSpeed(500);
		SetRange(1);
		SetLAST_TIME(10);
		SetDMG(GetDMG(false)*(Hero::GetHero().GetMagicLevel() + 2));
		SetPos(host->GetX(), host->GetY());
		break;
	case LENSBULLET:
		Setcost(8);
		SetSpeed(0);
		SetRange(1);
		SetPos(host->GetX(), host->GetY());
		hge->Input_GetMousePos(&x, &y);
		TX = x;
		TY = y;
		{
		GameObject *a = MakePosObject(x, y);
		GetAngle(a);
		delete a;
		a = NULL;
		}
		break;
	case LENSPART:
		SetRange(32);
		Setcost(0);
		SetSpeed(600);
		SetDMG(GetDMG(false)*(Hero::GetHero().GetMagicLevel() + 2));
		break;
	case FORCEFIELD:
		SetRange(400);
		Setcost(5);
		hge->Input_GetMousePos(&x, &y);
		SetPos(x, y);
		SetLAST_TIME(500);
		SetDMG(GetDMG() / 10);
		break;
	case FREEZEFIELD:
		delay = 45;
		Setcost(8);
		SetSpeed(0);
		hge->Input_GetMousePos(&x, &y);
		SetPos(x, y);
		SetRange(70);
		if (!last_time)
			SetLAST_TIME(500);
		else
		{
			SetLAST_TIME(last_time);
			SetSize((float)last_time / (float)70);
		}
		ReversePenetrate();
		break;
	case TOXICFIELD:
		Setcost(8);
		delay = 45;
		info2 = SceneManager::GetSceneManager().GetWave();
		SetDMG(GetDMG() / 4);
		SetSpeed(0);
		hge->Input_GetMousePos(&x, &y);
		SetPos(x, y);
		SetRange(70);
		if (!last_time)
			SetLAST_TIME(500);
		else
			SetLAST_TIME(last_time);
		ReversePenetrate();
		break;
	case HEALFIELD:
		//要么削时间加量要么削量。。。好难抉择。
		Setcost(8);
		delay = 45;
		info2 = SceneManager::GetSceneManager().GetWave();
		if (host->IsBoss())
		{
			SetPos(host->GetX(), host->GetY());
			SetDMG((host->GetMaxHP() - host->GetHP()) / 100);
			followhost = true;
		}
		else
		{
			SetDMG(GetDMG() / 10);
			hge->Input_GetMousePos(&x, &y);
			SetPos(x, y);
		}
		SetSpeed(0);
		SetRange(70);
		if (!last_time)
			SetLAST_TIME(500);
		else
			SetLAST_TIME(last_time);
		ReversePenetrate();
		break;
	case DASH:
		if (host->TellType() == HERO)
		{
			Setcost(4);
			Hero::GetHero().Lock();
			hge->Input_GetMousePos(&x, &y);
			SetPos(Hero::GetHero().GetX(), Hero::GetHero().GetY());
			TX = x;
			TY = y;
			if (TX > ACT_END_X)TX = ACT_END_X - 1;
			if (TX < ACT_START_X)TX = ACT_START_X + 1;
			if (TY > ACT_END_Y)TY = ACT_END_Y - 1;
			if (TY < ACT_START_Y)TY = ACT_START_Y + 1;
		}
		else
		{

			if (host->GetIsLocked())
				Suicide(true);
			else
			{
				Setcost(0);
				SetPos(host->GetX(), host->GetY());
				ctrltarget = host;
				ctrltarget->SetLockState(true);
				info = ctrltarget->GetSpeed();
				ctrltarget->SetSpeed(0);
				TX = ctrltarget->GetX() + hge->Random_Float(-100, 100);
				TY = ctrltarget->GetY() + hge->Random_Float(-100, 100);
				if (TX <= ACT_START_X + 40)TX = ACT_START_X + 40;
				if (TX >= ACT_END_X - 40)TX = ACT_END_X - 40;
				if (TY <= ACT_START_Y + 40)TY = ACT_START_Y + 40;
				if (TY >= ACT_END_Y - 40)TY = ACT_END_Y - 40;
			}
		}
		break;
	case FLASH:
		Setcost(0);
		SetLAST_TIME(50);
		TX = host->GetX() + hge->Random_Float(-100, 100);
		TY = host->GetY() + hge->Random_Float(-100, 100);
		if (TX <= ACT_START_X + 40)TX = ACT_START_X + 40;
		if (TX >= ACT_END_X - 40)TX = ACT_END_X - 40;
		if (TY <= ACT_START_Y + 40)TY = ACT_START_Y + 40;
		if (TY >= ACT_END_Y - 40)TY = ACT_END_Y - 40;
		SetPos(TX, TY);
		if (host->GetIsLocked())
			Suicide(true);
		else
			host->SetLockState(true);
		break;
	case SPAWNSHEILD:
		Setcost(5);
		if (host->GetHasShield())
			Suicide(true);
		break;
	case BOUNCEANDBOUNCE:
		Setcost(3);
		SetLAST_TIME(400);
		SetSpeed(300);
		SetPos(host->GetX(), host->GetY());
		SetDMG(0);
		SetRot(host->GetRot());
		if (host->GetIsLocked())
			Suicide(true);
		else
			host->SetLockState(true);
		break;
	case BOUNCEBULLETS:
		Setcost(4);
		SetLAST_TIME(100);
		SetPos(host->GetX(), host->GetY());
		SetSpeed(0);
		SetRot(host->GetRot());
		info = host->GetSpeed();
		if (host->GetIsLocked())
			Suicide(true);
		break;
	case FIVEBULLETS:
		info = host->GetSpeed();
		SetPos(host->GetX(), host->GetY());
		Setcost(4);
		SetLAST_TIME(20);
		SetSpeed(0);
		SetRot(host->GetRot());
		if (host->GetIsLocked())
			Suicide(true);
		break;
	case RECTBULLETS:
		Setcost(4);
		SetPos(host->GetX(), host->GetY());
		break;
	case FANBULLETSEX:
		Setcost(8);
		SetPos(host->GetX(), host->GetY());
		followhost = true;
		SetLAST_TIME(200);
		info = host->GetSpeed();
		break;
	case SHRINKBULLETS:
		Setcost(5);
		SetPos(host->GetX(), host->GetY());
		SetLAST_TIME(100);
		SetRot(host->GetRot());
		info = GetRot();
		TX = Hero::GetHero().GetX();
		TY = Hero::GetHero().GetY();
		info2 = GetLAST_TIME();
		break;
	case HEXABULLETS:
		Setcost(6);
		SetPos(host->GetX(), host->GetY());
		followhost = true;
		SetLAST_TIME(100);
		break;
	case WIFIBULLETS:
		Setcost(5);
		SetPos(host->GetX(), host->GetY());
		followhost = true;
		info2 = 25;
		SetLAST_TIME(160);
		break;
	case TOTEMEFFECT:
		Setcost(0);
		followhost = true;
		SetRange(70);
		info2 = last_time;
		break;
	}
	if (GetHost() && host->GetMP() - GetCost() < 0)
	{
		Suicide(true);
		if (host->TellType() == HERO)
			Hero::GetHero().Unlock();
	}
};









void Skills::action()
{
	if (followhost && GetHost())
		SetPos(GetHost()->GetX(), GetHost()->GetY());
	if (delay <= 0)SetDelayrender(false);
	if (IsDelayrender())
		delay--;
	else
	{
		HGE *hge = hgeCreate(HGE_VERSION);
		GameObject *pos = MakePosObject(TX, TY);
		switch (skilltype)
		{
		case BOOM:

			break;
		case FIREBALL:
			if (effective)
				Suicide();
			{
				GameObject *a = MakePosObject(info, info2);
				float delta = GetDistance(pos) / a->GetDistance(pos);
			SetSize(0.4-4 * delta*delta + 4 * delta);
			if (GetSize() < 0)
				SetSize(0);
				//SetSize(1);
			SetSpeed(200 - 200 * delta*delta + 200 * delta);
			if (!IsCoincide(pos))
				GoToAnotherObject(pos);
			else
			{
				effective = true;
				SetRange(100);
			}
			delete a;
			a = NULL;
			}
			break;
		case CHAIN:
			if (GetX()<ACT_START_X || GetY()<ACT_START_Y || GetX()>ACT_END_X || GetY()>ACT_END_Y)
				Suicide();
			if (!effective)
			{
				RenderPool::GetRenderPool().SpawnPS(this, MPUNIT);
				effective = true;
			}
			GoWithAnAngle(hge->Timer_GetDelta(), GetRot()-1.57);
			break;
		case CHAINPART:
			if (ctrltarget)
			{
				if (!effective)
				{
					RenderPool::GetRenderPool().SpawnPS(this, MPUNIT);
					effective = true;
				}
				if (ctrltarget->IsDead())
				{
					ctrltarget = NULL;
					Suicide();
				}
				else
				{
					if (!IsCoincide(ctrltarget,true))
					GoToAnotherObject(ctrltarget);
					else
					{
						if (!LAST_TIMER())
						{
							int damage = -GetDMG();
							ctrltarget->DmgDispose(damage, this->GetBreakArmor());
							ctrltarget->varyHP(damage);
							FontPool::GetFontPool().InsertFont(this, damage, ctrltarget, Damage);
							SetRange(100);
						}
					}
				}
			}
			else
				Suicide();
			break;
		case LENSBULLET:
			if (info2 < 8)
			{
				if (info < 10)
					info++;
				else
				{
					GameObject *a = MakePosObject(TX, TY);
					Skills *skill = new Skills(GetHost(), LENSPART);
					double angle = GetRot()-1.047*((float)info2/8);
					skill->SetPos(GetX()+100*sin(angle),GetY()-100*cos(angle));
					skill->GetAngle(a);
					skill->SetLAST_TIME(10 * 9 - info2);
					ObjectPool::GetObjPool().InsertObject(skill, true);
					Skills *skill1 = new Skills(GetHost(), LENSPART);
					double angle1 = GetRot() + 1.047*((float)info2 / 8);
					skill1->SetPos(GetX() + 100 * sin(angle1), GetY() - 100 * cos(angle1));
					skill1->GetAngle(a);
					skill1->SetLAST_TIME(10 * 9 - info2);
					ObjectPool::GetObjPool().InsertObject(skill1, true);
					delete a;
					a = NULL;
					info2++;
				}
			}
			else
			{
				Suicide();
			}
			break;
		case LENSPART:
			if (GetX()<ACT_START_X-200 || GetY()<ACT_START_Y-200 || GetX()>ACT_END_X+200 || GetY()>ACT_END_Y+200)
				Suicide();
			if (!effective)
			{
				RenderPool::GetRenderPool().SpawnPS(this, MPUNIT);
				effective = true;
			}
			if (!LAST_TIMER())
			GoWithAnAngle(hge->Timer_GetDelta(), GetRot() - 1.57);
			break;
		case FORCEFIELD:
			if (!LAST_TIMER())
				Suicide();
			else
			{
				if (GetLAST_TIME() % 25)
					effective = true;
				else
					effective = false;
			}
			break;
		case FREEZEFIELD:
			if (!LAST_TIMER())
			{
				Suicide();
			}
			else
			{
				if (GetLAST_TIME() % 20 == 0)
					effective = true;
				else
					effective = false;
			}
			break;
		case DASH:
			if (!ctrltarget)
			{
				if (!IsDead())
				{
					Hero::GetHero().SetPos(GetX(),GetY());
					if (!IsCoincide(pos))
					{
						int dis = Hero::GetHero().GetDistance(pos);
						GoToAnotherObject(pos);
						SetSpeed(dis*6);
					}
					else
					{
						Hero::GetHero().Unlock();
						Suicide();
					}
				}
			}
			else
			{
				if (!ctrltarget->IsDead())
				{
				
					if (!IsCoincide(pos, true))
					{
						SetSpeed(GetDistance(pos) * 5);
						GoToAnotherObject(pos);
						ctrltarget->SetPos(GetX(), GetY());
					}
					else
					{
						ctrltarget->SetSpeed(info);
						ctrltarget->SetLockState(false);
						Suicide();
					}
				}
				else
				{
					ctrltarget = NULL;
					Suicide();
				}
			}
			break;
		case TOXICFIELD:
			if (!LAST_TIMER())
			{
				Suicide();
			}
			else
			{
				if (GetLAST_TIME() % 20 == 0)
					effective = true;
				else
					effective = false;
			}
			break;
		case FLASH:
			if (LAST_TIMER())
			{
				if (GetHost())
				{
					GetHost()->SetReachable(false);
					GetHost()->SetDelayrender(true);
				}
			}
			else
			{
				if (GetHost())
				{
					GetHost()->SetPos(GetX(), GetY());
					GetHost()->SetReachable(true);
					GetHost()->SetDelayrender(false);
					GetHost()->SetLockState(false);
				}
				Suicide();
			}
			break;
		case HEALFIELD:
			if (info2 != SceneManager::GetSceneManager().GetWave())
				Suicide();
			if (!LAST_TIMER())
			{
				if (GetHost() && GetHost()->IsBoss())
					Suicide();
				SetLAST_TIME(100);
			}
			else
			{
				if (GetHost())
				{
					if (GetHost()->IsBoss())
					{
						SetDMG((GetHost()->GetMaxHP() - GetHost()->GetHP()) / 80);
					}
				}
				else
					Suicide();
			}
			break;
		case SPAWNSHEILD:
			if (!effective && GetHost())
			{
				GetHost()->SetHasShield(true);
				if (GetHost()->TellType() != HERO)
				{
					int count = 1;
					for (int i = 0; i < count; i++)
					{
						Shield *shield = new Shield(this->GetHost(), SkillReflect, SHIELD1, 500, false);
						shield->SetRot(6.28*i / count);
						ObjectPool::GetObjPool().InsertObject(shield, true);
					}
				}
				else
				{
					Shield *shield = new Shield(this->GetHost(), Guard, SHIELD2, 500, false);
					ObjectPool::GetObjPool().InsertObject(shield, true);
				}
				effective = true;
			}
			else
				Suicide();
			break;
		case DODGEMAX:
			if (!LAST_TIMER())
			{
				if (GetHost())
					GetHost()->SetDodge(info2);
				Suicide();
			}
			else
			{
				if (GetHost())
					GetHost()->SetDodge(100);
			}
			break;
		case BOUNCEANDBOUNCE:
			if (LAST_TIMER())
			{
				GoWithAnAngle(hge->Timer_GetDelta(),GetRot()-1.57);
				if (GetX()<ACT_START_X || GetY()<ACT_START_Y || GetX()>ACT_END_X || GetY()>ACT_END_Y)
				{
						if (GetY()>ACT_START_Y)
							SetRot(-GetRot());
						else
							SetRot(3.14 - GetRot());
						if (GetY()>ACT_END_Y)
							SetRot(3.14 + GetRot());
				}
				if (GetHost())
				{
					GetHost()->SetLockState(true);
					GetHost()->SetPos(GetX(), GetY());
				}
			}
			else
			{
				Suicide();
				if (GetHost())
					GetHost()->SetLockState(false);
			}
			break;
		case BOUNCEBULLETS:
			if (LAST_TIMER())
			{
				if (GetHost())
				{
					GetHost()->SetSpeed(0);
					GetHost()->SetLockState(true);
				if (GetLAST_TIME() % 5 == 0)
				{
					Bullet *bullet = new Bullet(this, BULLET4);
					bullet->SetSpeed(350);
					bullet->SetBounce(10);
					ObjectPool::GetObjPool().InsertObject(bullet, true);
				}
				}
				else
					SetLAST_TIME(1);
			}
			else
			{
				Suicide();
				if (GetHost())
				{
					GetHost()->SetSpeed(info);
					GetHost()->SetLockState(false);
				}
			}
			break;
		case FIVEBULLETS:
			if (LAST_TIMER())
			{
				if (GetHost())
				{
					GetHost()->SetSpeed(0);
					GetHost()->SetLockState(true);
					if (GetLAST_TIME() % 5 == 0)
					{
						for (int i = 0; i < 5; i++)
						{
							Bullet *bullet = new Bullet(this, BULLET4);
							bullet->SetSpeed(300);
							bullet->SetRot(GetRot() - 0.2 + 0.1*i);
							ObjectPool::GetObjPool().InsertObject(bullet, true);
						}
					}
				}
				else
					Suicide();
			}
			else
			{
				Suicide();
				if (GetHost())
				{
					GetHost()->SetSpeed(info);
					GetHost()->SetLockState(false);
				}
			}
			break;
		case RECTBULLETS:
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					Bullet *bullet = new Bullet(this, BULLET4);
					bullet->SetSpeed(100);
					bullet->SetRot(M_PI/2*i);
					bullet->SetPos(GetX() + 100 - 33 * j, GetY()-100);
					bullet->SetPos(GetX() +100*(cos(M_PI / 2 * i) - sin(M_PI / 2 * i)), GetY() +100*(cos(M_PI / 2 * i) + sin(M_PI / 2 * i)));
					ObjectPool::GetObjPool().InsertObject(bullet,true);
				}
			}
			Suicide();
			break;
		case FANBULLETSEX:
			if (LAST_TIMER())
			{
				if (GetHost())
				{
					GetHost()->SetSpeed(0);
					GetHost()->SetLockState(true);
					if (GetLAST_TIME() % 5 == 0)
					{
						for (int i = 0; i < 5; i++)
						{
							Bullet *bullet = new Bullet(this, BULLET4);
							bullet->SetSpeed(300);
							bullet->SetRot(GetRot() + M_PI * 2 * i / 5);
							ObjectPool::GetObjPool().InsertObject(bullet, true);
						}
					}
					SetRot(GetRot() + 0.01);
				}
				else
					Suicide();
			}
			else
			{
				Suicide();
				if (GetHost())
				{
					GetHost()->SetSpeed(info);
					GetHost()->SetLockState(false);
				}
			}
			break;
		case SHRINKBULLETS:
			if (LAST_TIMER())
			{
				if (GetLAST_TIME() % 4 == 0)
				{
					SetRot(info + 6.28*GetLAST_TIME() / (float)info2);
					Bullet *bullet = new Bullet(this, BULLET4);
					bullet->SetPos(TX + 100 * (cos(GetRot()) - sin(GetRot())), TY + 100 * (cos(GetRot()) + sin(GetRot())));
					bullet->GetAngle(pos);
					bullet->SetSpeed(100);
					bullet->SetStopTime(GetLAST_TIME());
					ObjectPool::GetObjPool().InsertObject(bullet, true);
				}
			}
			else
			  Suicide();
			break;
		case HEXABULLETS:
			if (LAST_TIMER())
			{
				if (GetLAST_TIME()%10==0)
				for (int i = 0; i < 6; i++)
				{
					SetRot(GetRot() + 6.28*i / 6);
					for (int j = 0; j < 3; j++)
					{
						Bullet *bullet = new Bullet(this, BULLET4);
						bullet->SetPos(GetX()-20+20*i + 200 * (cos(GetRot() - sin(GetRot()))), GetY()-20+20*i + 200 * (cos(GetRot() + sin(GetRot()))));
						bullet->SetSpeed(100);
						ObjectPool::GetObjPool().InsertObject(bullet, true);
					}
				}
			}
			else
				Suicide();
			break;
		case WIFIBULLETS:
			if (LAST_TIMER())
			{
				if (GetHost())
				{
					SetRot(GetHost()->GetRot());
					if (GetLAST_TIME() % 50 == 0)
					{
						for (int i = 0; i < info2; i++)
						{
							Bullet *bullet = new Bullet(this, BULLET4);
							bullet->SetRot(GetRot() - info2 / 2 * 0.08 + i * 0.08);
							bullet->SetSpeed(170);
							ObjectPool::GetObjPool().InsertObject(bullet, true);
						}
						info2 -= 6;
					}
				}
				else
					Suicide();		
			}
			else
				Suicide();
			break;
		case TOTEMEFFECT:
			if (!GetHost())
				Suicide();
			else
			{
				if (!LAST_TIMER())
				{
					SetLAST_TIME(101);
				}
				else
				{
					if (GetLAST_TIME() % 50 == 0)
						effective = true;
					else
						effective = false;
				}
			}
			break;
		}
		delete pos;
		pos = NULL;
	}
}

void Skills::effect(GameObject *vir)
{
	HGE *hge = hgeCreate(HGE_VERSION);
	int damage = -GetDMG();
	int armorbreak = vir->GetDEF();
	switch (skilltype)
	{
	case BOOM:
		vir->DmgDispose(damage, armorbreak);
		vir->varyHP(damage);
		FontPool::GetFontPool().InsertFont(this, damage, vir, Damage);
		if (vir->BulletBoomEffect())
		vir->BoomEffect(this, hge->Timer_GetDelta(), 30);
		break;
	case CHAIN:
	{
				  vir->DmgDispose(damage, armorbreak);
				  vir->varyHP(damage);
				  FontPool::GetFontPool().InsertFont(this, damage, vir, Damage);
				  Skills *skill = new Skills(GetHost(), CHAINPART);
				  skill->ctrltarget = vir;
				  ObjectPool::GetObjPool().InsertObject(skill, true);
				  Suicide();
	              break;
	}
	case CHAINPART:
		if (GetRange()!=1 && GetDistance(vir)>80 && vir != ctrltarget)
		{
			Skills *skill = new Skills(GetHost(), CHAINPART);
			skill->ctrltarget = vir;
			ObjectPool::GetObjPool().InsertObject(skill, true);
		}
		break;
	case LENSPART:
		vir->DmgDispose(damage, armorbreak);
		vir->varyHP(damage);
		FontPool::GetFontPool().InsertFont(this, damage, vir, Damage);
		break;
	case FORCEFIELD:
		if (GetDistance(vir) > 50)
		{
			int rate = -GetDistance(vir) / 40-1;
			vir->BoomEffect(this, hge->Timer_GetDelta(), rate);
		}
		else
		if (effective)
		{
			vir->DmgDispose(damage, armorbreak);
			vir->varyHP(damage);
			FontPool::GetFontPool().InsertFont(this, damage, vir, Damage);
		}
	    break;
	case FREEZEFIELD:
		if (effective)
		{
			if(vir->SetErrorState(freeze, -0.8, 49))
			RenderPool::GetRenderPool().InstantRender(vir->GetX(), vir->GetY(), FREEZE,0, 1, vir, 48);
		}
		break;
	case TOXICFIELD:
		if (effective)
		{
			if(vir->SetErrorState(toxic, GetDMG(), 49))
			RenderPool::GetRenderPool().InstantRender(vir->GetX(), vir->GetY(), TOXIC, 0, 1, vir, 48);
		}
		break;
	case HEALFIELD:
			vir->SetErrorState(heal, GetDMG(), 49);
		break;
	case FIREBALL:
		if (effective)
		{
			damage *= 1 + (float)(100 - GetDistance(vir)) / 100;
			vir->DmgDispose(damage, armorbreak);
			vir->varyHP(damage);
			vir->SetErrorState(vertigo, 1, 100);
			RenderPool::GetRenderPool().InstantRender(vir->GetX(), vir->GetY(), VERTIGO, 0, 1, vir, 24);
			FontPool::GetFontPool().InsertFont(this, damage, vir, Damage);
		}
		break;
	case TOTEMEFFECT:
		if (effective && vir != GetHost())
		{
			switch (info2)
			{
			case 1:
				//ATKUP
				vir->SetDMG(vir->GetDMG()*1.1 + 1);	
				break;
			case 2:
				//CRITUP
				vir->SetCrit(vir->GetCrit()*1.1 + 1);
				break;
			case 3:
				//HPUP
				vir->SetErrorState(heal, vir->GetMaxHP()*0.01, 50);
				break;
			case 4:
				//DODGEUP
				vir->SetDodge(vir->GetDodge()*1.1 + 1);
				break;
			case 5:
				//DEFUP
				vir->SetDEF(vir->GetDEF()*1.1 + 1);
				break;
			}
		}
		break;
	}
};


bool Skills::interaction(GameObject *vir)
{
	if (vir->IsShield())
	{
		dynamic_cast<Shield*>(vir)->MinusDurability();
		if (!GetIsPenetrate())
		{
			if (dynamic_cast<Shield*>(vir)->GetShieldType() == SkillReflect)
			{
				SetRot(GetRot() - 3.14);
				ReverseSkill();
				return false;
			}
			else
			{
				Suicide();
				ForceToStop();
				return true;
			}
			return true;
		}
		return true;
	}
	return true;
}


void Skills::AfterOneEffect()
{
	switch (skilltype)
	{
	case BOOM:
		Suicide(true);
		if (!IsPlayedDeadAnim())
		{
			SetPlayedDeadAnim();
			RenderPool::GetRenderPool().InstantRender(GetX(), GetY(), BOSS1,0,GetSize());
		}
		break;
	case CHAINPART:
		if (GetRange() != 1)
			Suicide(true);
		break;
	}
}


void Skills::born()
{
		if (!finishb)
		{
			RenderPool::GetRenderPool().InstantRender(GetX(), GetY(), skilltype, 0, GetSize(), this);
			SetAlive(true);
			finishb = true;
			if (GetHost())
			{
				if (!GetHost()->IsDead())
				{
					GetHost()->varyMP(-GetCost());
					if (GetHost()->TellType() != HERO)
						ReverseSkill();
				}
				else
					SetHost(NULL);
			}
		}
}

void Skills::death()
{
	Contract();
	if (!IsPlayedDeadAnim())
	{
		SetPlayedDeadAnim();
		RenderPool::GetRenderPool().InstantRender(GetX(), GetY(), skilltype+40, 0);
	}
};
