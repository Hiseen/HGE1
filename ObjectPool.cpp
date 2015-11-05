#include "ObjectPool.h"
#include "Mine.h"

ObjectPool::ObjectPool()
{
	focusskill = 0;
	startx = 0;
	starty = 0;
	dx = 0;
	dy = 0;
	movex = 0;
	movexx = 0;
	moveyy = 0;
	collisionwall = false;
	movey = 0;
	global = 0;
	count = 0;
	testcount = 0;
	countskilleffect = 25;
	direction = None;
	reverse = false;
	countfordie = 0;
	NeedCleanPadPool = false;
	m_bHasInitialized = true;
	//主角拥有7个技能：
}


ObjectPool::~ObjectPool()
{
}

void ObjectPool::CreatePool()
{
	pool.clear();
	/*
	void* data1=NULL;
	pool = (Node*)malloc(sizeof(Node));
	pool->data = (void*)malloc(sizeof(void*));
	pool->next = NULL;
	*/
}

bool ObjectPool::InsertObject(GameObject *vir,bool delay)
{
	/*
	Node *p;
	p = pool;
	while (!p->next)
		p = p->next;
	Node *newp;
	newp->data = data;
	p->next = newp;
	newp->next = NULL;
	*/
	if (!vir->IsDead() && vir->GetSceneNumber() == SceneManager::GetSceneManager().GetCurrentScene())
	{
		if (delay)
		{
			instantpool.push_back(vir);
		}
		else
		{
			pool.push_back(vir);
			swap(pool.begin(), pool.end());
		}
		return true;
	}

	if (!vir->IsSkill())
		delete vir;
	else
		delete dynamic_cast<SKILL*>(vir);
	vir = NULL;
	return false;
}

void ObjectPool::DelectObject()
{
	bool deleted = false;
	for (int i = 0; i < pool.size();i++)
	if (pool[i]->IsSkill())
		dynamic_cast<SKILL*>(pool[i])->DetectHost();
	for (int i = 0; i < pool.size(); i++)
	{
		if (IsDead(pool[i]))
		{
			if (!pool[i]->IsSkill())
				delete pool[i];
			else
				delete dynamic_cast<SKILL*>(pool[i]);
			pool[i] = NULL;
			deleted = true;
		}
	}
	for (int i = 0; i < pool.size();)
	{
		if (!pool[i])
			pool.erase(pool.begin() + i);
		else
			i++;
	}
	if (deleted)
	pool.shrink_to_fit();
}

void ObjectPool::UpdateObject()
{
	for (int i = 0; i < pool.size(); i++)
	{
		if (pool[i]->GetSceneNumber() == SceneManager::GetSceneManager().GetCurrentScene())
		{
			ObjectRun(pool[i]);			
			//物理碰撞↓
			if (!pool[i]->IsSkill() && pool[i]->IsReachable() && pool[i]->GetCollisionRange()!=0)
			for (int j = 0; j < pool.size(); j++)
			{
				if (!pool[j]->IsSkill() && pool[j]->IsReachable() && !pool[j]->IsSoild() && pool[j]->GetCollisionRange() != 0 && pool[i]->GetX() != pool[j]->GetX() && pool[i]->GetY() != pool[j]->GetY() && pool[i]->GetDistance(pool[j]) < pool[i]->GetCollisionRange())
				{
					double dis = pool[i]->GetDistance(pool[j]);
					double angle = pool[i]->GetAngle(pool[j], true);
					angle -= 3.14;
					pool[j]->SetPos(pool[j]->GetX() + (cos(angle) - sin(angle))*(pool[i]->GetCollisionRange() - dis), pool[j]->GetY()+(cos(angle) + sin(angle))*(pool[i]->GetCollisionRange() - dis));
				}
			}
			//物理碰撞↑
			/*
			碰撞核心：
			if (dy <= -5)
				ReverseDy();
			else
				dy = 5;
				*/

		}
	}
	for (int i = 0; i < instantpool.size(); i++)
	{
		InsertObject(instantpool[i]);
		instantpool.erase(instantpool.begin() + i);
	}
	//instantpool.clear();
}

