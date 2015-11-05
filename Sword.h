#pragma once
#include "FontPool.h"
#include "SKILL.h"
#include "GameObject.h"
#include "Hero.h"


class Sword :public SKILL
{
public:
	virtual void effect(GameObject *obj);
	virtual void Start();
	Sword() :SKILL(10, 0, 0, 0,0,true)
	{
		Box = new Point*[3];
		for (int i = 0; i < 4;i++)
		Box[i] = new Point[4];
		//first shock
		Box[0][0].MakePoint(0, 0);
		Box[0][1].MakePoint(117, 55, 39, 100);
		Box[0][2].MakePoint(204, 65, 39, 100);
		Box[0][3].MakePoint(338, 109, 39, 100);
		Box[0][4].MakePoint(396, 142, 39, 100);
        //second shock
		Box[1][0].MakePoint(0, 0);
		Box[1][1].MakePoint(61, 39, 7, 83);
		Box[1][2].MakePoint(157, 38, 7, 83);
		Box[1][3].MakePoint(152, 76, 7, 83);
		Box[1][4].MakePoint(119, 131, 7, 83);
		//third shock 
		Box[2][0].MakePoint(0, 0);
		Box[2][1].MakePoint(109, 0, 14, 61);
		Box[2][2].MakePoint(202, 23, 14, 61);
		Box[2][3].MakePoint(109, 103, 14, 61);
		Box[2][4].MakePoint(44, 112, 14, 61);
		//fourth shock
		Box[3][0].MakePoint(0, 0);
		Box[3][1].MakePoint(116, 19, 13, 81);
		Box[3][2].MakePoint(222, 57, 13, 81);
		Box[3][3].MakePoint(162, 124, 13, 81);
		Box[3][4].MakePoint(52, 130, 13, 81);

		for (int i = 0; i < 4;i++)
		for (int j = 0; j < 5; j++)
		{
			Box[i][j].CorrectPoint(91, 105);
		}

	};
	virtual int TellType(){ return ANIM; };
	virtual int GetAnimID(){ return SWORD; };
	void Move();

	~Sword();

	
};