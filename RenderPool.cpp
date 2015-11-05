#include "RenderPool.h"


RenderPool::RenderPool()
{
	m_bHasInitialized = true;
}


RenderPool::~RenderPool()
{
}
void RenderPool::SetCombination(int IDforImage, int IDforObject, bool isanim ,int IDforAnim)
{

	if (!isanim)
	{
		box box1;
		box1.imageID = IDforImage;
		box1.ObjectID = IDforObject;
		combs.push_back(box1);
		combs.shrink_to_fit();
	}
	else
	{
		box box1;
		box1.imageID = IDforImage;
		box1.ObjectID = IDforAnim;
		animcombs.push_back(box1);
		animcombs.shrink_to_fit();
	}
}

void RenderPool::RenderEveryObject()
{
	 
	for (int k = 0; k < ObjectPool::GetObjPool().pool.size();k++)
	{
		if (ObjectPool::GetObjPool().pool[k]->GetSceneNumber() == SceneManager::GetSceneManager().GetCurrentScene())
		{
			int nID = -1;
			if (!GetAfterBlur(ObjectPool::GetObjPool().pool[k]))
			{
				int num = GetType(ObjectPool::GetObjPool().pool[k]);
				//总感觉这个检测超级费速度
				if (num != ANIM)
				{
					for (int i = 0; i < combs.size(); i++)
					{
						if (combs[i].ObjectID == num)
						{
							nID = combs[i].imageID;
							break;
						}
					}
					for (int i = 0; i < pool.size(); i++)
					{
						if (pool[i]->GetID() == nID)
						{
							VarSizeAndRender(ObjectPool::GetObjPool().pool[k], pool[i]);
							break;
						}
					}
					if (ObjectPool::GetObjPool().pool[k]->GetInnerType() != 0)
					{
						nID = -1;
						num = ObjectPool::GetObjPool().pool[k]->GetInnerType();
						for (int i = 0; i < combs.size(); i++)
						{
							if (combs[i].ObjectID == num)
							{
								nID = combs[i].imageID;
								break;
							}
						}
						for (int i = 0; i < pool.size(); i++)
						{
							if (pool[i]->GetID() == nID)
							{
								RenderRune(ObjectPool::GetObjPool().pool[k], pool[i]);
								break;
							}
						}
					}
				}
			}
		}
	}

}

void RenderPool::RenderOne(GameObject *vir,ImageObject *img)
{
	if (vir->GetZ() != 0)img->SetZ(vir->GetZ());
	if (vir->IsNeedChangeColor())
    img->SetColor(vir->GetChangeColor(), vir->NeedChangeBlendMode());
	if (!vir->IsDelayrender())
	img->render(vir->GetX(), vir->GetY(), vir->GetImageAngle(), vir->GetSize());
	if (vir->IsNeedChangeColor())img->SetColor(ARGB(255, 255, 255, 255), vir->NeedChangeBlendMode());
	if (vir->GetZ() != 0)img->SetZ(0.5f);
}

void RenderPool::InsertImage(ImageObject* img)
{
	pool.push_back(img);
	pool.shrink_to_fit();
}

void RenderPool::RenderHero(Hero *hero)
{
	int nID=-1;
	for (int i = 0; i < animcombs.size(); i++)
	{
		if (animcombs[i].ObjectID == HERO)
		{
			nID = animcombs[i].imageID;
			break;
		}
	}
	for (int i = 0; i < anims.size(); i++)
	{
		if (anims[i]->GetID() == nID)
		{
			RenderOne(dynamic_cast<GameObject*>(hero), anims[i]);
			break;
		}
	}

}

void RenderPool::InsertAnim(AnimObject *anim)
{
	anims.push_back(anim);
	anims.shrink_to_fit();
}