void ObjectPool::SkillEffect()
{
	//效率很低。。
	for (int i = 0; i < pool.size();i++)
	{
		if (!pool[i]->IsDead() && pool[i]->IsSkill() && !pool[i]->IsItem())
		{
			SKILL *abc = dynamic_cast<SKILL*>(pool[i]);
			for (int j = 0; j < pool.size(); j++)
			{
				if (!pool[j]->IsDead() && !pool[j]->IsSkill() && pool[j]->IsShield())
					ShieldEffect(dynamic_cast<Shield*>(pool[j]), abc);
				if (pool[i]->IsDead())
					break;
			}
			if (!abc->EffectAll())
			{
				if (abc->IsThisSkillEnemy())
				{
					UseSkillForHero(&Hero::GetHero(), abc);
					for (int j = 0; j < pool.size(); j++)
					if (pool[j]->IsMine() && !abc->GetIsPenetrate() && ballcrash(pool[j]->GetX(), pool[j]->GetY(), pool[j]->GetCollisionRange(), abc->GetX(), abc->GetY()) && abc->interaction(pool[j]))
					{
						abc->effect(pool[j]);
						pool[j]->Underattack(pool[i]);
					}
				}
				else
				{
					for (int j = 0; j < pool.size(); j++)
					{
						if (!pool[j]->IsSkill() && !pool[j]->IsDead() && !pool[j]->IsShield())
						{
							if (!pool[j]->IsMine())
								UseSkill(pool[j], abc);
							else
							{
								if (!abc->IsDead() && !abc->GetIsPenetrate() && ballcrash(pool[j]->GetX(), pool[j]->GetY(), pool[j]->GetCollisionRange(), abc->GetX(), abc->GetY()) && abc->interaction(pool[j]))
								{
									abc->effect(pool[j]);
									pool[j]->Underattack(pool[i]);
								}
							}
						}
						else
						{
							if (pool[j]->IsItem() && !abc->IsDead() && abc->IsBullet() && ballcrash(pool[j]->GetX(), pool[j]->GetY(), 50, abc->GetX(), abc->GetY()))
							{
								abc->effect(pool[j]);
								pool[j]->Underattack(pool[i]);
							}
						}
						if (pool[i]->IsDead())
							break;
					}
				}
				abc->AfterOneEffect();
			}
			else
			{
				UseSkillForHero(&Hero::GetHero(), abc);
				for (int j = 0; j < pool.size(); j++)
				{
					if (!pool[j]->IsSkill() && !pool[j]->IsDead() && !pool[j]->IsShield())
					{
						if (!pool[j]->IsMine())
							UseSkill(pool[j], abc);
						else
						{
							if (!abc->IsDead() && !abc->GetIsPenetrate() && ballcrash(pool[j]->GetX(), pool[j]->GetY(), pool[j]->GetCollisionRange(), abc->GetX(), abc->GetY()) && abc->interaction(pool[j]))
							{
								abc->effect(pool[j]);
								pool[j]->Underattack(pool[i]);
							}
						}
					}
					else
					{
						if (pool[j]->IsItem() && !abc->IsDead() && abc->IsBullet() && abc->GetRange() > 1 && ballcrash(pool[j]->GetX(), pool[j]->GetY(), 50, abc->GetX(), abc->GetY()))
						{

							abc->effect(pool[j]);
							pool[j]->Underattack(pool[i]);
						}
					}
					if (pool[i]->IsDead())
						break;
				}
			abc->AfterOneEffect();
			}
		}
	}
}

void ObjectPool::SuicideOneType(int Type)
{
	for (int i = 0; i < pool.size(); i++)
	if (pool[i]->TellType() == Type)
		pool[i]->Suicide(true);
}


bool ObjectPool::BossArrive()
{
	if (countfordie == 0)
	{
		for (int i = 0; i < pool.size(); i++)
		if (pool[i]->NeedToDestroyWhenBossArrive())
		{
			pool[i]->Suicide(true);
			countfordie = 2;
			return false;
		}
		return true;
	}
	else
	{
		countfordie--;
		return false;
	}
}


