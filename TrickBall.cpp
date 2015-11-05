#include "TrickBall.h"


TrickBall::~TrickBall()
{
}


void TrickBall::action()
{
	BaseAction();
	if (innertype >= 1)
		SetNeedChangeColor();
	if (!(GetErrorState() & vertigo))
	{
		HGE *hge = hgeCreate(HGE_VERSION);
		switch (type)
		{
		case TRICKBALL:
			//pattern�ӵ� ��ת �������˺�����Ӣ�۵ȼ������ӵ�����Ƶ�����ӣ�
			//����1���ӵ��ж�
			//����2���ӵ�����
			//Զ��Ӣ��for3-5s ���޵�+��ת�ӵ���ѭ����
			if (!trigger)
			{
				SetSpeed(250 - GetDistance(&Hero::GetHero()));
				AwayFromOneObject(&Hero::GetHero(),hge->Timer_GetDelta());
				if (ReturnValid(bullettimer, 150, 250))
				{
					trigger = true;
					SetSpeed(50);
				}
			}
			else
			{
				SetReachable(false);
				if (bullets!=Hero::GetHero().GetLevel()*50)
				{
					if (bullets % 20 == 0)
					{
						for (int i = 0; i < 4; i++)
						{
							Bullet *bullet = new Bullet(this, BULLET4, 10);
							switch (innertype)
							{
							case 1:
								bullet->SetBulletStats(Toxic);
								break;
							case 2:
								bullet->SetBulletStats(Frost);
								break;
							}
							bullet->SetRot(GetRot() + 0.785 + 1.57*i);
							ObjectPool::GetObjPool().InsertObject(bullet, true);
						}
					}
					SetRot(GetRot() + (6.28 / ((float)Hero::GetHero().GetLevel() * 50)));
					bullets++;
				}
				else
				{
					SetReachable(true);
					trigger = false;
					bullets = 0;
				}
			}
			break;
		case BULLETBALL:




			break;
		}
	}
}