void RenderPool::RenderAnimations()
{
	for (int i = 0; i < anims.size(); i++)
	{
		for (int j = 0; j < ObjectPool::GetObjPool().pool.size();j++)
		{
			int num2 = -1;
			int nID = -1;
			int num = GetType(ObjectPool::GetObjPool().pool[j]);
			if (num == ANIM)
				num2 = GetAnimID(ObjectPool::GetObjPool().pool[j]);

			for (int k = 0; k < animcombs.size(); k++)
			{
				if (animcombs[k].ObjectID == num2)
				{
					nID = animcombs[k].imageID;
					break;
				}
			}
			if (anims[i]->GetID() == nID)
				RenderOne(ObjectPool::GetObjPool().pool[j], anims[i]);
		}
		anims[i]->UpdateStep();
	}



	

}

void RenderPool::RenderOne(GameObject *vir, AnimObject *anim)
{
	if (vir->IsNeedChangeColor())anim->SetColor(vir->GetChangeColor());
	if (!vir->IsSoild())
	{
		if (!vir->IsDelayrender())
		anim->Render(vir->GetX(), vir->GetY(), vir->GetImageAngle(),vir->GetSize());
	}
	else
	{
		if (!vir->IsDelayrender())
			anim->Render(vir->GetX(), vir->GetY(), 0, vir->GetSize());
	}
	if (vir->IsNeedChangeColor())anim->SetColor(ARGB(255, 255, 255, 255));
}

void RenderPool::InstantRender(float x,float y,int ID,float angle,float size,GameObject *host,float changeFPS)
{
	pos p;
	p.posx = x;
	p.posy = y;
	p.ID = ID;
	p.angle = angle;
	p.size = size;
	p.host = host;
	p.changedFPS = changeFPS;
	Instantpos.push_back(p);
}

void RenderPool::RenderDead()
{
	for (int i = 0; i < Instantpos.size(); i++)
	for (int j = 0; j < DeadAnimpool.size(); j++)
	{
		if (Instantpos[i].ID == DeadAnimpool[j]->GetID())
		{
			AnimObject *anim1;
			if (Instantpos[i].changedFPS)
			    anim1 = new AnimObject(DeadAnimpool[j],Instantpos[i].changedFPS);
			else
				anim1 = new AnimObject(DeadAnimpool[j]);
			anim1->ChangePos(Instantpos[i].posx, Instantpos[i].posy);
			anim1->SetAngle(Instantpos[i].angle);
			anim1->SetSize(Instantpos[i].size);
			AnimWithHost ah;
			ah.anim = anim1;
			ah.host = Instantpos[i].host;
			Instantpool.push_back(ah);
		}
	}
	Instantpos.clear();
	for (int i = 0; i < Instantpool.size(); i++)
	{
		if (!Instantpool[i].anim->Isfinish())
		{
			if (Instantpool[i].host)
			{
				if (Instantpool[i].host->IsDead())
				{
					Instantpool[i].host = NULL;
					Instantpool[i].anim->ForceToDie();
				}
				else
				{
					Instantpool[i].anim->ChangePos(Instantpool[i].host->GetX(), Instantpool[i].host->GetY());
					Instantpool[i].anim->Render();
				}
			}
			else
				Instantpool[i].anim->Render();
		}
	}
	bool deleted = false;
	for (int i = 0; i < Instantpool.size();)
	{
		if (Instantpool[i].anim->Isfinish())
		{
			deleted = true;
			delete Instantpool[i].anim;
			Instantpool[i].anim = NULL;
			if (Instantpool[i].host)Instantpool[i].host->AfterPlayDeadAnim();
			Instantpool.erase(Instantpool.begin() + i);
		}
		else
			i++;
	}
	if (deleted)
		Instantpool.shrink_to_fit();
}




void RenderPool::InsertDeadAnim(AnimObject *anim)
{
	DeadAnimpool.push_back(anim);
	DeadAnimpool.shrink_to_fit();
}

void RenderPool::Shuffle()
{
	srand(time(NULL));
//	random_shuffle(anims.begin(), anims.end());
	random_shuffle(DeadAnimpool.begin(), DeadAnimpool.end());
}