void ObjectPool::InitTokens()
{
	for (int i = 0; i < COUNT_HEROSKILL; i++)
	{
		SKILLToken *a = new SKILLToken();
		heroskills.push_back(a);
	}
	for (int i = 0; i < COUNT_HEROSKILL; i++)
		heroskills[i]->SetPos(485 + 51 * i, 15);
}

void ObjectPool::GetSkillToken(int skill)
{
	    HGE *hge = hgeCreate(HGE_VERSION);
		for (int i = 6; i > 0; i--)
		{
			if (heroskills[i]->GetState() != BANNED && heroskills[i - 1]->GetState() != BANNED)
				heroskills[i]->LockWithSkill(heroskills[i - 1]->GetSkillType());
		}
			if (heroskills[0]->GetState() != BANNED)
				heroskills[0]->LockWithSkill(skill);
}

void ObjectPool::LockedAllToken()
{
	for (int i = 0; i < 7; i++)
		heroskills[i]->Ban();
}
void ObjectPool::UnLockToken()
{
	for (int i = 0; i < COUNT_HEROSKILL; i++)
		if (heroskills[i]->GetState() == BANNED)
		{
			heroskills[i]->StartUp();
			break;
		}
}

void ObjectPool::ReviveTokens()
{
	for (int i = 0; i < 7; i++)
		heroskills[i]->Re_Active();
}

SKILLToken *ObjectPool::GetTokens(int tokennum)
{
			return heroskills[tokennum];
}


void ObjectPool::UseSkillToken()
{
	if (heroskills[focusskill]->GetState() == LOCKED)
	{
		if(heroskills[focusskill]->Start())
		{
			//for (int i = focusskill; i < 5; i++)
			//{
			//	if (heroskills[i]->GetState() != BANNED && heroskills[i + 1]->GetState() != BANNED)
			//	{
			//		heroskills[i]->LockWithSkill(heroskills[i + 1]->GetSkillType());
			//		heroskills[i + 1]->Release();
			//	}
			//}
		}
	}
}


void ObjectPool::NextToken()
{
	int finalone = 0;
	for (int i = 6; i >= 0; i--)
	{
		if (heroskills[i]->GetState() != BANNED)
		{
			finalone = i;
			break;
		}
	}
	focusskill++;
	if (focusskill > finalone)
		focusskill = finalone;
}

void ObjectPool::LastToken()
{
	focusskill--;
	if (focusskill < 0)
		focusskill = 0;
}


int ObjectPool::GetSelectedTokenX()
{
	if (heroskills[0]->GetState() != BANNED)
		return heroskills[focusskill]->GetX();
	else
		return -100;
}

int ObjectPool::GetSelectedTokenY()
{
	if (heroskills[0]->GetState() != BANNED)
		return heroskills[focusskill]->GetY();
	else
		return -100;
}

int ObjectPool::TestToken()
{
	return heroskills[focusskill]->GetAnimID();
}



void ObjectPool::SetBWbools(const char* filename, int NumOfObject)
{
	HGE *hge = hgeCreate(HGE_VERSION);
	HTEXTURE pic = hge->Texture_Load(filename);
	int width = hge->Texture_GetWidth(pic);
	int height = hge->Texture_GetHeight(pic);
	BWpic bwpic;
	bwpic.width = width;
	bwpic.height = height;
	bwpic.bools = (bool**)malloc(sizeof(bool*)*width);
	for (int i = 0; i < width; i++)
		bwpic.bools[i] = (bool*)malloc(sizeof(bool)*height);
	DWORD *color = hge->Texture_Lock(pic);
	for (int i = 0; i < width; i++)
	for (int j = 0; j < height;j++)	
	if (GETR(color[j*width + i]) == 0 && GETG(color[j*width + i]) == 0 && GETB(color[j*width + i]) == 0)
		bwpic.bools[j][i] = true;
	else bwpic.bools[j][i] = false;
	bwpic.number = NumOfObject;
	hge->Texture_Unlock(pic);
	hge->Texture_Free(pic);
	BWpics.push_back(bwpic);
	BWpics.shrink_to_fit();
}


