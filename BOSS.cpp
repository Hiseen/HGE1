#include "BOSS.h"

BOSS::~BOSS()
{
}


void BOSS::action()
{
	HGE* hge = hgeCreate(HGE_VERSION);
	BaseAction();
	if (!(GetErrorState() & vertigo))
	{
		UseSkills();
		if (ReturnValid(skilldelay, 300, 400))
		{
			switch (Mode)
			{
			case ATTACK:
				Attack();
				break;
			case GUARD:
				Guard();
				break;
			}
		}
		if (switchmodedelay)
			switchmodedelay--;
		if (GetMP()<MAX_MP && ReturnValid(recoverMP, 150, 150))
			varyMP(1);
		if (ReturnValid(countmove, 100, 1000))
		{
			tx = hge->Random_Int(ACT_START_X, ACT_END_X);
			ty = hge->Random_Int(ACT_START_Y, ACT_END_Y);
		}
		GameObject *a = GameObject::MakePosObject(tx, ty);
		if (!IsCoincide(a))
		GoToAnotherObject(a);
		else
		{
			tx = hge->Random_Int(ACT_START_X, ACT_END_X);
			ty = hge->Random_Int(ACT_START_Y, ACT_END_Y);
		}
		double d1 = GetRot() - GetAngle(&Hero::GetHero(), true);
		double d2 = GetAngle(&Hero::GetHero(), true) - GetRot();
		GetAngle(&Hero::GetHero());
	}
	if (Hero::GetHero().IsVulnerable(this))
	{
		int damage = -GetDMG(false);
		Hero::GetHero().DmgDispose(damage, this->GetBreakArmor());
		Hero::GetHero().varyHP(damage);
		FontPool::GetFontPool().InsertFont(Hero::GetHero().GetX(), Hero::GetHero().GetY(), damage, Damage);
	}
}

void BOSS::UseSkills()
{
}

void BOSS::Attack()
{
	HGE *hge = hgeCreate(HGE_VERSION);
	int skilla = attackskills[hge->Random_Int(0, 1)];
	if (skilla != SPAWNMINIONS)
	{
		Skills* skill = new Skills(this, skilla);
		if (!ObjectPool::GetObjPool().InsertObject(skill))
			skilldelay = 1;
	}
	else
	{
		Summon();
		skilldelay += 500;
	}
}


void BOSS::Guard()
{
	HGE *hge = hgeCreate(HGE_VERSION);
	Skills* skill = new Skills(this, guardskills[hge->Random_Int(0, 1)]);
	if (!ObjectPool::GetObjPool().InsertObject(skill))
		skilldelay = 1;
}






 void BOSS::Underattack(GameObject *obj)
{
	 
	 if (!(GetErrorState() & vertigo) && obj && !obj->IsBoss())
	 {
		
		 /*
		 switch (BossID)
		 {
		 case BOSS2:
		 if (GetReverseDMG())
		 {
		 count++;
		 if (Hero::GetHero().IsVulnerable(this,false))
		 {
		 int dmg = -((count/5)+1);
		 Hero::GetHero().DmgDispose(dmg);
		 FontPool::GetFontPool().InsertFont(Hero::GetHero().GetX(), Hero::GetHero().GetY(), dmg);
		 Hero::GetHero().varyHP(dmg);
		 }
		 }
		 break;
		 }
		 */
		 if (!switchmodedelay)
			 Think();
	 }
}

 bool BOSS::varyHP(int dHP)
 {
	 HGE *hge = hgeCreate(HGE_VERSION);
	 if (dHP<0 && hge->Random_Int(1, 3) == 2)
	 {
		 Skills *skill = new Skills(this, DASH);
		 ObjectPool::GetObjPool().InsertObject(skill, true);
	 }
	return GameObject::varyHP(dHP);
 }

 void BOSS::Think()
 {
	 //BOSS攻击时的移动速度会变缓慢 防御时的移动速度变为正常（攻击方式是子弹）
	 //攻击时释放攻击技能 防御时释放防御技能
	 //还有随机施放技能（在任何mode下都可能）
	 float HHPD, MHPD;
	 HHPD = (HHP - Hero::GetHero().GetHP())/HHP;
	 MHPD = (MHP - GetHP())/MHP;
	 HGE *hge = hgeCreate(HGE_VERSION);
	 float HPpercent = (float)GetHP() / (float)GetMaxHP();
	 //基于自身血量与敌人血量的判断 20 80
	 if (HPpercent > 0.8)
	 {
		 AttackMode();
	 }
	 else if (HPpercent < 0.2)
	 {
		 if (hge->Random_Float(0, HPpercent) < (HPpercent / 16))
			 AttackMode();
		 else
			 GuardMode();
	 }
	 else
	 {
		 //基于敌人一次造成的伤害以及自身一次能造成的伤害判断（敌人血量的变化和自身血量的变化）
		 if (HHPD>=MHPD)
		 {
			 //自身伤害大于敌方
			 AttackMode();
		 }
		 else
		 {
			 //自身伤害小于敌方
			 if (hge->Random_Int(0, 4) < 3)
				 GuardMode();
			 else
				 AttackMode();
		 }
	 }
 }


 void BOSS::Summon()
 {
	 HGE *hge = hgeCreate(HGE_VERSION);
	 int a, countforkids;
	 switch (Mode)
	 {
	 case ATTACK:
		 a = hge->Random_Int(0, 2);
		 switch (a)
		 {
		 case 0:
			  countforkids = hge->Random_Int(4,16);
			 for (int i = 0; i < countforkids; i++)
			 {
				 Enemy *kids = new Enemy(0,0,ENEMY);
				 kids->SetPos(GetX() + 100 * (cos(i*6.28 / countforkids) - sin(i*6.28 / countforkids))
					        , GetY() + 100 * (cos(i*6.28 / countforkids) + sin(i*6.28 / countforkids)));
				 ObjectPool::GetObjPool().InsertObject(kids, true);
			 }
			 randomdelay += 250;
			 summondelay += 1500;
			 break;
		 case 1:
			 for (int i = 0; i < 5; i++)
			 {
				 int x, y;
				 x =50;
				 y =-50 + 25 * i;
				 float angle = GetAngle(&Hero::GetHero(), true)-1.57;
				 Enemy *kids = new Enemy(0, 0,ENEMY);
				 kids->SetPos(GetX() +  cos(angle)*x - sin(angle)*y
					        , GetY() +  cos(angle)*y + sin(angle)*x);
				 ObjectPool::GetObjPool().InsertObject(kids, true);
			 }
			 randomdelay += 150;
			 summondelay += 1000;
			 break;
		 case 2:
		 {
				   Enemy *kids = new Enemy(hge->Random_Int(ACT_START_X, ACT_END_X), hge->Random_Int(ACT_START_Y, ACT_END_Y), hge->Random_Int(70, 73));
				   ObjectPool::GetObjPool().InsertObject(kids, true);
		 }
			 break;

		 }
		 break;
	 case GUARD:
		 for (int i = 0; i < 3; i++)
		 {
			 Enemy *kids = new Enemy(0,0, GUARDBALL);
			 kids->SetRot(6.28*i / 3);
			 kids->SpinWithRadius(this, 30);
			 ObjectPool::GetObjPool().InsertObject(kids, true);
		 }
		 summondelay += 1500;
		 break;
	 default:
	 {
				Enemy *kids = new Enemy(hge->Random_Int(ACT_START_X,ACT_END_X),hge->Random_Int(ACT_START_Y,ACT_END_Y),hge->Random_Int(70,73));
				ObjectPool::GetObjPool().InsertObject(kids, true);
	 }
		 break;
	 }


 }