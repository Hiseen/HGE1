#include "BlackHole.h"


BlackHole::BlackHole(float x, float y, int life1, int type,int direction) :GameObject(x, y, 0, (Hero::GetHero().GetBossKills() + 1) * 300 + SceneManager::GetSceneManager().GetWave() * 100, 0, 0)
{
	HGE *hge = hgeCreate(HGE_VERSION);
	life = life1;
	ready = 200;
	mytype = type;
	count = 0;
	SetSize(0);
	spawnmine = false;
	switch (mytype)
	{
	case BLACKHOLEFORENEMY:
	{spawndelay = 300;
	if (spawndelay > 300)
		spawndelay = 300;
	int max1 = Hero::GetHero().GetBossKills();
	if (max1 > 3)
		max1 = 3;
	spawntype = hge->Random_Int(ENEMY, ENEMY + max1);
	}
		break;
	case BLACKHOLEARROW:
		SetReachable(false);
		SetDelayrender(true);
		this->direction = direction;
		spawndelay = hge->Random_Int(100, 300);
		spawntype = 0;
	case BLACKHOLEFORHERO:
		SetReachable(false);
		this->direction = direction;
		eternal = true;
		if (GetDistance(&Hero::GetHero()) < 50)
		{
			canletheroin = false;
		}
		else
			canletheroin = true;
		ready = 0;
		break;
	case BLACKHOLEFORMINE:
		SetReachable(false);
		spawndelay = 1;
		this->direction = direction;
		eternal = true;
		life = 200;
		ready = 0;
		canletheroin = false;
		break;
	}
	SetCanHaveErrorState(false);
}

void BlackHole::action()
{
	if (ready)
	{
		ready--;
	}
	else
	{
		HGE *hge = hgeCreate(HGE_VERSION);
		switch (mytype)
		{
		case BLACKHOLEFORENEMY:
			if (life)
			{
				if (life % spawndelay == 0)
				{
					float x = -1, y = -1;
					switch (spawntype)
					{
					case SUPERSPEEDBALL:
						while (x > ACT_END_X || x<ACT_START_X || y>ACT_END_Y || y < ACT_START_Y)
						{
							count++;
							x = GetX() + 80 * (cos(0.628*count) - sin(0.628*count));
							y = GetY() + 80 * (cos(0.628*count) + sin(0.628*count));
						}
						break;
					default:
						x = GetX() + hge->Random_Int(-30, 30);
						y = GetY() + hge->Random_Int(-30, 30);
						break;
					}
					Enemy *enemy = new Enemy(x, y, spawntype);
					ObjectPool::GetObjPool().InsertObject(enemy, true);
				}
				life--;
			}
			else
				Suicide(true);
			if (count > 9)
				Suicide(true);
			break;
		case BLACKHOLEARROW:
			if (life)
			{
				life--;
				SetDelayrender(true);
				spawndelay--;
				if (!spawndelay)
				{
					Enemy *enemy = nullptr;
					int type = hge->Random_Int(ENEMY, TRICKBALL);
					if (type == TRICKBALL && hge->Random_Int(0, 100) < 90)
						type = SUPERSPEEDBALL;
					if (type == SUPERSPEEDBALL && hge->Random_Int(0, 100) < 50)
						type = ENEMY;
					int x = GetX(), y = GetY();
					switch (direction)
					{
					case DIRECTION_LEFT:
						enemy = new Enemy(ACT_END_X + 100, y + hge->Random_Int(-50, 50), type);
						enemy->SetRot(-M_PI / 2);
						break;
					case DIRECTION_RIGHT:
						enemy = new Enemy(ACT_START_X - 100, y + hge->Random_Int(-50, 50), type);
						enemy->SetRot(M_PI / 2);
						break;
					case DIRECTION_DOWN:
						enemy = new Enemy(x + hge->Random_Int(-50, 50), ACT_START_Y - 100, type);
						enemy->SetRot(M_PI);
						break;
					case DIRECTION_UP:
						enemy = new Enemy(x + hge->Random_Int(-50, 50), ACT_END_Y + 100, type);
						enemy->SetRot(0);
						break;
					default:
						break;
					}
					if (enemy)
						ObjectPool::GetObjPool().InsertObject(enemy, true);
					spawndelay = hge->Random_Int(200, 500);
				}
			}
			else
				Suicide(true);
			break;
		case BLACKHOLEFORHERO:
			if (canletheroin)
			{
				if (GetDistance(&Hero::GetHero()) < 40)
				{
					if (ready == 200)
					{
						ObjectPool::GetObjPool().SetMazeSave(true);
						if (direction != 0)
							ObjectPool::GetObjPool().SetEventDirection(direction);
						ready = 0;
					}
				}
			}
			else
			{
				if (GetDistance(&Hero::GetHero()) >= 50)
					canletheroin = true;
			}
			break;
		case BLACKHOLEFORMINE:
			if (life)
				life--;
			else
			if (canletheroin)
			{
				if (GetDistance(&Hero::GetHero()) < 40)
				{
					if (eternal)
					{
						ObjectPool::GetObjPool().SetMazeSave(true);
						if (direction != 0)
							ObjectPool::GetObjPool().SetEventDirection(direction);
						eternal = false;
					}
				}
			}
			else
			{
				if (GetDistance(&Hero::GetHero()) >= 50)
					canletheroin = true;
			}
			if (spawndelay)
				spawndelay--;
			else
			if (!spawnmine)
			{
				spawnmine = true;
				switch (direction)
				{
				case DIRECTION_DOWN:
					count = ACT_END_X - ACT_START_X;
					spawntype = hge->Random_Int(ACT_START_Y+50, ACT_END_Y-50);
					break;
				case DIRECTION_UP:
					count = ACT_END_X - ACT_START_X;
					spawntype = hge->Random_Int(ACT_START_Y+50, ACT_END_Y-50);
					break;
				case DIRECTION_LEFT:
					count = ACT_END_Y - ACT_START_Y;
					spawntype = hge->Random_Int(ACT_START_X+50, ACT_END_X-50);
					break;
				case DIRECTION_RIGHT:
					count = ACT_END_Y - ACT_START_Y;
					spawntype = hge->Random_Int(ACT_START_X+50, ACT_END_X-50);
					break;
				}
			}
			if (spawnmine)
			{
				Enemy *mine = new Enemy(0, 0, RINGBULLETBALL);
				switch (direction)
				{
				case DIRECTION_DOWN:
					mine->SetPos(ACT_START_X + count, spawntype);
					break;
				case DIRECTION_UP:
					mine->SetPos(ACT_END_X - count, spawntype);
					break;
				case DIRECTION_LEFT:
					mine->SetPos(spawntype, ACT_START_Y+count);
					break;
				case DIRECTION_RIGHT:
					mine->SetPos(spawntype, ACT_END_Y - count);
					break;
				}
				ObjectPool::GetObjPool().InsertObject(mine, true);
				if (count > 0)
					count -= 37;
				else
				{
					spawndelay = hge->Random_Int(100, 150);
					spawnmine = false;
				}
			}
			break;
		}
	}
}


void BlackHole::born()
{
	Swell2();
}
void BlackHole::death()
{
	Contract();

}
BlackHole::~BlackHole()
{
}
