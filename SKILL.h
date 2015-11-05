#pragma once
#include "VirtualSkill.h"
#include "GameObject.h"
#ifndef SKILL_H_
#define SKILL_H_
class SKILL :public VirtualSkill,  public GameObject
{
public:
	SKILL(int ATK1, int CD1, int LAST_TIME1, int cost1,int range1=0,bool Isenemy=false) ;
	~SKILL();
	virtual bool NeedToDestroyWhenBossArrive(){ return false; };
	SKILL(const SKILL &skill) :GameObject(skill)
	{
		if (skill.GetPointNum() != 0)
		{
			Box = new Point[skill.GetPointNum()];
			for (int i = 0; i < skill.GetPointNum(); i++)
			{
				Box[i].x = skill.Box[i].x;
				Box[i].y = skill.Box[i].y;
			}
		}
		else
			Box = NULL;
	}
	void DetectHost()
	{
		if (host && !host->avlid)
			host = NULL;
	}
	int GetCD()const;
	int GetLAST_TIME()const;
	void SetLAST_TIME(int newlast_time){ LAST_TIME = newlast_time; };
	bool LAST_TIMER()
	{
		if (LAST_TIME)
		{
			LAST_TIME--;
			return true;
		}
		else
			return false;
	};
	int GetCost()const;
	void SetCD(int nowCD);
	void Setcost(int nowcost);
	void SetRange(int newRange){ range = newRange; };
	void RESET(bool bCD=true,bool bATK=true,bool bcost=true);
	void ReversePenetrate(){ IsPenetrate = !IsPenetrate; };
	bool GetIsPenetrate()const{ return IsPenetrate; };
	virtual int GetRange()const{ return range; };
	void SetPointNum(int num)
	{
		pointnum = num;
		Box = new Point[num];
	};
	void MakePoint(int pnum, int x, int y)
	{
		Box[pnum].MakePoint(x, y);
	};
	int GetPointNum()const{ return pointnum; };
	virtual int GetAnimID(){ return 0; };
	virtual void effect(GameObject *vir){};
	virtual bool interaction(GameObject *vir){ return true; };
	virtual void AfterOneEffect(){};
	virtual bool Start(){ return false; };
	virtual bool IsSkill(){ return true; };
	bool IsThisSkillEnemy()const{ return IsEnemy; };
	void ReverseSkill(){ IsEnemy = !IsEnemy; };
	Point* GetBoundingBox();
	void SetHost(GameObject *host1){ host = host1; };
	GameObject* GetHost()const{ return host; };
	virtual bool EffectAll(){ return false; };
	virtual bool RenderFont(){ return false; };
protected:
	Point *Box;
private:
	int CD;
	int LAST_TIME;
	int cost;
	int recost;
	int reCD;
	int reATK;
	int range;
	int pointnum;
	bool IsEnemy;
	bool IsPenetrate;
	GameObject *host;
};
#endif