void RenderPool::RenderAfterBlur()
{
	for (int k = 0; k < ObjectPool::GetObjPool().pool.size(); k++)
	{
		if (ObjectPool::GetObjPool().pool[k]->GetSceneNumber() == SceneManager::GetSceneManager().GetCurrentScene())
		{
			int nID = -1;
			if (GetAfterBlur(ObjectPool::GetObjPool().pool[k]))
			{
				int num = GetType(ObjectPool::GetObjPool().pool[k]);
				if (num != ANIM)
				{
					for (int i = 0; i < combs.size(); i++)
					{
						if (combs[i].ObjectID == num)
						{
							nID = combs[i].imageID;
							break;
						}
					}
					for (int i = 0; i < pool.size(); i++)
					{
						if (pool[i]->GetID() == nID)
						{
							VarSizeAndRender(ObjectPool::GetObjPool().pool[k], pool[i]);
							break;
						}
					}
				}
			}
		}
	}
}



void RenderPool::RenderTokens()
{
	
	for (int j = 0; j < 7; j++)
	{
		int nID = -1;
		SKILLToken* skill=ObjectPool::GetObjPool().GetTokens(j);
	 int num2 = GetAnimID(skill);
		for (int i = 0; i < animcombs.size(); i++)
		{   
			if (animcombs[i].ObjectID == num2)
			{
				nID = animcombs[i].imageID;
				break;
			}
		}
		for (int i = 0; i < anims.size(); i++)
		{
			if (anims[i]->GetID() == nID)
			{
				RenderOne(dynamic_cast<GameObject*>(skill), anims[i]);
				break;
			}
		}
	}
}

void RenderPool::RenderOnEveryObject()
{
	for (vector<GameObject*>::iterator it = ObjectPool::GetObjPool().pool.begin(); it != ObjectPool::GetObjPool().pool.end(); it++)
	{
		for (int i = 0; i < pool.size(); i++)
		{
			if (pool[i]->GetID() == 1000)
			{
				VarSizeAndRender(*it, pool[i]);

			}
		}
	}
}


void RenderPool::InsertImageV2(const char* filename, float centerx, float centery, int IDforObj)
{
	int ID = combs.size();
	ImageObject *img = new ImageObject(filename, centerx, centery, ID);
	SetCombination(ID, IDforObj);
	InsertImage(img);
}

void RenderPool::InsertAnimV2(const char* animname, int frames, int fps, float w, float h, float cx, float cy, int IDforObj, bool isinfinte, bool isdeadanim)
{	
	if (!isdeadanim)
	{
		int ID = animcombs.size();
		AnimObject *anim = new AnimObject(animname, frames, fps, w, h, cx, cy, ID, isinfinte);
		SetCombination(ID, ANIM, true, IDforObj);
		InsertAnim(anim);
	}
	else
	{
		AnimObject *anim = new AnimObject(animname, frames, fps, w, h, cx, cy, IDforObj, isinfinte);
		InsertDeadAnim(anim);
	}
}

void RenderPool::SetCombinationV2(int IDforObject, int IDforObjectnew,bool isanim)
{
	if (isanim)
	{
		int a = -100;
		for (int i = 0; i < animcombs.size(); i++)
		{
			if (animcombs[i].ObjectID == IDforObject)
			{
				a = animcombs[i].imageID;
				break;
			}
		}
		if (a != -100)
			SetCombination(a, ANIM, true, IDforObjectnew);
	}
	else
	{
		int a = -100;
		for (int i = 0; i < combs.size(); i++)
		{
			if (combs[i].ObjectID == IDforObject)
			{
				a = combs[i].imageID;
				break;
			}
		}
		if (a != -100)
			SetCombination(a,IDforObjectnew);
	}
}

void RenderPool::InsertParticleV2(const char *psi,const char *pic,int ID,bool needchangeemission)
{
	HGE *hge = hgeCreate(HGE_VERSION);
	HTEXTURE texture= hge->Texture_Load(pic);
	hgeSprite *pica = new hgeSprite(texture, 0, 0, hge->Texture_GetWidth(texture), hge->Texture_GetHeight(texture));
	pica->SetHotSpot(hge->Texture_GetWidth(texture) / 2, hge->Texture_GetHeight(texture) / 2);
		pica->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_ZWRITE);
	hgeParticleSystem *a = new hgeParticleSystem(psi, pica);
	hgeParticleSystemInfo info = a->info;
	if (needchangeemission)
	info.nEmission = 60;
	delete a;
	a = NULL;
	delete pica;
	pica=NULL;
	box b;
	b.imageID = particles.size();
	b.ObjectID = ID;
	particlecombs.push_back(b);
	particles.push_back(info);
	particlecombs.shrink_to_fit();
	particles.shrink_to_fit();
}