bool ObjectPool::ChangeAllPos(Direction d, int delta)
{
	direction = d;
	global = ((float)delta / 1600);
	if (count == 40)
		reverse = true;
	if (reverse)
		count--;

	if (!reverse)
	if (count!=40)
		count++;

		switch (d)
		{
		case Left:
			for (int i = 0; i < pool.size(); i++)
			if (pool[i]->CanChangeGlobalPos())
				pool[i]->SetPos(pool[i]->GetX() - global*count, pool[i]->GetY());
			testcount++;
			break;
		case Up:
			for (int i = 0; i < pool.size(); i++)
			if (pool[i]->CanChangeGlobalPos())pool[i]->SetPos(pool[i]->GetX(), pool[i]->GetY() - global*count);
			break;
		case Down:
			for (int i = 0; i < pool.size(); i++)
			if (pool[i]->CanChangeGlobalPos())pool[i]->SetPos(pool[i]->GetX(), pool[i]->GetY() + global*count);
			break;
		case Right:
			for (int i = 0; i < pool.size(); i++)
			if (pool[i]->CanChangeGlobalPos())pool[i]->SetPos(pool[i]->GetX() + global*count, pool[i]->GetY());
			break;
		}
		if (reverse && count == 0)
		{
			reverse = false;
			global = 0;
			direction = None;
			return true;
		}
		else
			return false;
}

void ObjectPool::ExceptClean()
{
	for (int i = 0; i < pool.size(); i++)
	if (pool[i]->GetSceneNumber() != SceneManager::GetSceneManager().GetCurrentScene())
		pool[i]->Suicide();
}


void ObjectPool::ResetTokens()
{
	for (int i = 0; i < COUNT_HEROSKILL; i++)
		heroskills[i]->Reset();
}



int ObjectPool::getw(int x, int y)
{
	//0 空
	//1 4 5  ┃
	//2 8 10 ━
	//3 ┗
	//6 ┏
	//7 ┣
	//9 ┛
	//11 ┻
	//12 ┓
	//13 ┫
	//14 ┳
	//15 ╋
	return (!map[x][y - 1] ? 0 : 1) | (!map[x + 1][y] ? 0 : 2) | (!map[x][y + 1] ? 0 : 4) | (!map[x - 1][y] ? 0 : 8);
}
int ObjectPool::search(int x, int y)
{
	static int d[4][2] = { 0, 1, 1, 0, 0, -1, -1, 0 };
	int zx = x * 2, zy = y * 2, next, turn, i;
	map[zx][zy] = 1;
	turn = rand() % 2 ? 1 : 3;
	for (i = 0, next = rand() % 4; i<4; i++, next = (next + turn) % 4)
	if (map[zx + 2 * d[next][0]][zy + 2 * d[next][1]] == 0)
	{
		map[zx + d[next][0]][zy + d[next][1]] = 1;
		search(x + d[next][0], y + d[next][1]);
	}
	return 0;
}
void ObjectPool::Make_Maze(int x, int y)
{
	int z1, z2;
	for (z1 = 0, z2 = 2 * y + 2; z1 <= 2 * x + 2; z1++)map[z1][0] = 1, map[z1][z2] = 1;
	for (z1 = 0, z2 = 2 * x + 2; z1 <= 2 * y + 2; z1++)map[0][z1] = 1, map[z2][z1] = 1;
	srand((unsigned)time(NULL));
	search(x, y);
}


