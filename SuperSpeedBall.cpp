#include "SuperSpeedBall.h"



SuperSpeedBall::~SuperSpeedBall()
{
}


void SuperSpeedBall::action()
{
	BaseAction();
	GetAngle(&Hero::GetHero());
	if (!(GetErrorState() & vertigo))
	{
		HGE *hge = hgeCreate(HGE_VERSION);
		if (GetDistance(&Hero::GetHero()) >= 400)
		{
			if (skillcount >= 0 && !IsRage)
			{
				if (innertype != 2)
				{
					if (skillcount % 50 == 0)
					{
						Bullet *bul = new Bullet(this, BULLET4, 10);
						bul->SetSpeed(150);
						ObjectPool::GetObjPool().InsertObject(bul, true);
					}
				}
				else
				{
					if (skillcount % 25 == 0)
					{
						Bullet *bul = new Bullet(this, BULLET4, 10);
						bul->SetSpeed(150);
						ObjectPool::GetObjPool().InsertObject(bul, true);
					}
				}
				skillcount--;
			}
			else
			{
				IsRage = true;
				skillcount = hge->Random_Int(50, 250);
			}
		}
			if (IsRage)
			{
				if (innertype != 1)
				{
					int x = GetX() + hge->Random_Int(-150, 150);
					int y = GetY() + hge->Random_Int(-150, 150);
					if (x <= ACT_START_X - 20)x = ACT_START_X - 20;
					if (x >= ACT_END_X - 20)x = ACT_END_X - 20;
					if (y <= ACT_START_Y - 20)y = ACT_START_Y - 20;
					if (y >= ACT_END_Y - 20)y = ACT_END_Y - 20;
					posobj = GameObject::MakePosObject(x, y);
				}
				else
				{
					int x = GetX() + hge->Random_Int(-300, 300);
					int y = GetY() + hge->Random_Int(-300, 300);
					if (x <= ACT_START_X - 20)x = ACT_START_X - 20;
					if (x >= ACT_END_X - 20)x = ACT_END_X - 20;
					if (y <= ACT_START_Y - 20)y = ACT_START_Y - 20;
					if (y >= ACT_END_Y - 20)y = ACT_END_Y - 20;
					posobj = GameObject::MakePosObject(x, y);
				}
				IsRage = false;
				SetReachable(false);
			}
			if (posobj)
			{
				if (!IsCoincide(posobj))
				{
					GoToAnotherObject(posobj, hge->Timer_GetDelta());
					SetSpeed(50 + (GetDistance(posobj) * 2));
				}
				else
				{
					SetReachable(true);
					posobj = NULL;
				}
			}
	}
}