void RenderPool::SpawnPS(GameObject* obj,int ID)
{
	int imgID=-1;
	for (int i = 0; i < particlecombs.size();i++)
	if (particlecombs[i].ObjectID == ID)
	{
		imgID = particlecombs[i].imageID;
		break;
	}
	hgeParticleSystem *newsys = new hgeParticleSystem(&particles[imgID]);
	newsys->FireAt(obj->GetX(), obj->GetY());
	ParticleWithHost k;
	k.obj = obj;
	k.sys = newsys;
	particlepool.push_back(k);
}

void RenderPool::RenderParticles()
{
	HGE *hge = hgeCreate(HGE_VERSION);
	for (int i = 0; i < particlepool.size(); i++)
	{
		if (particlepool[i].obj)
		{
			if (!particlepool[i].obj->IsDead())
			{
				particlepool[i].sys->SetScale(particlepool[i].obj->GetSize());
				particlepool[i].sys->Update(hge->Timer_GetDelta());
				particlepool[i].sys->MoveTo(particlepool[i].obj->GetX(), particlepool[i].obj->GetY());
				particlepool[i].sys->Render();
			}
			else
			{
				particlepool[i].obj = NULL;
				particlepool[i].sys->Stop();
			}
		}
		else
		{
			particlepool[i].sys->Update(hge->Timer_GetDelta());
			particlepool[i].sys->Render();
		}
	}
	bool deleted = false;
	for (int i = 0; i < particlepool.size();i++)
	if (!particlepool[i].sys->GetParticlesAlive())
	{
		deleted = true;
		delete particlepool[i].sys;
		particlepool[i].sys = NULL;
		particlepool.erase(particlepool.begin() + i);
	}
	if (deleted)
		particlepool.shrink_to_fit();
}


void RenderPool::InsertMazePart(const char* filename,int partnum)
{
	HGE *hge = hgeCreate(HGE_VERSION);
	HTEXTURE h = hge->Texture_Load(filename);
	ImageObject *aimg = new ImageObject(h, MAZEPARTSIZE / 2, MAZEPARTSIZE/2, partnum);
	box aa;
	aa.imageID = mazecombs.size();
	aa.ObjectID = partnum;
	mazecombs.push_back(aa);
	mazepool.push_back(aimg);
}

void RenderPool::RenderMaze()
{
	int tarx = ObjectPool::GetObjPool().startx, tary = ObjectPool::GetObjPool().starty;
	for (int k = 0; k < mazepool.size(); k++)
	{
		for (int i = tarx - 8; i < tarx + 8; i++)
		for (int j = tary - 6; j < tary + 6; j++)
		if (ObjectPool::GetObjPool().map[i][j]==1)
		{
			int num = ObjectPool::GetObjPool().getw(i, j);
			if (mazepool[k]->GetID() == num)
				mazepool[k]->render((i - tarx) * MAZEPARTSIZE + SCREEN_WIDTH / 2 + ObjectPool::GetObjPool().movex, (j - tary) * MAZEPARTSIZE + SCREEN_HEIGHT / 2 + ObjectPool::GetObjPool().movey);
		}
		else if (ObjectPool::GetObjPool().map[i][j] > 1)
		{
			int num = ObjectPool::GetObjPool().map[i][j];
			if (mazepool[k]->GetID() == num)
				mazepool[k]->render((i - tarx) * MAZEPARTSIZE + SCREEN_WIDTH / 2 + ObjectPool::GetObjPool().movex, (j - tary) * MAZEPARTSIZE + SCREEN_HEIGHT / 2 + ObjectPool::GetObjPool().movey);
		}
		
	}
}