void ObjectPool::CreateMaze()
{
	HGE *hge = hgeCreate(HGE_VERSION);
	int x = 30, y = 30, z1, z2; //x和y的值指定了这个要生成的迷宫的大小
	for (int i = 0; i < 502; i++)
	for (int j = 0; j < 502; j++)
		map[i][j] = 0;
	Make_Maze(x, y);
	startx = x;
	starty = y;
	movexx = 0;
	moveyy = 0;
	movex = 0;
	movey = 0;
	map[1][2] = 1;
	map[2 * x + 1][2 * y] = 1;
	map[1][2 * y] = 1;
	map[2 * x + 1][2] = 1;
	//map[x + 1][y + 1] = 1;
	//map[x - 1][y - 1] = 1;
	//map[x - 1][y + 1] = 1;
	//map[x + 1][y - 1] = 1;
	//map[x - 1][y] = 1;
	//map[x + 1][y] = 1;
	//map[x][y + 1] = 1;
	//map[x][y - 1] = 1;
	for (z2 = 1; z2 <= y * 2 + 1; z2++)
	{
		for (z1 = 1; z1 <= x * 2 + 1; z1++)
		{
			if (map[z1][z2])
			{
				int counta = 0;
				int countb = 0;
				int count = 0;
				if (!map[z1 + 1][z2])
					counta++;
				else
				if (map[z1 + 1][z2] != 1)
					count++;
				if (!map[z1 - 1][z2])
					counta++;
				else
				if (map[z1 - 1][z2] != 1)
					count++;
				if (!map[z1][z2 + 1])
					countb++;
				else
				if (map[z1][z2 + 1]!=1)
					count++;
				if (!map[z1][z2 - 1])
					countb++;
				else
				if (map[z1][z2 - 1] != 1)
					count++;
				if ((counta==2 &&countb==1)||(counta==1 &&countb==2))
					map[z1][z2] = hge->Random_Int(MAZEPLUS,MAZEMPHEAL);
				//下面的语句可以将封口路变成MAZEMINUS(MAZEPLUS->MAZEMINUS)
				if (!count && ((counta == 2&&countb==0)||( countb==2 && counta==0)) && hge->Random_Int(0,100)<20)
				{
				    map[z1][z2] = hge->Random_Int(MAZEMINUS,MAZEMINES);
				}
			}
		}
	}
	for (z2 = 1; z2 <= y * 2 + 1; z2++)
	{
		for (z1 = 1; z1 <= x * 2 + 1; z1++)
		{
			if (abs(z1-x)>x/2 && abs(z2-y)>x/2 && map[z1][z2])
			{
				int counta = 0;
				int countb = 0;
				int count = 0;
				if (!map[z1 + 1][z2])
					counta++;
				else
				if (map[z1 + 1][z2] != 1)
					count++;
				if (!map[z1 - 1][z2])
					counta++;
				else
				if (map[z1 - 1][z2] != 1)
					count++;
				if (!map[z1][z2 + 1])
					countb++;
				else
				if (map[z1][z2 + 1] != 1)
					count++;
				if (!map[z1][z2 - 1])
					countb++;
				else
				if (map[z1][z2 - 1] != 1)
					count++;
				if (!count && ((counta == 2 && countb == 0) || (countb == 2 && counta == 0)) && hge->Random_Int(0, 100)<5)
				{
					map[z1][z2] = MAZESMALLBOSS;
				}
			}
		}
	}
	map[x][y] = 1;
	/*
	for (z2 = 1; z2 <= y * 2 + 1; z2++)
	{
		for (z1 = 1; z1 <= x * 2 + 1; z1++)
		{
			if (!map[z1][z2])
			{
				float x1 = (z1 - x) * MAZEPARTSIZE + SCREEN_WIDTH / 2;
				float y1 = (z2 - y) * MAZEPARTSIZE + SCREEN_HEIGHT / 2;
				Mine *wall = new Mine(x1, y1);
				wall->SetData(getw(z1, z2));
				InsertObject(wall);
			}
		}
	}
	*/
}

