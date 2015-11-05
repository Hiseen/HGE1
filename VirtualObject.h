#pragma once
#include <cstring>
#include <math.h>
#include"hge.h"
#ifndef VIRTUALOBJECT_H_
#define VIRTUALOBJECT_H_

//1为独显版 2为集显版
#define VERSION 1


struct Point
{
	Point()
	{
		x = 0; y = 0;
	}
	float x, y;
	void  MakePoint(float x1, float y1, float dx = 0, float dy = 0)
	{
		x = x1 - dx;
		y = y1 - dy;
	}
	void CorrectPoint(float centerx,float centery)
	{
		x -= centerx;
		//y += centery;
	}
};

#ifndef SCREEN_WIDTH
#define SCREEN_WIDTH 853
#endif
#ifndef SCREEN_HEIGHT
#define SCREEN_HEIGHT 480
#endif

#define ACT_START_X 0 
#define ACT_START_Y 73
#define ACT_END_X 853
#define ACT_END_Y 457

enum GameMode
{
	Undecided,MazeMode, WaveMode, StageMode,MazeEvent
};


enum SceneNumber
{
	LoadingScene, MenuScene, GameScene, PauseScene, GameOverScene, HelpScene, LogoScene, MapScene, LevelUpScene
};

#define ITEMHPBEGIN -10
#define ITEMMPBEGIN -11
#define ITEMADDBEGIN -12
#define ITEMMINUSBEGIN -13
#define ITEMUNKNOWNBEGIN -14
#define ITEMHPDEAD -15
#define ITEMMPDEAD -16
#define ITEMADDDEAD -17
#define ITEMMINUSDEAD -18
#define ITEMUNKNOWNDEAD -19
#define ITEMHPBOOM -20
#define ITEMMPBOOM -21
#define ITEMADDBOOM -22
#define ITEMMINUSBOOM -23
#define ITEMUNKNOWNBOOM -24



#define HERO 1
#define OBJECT 3
#define ITEM 4
#define ANIM 5
#define HPBAR 6

//items[up]:
#define ITEM_UP 13
//items[down]:
#define ITEM_DOWN 14
//itemHPMP:
#define ITEM_HP 15
#define ITEM_MP 16
//items[random]
#define ITEM_RANDOM 17


#define MPBAR 27
#define EXPBAR 28
#define SCOREBAR 29
#define BOSSHPBAR 30
//BOSS
#define BOSS1 31
#define BOSS2 32
#define BOSS3 33
#define BOSS4 34
#define BOSS5 35
#define BOSS6 36
#define BOSS7 37



//Shield:
#define SHIELD1 50
#define SHIELD2 51

//itemenemy：
#define ITEMENEMY1 60
#define ITEMENEMY2 61
#define ITEMENEMY3 62
#define ITEMENEMY4 63

//隶属enemy：
#define ENEMY 70
#define SUPERSPEEDBALL 71
#define TRICKBALL 72
#define RINGBULLETBALL 73
#define STEALMANABALL 74


//BOSS用：
#define GUARDBALL 80


//各种变异 
#define RUNE_LANSER 120
#define RUNE_DRAGOON 121
#define RUNE_TANK 122
#define RUNE_BLACKHOLE 123
#define RUNE_BOOMER 124

#define RUNE_HEAL 125
#define RUNE_ATKUP 126
#define RUNE_CRITUP 127
#define RUNE_DODGEUP 128
#define RUNE_DEFUP 129

#define RUNE_SHOTGUN 130
#define RUNE_UZI 131
#define RUNE_MAGICIAN 132
#define RUNE_ROCKET 133


#define MAZEITEM_ICEBULLET 160
#define MAZEITEM_TOXICBULLET 161
#define MAZEITEM_DTODMG 162
#define MAZEITEM_DTODMGDOWN 163
#define MAZEITEM_STUNBULLET 164
#define MAZEITEM_SUCKBLOOD 165
#define MAZEITEM_FOLLOWBULLET 166
#define MAZEITEM_DTOBOOM 167

