#pragma once
#include "Hero.h"
#include <time.h>
#include <algorithm>
#include "ObjectPool.h"
#include <vector>
#include "ImageObject.h"
#include "AnimObject.h"
#include "hgeparticle.h"
class Hero;
#ifndef RENDERPOOL_H_
#define RENDERPOOL_H_

using namespace std;


struct ParticleWithHost
{
	hgeParticleSystem* sys;
	GameObject *obj;
};


struct AnimWithHost
{
	AnimObject* anim;
	GameObject* host;
};

struct box
{
	int imageID;
	int ObjectID;
};
struct pos
{
	float posx, posy;
	int ID;
	float angle;
	float size;
	float changedFPS;
	GameObject *host;
};

class RenderPool
{
public:
	friend class ObjectPool;
	~RenderPool();
	static RenderPool& GetRenderPool()
	{
		static RenderPool pool;
		pool.AssureInitialized();
		return pool;
	}
	void RenderAnimations();
	void InsertImageV2(const char* filename, float centerx, float centery, int IDforObj);
	void InsertAnimV2(const char* animname, int frames, int fps, float w, float h, float cx, float cy, int IDforObj, bool isinfinte=false,bool isdeadanim=false);
	void InsertParticleV2(const char* psi, const char *pic, int ID, bool  needchangeemission = false);
	void InsertMazePart(const char* filename, int partnum);
	void SpawnPS(GameObject *obj,int ID);
	void SetCombination(int IDforImage,int IDforObject,bool isanim=false,int IDforAnim=0);
	void SetCombinationV2(int IDforObject, int IDforObjectnew,bool isanim=false);
	void SetParticleCombV2(int IDforObject, int IDforObjectnew);
	void RenderEveryObject();
	void RenderParticles();
	void RenderMaze();
	void InsertImage(ImageObject *img);
	void InsertAnim(AnimObject *anim);
	void RenderAfterBlur();
	void RenderHero(Hero *hero);
	void InsertDeadAnim(AnimObject *anim);
	void RenderDead();
	void Shuffle();
	void RenderTokens();
	void RenderOnEveryObject();
	void InstantRender(float x, float y, int ID, float angle = 0.0f, float size = 1.0f, GameObject* host = nullptr, float changeFPS=0);
	void AssureInitialized()
	{
		while (!m_bHasInitialized)
			Sleep(0);
	};
	void ResetAll()
	{
		Instantpool.clear();
		//pool.clear();
		//anims.clear();
		//DeadAnimpool.clear();
		Instantpos.clear();
		//combs.clear();
		//animcombs.clear();
	};
	void Exit()
	{
		for (int i = 0; i < Instantpool.size(); i++)
			Instantpool[i].anim->ForceToDie();
		for (int i = 0; i < pool.size(); i++)
		{
			pool[i]->ForceToDie();
			delete pool[i];
			pool[i] = NULL;
		}
		for (int i = 0; i < mazepool.size(); i++)
		{
			mazepool[i]->ForceToDie();
			delete mazepool[i];
			mazepool[i] = NULL;
		}
		for (int i = 0; i < anims.size(); i++)
		{
			anims[i]->ForceToDie();
			delete anims[i];
			anims[i] = NULL;
		}
		for (int i = 0; i < DeadAnimpool.size(); i++)
		{
			DeadAnimpool[i]->ForceToDie();
			delete DeadAnimpool[i];
			DeadAnimpool[i] = NULL;
		}
		for (int i = 0; i < particlepool.size(); i++)
		{
			delete particlepool[i].sys;
			particlepool[i].obj = NULL;
		}
		particlepool.clear();
		Instantpool.clear();
		pool.clear();
		mazepool.clear();
		anims.clear();
		DeadAnimpool.clear();
		Instantpos.clear();
		combs.clear();
		animcombs.clear();
		mazecombs.clear();
		particlecombs.clear();
	}

private:
	RenderPool();
	RenderPool(const RenderPool &);
	RenderPool & operator=(const RenderPool &);
	vector<ImageObject*> pool;
	vector<ImageObject*> mazepool;
	vector<AnimObject*> anims;
	vector<AnimWithHost> Instantpool;
	vector<AnimObject*> DeadAnimpool;
	vector<pos> Instantpos;
	vector<box> combs;
	vector<box> animcombs;
	vector<box> particlecombs;
	vector<box> mazecombs;
	vector<ParticleWithHost> particlepool;
	vector<hgeParticleSystemInfo> particles;
	bool m_bHasInitialized;
	bool GetAfterBlur(VirtualObject *vir)
	{
		return vir->AfterBlur();
	}
	int GetType(VirtualObject *vir)
	{
		return vir->TellType();
	};
	int GetAnimID(GameObject *skill)
	{
		return skill->GetAnimID();
	}
	void RenderOne(GameObject *vit, ImageObject *img);
	void RenderOne(GameObject *vit, AnimObject *anim);
	void VarSizeAndRender(GameObject *vir, ImageObject* img)
	{
		if (vir->GetImgH() != 1 || vir->GetImgW() != 1)
			img->RenderBars(vir->GetX(),vir->GetY(),vir->GetImgH(), vir->GetImgW());
		else
			RenderOne(vir, img);
	}
	void RenderRune(GameObject *vir, ImageObject *img)
	{
		if (!vir->IsDelayrender())
		img->render(vir->GetX(), vir->GetY(), vir->GetImageAngle(), vir->GetSize());
	}

};

#endif