bool ObjectPool::MazeMove()
{
	if (dx > 0 && map[startx - 1][starty] || (dx<0 && map[startx + 1][starty]))
		movex += dx;
	else
		dx = 0;
	if ((dy>0 && map[startx][starty - 1]) || (dy < 0 && map[startx][starty + 1]))
		movey += dy;
	else
		dy = 0;
	if (movex == MAZEPARTSIZE)
	{
		movex = 0;
		dx = 0;
		startx--;
	}
	if (movex == -MAZEPARTSIZE)
	{
		movex = 0;
		dx = 0;
		startx++;
	}
	if (movey == MAZEPARTSIZE)
	{
		movey -= MAZEPARTSIZE;
		dy = 0;
		starty--;
	}
	if (movey == -MAZEPARTSIZE)
	{
		movey += MAZEPARTSIZE;
		dy = 0;
		starty++;
	}
	/*
	dx *= 0.82;
	dy *= 0.82;
	if (abs(dx) < 0.05)
		dx = 0;
	if (abs(dy) < 0.05)
		dy = 0;
		*/
	if (dx || dy)
		return false;
	else
		return true;
}

void ObjectPool::MazeEvent(int &x,int &y,int &eventname)
{
	for (int i = -2; i <= 2; i++)
	for (int j = -2; j <= 2; j++)
	{
		if (MazeCollision(startx+i, starty+j, eventname, EventDirection))
		{
			x = startx+i;
			y = starty+j;
			return;
		}
	}
}





bool ObjectPool::MazeCollision(int x, int y,int &eventname,int &direction)
{
	float hx = Hero::GetHero().GetX();
	float hy = Hero::GetHero().GetY();
	float x1 = (x - startx) * MAZEPARTSIZE + SCREEN_WIDTH / 2 + movex;
	float y1 = (y - starty) * MAZEPARTSIZE + SCREEN_HEIGHT / 2 + movey;
	if (!map[x][y])
	{
		if (abs(hy - y1) < 37 && abs(hx - x1) < 37)
		{
			GameObject *a = GameObject::MakePosObject(x1, y1);
			float angle = a->GetAngle(&Hero::GetHero(), true);
			if (angle>=-0.785 && angle < 0.785)
			{
				//if (dy <= -5)
					//ReverseDy();
				//else
					//dy = 5;

			}
			if (angle >= 0.785 && angle < 2.355)
			{

				if (dx >= 5)
					ReverseDx();
				else
					dx = -5;
			}
			if (angle >= 2.355 && angle < 3.925)
			{

				if (dy >= 5)
					ReverseDy();
				else
					dy = -5;
			}
			if (angle >= 3.925 || angle < -0.785)
			{
				if (dx <= -5)
					ReverseDx();
				else
					dx = 5;
			}
			delete a;
			a = NULL;
		}
	}
	else if (map[x][y]!=1)
	{
		if (abs(hy - y1) < 37 && abs(hx - x1) < 37)
		{
			GameObject *a = GameObject::MakePosObject(x1, y1);
			float angle =a->GetAngle(&Hero::GetHero(), true);
			delete a;
			a = NULL;
			if (angle >= -0.785 && angle < 0.785)
				direction = DIRECTION_UP;
			if (angle >= 0.785 && angle < 2.355)
				direction = DIRECTION_RIGHT;
			if (angle >= 2.355 && angle < 3.925)
				direction = DIRECTION_DOWN;
			if (angle >= 3.925 || angle < -0.785)
				direction = DIRECTION_LEFT;
			HGE *hge = hgeCreate(HGE_VERSION);
			switch (map[x][y])
			{
			case MAZEPLUS:
				Hero::GetHero().varyHP(Hero::GetHero().GetMaxHP());
				break;
			case MAZEMINUS:
			    eventname = 2;
				break;
			case MAZESPICK:
				if (!Hero::GetHero().IsGod())
					Hero::GetHero().varyHP(-hge->Random_Int(10, 20));
				break;
			case MAZEITEM:
				eventname = 3;
				break;
			case MAZESKILL:
				eventname = 4;
				break;
			case MAZEMPHEAL:
				Hero::GetHero().varyMP(Hero::GetHero().GetMaxMP());
				break;
			case MAZEMINES:
				eventname = 5;
				break;
			case MAZESMALLBOSS:
				eventname = 6;
				break;
			case MAZEBOSS:
				eventname = 2;
				break;
			}
			return true;
		}
	}
	return false;
}


void ObjectPool::SaveMazePart(int x, int y, int eventname)
{
	map[x][y] = eventname;
	MazeSave = false;
}