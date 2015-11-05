#pragma once


#ifndef VITUALSKILL_H_
#define VITUALSKILL_H_
//技能+40=TOKEN_技能
#define TOKEN_FORCEFIELD 147
#define TOKEN_LENSBULLET 146
#define TOKEN_CHAIN 145
#define TOKEN_FIREBALL 144

#define FORCEFIELD 107
#define LENSBULLET 106
#define CHAIN 105
#define FIREBALL 104 


#define TOTEMEFFECT 103

#define TOKEN_BANNED 102
#define TOKEN_ACTIVE 101
#define CHAINPART 100
#define LENSPART 99

#define BULLET1 97
#define BULLET2 96
#define BULLET3 95
#define BULLET4 94
#define BULLET5 93
#define BOOM    92
#define DASH    91
#define FLASH   90
#define STEALMANAFIELD 89

//boss1：84 81
//boss2：82 80 
//boss3：79
//boss4：78 77
//boss5：75 74
//boss6：76 83



//啥都可以spawn（包括地雷）
#define SPAWNMINIONS 84
//全反击：
#define FULLCOUNTER 83
//弹弹乐：撞到墙反弹
#define BOUNCEANDBOUNCE 82
//五束高能子弹：
#define FIVEBULLETS 81
//一束反弹N次的子弹：
#define BOUNCEBULLETS 80
//地上留下印记 数秒后爆炸（无差别伤害）
#define BOOMRUNE 79
//正方形向外扩散
#define RECTBULLETS 78
//FANBULLETS升级版（速度变慢）
#define FANBULLETSEX 77
//包围主角向内收缩的子弹（速度极慢）
#define SHRINKBULLETS 76
//WIFI型子弹
#define WIFIBULLETS 75
//六条横着的向外旋转扩散的子弹
#define HEXABULLETS 74



#define SPAWNSHEILD 73//10
#define DODGEMAX 69


#define TOKEN_TOXICFIELD 52
#define TOKEN_FREEZEFIELD 51
#define TOKEN_HEALFIELD 50
#define TOXICFIELD 12 //2
#define FREEZEFIELD 11 //1
#define HEALFIELD 10 //3


#include "GameObject.h"






class VirtualSkill
{
public:
	VirtualSkill();
	~VirtualSkill();
	virtual void effect(GameObject *vir) = 0;
	virtual bool interaction(GameObject *vir) = 0;
	virtual void AfterOneEffect() = 0;
	virtual bool Start() = 0;
	virtual int GetAnimID() = 0;
	virtual bool EffectAll() = 0;

};


#endif