#define MAZEITEM_DMGUP 168
#define MAZEITEM_DEFUP 169
#define MAZEITEM_HPUP 170
#define MAZEITEM_MPUP 171
#define MAZEITEM_HEALMORE 172
//Button:
#define BUTTON_1_OFF   200
#define BUTTON_1_OVER  201
#define BUTTON_1_DOWN  202
#define BUTTON_2_OFF   203
#define BUTTON_2_OVER  204
#define BUTTON_2_DOWN  205
#define BUTTON_3_OFF   206
#define BUTTON_3_OVER  207
#define BUTTON_3_DOWN  208
#define BUTTON_4_OFF   209
#define BUTTON_4_OVER  210
#define BUTTON_4_DOWN  211
#define BUTTON_5_OFF   212
#define BUTTON_5_OVER  213
#define BUTTON_5_DOWN  214
#define BUTTON_6_OFF   215
#define BUTTON_6_OVER  216
#define BUTTON_6_DOWN  217
#define BUTTON_7_OFF   218
#define BUTTON_7_OVER  219
#define BUTTON_7_DOWN  220
#define BUTTON_8_OFF   221
#define BUTTON_8_OVER  222
#define BUTTON_8_DOWN  223
#define BUTTON_9_OFF   224
#define BUTTON_9_OVER  225
#define BUTTON_9_DOWN  226
#define BUTTON_10_OFF   227
#define BUTTON_10_OVER  228
#define BUTTON_10_DOWN  229
#define BUTTON_11_OFF   230
#define BUTTON_11_OVER  231
#define BUTTON_11_DOWN  232
#define BUTTON_12_OFF   233
#define BUTTON_12_OVER  234
#define BUTTON_12_DOWN  235
#define BUTTON_13_OFF   236
#define BUTTON_13_OVER  237
#define BUTTON_13_DOWN  238
#define BUTTON_14_OFF   239
#define BUTTON_14_OVER  240
#define BUTTON_14_DOWN  241
#define BUTTON_15_OFF   242
#define BUTTON_15_OVER  243
#define BUTTON_15_DOWN  244
#define BUTTON_16_OFF   245
#define BUTTON_16_OVER  246
#define BUTTON_16_DOWN  247
#define BUTTON_17_OFF   248
#define BUTTON_17_OVER  249
#define BUTTON_17_DOWN  250
#define BUTTON_18_OFF   251
#define BUTTON_18_OVER  252
#define BUTTON_18_DOWN  253
#define BUTTON_19_OFF   254
#define BUTTON_19_OVER  255
#define BUTTON_19_DOWN  256

//Title等杂项:
#define BLACKHOLEFORHERO 295
#define BLACKHOLEFORENEMY 296
#define BLACKHOLEARROW 297
#define BLACKHOLEFORMINE 298
#define BLACKHOLE 299
#define TITLE 300
#define MINE 301
#define BLOCK 302
#define EXPUNIT 303


#define VERTIGO 304
#define TOXIC 305
#define FREEZE 306
#define MPUNIT 307


#define MAZEPLUS 500 // HP HEAL
#define MAZEITEM 501
#define MAZESKILL 502
#define MAZEMPHEAL 503



#define MAZEMINUS 551//遭遇战、
#define MAZESPICK 552//地刺
#define MAZEMINES 553
#define MAZESMALLBOSS 554
#define MAZEBOSS 555






static bool ReturnValid(int &count, int max, int min)
{
	if (count <= 0)
	{
		HGE *hge = hgeCreate(HGE_VERSION);
		count = hge->Random_Int(max, min);
		return true;
	}
	else 
	{
		count--;
		return false;
	}
}

class VirtualObject
{
public:
	VirtualObject();
	~VirtualObject();
	virtual void action() = 0;
	virtual void born() = 0;
	virtual void death() = 0;
	virtual int TellType() = 0;
	virtual bool IsSkill() = 0;
	virtual bool IsBoss() = 0;
	virtual bool IsItem() = 0;
	virtual bool AfterBlur() = 0;
	virtual bool IsShield() = 0;
	virtual bool CanChangeGlobalPos() = 0;
	virtual int GetCollisionRange() = 0;
	virtual bool NeedChangeBlendMode() = 0;
	virtual bool IsEnemy() = 0;
	virtual bool NeedRenderFont() = 0;
	virtual bool IsMine() = 0;
	virtual void AfterPlayDeadAnim() = 0;
	virtual bool BulletBoomEffect() = 0;
	bool avlid;
	bool finishb, finishd;

};

#endif