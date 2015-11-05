#include "Item.h"


 void Item::action()
{
	 if (!IsDelayrender())
	 {
		 HGE *hge = hgeCreate(HGE_VERSION);
		 /*
		 if (number_of_item ==ITEM_DOWN)
		 {
			 GoWithAnAngle(hge->Timer_GetDelta(), GetRot() - 1.57);
			 if (GetX()<ACT_START_X || GetY()<ACT_START_Y || GetX()>ACT_END_X || GetY()>ACT_END_Y)
			 {
				 if (GetY() > ACT_START_Y)
					 SetRot(-GetRot());
				 else
					 SetRot(3.14 - GetRot());
				 if (GetY() > ACT_END_Y)
					 SetRot(3.14 + GetRot());
			 }
		 }
		 */
		 if (number_of_item==ITEM_RANDOM)
		 item_last_time--;
		 if (ballcrash(GetX(), GetY(), 40, Hero::GetHero().GetX(), Hero::GetHero().GetY()) && !used)
			 effect();
	 }
	 if (item_last_time == 0)Suicide();
	 if (itemHP <= 0)Suicide(true);
	 if (count == 0)
	 {
		 SetDelayrender(false);
		 count = -1;
	 }
}

Item::~Item()
{
}
void Item::effect()
{
	HGE *hge = hgeCreate(HGE_VERSION);
	item_last_time = 0;
	int type;
	switch (number_of_item)
	{
	case ITEM_UP:
		type = hge->Random_Int(0, 7);
		switch (type)
		{
		case 0:
			Hero::GetHero().SetMaxHP(Hero::GetHero().GetMaxHP() + 50 * (Hero::GetHero().GetBossKills() + 1));
			NoticeManager::GetNoticeManager().InsertNotice("Tank Expend", "");
			break;
		case 1:
			Hero::GetHero().SetExtraDMG(Hero::GetHero().GetExtraDMG()+5*(Hero::GetHero().GetBossKills()+1));
			NoticeManager::GetNoticeManager().InsertNotice("Power Upgrade", "");
			break;
		case 2:
			Hero::GetHero().SetDEF(Hero::GetHero().GetDEF()+5 * (Hero::GetHero().GetBossKills() + 1));
			NoticeManager::GetNoticeManager().InsertNotice("Armor Upgrade", "");
			break;
		case 3:
			Hero::GetHero().SetShootCD(Hero::GetHero().GetShootCD() - 2);
			NoticeManager::GetNoticeManager().InsertNotice("ShootCD Up", "ShootCD - 2");
			break;
		case 4:
			Hero::GetHero().SetCrit(Hero::GetHero().GetCrit() + 2);
			NoticeManager::GetNoticeManager().InsertNotice("CritChance Up", "Crit + 2%");
			break;
		case 5:
			Hero::GetHero().SetCritDMG(Hero::GetHero().GetCritDMG() + 0.1);
			NoticeManager::GetNoticeManager().InsertNotice("CritDMG Up", "CritDMG + 10%");
			break;
		case 6:
			Hero::GetHero().SetDodge(Hero::GetHero().GetDodge()+2);
			NoticeManager::GetNoticeManager().InsertNotice("Dodge Up", "Dodge + 2%");
			break;
		case 7:
			Hero::GetHero().SetMaxMP(Hero::GetHero().GetMaxMP() + 5);
			NoticeManager::GetNoticeManager().InsertNotice("ManaTank Expend", "MAX MP + 5");
			break;
		}
		break;
	case ITEM_DOWN:
		Hero::GetHero().SetMaxHP(Hero::GetHero().GetMaxHP() - 50 * (Hero::GetHero().GetBossKills() + 1));
		Hero::GetHero().SetMaxMP(Hero::GetHero().GetMaxMP() - 5);
		Hero::GetHero().SetDodge(Hero::GetHero().GetDodge() - 2);
		Hero::GetHero().SetCritDMG(2);
		Hero::GetHero().SetCrit(Hero::GetHero().GetCrit() - 2);
		Hero::GetHero().SetExtraDMG(Hero::GetHero().GetExtraDMG() - 5 * (Hero::GetHero().GetBossKills() + 1));
		Hero::GetHero().SetShootCD(Hero::GetHero().GetShootCD() + 2);
		Hero::GetHero().SetDEF(Hero::GetHero().GetDEF() - 5 * (Hero::GetHero().GetBossKills() + 1));
		NoticeManager::GetNoticeManager().InsertNotice("Down Down Down", "");
		break;
	case ITEM_RANDOM:
		Hero::GetHero().SetExtraDMG(Hero::GetHero().GetExtraDMG() / 2);
		type = hge->Random_Int(0, 21);
		switch (type)
		{
		case 0:
			Hero::GetHero().SetMaxHP(Hero::GetHero().GetMaxHP()*4);
			NoticeManager::GetNoticeManager().InsertNotice("Tanker", "MAX HP X4");
			break;
		case 1:
			Hero::GetHero().SetDMG(Hero::GetHero().GetDMG() * 2);
			NoticeManager::GetNoticeManager().InsertNotice("Super Powerful", "DMG *= 2");
			break;
		case 2:
			if (Hero::GetHero().AddBulletStats(DtoDMGdown))
			{
				NoticeManager::GetNoticeManager().InsertNotice("Warrior", "Distance DMG Down");
			}
			else
				NoticeManager::GetNoticeManager().InsertNotice("Empty", "No Effect");
			break;
		case 3:
			if (Hero::GetHero().AddBulletStats(Follow))
				NoticeManager::GetNoticeManager().InsertNotice("Stalker", "Magic Bullets");
			else
				NoticeManager::GetNoticeManager().InsertNotice("Empty", "No Effect");
			break;
		case 4:
			if (Hero::GetHero().AddBulletStats(DtoDMG))
				NoticeManager::GetNoticeManager().InsertNotice("SuperMan", "Distance Stronger Bullets");
			else
				NoticeManager::GetNoticeManager().InsertNotice("Empty", "No Effect");
			break;
		case 5:
			if (Hero::GetHero().AddBulletStats(DtoBOOM))
			{
				NoticeManager::GetNoticeManager().InsertNotice("BoooooM", "Bullets -> Bombs");
			}
			else
				NoticeManager::GetNoticeManager().InsertNotice("Empty", "No Effect");
			break;
		case 6:
			if (Hero::GetHero().AddBulletStats(Toxic))
				NoticeManager::GetNoticeManager().InsertNotice("Poison Creater", "Toxic Bullets");
			else
				NoticeManager::GetNoticeManager().InsertNotice("Empty", "No Effect");
			break;
		case 7:
			NoticeManager::GetNoticeManager().InsertNotice("Life Pack", "Extra Lives X3");
			Hero::GetHero().SetLives(Hero::GetHero().GetLives() + 3);
			break;
		case 8:
			if (Hero::GetHero().AddBulletStats(Frost))
			{
				NoticeManager::GetNoticeManager().InsertNotice("Glacier", "Ice Bullets");
			}
			else
				NoticeManager::GetNoticeManager().InsertNotice("Empty", "No Effect");
			break;
		case 9:
			if (Hero::GetHero().AddBulletStats(SuckBlood))
				NoticeManager::GetNoticeManager().InsertNotice("Blood++", "Bullets Get Blood");
			else
				NoticeManager::GetNoticeManager().InsertNotice("Empty", "No Effect");
			break;
		case 10:
			Hero::GetHero().SetDEF(Hero::GetHero().GetDEF() * 2);
			NoticeManager::GetNoticeManager().InsertNotice("Super Sturdy", "DEF *= 2");
			break;
		case 11:
			if (Hero::GetHero().AddBulletStats(Stun))
				NoticeManager::GetNoticeManager().InsertNotice("Stun!", "Bullets may stun enemy");
			else
				NoticeManager::GetNoticeManager().InsertNotice("Empty", "No Effect");
			break;
		case 12:
			Hero::GetHero().SetExtraDMG(0);
			NoticeManager::GetNoticeManager().InsertNotice("Eraser", "Clean ExtraDMG");
			break;
		case 13:
			Hero::GetHero().SetDMG(Hero::GetHero().GetDMG() / 2);
			NoticeManager::GetNoticeManager().InsertNotice("Disaster", "DMG /= 2");
			break;
		case 14:
			Hero::GetHero().SetDEF(Hero::GetHero().GetDEF() / 2);
			NoticeManager::GetNoticeManager().InsertNotice("Disaster", "DEF /= 2");
			break;
		case 15:
			Hero::GetHero().SetMaxHP(Hero::GetHero().GetMaxHP() / 2);
			NoticeManager::GetNoticeManager().InsertNotice("Disaster", "MAXHP /= 2");
			break;
		case 16:
			Hero::GetHero().SetCrit(Hero::GetHero().GetCrit() / 2);
			NoticeManager::GetNoticeManager().InsertNotice("Distaster", "Crit /= 2");
			break;
		case 17:
			Hero::GetHero().SetDodge(Hero::GetHero().GetDodge() / 2);
			NoticeManager::GetNoticeManager().InsertNotice("Distaster", "Dodge /= 2");
			break;
		case 18:
			Hero::GetHero().ResetBulletStats();
			NoticeManager::GetNoticeManager().InsertNotice("Eraser", "Clean Bullet Effect");
			break;
		case 19:
			NoticeManager::GetNoticeManager().InsertNotice("Empty", "No Effect");
			break;
		case 20:
			Hero::GetHero().BulletsAdjust(Hero::GetHero().GetBullets() + 1);
			NoticeManager::GetNoticeManager().InsertNotice("Bless", "Bullet++");
			break;
		case 21:
			Hero::GetHero().BulletsAdjust(1);
			NoticeManager::GetNoticeManager().InsertNotice("Curse", "Reset Bullet");
			break;
		}
		break;
	case ITEM_HP:
	    Hero::GetHero().varyHP(Hero::GetHero().GetMaxHP()*0.2);
	    NoticeManager::GetNoticeManager().InsertNotice("Heal", "Heal HP");
		  break;
	case ITEM_MP:
		Hero::GetHero().AddRecoverMP(100*(Hero::GetHero().GetBossKills()+1));
		NoticeManager::GetNoticeManager().InsertNotice("Heal", "Heal MP");
		break;
	case MAZEITEM_DEFUP:
		Hero::GetHero().SetDEF(Hero::GetHero().GetDEF() + 2*Hero::GetHero().GetLevel());
		break;
	case MAZEITEM_DMGUP:
		Hero::GetHero().SetDMG(Hero::GetHero().GetDMG()*1.2);
		break;
	case MAZEITEM_DTOBOOM:
		Hero::GetHero().AddBulletStats(DtoBOOM);
		break;
	case MAZEITEM_DTODMG:
		Hero::GetHero().AddBulletStats(DtoDMG);
		break;
	case MAZEITEM_DTODMGDOWN:
		Hero::GetHero().AddBulletStats(DtoDMGdown);
		break;
	case MAZEITEM_FOLLOWBULLET:
		Hero::GetHero().AddBulletStats(Follow);
		break;
	case MAZEITEM_HPUP:
		Hero::GetHero().SetMaxHP(Hero::GetHero().GetMaxHP()+20*Hero::GetHero().GetLevel());
		Hero::GetHero().varyHP(20 * Hero::GetHero().GetLevel());
		break;
	case MAZEITEM_MPUP:
		Hero::GetHero().SetMaxMP(Hero::GetHero().GetMaxMP() + 5 * Hero::GetHero().GetLevel());
		Hero::GetHero().varyMP(5 * Hero::GetHero().GetLevel());
		break;
	case MAZEITEM_ICEBULLET:
		Hero::GetHero().AddBulletStats(Frost);
		break;
	case MAZEITEM_STUNBULLET:
		Hero::GetHero().AddBulletStats(Stun);
		break;
	case MAZEITEM_SUCKBLOOD:
		Hero::GetHero().AddBulletStats(SuckBlood);
		break;
	case MAZEITEM_TOXICBULLET:
		Hero::GetHero().AddBulletStats(Toxic);
		break;
	}
	ObjectPool::GetObjPool().SetNeedCleanPadPool(true);
	Hero::GetHero().ItemAdd();
	used = true;
}