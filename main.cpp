

#ifndef _DLL
#define _DLL
#endif
#include "DMGFont.h"
#include"atlimage.h"
#include<math.h>
#include<MMSystem.h>
#include<vector>
#include<malloc.h>
#include <dshow.h>
#include <fstream> 
#include <map>





#pragma comment(lib,"strmiids.lib")
#pragma comment(lib,"quartz.lib")
using namespace std;
//需要include的
#include <vld.h>
#include "Hero.h"
#include "hgeSprite.h" 
#include "hgefont.h"
#include "hgeparticle.h"
#include "hgerect.h"
#include "hgeanim.h"
#include "hgegui.h"
#include "hgeguictrls.h"
#include "main.h"
#include "ImageObject.h"
#include "Enemy.h"
#include "ObjectPool.h"
#include "SKILL.h"
#include "RenderPool.h"
#include "BOSS.h"
#include "Item.h"
#include "Bar.h"
#include "Range.h"
#include "Skills.h"
#include "Button.h"
#include "FloatingObject.h"
#include "SFXManager.h"
#include "ZoomObject.h"
#include "Mine.h"
#include "ItemEnemy.h"
#include "BuffManager.h"
#include "BlackHole.h"
//预编译常量


#define TERRAINNUM 0
#define BULIDINGSTOP 10
#define MAX_ENEMY 200
#define BLOCKDISTANCE 150
//LIB:
#pragma comment(lib,"hge.lib")
#pragma comment(lib,"hgehelp.lib")
#pragma comment(lib,"winmm.lib")
//#pragma comment(linker, "/NODEFAULTLIB:libcmt.lib")



bool GenerateEnemy(int mode, int num, int &i,int vary1)
{
	HGE *hge = hgeCreate(HGE_VERSION);
	//一线式+圆圈式+矩形式//+散点式
	int var = Hero::GetHero().GetLevel() / 4;
	Point pos;
	Point pos2;
	static Point *pos3=new Point[3]();
	static int numofhole=0;
	if (!numofhole)
	{
		numofhole = hge->Random_Int(1, 3);
		switch (numofhole)
		{
		case 3:
			pos3[2].x = 1;
			pos3[2].y = 1;
		case 2:
			pos3[1].x = 1;
			pos3[1].y = 1;
		case 1:
			pos3[0].x = 1;
			pos3[0].y = 1;
			break;
		}
	}




	//等级越高低频率的怪出现的几率就越大
	float x, y, z=0;
	int zx;
	int r = 50;
	//0单位的地形
	//地形之后1s出现
	if (i >TERRAINNUM+BULIDINGSTOP && i% 10 == 0)
	{
		/*
	switch (hge->Random_Int(0, 3))
	{
	case 0:
		pos2.x = hge->Random_Int(ACT_START_X, ACT_END_X);
		pos2.y = hge->Random_Int(-200, 0);
		break;
	case 1:
		pos2.x = hge->Random_Int(ACT_START_X, ACT_END_X);
		pos2.y = hge->Random_Int(457, 657);
		break;
	case 2:
		pos2.x = hge->Random_Int(-200, 0);
		pos2.y = hge->Random_Int(ACT_START_Y, ACT_END_Y);
		break;
	case 3:
		pos2.x = hge->Random_Int(853, 1053);
		pos2.y = hge->Random_Int(ACT_START_Y, ACT_END_Y);
		break;
	default:
		break;
	}
	*/
		Enemy *e = NULL;
		int type = 0;
		int abcdf = Hero::GetHero().GetLevel() / 10;
		switch (abcdf)
		{
		case 0:
			abcdf = ENEMY;
			break;
		case 1:
			abcdf = SUPERSPEEDBALL;
			break;
		case 2:
			abcdf = TRICKBALL;
			break;
		case 3:
			abcdf = RINGBULLETBALL;
			break;
		default:
			abcdf = RINGBULLETBALL;
			break;
		}

		int locat = hge->Random_Int(var/5, var);
		switch (locat)
		{
		case 0:
			type = ENEMY;
			break;
		case 1:
			if (hge->Random_Int(0, 100) <= 90)
			{
				type = SUPERSPEEDBALL;
			}
			else
				type = STEALMANABALL;
			break;
		case 2:
			if (hge->Random_Int(0, 100) <= 90)
			{
				type = TRICKBALL;
			}
			else
				type = STEALMANABALL;
			break;
		case 3:
			if (hge->Random_Int(0, 100) <= 90)
			{
				type = RINGBULLETBALL;
			}
			else
				type = STEALMANABALL;
			break;
		default:
			type = abcdf;
			break;
		}

		if (type != 0)
		{
			e = new Enemy(pos3[hge->Random_Int(0, numofhole - 1)].x+hge->Random_Int(-30,30), pos3[hge->Random_Int(0, numofhole - 1)].y+hge->Random_Int(-30,30), type);
			ObjectPool::GetObjPool().InsertObject(e);
			e = NULL;
		}
	}
			i++;
			if (i == num)
			{
				for (int j = 0; j < 3; j++)
				{
					pos3[j].x = 0;
					pos3[j].y = 0;
				}
				return true;
			}
			else
			    return false;
	}


HBITMAP GetSrcBit(HDC hDC, DWORD BitWidth, DWORD BitHeight)
{
	HDC hBufDC;
	HBITMAP hBitmap, hBitTemp;
	hBufDC = CreateCompatibleDC(hDC); // 创建一个和hDC(一般是屏幕)兼容的内存DC
	hBitmap = CreateCompatibleBitmap(hDC, BitWidth, BitHeight); // 创建一个和hDC兼容的空位图（例如，hDC是16位彩色，这个hBitmap也是16位彩色；hDC是24位彩色，这个hBitmap也是16位彩色)
	hBitTemp = (HBITMAP)SelectObject(hBufDC, hBitmap); // 把内存DC和空位图绑定，内存DC的原始的位图句柄被hBitTemp暂时保存起来。
	StretchBlt(hBufDC, 0, 0, BitWidth, BitHeight, hDC, 0, 0, BitWidth, BitHeight, SRCCOPY); // 将hDC的一个矩形位图复制到内存DC中（其实就是复制到和内存DC绑定的空位图中，现在它就不再是空位图了）
	hBitmap = (HBITMAP)SelectObject(hBufDC, hBitTemp); // 内存DC选择和原先的原始位图再次绑定，把刚刚获得像素的位图踢出来，还给hBitmap。
	::DeleteDC(hBufDC); // 删除内存DC（同时也会删除它绑定的位图。这就是为什么上面要把位图踢出来的原因，为了避免一起死。）
	return hBitmap; // 将这个位图返回。不过外界用完这个位图，要记得释放它。
}
bool Blur(HBITMAP hBmp, double a = 1, double b = 0.08)
{
	static int modles[][3] =
	{
		{ 1, 0, 0 },
		{ 0, 1, 0 },
		{ -1, 0, 0 },
		{ 0, -1, 0 },
		{ 1, 1, 0 },
		{ 1, -1, 0 },
		{ -1, 1, 0 },
		{ -1, -1, 0 }
	};
	int i;
	for (i = 7; i >= 0; i--)
	{
		modles[i][2] = (int)(255 * b);
		b = b / (1 - b);
		if (i == 4)
		{
			b *= 1.4142135623;
		}
	}
	CWindowDC wdc(NULL);
	CDC dc;
	dc.CreateCompatibleDC(&wdc);
	CImage imgSrc;
	imgSrc.Attach(hBmp);
	CBitmap bmp;
	if (bmp.CreateCompatibleBitmap(&wdc, imgSrc.GetWidth(), imgSrc.GetHeight()))
	{
		dc.SelectObject(&bmp);
		int alpha = (int)(a * 255);
		if (alpha > 254)
		{
			imgSrc.BitBlt(dc, 0, 0);
		}
		else if (alpha > 1)
		{
			imgSrc.AlphaBlend(dc, 0, 0, alpha);
		}
		for (i = 0; i < 8; i++)
		{
			imgSrc.AlphaBlend(dc, modles[i][0], modles[i][1], modles[i][2]);
		}
		imgSrc.Detach();
		dc.SelectObject(hBmp);
		imgSrc.Attach(bmp);
		imgSrc.BitBlt(dc, 0, 0);
		imgSrc.Detach();
		return true;
	}
	imgSrc.Detach();
	return false;
}

typedef pair<int, bool> ItemCount;
typedef pair<int, int> position;
struct PosAndData
{
	position pos;
	int data;
};
vector<ItemCount> countpool;
vector<PosAndData> padpool;
bool test = false;


//基本类型初始化:
HGE *hge=0; 
ImageObject *menumouse = nullptr;
ImageObject *selectlayer1 = nullptr;
ImageObject *selectlayer2 = nullptr;
AnimObject *mouse = NULL;
ImageObject *menubg = NULL;
ImageObject *BG = NULL;
Bar *barHP = NULL;
Bar *barMP = NULL;
Bar *barEXP = NULL;
Bar *barSCORE = NULL;
ImageObject *Frame = NULL;
Bar *barBOSSHP = NULL;
hgeFont *fontfornumbers = nullptr;
hgeFont *fontforalpha = nullptr;
ImageObject *blackin = nullptr;
ImageObject *pausemenu = nullptr;
ImageObject *resultbg = nullptr;
AnimObject *TokenL = nullptr;
//计时用三人组：
int b, c, d;
bool a;
//scene初始化：
bool gamesceneinit = false;
bool menusceneinit = false;
bool pausesceneinit = false;
bool resultsceneinit = false;
bool levelupsceneinit = false;
//result用：
int stage;//标识进行到的阶段
int charge;//用来累加分数
int gametime;
int timecount;
int alphaforfont;
int addscore;
bool pauseforstage;
bool isaddedscore;
bool pauseforstage2;

int resultalpha;

int range;
bool doubleknock;
int countdouble;
int finalscore;
float countresult;
int countpause;
float countpauseblur=0;
int pausemenualpha;
bool gamepause2;
bool gamepause;
bool gameblackin;
int count2inmenu;
int countinmenu;
int countalpha;
bool finishiblackin;
bool generating;
int mode, num, i;
int vary;
int countgenerating;
float layer1x, layer2x, layer1y, layer2y;
int doubleklick;
int open;
bool gameover;
int std_score;
int FPS = 50;
bool boss;
bool wavestart;
int generatetype;
int countforskill;
Point startpos;
int startd;
int MazeToEvent;
int EventName;
bool startlevelup;
bool startlevelup2;
int eventx, eventy;
DWORD  ThreadFunc(LPVOID lpraram)
{
	RenderPool::GetRenderPool().InsertImageV2("rune1.png", 8, 8, RUNE_DRAGOON);
	RenderPool::GetRenderPool().InsertImageV2("rune2.png", 8, 8, RUNE_LANSER);
	RenderPool::GetRenderPool().InsertImageV2("rune3.png", 8, 8, RUNE_TANK);
	RenderPool::GetRenderPool().InsertImageV2("rune4.png", 8, 8, RUNE_BLACKHOLE);
	RenderPool::GetRenderPool().InsertImageV2("rune5.png", 8, 8, RUNE_BOOMER);
	RenderPool::GetRenderPool().InsertImageV2("rune6.png", 8, 8, RUNE_HEAL);
	RenderPool::GetRenderPool().InsertImageV2("rune7.png", 8, 8, RUNE_ATKUP);
	RenderPool::GetRenderPool().InsertImageV2("rune8.png", 8, 8, RUNE_CRITUP);
	RenderPool::GetRenderPool().InsertImageV2("rune9.png", 8, 8, RUNE_DODGEUP);
	RenderPool::GetRenderPool().InsertImageV2("rune10.png", 8, 8, RUNE_DEFUP);
	RenderPool::GetRenderPool().InsertImageV2("rune11.png", 8, 8, RUNE_SHOTGUN);
	RenderPool::GetRenderPool().InsertImageV2("rune12.png", 8, 8, RUNE_UZI);
	RenderPool::GetRenderPool().InsertImageV2("rune13.png", 8, 8, RUNE_MAGICIAN);
	RenderPool::GetRenderPool().InsertImageV2("rune14.png", 8, 8, RUNE_ROCKET);
	RenderPool::GetRenderPool().InsertImageV2("normal_ball.png", 25, 25, ENEMY);
	RenderPool::GetRenderPool().InsertImageV2("SuperSpeedBall.png", 25, 25, SUPERSPEEDBALL);
	RenderPool::GetRenderPool().InsertImageV2("TrickBall.png", 25, 25, TRICKBALL);
	RenderPool::GetRenderPool().InsertImageV2("boss1.png", 25, 25, BOSS1);
	RenderPool::GetRenderPool().InsertImageV2("bulletball.png", 25, 25, RINGBULLETBALL);
	RenderPool::GetRenderPool().InsertImageV2("HPbar.png", 0, 0, HPBAR);
	RenderPool::GetRenderPool().InsertImageV2("MPbar.png", 0, 0, MPBAR);
	RenderPool::GetRenderPool().InsertImageV2("EXPbar.png", 0, 0, EXPBAR);
	RenderPool::GetRenderPool().InsertImageV2("SCOREbar.png", 0, 0, SCOREBAR);
	RenderPool::GetRenderPool().InsertImageV2("BOSSHPbar.png", 0, 0, BOSSHPBAR);
	RenderPool::GetRenderPool().InsertImageV2("START_N.png", 0, 0, BUTTON_1_OFF);
	RenderPool::GetRenderPool().InsertImageV2("START_M.png", 0, 0, BUTTON_1_OVER);
	RenderPool::GetRenderPool().InsertImageV2("START_M.png", 0, 0, BUTTON_1_DOWN);
	RenderPool::GetRenderPool().InsertImageV2("SHIELD1.png", 64, 64, SHIELD1);
	RenderPool::GetRenderPool().InsertImageV2("title.png", 200, 100, TITLE);
	RenderPool::GetRenderPool().InsertImageV2("OPTION_N.png", 0, 0, BUTTON_2_OFF);
	RenderPool::GetRenderPool().InsertImageV2("OPTION_M.png", 0, 0, BUTTON_2_OVER);
	RenderPool::GetRenderPool().InsertImageV2("OPTION_M.png", 0, 0, BUTTON_2_DOWN);
	RenderPool::GetRenderPool().InsertImageV2("EXTRA_N.png", 0, 0, BUTTON_3_OFF);
	RenderPool::GetRenderPool().InsertImageV2("EXTRA_M.png", 0, 0, BUTTON_3_OVER);
	RenderPool::GetRenderPool().InsertImageV2("EXTRA_M.png", 0, 0, BUTTON_3_DOWN);
	RenderPool::GetRenderPool().InsertImageV2("EXIT_N.png", 0, 0, BUTTON_4_OFF);
	RenderPool::GetRenderPool().InsertImageV2("EXIT_M.png", 0, 0, BUTTON_4_OVER);
	RenderPool::GetRenderPool().InsertImageV2("EXIT_M.png", 0, 0, BUTTON_4_DOWN);
	RenderPool::GetRenderPool().InsertImageV2("BACK_N.png", 0, 0, BUTTON_5_OFF);
	RenderPool::GetRenderPool().InsertImageV2("BACK_M.png", 0, 0, BUTTON_5_OVER);
	RenderPool::GetRenderPool().InsertImageV2("BACK_CLICK.png", 0, 0, BUTTON_5_DOWN);
	RenderPool::GetRenderPool().InsertImageV2("chapter.png", 0, 0, BUTTON_6_OFF);
	RenderPool::GetRenderPool().InsertImageV2("chapter.png", 0, 0, BUTTON_6_OVER);
	RenderPool::GetRenderPool().InsertImageV2("chapter.png", 0, 0, BUTTON_6_DOWN);
	RenderPool::GetRenderPool().InsertImageV2("survival.png", 0, 0, BUTTON_7_OFF);
	RenderPool::GetRenderPool().InsertImageV2("survival_o.png", 0, 0, BUTTON_7_OVER);
	RenderPool::GetRenderPool().InsertImageV2("survival.png", 0, 0, BUTTON_7_DOWN);
	RenderPool::GetRenderPool().InsertImageV2("minigame.png", 0, 0, BUTTON_8_OFF);
	RenderPool::GetRenderPool().InsertImageV2("minigame.png", 0, 0, BUTTON_8_OVER);
	RenderPool::GetRenderPool().InsertImageV2("minigame.png", 0, 0, BUTTON_8_DOWN);
	RenderPool::GetRenderPool().InsertImageV2("stats-1.png", 0, 0, BUTTON_9_OFF);
	RenderPool::GetRenderPool().InsertImageV2("stats-2.png", 0, 0, BUTTON_9_OVER);
	RenderPool::GetRenderPool().InsertImageV2("stats-2.png", 0, 0, BUTTON_9_DOWN);
	RenderPool::GetRenderPool().InsertImageV2("est-1.png", 0, 0, BUTTON_10_OFF);
	RenderPool::GetRenderPool().InsertImageV2("est-2.png", 0, 0, BUTTON_10_OVER);
	RenderPool::GetRenderPool().InsertImageV2("est-2.png", 0, 0, BUTTON_10_DOWN);
	RenderPool::GetRenderPool().InsertImageV2("ach-1.png", 0, 0, BUTTON_11_OFF);
	RenderPool::GetRenderPool().InsertImageV2("ach-2.png", 0, 0, BUTTON_11_OVER);
	RenderPool::GetRenderPool().InsertImageV2("ach-2.png", 0, 0, BUTTON_11_DOWN);
	RenderPool::GetRenderPool().InsertImageV2("res-1.png", 0, 0, BUTTON_12_OFF);
	RenderPool::GetRenderPool().InsertImageV2("res-2.png", 0, 0, BUTTON_12_OVER);
	RenderPool::GetRenderPool().InsertImageV2("res-2.png", 0, 0, BUTTON_12_DOWN);
	RenderPool::GetRenderPool().InsertImageV2("ret-1.png", 0, 0, BUTTON_13_OFF);
	RenderPool::GetRenderPool().InsertImageV2("ret-2.png", 0, 0, BUTTON_13_OVER);
	RenderPool::GetRenderPool().InsertImageV2("ret-2.png", 0, 0, BUTTON_13_DOWN);
	RenderPool::GetRenderPool().InsertImageV2("hom-1.png", 0, 0, BUTTON_14_OFF);
	RenderPool::GetRenderPool().InsertImageV2("hom-2.png", 0, 0, BUTTON_14_OVER);
	RenderPool::GetRenderPool().InsertImageV2("hom-2.png", 0, 0, BUTTON_14_DOWN);
	RenderPool::GetRenderPool().InsertImageV2("set-1.png", 0, 0, BUTTON_15_OFF);
	RenderPool::GetRenderPool().InsertImageV2("set-2.png", 0, 0, BUTTON_15_OVER);
	RenderPool::GetRenderPool().InsertImageV2("set-2.png", 0, 0, BUTTON_15_DOWN);
	RenderPool::GetRenderPool().InsertImageV2("addatk.png", 0, 0, BUTTON_17_OFF);
	RenderPool::GetRenderPool().InsertImageV2("addatk.png", 0, 0, BUTTON_17_OVER);
	RenderPool::GetRenderPool().InsertImageV2("addatk.png", 0, 0, BUTTON_17_DOWN);
	RenderPool::GetRenderPool().InsertImageV2("adddex.png", 0, 0, BUTTON_18_OFF);
	RenderPool::GetRenderPool().InsertImageV2("adddex.png", 0, 0, BUTTON_18_OVER);
	RenderPool::GetRenderPool().InsertImageV2("adddex.png", 0, 0, BUTTON_18_DOWN);
	RenderPool::GetRenderPool().InsertImageV2("addmagic.png", 0, 0, BUTTON_19_OFF);
	RenderPool::GetRenderPool().InsertImageV2("addmagic.png", 0, 0, BUTTON_19_OVER);
	RenderPool::GetRenderPool().InsertImageV2("addmagic.png", 0, 0, BUTTON_19_DOWN);
	RenderPool::GetRenderPool().InsertImageV2("itemenemy.png", 16, 16, ITEMENEMY1);
	RenderPool::GetRenderPool().InsertImageV2("Mine.png", 32, 32, MINE);
	RenderPool::GetRenderPool().InsertImageV2("expunit.png", 8, 8, EXPUNIT);
	RenderPool::GetRenderPool().InsertImageV2("block.png", 25, 25, BLOCK);
	RenderPool::GetRenderPool().InsertImageV2("stealmanaball.png", 25, 25, STEALMANABALL);
	RenderPool::GetRenderPool().InsertImageV2("heroshield.png", 64, 64, SHIELD2);

	RenderPool::GetRenderPool().InsertMazePart("maze1.png", 1);
	RenderPool::GetRenderPool().InsertMazePart("maze2.png", 2);
	RenderPool::GetRenderPool().InsertMazePart("maze3.png", 3);
	RenderPool::GetRenderPool().InsertMazePart("maze4.png", 4);
	RenderPool::GetRenderPool().InsertMazePart("maze5.png", 5);
	RenderPool::GetRenderPool().InsertMazePart("maze6.png", 6);
	RenderPool::GetRenderPool().InsertMazePart("maze7.png", 7);
	RenderPool::GetRenderPool().InsertMazePart("maze8.png", 8);
	RenderPool::GetRenderPool().InsertMazePart("maze9.png", 9);
	RenderPool::GetRenderPool().InsertMazePart("maze10.png", 10);
	RenderPool::GetRenderPool().InsertMazePart("maze11.png", 11);
	RenderPool::GetRenderPool().InsertMazePart("maze12.png", 12);
	RenderPool::GetRenderPool().InsertMazePart("maze13.png", 13);
	RenderPool::GetRenderPool().InsertMazePart("maze14.png", 14);
	RenderPool::GetRenderPool().InsertMazePart("maze15.png", 15);
	RenderPool::GetRenderPool().InsertMazePart("mazeplus.png", MAZEPLUS);
	RenderPool::GetRenderPool().InsertMazePart("mazeminus.png", MAZEMINUS);
	RenderPool::GetRenderPool().InsertMazePart("mazemine.png", MAZEMINES);
	RenderPool::GetRenderPool().InsertMazePart("mazempheal.png", MAZEMPHEAL);
	RenderPool::GetRenderPool().InsertMazePart("mazesmallboss.png", MAZESMALLBOSS);
	RenderPool::GetRenderPool().InsertMazePart("mazespick.png", MAZESPICK);
	RenderPool::GetRenderPool().InsertMazePart("mazeskill.png", MAZESKILL);
	RenderPool::GetRenderPool().InsertMazePart("mazeitem.png", MAZEITEM);
	RenderPool::GetRenderPool().InsertMazePart("mazeboss.png", MAZEBOSS);

	RenderPool::GetRenderPool().InsertAnimV2("fireball.png", 49, 49, 64, 64, 32, 32, FIREBALL, true);
	RenderPool::GetRenderPool().InsertAnimV2("totemeffect.png", 49, 49, 256, 256, 128, 128, TOTEMEFFECT, true);
	RenderPool::GetRenderPool().InsertAnimV2("3blackhole.png", 49, 25, 64, 64, 32, 32, BLACKHOLEARROW,true);
	RenderPool::GetRenderPool().InsertAnimV2("4blackhole.png", 49, 25, 64, 64, 32, 32, BLACKHOLEFORHERO,true);
	RenderPool::GetRenderPool().InsertAnimV2("5blackhole.png", 49, 25, 64, 64, 32, 32, BLACKHOLEFORENEMY,true);
	RenderPool::GetRenderPool().InsertAnimV2("6blackhole.png", 49, 25, 64, 64, 32, 32, BLACKHOLEFORMINE,true);
	RenderPool::GetRenderPool().InsertAnimV2("bullet.png", 1, 100, 32, 32, 16, 16, BULLET1);
	RenderPool::GetRenderPool().InsertAnimV2("enemybullet.png", 1, 100, 32, 32, 15, 11, BULLET4);
	RenderPool::GetRenderPool().InsertAnimV2("myhero.png", 50, 50, 100, 100, 50, 50, HERO, true);
	RenderPool::GetRenderPool().InsertAnimV2("item_minus.png", 61, 30, 100, 100, 50, 50, ITEM_DOWN, true);
	RenderPool::GetRenderPool().InsertAnimV2("item_unknown.png", 61, 30, 100, 100, 50, 50, ITEM_RANDOM, true);
	RenderPool::GetRenderPool().InsertAnimV2("item_add.png", 61, 30, 100, 100, 50, 50, ITEM_UP, true);
	RenderPool::GetRenderPool().InsertAnimV2("TOKEN_BANNED.png", 1, 100, 43, 42, 0, 0, TOKEN_BANNED);
	RenderPool::GetRenderPool().InsertAnimV2("TOKEN_ACTIVE.png", 1, 100, 43, 42, 0, 0, TOKEN_ACTIVE);
	RenderPool::GetRenderPool().InsertAnimV2("item_hp.png", 61, 30, 100, 100, 50, 50, ITEM_HP, true);
	RenderPool::GetRenderPool().InsertAnimV2("item_mp.png", 61, 30, 100, 100, 50, 50, ITEM_MP, true);
	RenderPool::GetRenderPool().InsertAnimV2("icefield.png", 49, 25, 200, 200, 100, 100, FREEZEFIELD, true);
	RenderPool::GetRenderPool().InsertAnimV2("healfield.png", 49, 25, 200, 200, 100, 100, HEALFIELD, true);
	RenderPool::GetRenderPool().InsertAnimV2("toxicfield.png", 49, 25, 200, 200, 100, 100, TOXICFIELD, true);
	RenderPool::GetRenderPool().InsertAnimV2("stealmanafield.png", 1, 100, 128, 128, 64, 64, STEALMANAFIELD);

	
	RenderPool::GetRenderPool().InsertAnimV2("DEFUP.png", 1, 1, 64, 64, 32, 32, MAZEITEM_DEFUP, true);
	RenderPool::GetRenderPool().InsertAnimV2("DMGUP.png", 1, 1, 64, 64, 32, 32, MAZEITEM_DMGUP, true);
	RenderPool::GetRenderPool().InsertAnimV2("toxicbullet.png", 1, 1, 64, 64, 32, 32, MAZEITEM_TOXICBULLET, true);
	RenderPool::GetRenderPool().InsertAnimV2("icebullet.png", 1, 1, 64, 64, 32, 32, MAZEITEM_ICEBULLET, true);
	RenderPool::GetRenderPool().InsertAnimV2("stunbullet.png", 1, 1, 64, 64, 32, 32, MAZEITEM_STUNBULLET, true);
	RenderPool::GetRenderPool().InsertAnimV2("DtoDMGdownbullet.png", 1, 1, 64, 64, 32, 32, MAZEITEM_DTODMGDOWN, true);
	RenderPool::GetRenderPool().InsertAnimV2("DtoDMGbullet.png", 1, 1, 64, 64, 32, 32, MAZEITEM_DTODMG, true);
	RenderPool::GetRenderPool().InsertAnimV2("DtoBoombullet.png", 1, 1, 64, 64, 32, 32, MAZEITEM_DTOBOOM, true);
	RenderPool::GetRenderPool().InsertAnimV2("followbullets.png", 1, 1, 64, 64, 32, 32, MAZEITEM_FOLLOWBULLET, true);
	RenderPool::GetRenderPool().InsertAnimV2("healmore.png", 1, 1, 64, 64, 32, 32, MAZEITEM_HEALMORE, true);
	RenderPool::GetRenderPool().InsertAnimV2("HPUP.png", 1, 1, 64, 64, 32, 32, MAZEITEM_HPUP, true);
	RenderPool::GetRenderPool().InsertAnimV2("MPUP.png", 1, 1, 64, 64, 32, 32, MAZEITEM_MPUP, true);
	RenderPool::GetRenderPool().InsertAnimV2("suckbloodbullet.png", 1, 1, 64, 64, 32, 32, MAZEITEM_SUCKBLOOD, true);





	RenderPool::GetRenderPool().InsertAnimV2("fireballboom.png", 49, 100, 256, 256, 128, 128, TOKEN_FIREBALL, false,true);
	RenderPool::GetRenderPool().InsertAnimV2("enemybulletboom.png", 20, 50, 64, 64, 32, 32, BULLET4, false, true);
	RenderPool::GetRenderPool().InsertAnimV2("enemyboom.png", 35, 60, 350, 350, 175, 175, ENEMY,false,true);
	RenderPool::GetRenderPool().InsertAnimV2("flash3.png", 20, 60, 192, 192, 96, 96, HERO, false, true);
	RenderPool::GetRenderPool().InsertAnimV2("enemyboom4.png", 35, 60, 350, 350, 175, 175, TRICKBALL, false, true);
	RenderPool::GetRenderPool().InsertAnimV2("enemyboom5.png", 35, 60, 350, 350, 175, 175, SUPERSPEEDBALL, false, true);
	RenderPool::GetRenderPool().InsertAnimV2("enemyboom6.png", 35, 60, 350, 350, 175, 175, RINGBULLETBALL, false, true);
	RenderPool::GetRenderPool().InsertAnimV2("enemyboomc.png", 35, 60, 350, 350, 175, 175, BOSS1, false, true);
	RenderPool::GetRenderPool().InsertAnimV2("bulletboom.png", 20, 50, 64, 64, 32, 32, BULLET1, false, true);
	RenderPool::GetRenderPool().InsertAnimV2("overbutton.png", 42, 25, 256, 128, 68, 47, BUTTON_1_OFF, false, true);
	RenderPool::GetRenderPool().InsertAnimV2("item_minusbegin.png", 28, 28, 200, 200, 100, 100, ITEMMINUSBEGIN, false, true);
	RenderPool::GetRenderPool().InsertAnimV2("item_unknownbegin.png", 28, 28, 200, 200, 100, 100, ITEMUNKNOWNBEGIN, false, true);
	RenderPool::GetRenderPool().InsertAnimV2("item_addbegin.png", 28, 28, 200, 200, 100, 100, ITEMADDBEGIN, false, true);
	RenderPool::GetRenderPool().InsertAnimV2("item_hpbegin.png", 28, 28, 200, 200, 100, 100, ITEMHPBEGIN, false, true);
	RenderPool::GetRenderPool().InsertAnimV2("item_mpbegin.png", 28, 28, 200, 200, 100, 100, ITEMMPBEGIN, false, true);
	RenderPool::GetRenderPool().InsertAnimV2("item_minusboom.png", 32, 32, 350, 350, 175, 175, ITEMMINUSBOOM, false, true);
	RenderPool::GetRenderPool().InsertAnimV2("item_unknownboom.png", 32, 32, 350, 350, 175, 175, ITEMUNKNOWNBOOM, false, true);
	RenderPool::GetRenderPool().InsertAnimV2("item_addboom.png", 32, 32, 350, 350, 175, 175, ITEMADDBOOM, false, true);
	RenderPool::GetRenderPool().InsertAnimV2("item_hpboom.png", 32, 32, 350, 350, 175, 175, ITEMHPBOOM, false, true);
	RenderPool::GetRenderPool().InsertAnimV2("item_mpboom.png", 32, 32, 350, 350, 175, 175, ITEMMPBOOM, false, true);
	RenderPool::GetRenderPool().InsertAnimV2("item_minusdead.png", 28, 28, 200, 200, 100, 100, ITEMMINUSDEAD, false, true);
	RenderPool::GetRenderPool().InsertAnimV2("item_unknowndead.png", 28, 28, 200, 200, 100, 100, ITEMUNKNOWNDEAD, false, true);
	RenderPool::GetRenderPool().InsertAnimV2("item_adddead.png", 28, 28, 200, 200, 100, 100, ITEMADDDEAD, false, true);
	RenderPool::GetRenderPool().InsertAnimV2("item_hpdead.png", 28, 28, 200, 200, 100, 100, ITEMHPDEAD, false, true);
	RenderPool::GetRenderPool().InsertAnimV2("item_mpdead.png", 28, 28, 200, 200, 100, 100, ITEMMPDEAD, false, true);
	RenderPool::GetRenderPool().InsertAnimV2("icefield_start.png", 72, 72, 200, 200, 100, 100, FREEZEFIELD, false, true);
	RenderPool::GetRenderPool().InsertAnimV2("icefield_end.png", 49, 49, 200, 200, 100, 100, TOKEN_FREEZEFIELD, false, true);
	RenderPool::GetRenderPool().InsertAnimV2("healfield_start.png", 72, 72, 200, 200, 100, 100, HEALFIELD, false, true);
	RenderPool::GetRenderPool().InsertAnimV2("healfield_end.png", 49, 49, 200, 200, 100, 100, TOKEN_HEALFIELD, false, true);
	RenderPool::GetRenderPool().InsertAnimV2("toxicfield_start.png", 64, 64, 200, 200, 100, 100, TOXICFIELD, false, true);
	RenderPool::GetRenderPool().InsertAnimV2("toxicfield_end.png", 49, 49, 200, 200, 100, 100, TOKEN_TOXICFIELD, false, true);
	RenderPool::GetRenderPool().InsertAnimV2("toxiceffect.png", 48, 48, 24, 24, 12, 12, TOXIC, false, true);
	RenderPool::GetRenderPool().InsertAnimV2("freezeeffect.png", 48, 48, 24, 24, 12, 12, FREEZE, false, true);
	RenderPool::GetRenderPool().InsertAnimV2("vertigoeffect.png", 48, 48, 24, 24, 12, 12, VERTIGO, false, true);

	RenderPool::GetRenderPool().SetCombinationV2(BOSS1, BOSS2, false);

	RenderPool::GetRenderPool().InsertParticleV2("particle1.psi", "particle1.png", MPUNIT);
	RenderPool::GetRenderPool().InsertParticleV2("exp.psi", "particle1.png", EXPUNIT);
	RenderPool::GetRenderPool().InsertParticleV2("bullet2.psi", "bullet2.png", BULLET2);


	HGE *hge = hgeCreate(HGE_VERSION);
	fontfornumbers = new hgeFont("font.fnt");
	fontforalpha = new hgeFont("font.fnt");
	//加载图片文件：
	BG = new ImageObject("background.png", 0, 0);
	menubg = new ImageObject("menubg.png", 0, 0, 0);
	menubg->SetZ(0.9);
	Frame = new ImageObject("frame.png", 0, 0, 0);
	menumouse = new ImageObject("menumouse.png", 23, 25, 0);
	blackin = new ImageObject("blackin.png", 0, 0, 0);
	selectlayer1 = new ImageObject("selectlayer1.png", 0, 0, 0);
	selectlayer2 = new ImageObject("selectlayer2.png", 0, 0, 0);
	pausemenu = new ImageObject("pausemenu.png", 0, 0, 0);
	resultbg = new ImageObject("result.png", 0, 0, 0);
	pausemenu->SetAlpha(0);
	blackin->SetZ(0.2);
	ObjectPool::GetObjPool().SetBWbools("heroshieldboundingbox.png", SHIELD2);
	ObjectPool::GetObjPool().SetBWbools("SHIELD1_BOUNDINGBOX.png", SHIELD1);
	//加载动画文件：
	mouse = new AnimObject("mouse.png", 50, 50, 50, 50, 25, 25, 0, true);
	TokenL = new AnimObject("TOKEN_LOCKED.png", 1, 100, 43, 42, 0, 0, 0);
	ObjectPool::GetObjPool().InitTokens();
	//音乐载入：
	SFXManager::GetSFXManager().InsertSFX("menubgm.mp3", MENUBGM, false);
	SFXManager::GetSFXManager().InsertSFX("gamebgm.mp3", GAMEBGM, false);
	SFXManager::GetSFXManager().InsertSFX("buttonsfx.mp3", BUTTONSFX, true);
	SFXManager::GetSFXManager().InsertSFX("buttonsfx2.mp3", BUTTONSFX2, true);
	SFXManager::GetSFXManager().InsertSFX("boss.mp3", BOSSBGM, false);
	SFXManager::GetSFXManager().InsertSFX("resultmode.mp3", RESULTBGM, false);
	SFXManager::GetSFXManager().InsertSFX("shoot.mp3", SHOOTSFX, true);
	SFXManager::GetSFXManager().InsertSFX("hit.mp3", HITSFX1, true);
	SFXManager::GetSFXManager().InsertSFX("enemyboom1.mp3", ENEMYBOOMSFX1, true);
	SFXManager::GetSFXManager().InsertSFX("enemyboom2.mp3", ENEMYBOOMSFX2, true);
	SFXManager::GetSFXManager().InsertSFX("hit2.mp3", HITSFX2, true);
	SFXManager::GetSFXManager().InsertSFX("getexp.wav", GETEXPSFX, true);
	SFXManager::GetSFXManager().InsertSFX("Notice.mp3", NOTICESFX, true);
	SFXManager::GetSFXManager().InsertSFX("getskill.mp3", GETSKILLSFX, true);
	SFXManager::GetSFXManager().InsertSFX("lvup.mp3", LVUPSFX, true);

	AfxEndThread(1000);
	return 0;
}


HTARGET canvas2 = NULL;


//从这里开始rander
bool RenderFunc()
{
	HTARGET canvas = hge->Target_Create(1024, 512, true);// (SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	hgeSprite *canvaspic;
	float x, y;
	switch (SceneManager::GetSceneManager().GetCurrentScene())
	{
	case GameScene:
	{
					  HTARGET UPblur = hge->Target_Create(753, 73, true);
					  hge->Gfx_BeginScene(UPblur);//开始渲染 
					  hge->Gfx_Clear(0xFF000000);
					  BG->render(0, 0);
					  hge->Gfx_EndScene();//结束渲染(这时才把所有东西全都渲染出来）
					  canvaspic = new hgeSprite(hge->Target_GetTexture(UPblur), 0, 0, 753, 73);
					  //执行操作：截取像素
					  hge->Gfx_BeginScene();//开始渲染
					  hge->Gfx_Clear(0xFF000000);
					  BG->render(0, 0);
					  canvaspic->Render(0, 0);
					  FontPool::GetFontPool().RenderFont();
					  RenderPool::GetRenderPool().RenderEveryObject();
					  RenderPool::GetRenderPool().RenderAnimations();
					  RenderPool::GetRenderPool().RenderParticles();
					  RenderPool::GetRenderPool().RenderDead();
					  RenderPool::GetRenderPool().RenderHero(&Hero::GetHero());
					  if (SceneManager::GetSceneManager().GetCurrentMode() == MazeMode)
						  RenderPool::GetRenderPool().RenderMaze();
					  Frame->render(0, 0);
					  RenderPool::GetRenderPool().RenderAfterBlur();
					  BuffManager::GetBuffManager().Update();
					  TokenL->Render(ObjectPool::GetObjPool().GetSelectedTokenX(), ObjectPool::GetObjPool().GetSelectedTokenY(), 0, 1);
					  fontfornumbers->printf(39, 29, HGETEXT_CENTER, "%d", Hero::GetHero().GetLives());
					  fontfornumbers->printf(183, 16, HGETEXT_CENTER, "%d/%d", Hero::GetHero().GetHP(), Hero::GetHero().GetMaxHP());
					  //fontfornumbers->printf(183, 42, HGETEXT_CENTER, "%d/%d", Hero::GetHero().GetMP(), Hero::GetHero().GetMaxMP());
					  fontfornumbers->printf(39, 43, HGETEXT_CENTER, "LV:%d", Hero::GetHero().GetLevel());
					  fontfornumbers->printf(389, 37, HGETEXT_CENTER, "%f", Hero::GetHero().GetRot());
					  hge->Input_GetMousePos(&x, &y);
					  mouse->Render(x, y, 0);
					  mouse->UpdateStep();
					  RenderPool::GetRenderPool().RenderTokens();
					  NoticeManager::GetNoticeManager().Update();
					  blackin->render(0, 0);
					  hge->Gfx_EndScene();//结束渲染
					  if ((gamepause || startlevelup || (Hero::GetHero().GameOver() && !NoticeManager::GetNoticeManager().IsOccopied())))
					  {
						  canvas2 = hge->Target_Create(1024, 512, true);
						  hge->Gfx_BeginScene(canvas2);
						  hge->Gfx_Clear(0xFF000000);
						  BG->render(0, 0);
						  canvaspic->Render(0, 0);
						  Frame->render(0, 0);
						  FontPool::GetFontPool().RenderFont();
						  RenderPool::GetRenderPool().RenderEveryObject();
						  RenderPool::GetRenderPool().RenderAnimations();
						  RenderPool::GetRenderPool().RenderDead();
						  RenderPool::GetRenderPool().RenderHero(&Hero::GetHero());
						  RenderPool::GetRenderPool().RenderAfterBlur();
						  TokenL->Render(ObjectPool::GetObjPool().GetSelectedTokenX(), ObjectPool::GetObjPool().GetSelectedTokenY(), 0);
						  fontfornumbers->printf(39, 29, HGETEXT_CENTER, "%d", Hero::GetHero().GetLives());
						  fontfornumbers->printf(183, 16, HGETEXT_CENTER, "%d/%d", Hero::GetHero().GetHP(), Hero::GetHero().GetMaxHP());
						  //fontfornumbers->printf(183, 42, HGETEXT_CENTER, "%d/%d", Hero::GetHero().GetMP(), Hero::GetHero().GetMaxMP());
						  fontfornumbers->printf(39, 43, HGETEXT_CENTER, "LV:%d", Hero::GetHero().GetLevel());
						  fontfornumbers->printf(389, 37, HGETEXT_CENTER, "%d", Hero::GetHero().GetScore());
						  RenderPool::GetRenderPool().RenderTokens();
						  NoticeManager::GetNoticeManager().Update();
						  hge->Gfx_EndScene();//结束渲染
						  if (gamepause)
							  gamepause2 = true;
						  else if (startlevelup)
							  startlevelup2 = true;
						  else
						  gameover = true;
					  }
					  hge->Target_Free(UPblur);
					  delete canvaspic;
					  canvaspic = NULL;
					  break;
	}
	case LoadingScene:
		break;
	case MenuScene:
		hge->Gfx_BeginScene(canvas);//开始渲染 
		hge->Gfx_Clear(0xFF000000);
		menubg->render(0, 0);
		RenderPool::GetRenderPool().RenderEveryObject();
		RenderPool::GetRenderPool().RenderAnimations();
		RenderPool::GetRenderPool().RenderDead();
		hge->Gfx_EndScene();//结束渲染
		canvaspic = new hgeSprite(hge->Target_GetTexture(canvas), 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		canvaspic->SetZ(0.75);
		hge->Gfx_BeginScene();//开始渲染
		hge->Gfx_Clear(0xFF000000);
		for (float a = 0.2; a <= 1.2; a += 0.2)
		{
			canvaspic->SetColor(ARGB(255 -a*212 , 255, 255, 255));
			canvaspic->Render(0 + a, 0);
		    canvaspic->Render(0 - a, 0);
			canvaspic->Render(0, 0 + a);
			canvaspic->Render(0, 0 - a);
		}
		RenderPool::GetRenderPool().RenderAfterBlur();
		selectlayer1->render(layer1x, layer1y);
		selectlayer2->render(layer2x, layer2y);
		hge->Input_GetMousePos(&x, &y);
		menumouse->render(x, y, 0);
		blackin->render(0, 0);
		hge->Gfx_EndScene();//结束渲染
		delete canvaspic;
		canvaspic = NULL;
		break;
	case LogoScene:
	{
					  TCHAR exeFullPath[MAX_PATH]; // MAX_PATH
					  GetModuleFileName(NULL, exeFullPath, MAX_PATH);//得到程序模块名称，全路径
					  *_tcsrchr(exeFullPath, _T('\\')) = 0;
					  CString str2 = exeFullPath;
					  IGraphBuilder * pGraph = NULL;
					  IMediaControl * pControl = NULL;
					  IMediaEvent * pEvent = NULL;
					  //改动了！
					  IVideoWindow* m_pVidWin = NULL;
					  HWND m_hwnd;
					  m_hwnd = hge->System_GetState(HGE_HWND);
					  //不支持com组件的就会失败!
					  HRESULT hr = CoInitialize(NULL);
					  if (FAILED(hr))MessageBox(NULL, "不支持COM组件的初始化，导致动画未被载入", "初始化错误！", MB_OK);
					  //可能会失败!
					  CString strx, stry;
					  strx = "\\Logo.mov";
					  stry = str2 + strx;
					  USES_CONVERSION;
					  LPCWSTR pwcStr = A2CW((LPCSTR)stry);
					  hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&pGraph);
					  hr = pGraph->QueryInterface(IID_IMediaControl, (void**)&pControl);
					  hr = pGraph->QueryInterface(IID_IMediaEvent, (void**)&pEvent);
					  hr = pGraph->QueryInterface(IID_IVideoWindow, (void **)&m_pVidWin);
					  hr = pGraph->RenderFile(pwcStr, NULL);
					  RECT rc;
					  GetClientRect(m_hwnd, &rc);
					  hr = m_pVidWin->SetWindowPosition(0, 0, rc.right, rc.bottom);
					  hr = m_pVidWin->put_Owner((OAHWND)m_hwnd);
					  hr = m_pVidWin->put_MessageDrain((OAHWND)m_hwnd);
					  hr = m_pVidWin->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);
					  if (SUCCEEDED(hr))
					  {
						  hr = pControl->Run();
						  if (SUCCEEDED(hr))
						  {
							  long evCode;
							  pEvent->WaitForCompletion(INFINITE, &evCode);
						  }
					  }

					  pControl->Release();
					  pEvent->Release();
					  pGraph->Release();
					  m_pVidWin->Release();
					  CoUninitialize();
					  SceneManager::GetSceneManager().GoToOneScene(MenuScene, false);
	}
		break;
	case PauseScene:
	{
	  canvaspic = new hgeSprite(hge->Target_GetTexture(canvas2), 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	  canvaspic->SetZ(0.75);
	   hge->Gfx_BeginScene();
	   hge->Gfx_Clear(0xFF000000);
	   if (countpauseblur < 3)
	   {
		   countpauseblur+=0.1;
	   }
	   for (float a = 0.2; a <= 0.4*(1+countpauseblur); a += 0.2)
	   {
		   canvaspic->SetColor(ARGB(255 - a * 159, 255, 255, 255));
		   canvaspic->Render(0 + a, 0);
		   canvaspic->Render(0 - a, 0);
		   canvaspic->Render(0, 0 + a);
		   canvaspic->Render(0, 0 - a);
	   }
	   pausemenu->render(0, 0);
	   RenderPool::GetRenderPool().RenderAfterBlur();
	   hge->Input_GetMousePos(&x, &y);
	   menumouse->render(x, y, 0);
	   hge->Gfx_EndScene();
	   delete canvaspic;
	}
		break;
	case GameOverScene:
		canvaspic = new hgeSprite(hge->Target_GetTexture(canvas2), 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		canvaspic->SetZ(0.75);
		if (countresult < 3)
			countresult+=0.1;
		hge->Gfx_BeginScene();
		hge->Gfx_Clear(0xFF000000);
		for (float a = 0.2; a <= 0.4*(1 + countresult); a += 0.2)
		{
			canvaspic->SetColor(ARGB(255 - a * 159, 255, 255, 255));
			canvaspic->Render(0 + a, 0);
			canvaspic->Render(0 - a, 0);
			canvaspic->Render(0, 0 + a);
			canvaspic->Render(0, 0 - a);
		}
		resultbg->render(0, 0);
		RenderPool::GetRenderPool().RenderAfterBlur();
		RenderPool::GetRenderPool().RenderAnimations();
		RenderPool::GetRenderPool().RenderHero(&Hero::GetHero());
		fontfornumbers->printf(712, 356, HGETEXT_CENTER, "%d", addscore);
		fontfornumbers->printf(686, 203, HGETEXT_CENTER, "%d", Hero::GetHero().GetMaxHP());
		fontfornumbers->printf(686, 230, HGETEXT_CENTER, "%d", Hero::GetHero().GetMaxMP());
		fontfornumbers->printf(761, 263, HGETEXT_CENTER, "%d", Hero::GetHero().GetDEF());
		fontfornumbers->printf(690, 263, HGETEXT_CENTER, "%d", Hero::GetHero().GetDMG(false));
		fontfornumbers->printf(733, 283, HGETEXT_CENTER, "%d", Hero::GetHero().GetEXP());
		fontfornumbers->printf(733, 303, HGETEXT_CENTER, "%d", Hero::GetHero().GetScore());
		switch (stage)
		{
		case 11:
		case 10:
			if (stage == 10)
			{
				if (!pauseforstage)
				fontfornumbers->printf(604, 383, HGETEXT_RIGHT, "%d", charge);
				else
				{
					fontfornumbers->printf(604, 383, HGETEXT_RIGHT, "%d", gametime);
					if (alphaforfont > 0)alphaforfont -= 5;
					else
					{
						alphaforfont = 255;
						pauseforstage2 = true;
					}
					fontforalpha->SetColor(ARGB(alphaforfont, 255, 255, 255));
					fontforalpha->printf(711, 390, HGETEXT_CENTER, "+%d", gametime);
				}
			}
			else
				fontfornumbers->printf(604, 383, HGETEXT_RIGHT, "%d", gametime);

		case 9:
			if (stage == 9)
			{
				if (!pauseforstage)
				fontfornumbers->printf(604, 365, HGETEXT_RIGHT, "%d", charge);
				else
				{
					fontfornumbers->printf(604, 365, HGETEXT_RIGHT, "%d", Hero::GetHero().GetLives());
					if (alphaforfont > 0)alphaforfont -= 5;
					else
					{
						alphaforfont = 255;
						pauseforstage2 = true;
					}
					fontforalpha->SetColor(ARGB(alphaforfont, 255, 255, 255));
					fontforalpha->printf(711, 390, HGETEXT_CENTER, "X%d", Hero::GetHero().GetLives());
				}
			}
			else
				fontfornumbers->printf(604,365, HGETEXT_RIGHT, "%d", Hero::GetHero().GetLives());
			
		case 8:
			if (stage == 8)
			{
				if (!pauseforstage)
				fontfornumbers->printf(604, 343, HGETEXT_RIGHT, "%d", charge);
				else
				{
					fontfornumbers->printf(604, 343, HGETEXT_RIGHT, "%d", Hero::GetHero().GetMP());
					if (alphaforfont > 0)alphaforfont -= 5;
					else
					{
						alphaforfont = 255;
						pauseforstage2 = true;
					}
					fontforalpha->SetColor(ARGB(alphaforfont, 255, 255, 255));
					fontforalpha->printf(711, 390, HGETEXT_CENTER, "+%d", Hero::GetHero().GetMP());
				}
			}
			else
				fontfornumbers->printf(604,343, HGETEXT_RIGHT, "%d", Hero::GetHero().GetMP());
			
		case 7:
			if (stage == 7)
			{
				if (!pauseforstage)
				fontfornumbers->printf(604, 323, HGETEXT_RIGHT, "%d", charge);
				else
				{
					fontfornumbers->printf(604, 323, HGETEXT_RIGHT, "%d", Hero::GetHero().GetHP());
					if (alphaforfont > 0)alphaforfont -= 5;
					else
					{
						alphaforfont = 255;
						pauseforstage2 = true;
					}
					fontforalpha->SetColor(ARGB(alphaforfont, 255, 255, 255));
					fontforalpha->printf(711, 390, HGETEXT_CENTER, "+%d", Hero::GetHero().GetHP());
				}
			}
			else
				fontfornumbers->printf(604,323, HGETEXT_RIGHT, "%d", Hero::GetHero().GetHP());
			
			
		case 6:
			if (stage == 6)
			{
				if (!pauseforstage)
				fontfornumbers->printf(604, 302, HGETEXT_RIGHT, "%d", charge);	
				else
				{
					fontfornumbers->printf(604, 302, HGETEXT_RIGHT, "%d", Hero::GetHero().GetAllEXP());
					if (alphaforfont > 0)alphaforfont-=5;
					else
					{
						alphaforfont = 255;
						pauseforstage2 = true;
					}
					fontforalpha->SetColor(ARGB(alphaforfont, 255, 255, 255));
					fontforalpha->printf(711, 390, HGETEXT_CENTER, "+%d", Hero::GetHero().GetAllEXP());
				}
			}
			else
				fontfornumbers->printf(604, 302, HGETEXT_RIGHT, "%d", Hero::GetHero().GetAllEXP());
		
		case 5:
			if (stage == 5)
			{
				if (!pauseforstage)
				fontfornumbers->printf(604, 282, HGETEXT_RIGHT, "%d", charge); 
				else
				{
					fontfornumbers->printf(604, 282, HGETEXT_RIGHT, "%d", Hero::GetHero().GetBossKills());
					if (alphaforfont > 0)alphaforfont -= 5;
					else
					{
						alphaforfont = 255;
						pauseforstage2 = true;
					}
					fontforalpha->SetColor(ARGB(alphaforfont, 255, 255, 255));
					fontforalpha->printf(711, 390, HGETEXT_CENTER, "+%d", Hero::GetHero().GetBossKills());
				}
			}
			else
				fontfornumbers->printf(604, 282, HGETEXT_RIGHT, "%d", Hero::GetHero().GetBossKills());
			
		case 4:
			if (stage == 4)
			{
				if (!pauseforstage)
				fontfornumbers->printf(604, 260, HGETEXT_RIGHT, "%d", charge);
				else
				{
					fontfornumbers->printf(604, 260, HGETEXT_RIGHT, "%d", Hero::GetHero().GetItem());
					if (alphaforfont > 0)alphaforfont -= 5;
					else
					{
						alphaforfont = 255;
						pauseforstage2 = true;
					}
					fontforalpha->SetColor(ARGB(alphaforfont, 255, 255, 255));
					fontforalpha->printf(711, 390, HGETEXT_CENTER, "+%d", Hero::GetHero().GetItem());
				}
			}
			else
				fontfornumbers->printf(604,260, HGETEXT_RIGHT, "%d", Hero::GetHero().GetItem());
		case 3:
			if (stage == 3)
			{
				if (!pauseforstage)
				fontfornumbers->printf(604, 238, HGETEXT_RIGHT, "%d", charge);
			    else
				{
					fontfornumbers->printf(604, 238, HGETEXT_RIGHT, "%d", Hero::GetHero().GetMaxCombo());
					if (alphaforfont > 0)alphaforfont -= 5;
					else
					{
						alphaforfont = 255;
						pauseforstage2 = true;
					}
					fontforalpha->SetColor(ARGB(alphaforfont, 255, 255, 255));
					fontforalpha->printf(711, 390, HGETEXT_CENTER, "+%d", Hero::GetHero().GetMaxCombo());
				}
			}
			else
				fontfornumbers->printf(604,238, HGETEXT_RIGHT, "%d", Hero::GetHero().GetMaxCombo());
		case 2:
			if (stage == 2)
			{
				if (!pauseforstage)
				fontfornumbers->printf(604, 215, HGETEXT_RIGHT, "%d", charge);
				else
				{
					fontfornumbers->printf(604, 215, HGETEXT_RIGHT, "%d", Hero::GetHero().GetKills());
					if (alphaforfont > 0)alphaforfont -= 5;
					else
					{
						alphaforfont = 255;
						pauseforstage2 = true;
					}
					fontforalpha->SetColor(ARGB(alphaforfont, 255, 255, 255));
					fontforalpha->printf(711, 390, HGETEXT_CENTER, "+%d", Hero::GetHero().GetKills());
				}
			}
			else
				fontfornumbers->printf(604,215, HGETEXT_RIGHT, "%d", Hero::GetHero().GetKills());
			
		case 1:
			if (stage == 1)
			{
				if (!pauseforstage)
				fontfornumbers->printf(604, 195, HGETEXT_RIGHT, "%d", charge);
				else
				{
					fontfornumbers->printf(604, 195, HGETEXT_RIGHT, "%d", Hero::GetHero().GetModeScore());
					if (alphaforfont > 0)alphaforfont-=5;
					else
					{
						alphaforfont = 255;
						pauseforstage2 = true;
					}
					fontforalpha->SetColor(ARGB(alphaforfont, 255, 255, 255));
					fontforalpha->printf(711, 390, HGETEXT_CENTER, "+%d", Hero::GetHero().GetModeScore());
				}
			}
			else
				fontfornumbers->printf(604,195, HGETEXT_RIGHT, "%d", Hero::GetHero().GetModeScore());
		}
		hge->Input_GetMousePos(&x, &y);
		menumouse->render(x, y, 0);
		hge->Gfx_EndScene();
		delete canvaspic;
		break;
		case LevelUpScene:
		{
						   canvaspic = new hgeSprite(hge->Target_GetTexture(canvas2), 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
						   canvaspic->SetZ(0.75);
						   hge->Gfx_BeginScene();
						   hge->Gfx_Clear(0xFF000000);
						   if (countpauseblur < 3)
						   {
							   countpauseblur += 0.1;
						   }
						   for (float a = 0.2; a <= 0.4*(1 + countpauseblur); a += 0.2)
						   {
							   canvaspic->SetColor(ARGB(255 - a * 159, 255, 255, 255));
							   canvaspic->Render(0 + a, 0);
							   canvaspic->Render(0 - a, 0);
							   canvaspic->Render(0, 0 + a);
							   canvaspic->Render(0, 0 - a);
						   }
						   RenderPool::GetRenderPool().RenderAfterBlur();
						   hge->Input_GetMousePos(&x, &y);
						   menumouse->render(x, y, 0);
						   hge->Gfx_EndScene();
						   delete canvaspic;
		}
			break;
	}
	hge->Target_Free(canvas);
	return false;//程序正常时总是返回false，返回true将从System_Start往下执行 
} 





bool FrameFunc() 
{   
	SFXManager::GetSFXManager().UpdatePool();
	switch (SceneManager::GetSceneManager().GetCurrentScene())
	{
	case GameScene:
		switch (SceneManager::GetSceneManager().GetCurrentMode())
		{
		case WaveMode:
			if (!gamesceneinit)
			{
				gameblackin = false;
				gamepause = false;
				gamepause2 = false;
				a = false;
				b = 10;
				c = 0;
				d = 0;
				startpos.x = 0;
				startpos.y = 0;
				countalpha = 255;
				generating = false;
				doubleknock = false;
				countdouble = 0;
				boss = false;
				gameover = false;
				std_score = 0;
				doubleklick = 0;
				open = 0;
				timecount = 0;
				gametime = 0;
				wavestart = false;
				startlevelup = false;
				startlevelup2 = false;
				generatetype = 0;
				NoticeManager::GetNoticeManager().Clean();
				SceneManager::GetSceneManager().WaveInit();
				SceneManager::GetSceneManager().SetWaveDelay(true);
				NoticeManager::GetNoticeManager().InsertNotice("GAME START", "Ready?");
				//读取存档
				ifstream infile;
				TCHAR exeFullPath[MAX_PATH]; // MAX_PATH
				GetModuleFileName(NULL, exeFullPath, MAX_PATH);//得到程序模块名称，全路径
				*_tcsrchr(exeFullPath, _T('\\')) = 0;
				CString str1, str2, allstr;
				str1 = "\\save.txt";
				str2 = exeFullPath;
				allstr = str2 + str1;
				infile.open(allstr, ios::in);
				char *s = new char[10];
				infile.getline(s, 10);
				std_score = atoi(s);
				infile.close();
				//内部定义类初始化：
				ObjectPool::GetObjPool().ResetTokens();
				Hero::GetHero().ResetHero();
				SFXManager::GetSFXManager().PlaySFX(GAMEBGM);
				barHP = new Bar(HPBAR);
				barHP->SetPos(85, 19);
				barHP->Link(0, 0);
				barMP = new Bar(MPBAR);
				barMP->SetPos(85, 45);
				barMP->Link(0, 0);
				barEXP = new Bar(EXPBAR);
				barEXP->SetPos(33, 464);
				barEXP->Link(0, 0);
				barSCORE = new Bar(SCOREBAR);
				barSCORE->SetPos(317, 35);
				barSCORE->Link(0, 0);
				barBOSSHP = new Bar(BOSSHPBAR);
				barBOSSHP->SetPos(47, 70);
				//以下三行将在Scene加入之后按照scene来载入（也就是说需要改地方）
				ObjectPool::GetObjPool().InsertObject(barHP);
				ObjectPool::GetObjPool().InsertObject(barMP);
				ObjectPool::GetObjPool().InsertObject(barEXP);
				ObjectPool::GetObjPool().InsertObject(barSCORE);
				ObjectPool::GetObjPool().InsertObject(barBOSSHP);
				gamesceneinit = true;
			}
			//怪物生成相关：
			//用wave来统计哪波该生成什么怪。。
			//wave0-4(一般怪物）
			//wave 5 奖励关卡
			//wave 10 BOSS
			if (!NoticeManager::GetNoticeManager().IsOccopied())
			{
				switch (generatetype)
				{
				case 0:
					break;
				case 1:
				{
						  Enemy *newenemy = new Enemy(hge->Random_Int(ACT_START_X, ACT_END_X), hge->Random_Int(ACT_START_Y, ACT_END_Y), hge->Random_Int(ENEMY, RINGBULLETBALL), 8);
						  newenemy->SetSize(2);
						  newenemy->SetMaxHP(newenemy->GetHP() * 8);
						  newenemy->SetHP(newenemy->GetMaxHP());
						  newenemy->SetDMG(newenemy->GetDMG() * 2);
						  newenemy->SetExp(16);
						  ObjectPool::GetObjPool().InsertObject(newenemy);
						  generatetype = 0;
				}
					break;
				case 2:
					for (int i = 1; i <= 3; i++)
					{
						Item *item = new Item((ACT_END_X - ACT_START_X) * i / 4, ACT_START_Y + (ACT_END_Y - ACT_START_Y) / 2, hge->Random_Int(13, 16));
						ObjectPool::GetObjPool().InsertObject(item);
					}
					generatetype = 0;
					break;
				case 3:
				{	SFXManager::GetSFXManager().PlaySFX(BOSSBGM);
				BOSS *b1 = new BOSS(BOSS2);
				ObjectPool::GetObjPool().InsertObject(b1);
				generatetype = 0;
				}
					break;
				case 4:
					int numofhole = hge->Random_Int(2, 5);
					for (int i = 0; i < numofhole; i++)
					{
						Point pos2;
						switch (hge->Random_Int(0, 3))
						{
						case 0:
							pos2.x = hge->Random_Int(ACT_START_X, ACT_END_X);
							pos2.y = hge->Random_Int(73, 123);
							break;
						case 1:
							pos2.x = hge->Random_Int(753, 803);
							pos2.y = hge->Random_Int(ACT_START_Y, ACT_END_Y);
							break;
						case 2:
							pos2.x = hge->Random_Int(ACT_START_X, ACT_END_X);
							pos2.y = hge->Random_Int(357, 407);
							break;
						case 3:
							pos2.x = hge->Random_Int(0, 50);
							pos2.y = hge->Random_Int(ACT_START_Y, ACT_END_Y);
							break;
						}
						int mobcount = 10 + 4 * Hero::GetHero().GetBossKills();
						BlackHole *bh = new BlackHole(pos2.x, pos2.y, mobcount * 300 / numofhole,BLACKHOLEFORENEMY);
						ObjectPool::GetObjPool().InsertObject(bh);
					}
					generatetype = 0;
					break;
				}

				if (ObjectPool::GetObjPool().IsStageCleared())
				{
					if (!SceneManager::GetSceneManager().GetWaveDelay())
					{
						generatetype = 2;
						NoticeManager::GetNoticeManager().InsertNotice("ITEMS!", "Enjoy!");
						SceneManager::GetSceneManager().SetWaveDelay(true);
					}
					else
					{
						SceneManager::GetSceneManager().SetWaveDelay(false);
						CString str = "Wave ", str2 = "", str3;
						if (SceneManager::GetSceneManager().GetWave() % 10 == 0)
						{
							generatetype = 3;
							NoticeManager::GetNoticeManager().InsertNotice("BOSS STAGE", "Keep alive!");
						}
						else
						{
							if (SceneManager::GetSceneManager().GetWave() % 5 == 0)
							{
								switch (0)
								{
								case 0:
									generatetype = 1;
									NoticeManager::GetNoticeManager().InsertNotice("Elite Unit!", "Waring!!!\na lot of EXP");
									break;
								}
							}
							else
							{

								generatetype = 4;
								countforskill = hge->Random_Int(8 - 8 * ((float)Hero::GetHero().GetLevel() / MAX_LEVEL), 16 - 15 * ((float)Hero::GetHero().GetLevel() / MAX_LEVEL));
								str2.Format("%d", SceneManager::GetSceneManager().GetWave());
								str3 = str + str2;
								NoticeManager::GetNoticeManager().InsertNotice(str3, "Start!");
							}
						}
						SceneManager::GetSceneManager().WaveClear();
					}
				}
				if (countforskill > 0)
				{
					b--;
					if (b < 0)
					{
						int direction = hge->Random_Int(0, 1);
						int random = hge->Random_Int(-40, -20);
						int itemx = random + direction*(ACT_END_X + random*(-2));
						ItemEnemy *newitem = new ItemEnemy(itemx, hge->Random_Int(ACT_START_Y, ACT_END_Y));
						newitem->SetType(0);
						ObjectPool::GetObjPool().InsertObject(newitem);
						b = hge->Random_Int(1000 + Hero::GetHero().GetLevel() * 20, 2000 + Hero::GetHero().GetLevel() * 40);
						countforskill--;
					}
				}
			}
			if (!Hero::GetHero().IsLocked())
			{
					if (hge->Input_GetKeyState(HGEK_W))
					{
						Hero::GetHero().AddDy(-(Hero::GetHero().GetSpeed() / SPEED));
						Hero::GetHero().SetState(WALK);
					}
					if (hge->Input_GetKeyState(HGEK_A))
					{
						Hero::GetHero().AddDx(-(Hero::GetHero().GetSpeed() / SPEED));
						Hero::GetHero().SetState(WALK);
					}
					if (hge->Input_GetKeyState(HGEK_S))
					{
						Hero::GetHero().AddDy(Hero::GetHero().GetSpeed() / SPEED);
						Hero::GetHero().SetState(WALK);
					}
					if (hge->Input_GetKeyState(HGEK_D))
					{
						Hero::GetHero().AddDx(Hero::GetHero().GetSpeed() / SPEED);
						Hero::GetHero().SetState(WALK);
					}
				if (hge->Input_KeyDown(HGEK_E))
					ObjectPool::GetObjPool().NextToken();
				if (hge->Input_KeyDown(HGEK_Q))
					ObjectPool::GetObjPool().LastToken();


				if (//Hero::GetHero().GetSKPoint()>=100 &&
					hge->Input_KeyDown(HGEK_RBUTTON))
				{
					ObjectPool::GetObjPool().UseSkillToken();

				}

				if (hge->Input_GetKeyState(HGEK_LBUTTON))
				{
					if (c < 0
						//&& open < 2
						)
					{
						for (int i = 0; i < Hero::GetHero().GetBullets(); i++)
						{
							Bullet *bullet = new Bullet(&Hero::GetHero(), BULLET1, 16, i);
							ObjectPool::GetObjPool().InsertObject(bullet);
							SFXManager::GetSFXManager().PlaySFX(SHOOTSFX);
						}

						c = Hero::GetHero().GetShootCD();
						if (Hero::GetHero().GetBattleState() == WEAKEN)
							c = 50;
						if (Hero::GetHero().GetBattleState() == RAGE)
							c = 5;
					}
				}
				c--;
			}
			//测试用：
			if (hge->Input_KeyDown(HGEK_Z))
			{
				//Hero::GetHero().SetMaxMP(0);
				Hero::GetHero().varyMP(100);
				Hero::GetHero().AddBulletStats(DtoBOOM);
			}

			if (hge->Input_KeyDown(HGEK_C))
			{
				Hero::GetHero().Suicide();
				Hero::GetHero().SetLives(2);
			}
			if (hge->Input_KeyDown(HGEK_X))
			{
				//ObjectPool::GetObjPool().GetSkillToken(TOXICFIELD);
				ObjectPool::GetObjPool().UnLockToken();
				Hero::GetHero().varyHP(1000);
				Hero::GetHero().SetBattleState(RAGE);
			}
			ObjectPool::GetObjPool().SkillEffect();
			if (hge->Input_KeyDown(HGEK_SPACE))test = !test;
			if (test)Sleep(100);
			break;
			case MazeMode:
				if (!gamesceneinit)
			{
					countpool.clear();
					countpool.push_back(make_pair(MAZEITEM_TOXICBULLET, false));
					countpool.push_back(make_pair(MAZEITEM_ICEBULLET, false));
					countpool.push_back(make_pair(MAZEITEM_DTODMG, false));
					countpool.push_back(make_pair(MAZEITEM_DTODMGDOWN, false));
					countpool.push_back(make_pair(MAZEITEM_STUNBULLET, false));
					countpool.push_back(make_pair(MAZEITEM_SUCKBLOOD, false));
					countpool.push_back(make_pair(MAZEITEM_FOLLOWBULLET, false));
					countpool.push_back(make_pair(MAZEITEM_HPUP, true));
					countpool.push_back(make_pair(MAZEITEM_MPUP, true));
					countpool.push_back(make_pair(MAZEITEM_HEALMORE, false));
					countpool.push_back(make_pair(MAZEITEM_DMGUP, true));
					countpool.push_back(make_pair(MAZEITEM_DEFUP, true));
				gameblackin = false;
				gamepause = false;
				gamepause2 = false;
				a = false;
				b = 10;
				c = 0;
				d = 0;
				startpos.x = 0;
				startpos.y = 0;
				countalpha = 255;
				generating = false;
				doubleknock = false;
				countdouble = 0;
				boss = false;
				gameover = false;
				std_score = 0;
				doubleklick = 0;
				open = 0;
				timecount = 0;
				gametime = 0;
				wavestart = false;
				startlevelup = false;
				startlevelup2 = false;
				generatetype = 0;
				NoticeManager::GetNoticeManager().Clean();
				NoticeManager::GetNoticeManager().InsertNotice("GAME START", "Ready?");
				//读取存档
				ifstream infile;
				TCHAR exeFullPath[MAX_PATH]; // MAX_PATH
				GetModuleFileName(NULL, exeFullPath, MAX_PATH);//得到程序模块名称，全路径
				*_tcsrchr(exeFullPath, _T('\\')) = 0;
				CString str1, str2, allstr;
				str1 = "\\save.txt";
				str2 = exeFullPath;
				allstr = str2 + str1;
				infile.open(allstr, ios::in);
				char *s = new char[10];
				infile.getline(s, 10);
				std_score = atoi(s);
				delete []s;
				infile.close();
				//内部定义类初始化：
				ObjectPool::GetObjPool().ResetTokens();
				Hero::GetHero().ResetHero();
				SFXManager::GetSFXManager().PlaySFX(GAMEBGM);
				barHP = new Bar(HPBAR);
				barHP->SetPos(85, 19);
				barHP->Link(0, 0);
				barMP = new Bar(MPBAR);
				barMP->SetPos(85, 45);
				barMP->Link(0, 0);
				barEXP = new Bar(EXPBAR);
				barEXP->SetPos(33, 464);
				barEXP->Link(0, 0);
				barSCORE = new Bar(SCOREBAR);
				barSCORE->SetPos(317, 35);
				barSCORE->Link(0, 0);
				barBOSSHP = new Bar(BOSSHPBAR);
				barBOSSHP->SetPos(47, 70);
				ObjectPool::GetObjPool().InsertObject(barHP);
				ObjectPool::GetObjPool().InsertObject(barMP);
				ObjectPool::GetObjPool().InsertObject(barEXP);
				ObjectPool::GetObjPool().InsertObject(barSCORE);
				ObjectPool::GetObjPool().InsertObject(barBOSSHP);
				ObjectPool::GetObjPool().CreateMaze();
				MazeToEvent = 0;
				EventName = 0;
				ObjectPool::GetObjPool().SetEventDirection(0);
				gamesceneinit = true;
			}
				Hero::GetHero().SetPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
				if (!MazeToEvent)
				{
					if (ObjectPool::GetObjPool().MazeMove() && !Hero::GetHero().IsLocked())
					{
						if (hge->Input_GetKeyState(HGEK_W))
						{
							ObjectPool::GetObjPool().AddDy(4);
							Hero::GetHero().SetRot(0);
							goto Finish;
						}
						if (hge->Input_GetKeyState(HGEK_A))
						{
							ObjectPool::GetObjPool().AddDx(4); 
							Hero::GetHero().SetRot(-M_PI/2);
							goto Finish;
						}
						if (hge->Input_GetKeyState(HGEK_S))
						{
							ObjectPool::GetObjPool().AddDy(-4);
							Hero::GetHero().SetRot(M_PI);
							goto Finish;
						}
						if (hge->Input_GetKeyState(HGEK_D))
						{
							ObjectPool::GetObjPool().AddDx(-4);
							Hero::GetHero().SetRot(M_PI/2);
						}
					Finish:
						;
					}
			        ObjectPool::GetObjPool().MazeEvent(eventx,eventy,EventName);
					if (EventName)
					{	
						MazeToEvent = 1;
						countalpha = 0;
					}
				}
				else
				{
					ObjectPool::GetObjPool().MazeMove();
					switch (MazeToEvent)
					{
					case 1:
						if (countalpha < 255)
						{
							countalpha += 5;
							blackin->SetAlpha(countalpha);
						}
						else
						{
							countalpha = 255;
							switch (ObjectPool::GetObjPool().GetEventDirection())
							{
							case DIRECTION_UP:
								Hero::GetHero().SetPos(SCREEN_WIDTH / 2, ACT_START_Y + 50);
								break;
							case DIRECTION_DOWN:
								Hero::GetHero().SetPos(SCREEN_WIDTH / 2, ACT_END_Y - 50);
								break;
							case DIRECTION_RIGHT:
								Hero::GetHero().SetPos(ACT_END_X - 50, SCREEN_HEIGHT / 2);
								break;
							case DIRECTION_LEFT:
								Hero::GetHero().SetPos(ACT_START_X + 50, SCREEN_HEIGHT / 2);
								break;
							default:
								break;
							}
							switch (EventName)
							{
							case 1:
								break;
							case 2:
								for (int i = 0; i < 5; i++)
								{
									BlackHole *bh = nullptr;
									switch (ObjectPool::GetObjPool().GetEventDirection())
									{
									case DIRECTION_UP:
										bh = new BlackHole(ACT_START_X + 100 + (ACT_END_X - 100 - ACT_START_X)*i / 5, ACT_END_Y - 50, 900 + (Hero::GetHero().GetLevel() / 4) * 600, BLACKHOLEARROW, ObjectPool::GetObjPool().GetEventDirection());
										break;
									case DIRECTION_DOWN:
										bh = new BlackHole(ACT_START_X + 100 + (ACT_END_X - 100 - ACT_START_X)*i / 5, ACT_START_Y + 50, 900 + (Hero::GetHero().GetLevel() / 4) * 600, BLACKHOLEARROW, ObjectPool::GetObjPool().GetEventDirection());
										break;
									case DIRECTION_RIGHT:
										bh = new BlackHole(ACT_START_X + 50, ACT_START_Y + 100 + (ACT_END_Y - 100 - ACT_START_Y)*i / 5, 900 + (Hero::GetHero().GetLevel() / 4) * 600, BLACKHOLEARROW, ObjectPool::GetObjPool().GetEventDirection());
										break;
									case DIRECTION_LEFT:
										bh = new BlackHole(ACT_END_X - 50, ACT_START_Y + 100 + (ACT_END_Y - 100 - ACT_START_Y)*i / 5, 900 + (Hero::GetHero().GetLevel() / 4) * 600, BLACKHOLEARROW, ObjectPool::GetObjPool().GetEventDirection());
										break;
									default:
										break;
									}
									if (bh)
										ObjectPool::GetObjPool().InsertObject(bh);
								}
								SceneManager::GetSceneManager().WaveClear();
								break;
							case 3:
							{
									  int type = 0, pos = hge->Random_Int(0, countpool.size() - 1);
									  for (int i = 0; i < padpool.size(); i++)
									  {
										  if (padpool[i].pos == make_pair(eventx, eventy))
										  {
											  type = padpool[i].data;
											  break;
										  }
									  }
									  if (!type)
									  {
										  if (!countpool[pos].second)
										  {
											  type = countpool[pos].first;
											  countpool.erase(countpool.begin() + pos);
										  }
										  else
											  type = countpool[pos].first;
										  PosAndData a;
										  a.pos = make_pair(eventx, eventy);
										  a.data = type;
										  padpool.push_back(a);
									  }
									  BlackHole *bh;
									  switch (ObjectPool::GetObjPool().GetEventDirection())
									  {
									  case DIRECTION_UP:
										  bh = new BlackHole(SCREEN_WIDTH/2, ACT_START_Y + 50,0,BLACKHOLEFORHERO);
										  break;
									  case DIRECTION_DOWN:
										  bh = new BlackHole(SCREEN_WIDTH / 2, ACT_END_Y - 50,0, BLACKHOLEFORHERO);
										  break;
									  case DIRECTION_RIGHT:
										  bh = new BlackHole(ACT_END_X - 50, SCREEN_HEIGHT / 2,0, BLACKHOLEFORHERO);
										  break;
									  case DIRECTION_LEFT:
										  bh = new BlackHole(ACT_START_X + 50, SCREEN_HEIGHT / 2,0, BLACKHOLEFORHERO);
										  break;
									  default:
										  break;
									  }
									  if (bh)
										  ObjectPool::GetObjPool().InsertObject(bh);
									  Item *item = new Item(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, type);
									  ObjectPool::GetObjPool().InsertObject(item);
							}
								break;
							case 5:
								Hero::GetHero().BeTheGod(50);
								BlackHole *bh1,*bh2,*bh3,*bh4;
								if (!ObjectPool::GetObjPool().Maze_IsWall(eventx, eventy-1))
								{
									bh1 = new BlackHole(SCREEN_WIDTH / 2, ACT_START_Y + 50, 0,BLACKHOLEFORMINE, DIRECTION_UP);
									ObjectPool::GetObjPool().InsertObject(bh1);
								}
								if (!ObjectPool::GetObjPool().Maze_IsWall(eventx, eventy + 1))
								{
									bh2 = new BlackHole(SCREEN_WIDTH / 2, ACT_END_Y - 50, 0, BLACKHOLEFORMINE, DIRECTION_DOWN);
									ObjectPool::GetObjPool().InsertObject(bh2);
								}
								if (!ObjectPool::GetObjPool().Maze_IsWall(eventx+1, eventy))
								{
									bh3 = new BlackHole(ACT_END_X - 50, SCREEN_HEIGHT / 2, 0, BLACKHOLEFORMINE, DIRECTION_RIGHT);
									ObjectPool::GetObjPool().InsertObject(bh3);
								}
								if (!ObjectPool::GetObjPool().Maze_IsWall(eventx - 1, eventy))
								{
									bh4 = new BlackHole(ACT_START_X + 50, SCREEN_HEIGHT / 2, 0, BLACKHOLEFORMINE, DIRECTION_LEFT);
									ObjectPool::GetObjPool().InsertObject(bh4);
								}
								break;
							case 6:
							{
									  BOSS *boss = new BOSS(BOSS1);
									  ObjectPool::GetObjPool().InsertObject(boss);
							}
								break;
							default:
								break;
							}

							SceneManager::GetSceneManager().SetGameMode(MazeEvent);
							ObjectPool::GetObjPool().ResetDxDy();
							ObjectPool::GetObjPool().BreakMaze(eventx, eventy);
						}
						break;
					case 2:
						if (countalpha > 0)
						{
							countalpha -= 5;
							blackin->SetAlpha(countalpha);
						}
						else
						{
							MazeToEvent = 0;
							blackin->SetAlpha(0);
						}
						break;
					}
				}
			break;
			case MazeEvent:
					if (!Hero::GetHero().IsLocked())
					{
						if (hge->Input_GetKeyState(HGEK_W))
						{
							Hero::GetHero().AddDy(-(Hero::GetHero().GetSpeed() / SPEED));
							Hero::GetHero().SetState(WALK);
						}
						if (hge->Input_GetKeyState(HGEK_A))
						{
							Hero::GetHero().AddDx(-(Hero::GetHero().GetSpeed() / SPEED));
							Hero::GetHero().SetState(WALK);
						}
						if (hge->Input_GetKeyState(HGEK_S))
						{
							Hero::GetHero().AddDy((Hero::GetHero().GetSpeed() / SPEED));
							Hero::GetHero().SetState(WALK);
						}
						if (hge->Input_GetKeyState(HGEK_D))
						{
							Hero::GetHero().AddDx((Hero::GetHero().GetSpeed() / SPEED));
							Hero::GetHero().SetState(WALK);
						}
						if (hge->Input_KeyDown(HGEK_E))
							ObjectPool::GetObjPool().NextToken();
						if (hge->Input_KeyDown(HGEK_Q))
							ObjectPool::GetObjPool().LastToken();
						if (//Hero::GetHero().GetSKPoint()>=100 &&
							hge->Input_KeyDown(HGEK_RBUTTON))
						{
							ObjectPool::GetObjPool().UseSkillToken();

						}
						if (hge->Input_GetKeyState(HGEK_UP))
						{
							Hero::GetHero().SetRot(0);
							goto Finish2;
						}
						if (hge->Input_GetKeyState(HGEK_LEFT))
						{
							Hero::GetHero().SetRot(-M_PI / 2);
							goto Finish2;
						}
						if (hge->Input_GetKeyState(HGEK_DOWN))
						{
							Hero::GetHero().SetRot(M_PI);
							goto Finish2;
						}
						if (hge->Input_GetKeyState(HGEK_RIGHT))
						{
							Hero::GetHero().SetRot(M_PI / 2);
						}
					Finish2:
						;
						if (hge->Input_GetKeyState(HGEK_UP) || hge->Input_GetKeyState(HGEK_DOWN)
						 || hge->Input_GetKeyState(HGEK_LEFT) || hge->Input_GetKeyState(HGEK_RIGHT))
						{
							if (c < 0
								//&& open < 2
								)
							{
								for (int i = 0; i < Hero::GetHero().GetBullets(); i++)
								{
									Bullet *bullet = new Bullet(&Hero::GetHero(), BULLET1, 16, i);
									ObjectPool::GetObjPool().InsertObject(bullet);
									SFXManager::GetSFXManager().PlaySFX(SHOOTSFX);
								 }

								c = Hero::GetHero().GetShootCD();
							}
						}
						c--;
					}
					if (MazeToEvent!=2)
					{
						if ((EventName == 2 || EventName == 6) && ObjectPool::GetObjPool().IsStageCleared())
						{
							EventName = 0;
							ObjectPool::GetObjPool().SetEventDirection(0);
							MazeToEvent = 2;
						}
						if ((EventName==3 || EventName==5) && ObjectPool::GetObjPool().IsMazeSave())
						{
							if (ObjectPool::GetObjPool().GetNeedCleanPadPool() && padpool.size() > 0)
							{
								padpool.erase(padpool.begin());
								ObjectPool::GetObjPool().SetNeedCleanPadPool(false);
							}
							else
							{
								switch (EventName)
								{
								case 3:
									ObjectPool::GetObjPool().SaveMazePart(eventx, eventy, MAZEITEM);
									break;
								case 5:
									ObjectPool::GetObjPool().SaveMazePart(eventx, eventy, MAZEMINES);
									break;
								}
								switch (ObjectPool::GetObjPool().GetEventDirection())
								{
								case DIRECTION_DOWN:
									ObjectPool::GetObjPool().ResetMazePos(eventx, eventy + 1);
									break;
								case DIRECTION_UP:
									ObjectPool::GetObjPool().ResetMazePos(eventx, eventy - 1);
									break;
								case DIRECTION_LEFT:
									ObjectPool::GetObjPool().ResetMazePos(eventx - 1, eventy);
									break;
								case DIRECTION_RIGHT:
									ObjectPool::GetObjPool().ResetMazePos(eventx + 1, eventy);
									break;
								}
							}
							EventName = 0;
							ObjectPool::GetObjPool().SetEventDirection(0);
							MazeToEvent = 2;
						}
					}
					ObjectPool::GetObjPool().SkillEffect();
					switch (MazeToEvent)
					{
					case 1:
						if (countalpha > 0)
						{
							countalpha -= 5;
							blackin->SetAlpha(countalpha);
						}
						else
						{
							MazeToEvent = 0;
							blackin->SetAlpha(0);
						}
						break;
					case 2:
						if (countalpha < 255)
						{
							countalpha += 5;
							blackin->SetAlpha(countalpha);
						}
						else
						{
							ObjectPool::GetObjPool().KillAllEnemies();
							SceneManager::GetSceneManager().SetGameMode(MazeMode);
						}
						break;
					}
				break;
		}

		if (!gameblackin)
		if (countalpha > 0)
		{
			countalpha -= 5;
			blackin->SetAlpha(countalpha);
		}
		else
		{
			gameblackin = true;
			blackin->SetAlpha(0);
		}
		//游戏时间统计：
		if (timecount == 50)
		{
			gametime++;
			timecount = 0;
		}
		timecount++;
		if (!ObjectPool::GetObjPool().IsBossDied())
		{
			int HPmax = 0, HPcur = 0;
			for (int i = 0; i < ObjectPool::GetObjPool().GetPool().size();i++)
			if (ObjectPool::GetObjPool().GetPool()[i]->IsBoss())
			{
				HPmax += ObjectPool::GetObjPool().GetPool()[i]->GetMaxHP();
				HPcur += ObjectPool::GetObjPool().GetPool()[i]->GetHP();
			}
			barBOSSHP->Link(HPcur, HPmax);
		}
		else
			barBOSSHP->Link(0, 0);
		barHP->Link(Hero::GetHero().GetHP(), Hero::GetHero().GetMaxHP());
		barMP->Link(Hero::GetHero().GetMP(), Hero::GetHero().GetMaxMP());
		barEXP->Link(Hero::GetHero().GetEXP(), Hero::GetHero().GetMaxEXP());
		barSCORE->Link(Hero::GetHero().GetScore(), std_score);
		RenderPool::GetRenderPool().Shuffle();
		FontPool::GetFontPool().Delete();
		ObjectPool::GetObjPool().UpdateObject();
		ObjectPool::GetObjPool().DelectObject();
		Hero::GetHero().action();
		Hero::GetHero().born();
		if (Hero::GetHero().LevelUp())
			startlevelup = true;
		Hero::GetHero().death();
		if (startlevelup2)
		{
			startlevelup2 = false;
			startlevelup = false;
			SceneManager::GetSceneManager().GoToOneScene(LevelUpScene, false);
		}
		if (hge->Input_KeyDown(HGEK_P) || hge->Input_KeyDown(HGEK_ESCAPE))
			gamepause = true;
		if (gamepause2)
		{
			pausemenu->SetAlpha(0);
			countpauseblur = 0;
			pausemenualpha = 0;
			gamepause = false;
			gamepause2 = false;
			SceneManager::GetSceneManager().GoToOneScene(PauseScene, false);
		}
		//游戏结束条件（完美：需要再render一次再退出）
		if (gameover){
			SceneManager::GetSceneManager().CleanOneScene(gamesceneinit);
			TCHAR exeFullPath[MAX_PATH]; // MAX_PATH
			GetModuleFileName(NULL, exeFullPath, MAX_PATH);//得到程序模块名称，全路径
			*_tcsrchr(exeFullPath, _T('\\')) = 0;
			CString str1, str2, allstr;
			str1 = "\\save.txt";
			str2 = exeFullPath;
			allstr = str2 + str1;
			remove(allstr);
			ofstream of;
			of.open(allstr, ios::_Noreplace);
			CString strMsg4;
			if (Hero::GetHero().GetScore() > std_score)
			{
				strMsg4 = _T("NEW BEST!!!");
				of << Hero::GetHero().GetScore();
			}
			else
			{
				strMsg4.Format("THE BEST SCORE: %d", std_score);
				of << std_score;
			}
			of.close();
			/*
			CString strMsg1;
			CString strMsg2;
			CString strMsg3;

			strMsg2 = _T("\r\n");
			strMsg1 = _T("YOU DIED !");
			strMsg3.Format("SCORE:%d", Hero::GetHero().GetScore());
			strMsg1 = strMsg1 + strMsg2 + strMsg3 + strMsg2 + strMsg4;
			MessageBox(NULL, strMsg1, "GAME OVER!!", MB_OK);
			*/
			//SendMessage(hge->System_GetState(HGE_HWND), WM_CLOSE, 0, 0);
			//这个要有一个restart
			//return true;
			SceneManager::GetSceneManager().GoToOneScene(GameOverScene);
		}
		if (SceneManager::GetSceneManager().GetCurrentScene() != GameScene)
		if (SceneManager::GetSceneManager().ReturnAllclean() || SceneManager::GetSceneManager().ReturnPartClean())
			SceneManager::GetSceneManager().CleanOneScene(gamesceneinit);
		break;
	case LoadingScene:
		break;
	case MenuScene:
		if (!menusceneinit)
		{
			selectlayer1->SetColor(ARGB(255, 255, 255, 255),false);
			selectlayer2->SetColor(ARGB(255, 255, 255, 255), false);
			RenderPool::GetRenderPool().ResetAll();
			FontPool::GetFontPool().ResetAll();
			SFXManager::GetSFXManager().PlaySFX(MENUBGM);
			SceneManager::GetSceneManager().TitleInitFinish(false);
			count2inmenu = 0;
			countinmenu = 0;
			countalpha = 255;
			finishiblackin = false;
			layer1x = 853;
			layer2x = 853;
			layer1y = 0;
			layer2y = 0;
			SceneManager::GetSceneManager().SetMenuStage(-1);
			Button *button5 = new Button(853 + 26, 419, MenuScene, BUTTON_5_OFF, 42, 45);
			ObjectPool::GetObjPool().InsertObject(button5);
			Button *button6 = new Button(853 + 110, 303 - 159, MenuScene, BUTTON_6_OFF, 159, 159);
			ObjectPool::GetObjPool().InsertObject(button6);
			Button *button7 = new Button(853 + 350, 303 - 159, MenuScene, BUTTON_7_OFF, 159, 159);
			ObjectPool::GetObjPool().InsertObject(button7);
			Button *button8 = new Button(853 + 589, 303 - 159, MenuScene, BUTTON_8_OFF, 159, 159);
			ObjectPool::GetObjPool().InsertObject(button8);
			ZoomObject *title = new ZoomObject(TITLE);
			ObjectPool::GetObjPool().InsertObject(title);
			for (int i = 0; i < 60; i++)
			{
				FloatingObject *obj;
				switch (hge->Random_Int(0, 4))
				{
				case 0:
					obj = new FloatingObject(hge->Random_Int(0, SCREEN_WIDTH), hge->Random_Int(0, SCREEN_HEIGHT), hge->Random_Float(0.3, 0.8));
					break;
				case 1:
					obj = new FloatingObject(hge->Random_Int(0, SCREEN_WIDTH), hge->Random_Int(0, SCREEN_HEIGHT), hge->Random_Float(0.3, 0.8), 71);
					break;
				case 2:
					obj = new FloatingObject(hge->Random_Int(0, SCREEN_WIDTH), hge->Random_Int(0, SCREEN_HEIGHT), hge->Random_Float(0.3, 0.8), 72);
					break;
				case 3:
					obj = new FloatingObject(hge->Random_Int(0, SCREEN_WIDTH), hge->Random_Int(0, SCREEN_HEIGHT), hge->Random_Float(0.3, 0.5), 73);
					break;
				case 4:
					obj = new FloatingObject(hge->Random_Int(0, SCREEN_WIDTH), hge->Random_Int(0, SCREEN_HEIGHT), hge->Random_Float(0.3, 0.7), 31);
					break;
				}
				ObjectPool::GetObjPool().InsertObject(obj);
			}
			for (int i = 0; i < 3; i++)
			{
				FloatingObject *obj;
				switch (hge->Random_Int(0, 4))
				{
				case 0:
					obj = new FloatingObject(hge->Random_Int(0, SCREEN_WIDTH), hge->Random_Int(0, SCREEN_HEIGHT), hge->Random_Float(2, 2.5));
					break;
				case 1:
					obj = new FloatingObject(hge->Random_Int(0, SCREEN_WIDTH), hge->Random_Int(0, SCREEN_HEIGHT), hge->Random_Float(2, 2.6), 71);
					break;
				case 2:
					obj = new FloatingObject(hge->Random_Int(0, SCREEN_WIDTH), hge->Random_Int(0, SCREEN_HEIGHT), hge->Random_Float(2, 3.2), 72);
					break;
				case 3:
					obj = new FloatingObject(hge->Random_Int(0, SCREEN_WIDTH), hge->Random_Int(0, SCREEN_HEIGHT), hge->Random_Float(2, 2.1), 73);
					break;
				case 4:
					obj = new FloatingObject(hge->Random_Int(0, SCREEN_WIDTH), hge->Random_Int(0, SCREEN_HEIGHT), hge->Random_Float(2, 3), 31);
					break;
				}
				ObjectPool::GetObjPool().InsertObject(obj);
			}
			menusceneinit = true;
		}
		if (SceneManager::GetSceneManager().GetTitleFinish())
		{
			if (count2inmenu == 0)
			{
				Button *button1 = new Button(360, 206, MenuScene, BUTTON_1_OFF, 122, 30);
				ObjectPool::GetObjPool().InsertObject(button1);
			}

			if (count2inmenu == 10)
			{
				Button *button2 = new Button(360, 258, MenuScene, BUTTON_2_OFF, 122, 30);
				ObjectPool::GetObjPool().InsertObject(button2);
			}
			if (count2inmenu == 20)
			{
				Button *button3 = new Button(360, 312, MenuScene, BUTTON_3_OFF, 122, 30);
				ObjectPool::GetObjPool().InsertObject(button3);
			}
			if (count2inmenu == 30)
			{
				Button *button4 = new Button(360, 363, MenuScene, BUTTON_4_OFF, 122, 30);
				ObjectPool::GetObjPool().InsertObject(button4);
			}
			if (count2inmenu<100)count2inmenu++;
		}
		if (!finishiblackin)
		if (countalpha > 0)
			countalpha -= 5;
		else
		{
			SceneManager::GetSceneManager().SetMenuStage(0);
			finishiblackin = true;
			blackin->SetZ(0.65);
		}
		blackin->SetAlpha(countalpha);
		ObjectPool::GetObjPool().UpdateObject();
		ObjectPool::GetObjPool().DelectObject();

		switch (ObjectPool::GetObjPool().GetNowDirection())
		{
		case Left:
			layer1x -= ObjectPool::GetObjPool().GetGlobalDelta();
			layer2x -= ObjectPool::GetObjPool().GetGlobalDelta();
			break;
		case Right:
			layer1x += ObjectPool::GetObjPool().GetGlobalDelta();
			layer2x += ObjectPool::GetObjPool().GetGlobalDelta();
			break;
		case Up:
			layer1y -= ObjectPool::GetObjPool().GetGlobalDelta();
			layer2y -= ObjectPool::GetObjPool().GetGlobalDelta();
			break;
		case Down:
			layer1y += ObjectPool::GetObjPool().GetGlobalDelta();
			layer2y += ObjectPool::GetObjPool().GetGlobalDelta();
			break;
		}
		switch (SceneManager::GetSceneManager().ReturnMenuStage())
		{
		case 0:
			if (countalpha > 0)countalpha--;
			break;
		case 1:
			if (countalpha < 80)
				countalpha++;
			else
				SceneManager::GetSceneManager().SetMenuStage(2);
			break;
		case 3:
			if (countalpha < 255)countalpha++;
			else
			{
				blackin->SetZ(0.1);
				SceneManager::GetSceneManager().GoToOneScene(GameScene);
				SceneManager::GetSceneManager().SetGameMode(WaveMode);
			}
			break;
		case 4:
			if (countalpha < 255)countalpha++;
			else
			{
				blackin->SetZ(0.1);
				SceneManager::GetSceneManager().GoToOneScene(GameScene);
				SceneManager::GetSceneManager().SetGameMode(MazeMode);
			}
			break;
		}




		if (countinmenu == 0)
		{
			countinmenu = hge->Random_Int(10, 20);
			FloatingObject *obj;
			switch (hge->Random_Int(0, 4))
			{
			case 0:
				obj = new FloatingObject(hge->Random_Int(0, SCREEN_WIDTH), SCREEN_HEIGHT + hge->Random_Int(10, 80), hge->Random_Float(0.1, 1.2));
				break;
			case 1:
				obj = new FloatingObject(hge->Random_Int(0, SCREEN_WIDTH), SCREEN_HEIGHT + hge->Random_Int(10, 80), hge->Random_Float(0.2, 1.2), 71);
				break;
			case 2:
				obj = new FloatingObject(hge->Random_Int(0, SCREEN_WIDTH), SCREEN_HEIGHT + hge->Random_Int(10, 80), hge->Random_Float(0.3, 1.2), 72);
				break;
			case 3:
				obj = new FloatingObject(hge->Random_Int(0, SCREEN_WIDTH), SCREEN_HEIGHT + hge->Random_Int(10, 80), hge->Random_Float(0.4, 1.2), 73);
				break;
			case 4:
				obj = new FloatingObject(hge->Random_Int(0, SCREEN_WIDTH), SCREEN_HEIGHT + hge->Random_Int(10, 80), hge->Random_Float(0.5, 1.2), BOSS1);
				break;
			}
			ObjectPool::GetObjPool().InsertObject(obj);
		}
		countinmenu--;
		if (countinmenu == 19)
		{
			FloatingObject *obj;
			switch (hge->Random_Int(0, 4))
			{
			case 0:
				obj = new FloatingObject(hge->Random_Int(0, SCREEN_WIDTH), SCREEN_HEIGHT + hge->Random_Int(10, 80), hge->Random_Float(1.7, 2.4));
				break;
			case 1:
				obj = new FloatingObject(hge->Random_Int(0, SCREEN_WIDTH), SCREEN_HEIGHT + hge->Random_Int(10, 80), hge->Random_Float(1.7, 2.2), 71);
				break;
			case 2:
				obj = new FloatingObject(hge->Random_Int(0, SCREEN_WIDTH), SCREEN_HEIGHT + hge->Random_Int(10, 80), hge->Random_Float(1.7, 2.3), 72);
				break;
			case 3:
				obj = new FloatingObject(hge->Random_Int(0, SCREEN_WIDTH), SCREEN_HEIGHT + hge->Random_Int(10, 80), hge->Random_Float(1.7, 2.2), 73);
				break;
			case 4:
				obj = new FloatingObject(hge->Random_Int(0, SCREEN_WIDTH), SCREEN_HEIGHT + hge->Random_Int(10, 80), hge->Random_Float(1.8, 2.1), BOSS1);
				break;
			}
			ObjectPool::GetObjPool().InsertObject(obj);
		}
		if (SceneManager::GetSceneManager().GetCurrentScene() != MenuScene)
			SceneManager::GetSceneManager().CleanOneScene(menusceneinit);
		break;
	case PauseScene:
		if (!pausesceneinit)
		{
			countpause = 0;
			countpauseblur = 0;
			pausemenualpha = 0;
			pausesceneinit = true;
		}
		if (pausemenualpha < 255)
			pausemenualpha += 5;
		else
		{
			pausemenualpha = 255;
			if (countpause < 100)countpause++;
			switch (countpause)
			{
			case 1:
			{
					  Button *bot = new Button(377, 162, PauseScene, BUTTON_9_OFF, 122, 30);
					  ObjectPool::GetObjPool().InsertObject(bot);
					  break;
			}
			case 6:
			{
					   Button *bot = new Button(377, 222, PauseScene, BUTTON_10_OFF, 122, 30);
					   ObjectPool::GetObjPool().InsertObject(bot);
					   break;
			}
			case 11:
			{
					   Button *bot = new Button(377, 279, PauseScene, BUTTON_11_OFF, 122, 30);
					   ObjectPool::GetObjPool().InsertObject(bot);
					   break;
			}
			case 16:
			{
					   Button *bot = new Button(377, 337, PauseScene, BUTTON_12_OFF, 122, 30);
					   ObjectPool::GetObjPool().InsertObject(bot);
					   break;
			}
			case 21:
			{
					   Button *bot = new Button(363 - 21, 441 - 22.5, PauseScene, BUTTON_13_OFF, 42, 45);//21,22.5
					   ObjectPool::GetObjPool().InsertObject(bot);
					   break;
			}
			case 26:
			{
					   Button *bot = new Button(440 - 21, 441 - 22.5, PauseScene, BUTTON_14_OFF, 42, 45);
					   ObjectPool::GetObjPool().InsertObject(bot);
					   break;
			}
			case 31:
			{
					   Button *bot = new Button(514 - 21, 441 - 22.5, PauseScene, BUTTON_15_OFF, 42, 45);
					   ObjectPool::GetObjPool().InsertObject(bot);
					   break;
			}
			}
		}
		ObjectPool::GetObjPool().UpdateObject();
		ObjectPool::GetObjPool().DelectObject();
		pausemenu->SetAlpha(pausemenualpha);
		if (hge->Input_KeyDown(HGEK_P) || hge->Input_KeyDown(HGEK_ESCAPE))
		{
			SceneManager::GetSceneManager().GoToOneScene(GameScene, false, true);
			hge->Target_Free(canvas2);
		}
		if (SceneManager::GetSceneManager().GetCurrentScene() == MenuScene)
		{
			SceneManager::GetSceneManager().CleanOneScene(gamesceneinit);
			hge->Target_Free(canvas2);
		}
		if (SceneManager::GetSceneManager().GetCurrentScene() == GameScene && SceneManager::GetSceneManager().ReturnAllclean())
		{
			SceneManager::GetSceneManager().CleanOneScene(gamesceneinit);
			hge->Target_Free(canvas2);
		}
		if (SceneManager::GetSceneManager().GetCurrentScene() != PauseScene)
		{
			SceneManager::GetSceneManager().CleanOneScene(pausesceneinit);
			hge->Target_Free(canvas2);
		}
		break;
	case GameOverScene:
		if (!resultsceneinit)
		{
			Hero::GetHero().SetPos(753, 226);
			Hero::GetHero().SetImageAngle(-0.785);
			if (Hero::GetHero().GetLives() == 0)
				Hero::GetHero().SetLives(1);
			countresult = 0;
			SFXManager::GetSFXManager().PlaySFX(RESULTBGM);
			Button *bot = new Button(440 - 21, 441 - 22.5, GameOverScene, BUTTON_14_OFF, 42, 45);
			ObjectPool::GetObjPool().InsertObject(bot);
			finalscore = 0;
			resultalpha = 0;
			resultsceneinit = true;
			stage = 1;
			charge = 0;
			alphaforfont = 255;
			addscore = 0;
			finalscore = 0;
			isaddedscore = false;
			pauseforstage = false;
			pauseforstage2 = false;
		}
		switch (stage)
		{
		case 11:
				
				break;
		case 10:
			AddUntil(stage, charge, gametime, pauseforstage);
			if (pauseforstage2)
			{
				if (!isaddedscore)
				{
					finalscore += gametime;
					isaddedscore = true;
				}
				if (addscore < finalscore)
					addscore++;
				else
				{
					pauseforstage2 = false;
					isaddedscore = false;
					stage++;
				}
			}
			break;
		case 9:
			AddUntil(stage, charge, Hero::GetHero().GetLives(), pauseforstage);
			if (pauseforstage2 && stage == 9)
			{
				if (!isaddedscore)
				{
					finalscore *= Hero::GetHero().GetLives();
					isaddedscore = true;
				}
				if (addscore < finalscore)
					addscore++;
				else
				{
					pauseforstage2 = false;
					isaddedscore = false;
					stage++;
				}
			}
			break;
		case 8:
			AddUntil(stage, charge, Hero::GetHero().GetMP(), pauseforstage);
			if (pauseforstage2)
			{
				if (!isaddedscore)
				{
					finalscore += Hero::GetHero().GetMP();
					isaddedscore = true;
				}
				if (addscore < finalscore)
					addscore++;
				else
				{
					pauseforstage2 = false;
					isaddedscore = false;
					stage++;
				}
			}
			break;
		case 7:
			AddUntil(stage, charge, Hero::GetHero().GetHP(), pauseforstage);
			if (pauseforstage2)
			{
				if (!isaddedscore)
				{
					finalscore += Hero::GetHero().GetHP();
					isaddedscore = true;
				}
				if (addscore < finalscore)
					addscore++;
				else
				{
					pauseforstage2 = false;
					isaddedscore = false;
					stage++;
				}
			}
			break;
		case 6:
			AddUntil(stage, charge, Hero::GetHero().GetAllEXP(), pauseforstage);
			if (pauseforstage2)
			{
				if (!isaddedscore)
				{
					finalscore += Hero::GetHero().GetAllEXP();
					isaddedscore = true;
				}
				if (addscore < finalscore)
					addscore++;
				else
				{
					pauseforstage2 = false;
					isaddedscore = false;
					stage++;
				}
			}
			break;
		case 5:
			AddUntil(stage, charge, Hero::GetHero().GetBossKills(), pauseforstage);
			if (pauseforstage2)
			{
				if (!isaddedscore)
				{
					finalscore += Hero::GetHero().GetBossKills();
					isaddedscore = true;
				}
				if (addscore < finalscore)
					addscore++;
				else
				{
					pauseforstage2 = false;
					isaddedscore = false;
					stage++;
				}
			}
			break;
		case 4:
			AddUntil(stage, charge, Hero::GetHero().GetItem(), pauseforstage);
			if (pauseforstage2)
			{
				if (!isaddedscore)
				{
					finalscore += Hero::GetHero().GetItem();
					isaddedscore = true;
				}
				if (addscore < finalscore)
					addscore++;
				else
				{
					pauseforstage2 = false;
					isaddedscore = false;
					stage++;
				}
			}
			break;
		case 3:
			AddUntil(stage, charge, Hero::GetHero().GetMaxCombo(), pauseforstage);
			if (pauseforstage2)
			{
				if (!isaddedscore)
				{
					finalscore += Hero::GetHero().GetMaxCombo();
					isaddedscore = true;
				}
				if (addscore < finalscore)
					addscore++;
				else
				{
					pauseforstage2 = false;
					isaddedscore = false;
					stage++;
				}
			}
			break;
		case 2:
			AddUntil(stage, charge, Hero::GetHero().GetKills(), pauseforstage);
			if (pauseforstage2)
			{
				if (!isaddedscore)
				{
					finalscore += Hero::GetHero().GetKills();
					isaddedscore = true;
				}
				if (addscore < finalscore)
					addscore++;
				else
				{
					pauseforstage2 = false;
					pauseforstage = false;
					isaddedscore = false;
					stage++;
				}
			}
			break;
		case 1:
			AddUntil(stage, charge, Hero::GetHero().GetModeScore(),pauseforstage);
			if (pauseforstage2)
			{
				if (!isaddedscore)
				{
					finalscore += Hero::GetHero().GetModeScore();
					isaddedscore = true;
				}
				if (addscore < finalscore)
					addscore++;
				else
				{
					pauseforstage2 = false;
					isaddedscore = false;
					stage++;
				}
			}
			break;
		}
		if (hge->Input_KeyDown(HGEK_SPACE))
		{
			stage = 11;
			int finalscorecopy =
				Hero::GetHero().GetModeScore()
				+ Hero::GetHero().GetKills()
				+ Hero::GetHero().GetItem()
				+ Hero::GetHero().GetMaxCombo()
				+ Hero::GetHero().GetBossKills()
				+ Hero::GetHero().GetAllEXP()
				+ Hero::GetHero().GetHP()
				+ Hero::GetHero().GetMP();
			finalscorecopy *= Hero::GetHero().GetLives();
			finalscorecopy += gametime;
			addscore = finalscorecopy;
		}
		if (resultalpha < 255)
		{
			resultalpha+=5;
			resultbg->SetColor(ARGB(resultalpha, 255, 255, 255),false);
		}

		ObjectPool::GetObjPool().UpdateObject();
		ObjectPool::GetObjPool().DelectObject();
		if (SceneManager::GetSceneManager().GetCurrentScene() != GameOverScene)
			SceneManager::GetSceneManager().CleanOneScene(resultsceneinit);
		break;
		case LevelUpScene:
			if (!levelupsceneinit)
			{
				Button *a = new Button(SCREEN_WIDTH / 5 - 64, SCREEN_HEIGHT / 2 - 64, LevelUpScene, BUTTON_17_OFF, 128, 128);
				Button *b = new Button(SCREEN_WIDTH / 2 - 64, SCREEN_HEIGHT / 2 - 64, LevelUpScene, BUTTON_18_OFF, 128, 128);
				Button *c = new Button(SCREEN_WIDTH*4 / 5 - 64, SCREEN_HEIGHT / 2 - 64, LevelUpScene, BUTTON_19_OFF, 128, 128);
				ObjectPool::GetObjPool().InsertObject(a);
				ObjectPool::GetObjPool().InsertObject(b);
				ObjectPool::GetObjPool().InsertObject(c);
				levelupsceneinit = true;
			}
			ObjectPool::GetObjPool().UpdateObject();
			ObjectPool::GetObjPool().DelectObject();
			if (SceneManager::GetSceneManager().GetCurrentScene() != LevelUpScene)
				SceneManager::GetSceneManager().CleanOneScene(levelupsceneinit);
			break;
	}
	if (SceneManager::GetSceneManager().IsExit())
	{
		SendMessage(hge->System_GetState(HGE_HWND), WM_CLOSE, 0, 0);
		return true;
	}
	return false;//总是返回false 
} 


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)//WinMain函数，程序的入口。 
{ 



hge=hgeCreate(HGE_VERSION);//使用hgeCreate函数创建HGE接口，参数必须传递正确的HGE_VERSION,它是在hge.h中定义的 
hge->System_SetState(HGE_SCREENWIDTH, SCREEN_WIDTH);//屏幕宽度 
hge->System_SetState(HGE_SCREENHEIGHT,SCREEN_HEIGHT);//屏幕高度
hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);//设置逻辑函数为FrameFunc函数 
hge->System_SetState(HGE_RENDERFUNC,RenderFunc);//设置绘制函数为RenderFunc函数 
hge->System_SetState(HGE_TITLE, "EScope Alpha");//设置窗口标题 
hge->System_SetState(HGE_WINDOWED,true);//设置使用窗口模式 
hge->System_SetState(HGE_LOGFILE,"game.log.txt");
hge->System_SetState(HGE_USESOUND,true);
hge->System_SetState(HGE_ZBUFFER,true); 
hge->System_SetState(HGE_SHOWSPLASH,false);
hge->System_SetState(HGE_SCREENBPP, 32);
hge->System_SetState(HGE_FPS,FPS);
hge->System_SetState(HGE_HIDEMOUSE,true);
hge->System_SetState(HGE_FXVOLUME, 0);
if(hge->System_Initiate())//用hge类的System_Initiate()方法，检测初始化是否有错误出现。 
{ 

	CWinThread* pThread = AfxBeginThread((AFX_THREADPROC)ThreadFunc, NULL);
	TCHAR exeFullPath[MAX_PATH]; // MAX_PATH
	GetModuleFileName(NULL, exeFullPath, MAX_PATH);//得到程序模块名称，全路径
	*_tcsrchr(exeFullPath, _T('\\')) = 0;
	CString str2 = exeFullPath;
	IGraphBuilder * pGraph = NULL;
	IMediaControl * pControl = NULL;
	IMediaEvent * pEvent = NULL;
	//改动了！
	IVideoWindow* m_pVidWin = NULL;
	HWND m_hwnd;
	m_hwnd = hge->System_GetState(HGE_HWND);
	//不支持com组件的就会失败!
	HRESULT hr = CoInitialize(NULL);
	if (FAILED(hr))MessageBox(NULL, "不支持COM组件的初始化，导致动画未被载入", "初始化错误！", MB_OK);
	//可能会失败!
	CString strx, stry;
	strx = "\\Gecore Final.mp4";
	stry = str2 + strx;
	USES_CONVERSION;
	LPCWSTR pwcStr = A2CW((LPCSTR)stry);
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&pGraph);
	hr = pGraph->QueryInterface(IID_IMediaControl, (void**)&pControl);
	hr = pGraph->QueryInterface(IID_IMediaEvent, (void**)&pEvent);
	hr = pGraph->QueryInterface(IID_IVideoWindow, (void **)&m_pVidWin);
	hr = pGraph->RenderFile(pwcStr, NULL);
	RECT rc;
	GetClientRect(m_hwnd, &rc);
	hr = m_pVidWin->SetWindowPosition(0, 0, rc.right, rc.bottom);
	hr = m_pVidWin->put_Owner((OAHWND)m_hwnd);
	hr = m_pVidWin->put_MessageDrain((OAHWND)m_hwnd);
	hr = m_pVidWin->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);
	if (SUCCEEDED(hr))
	{
		hr = pControl->Run();
		if (SUCCEEDED(hr))
		{
			long evCode;
			pEvent->WaitForCompletion(INFINITE, &evCode);
		}
	}

	pControl->Release();
	pEvent->Release();
	pGraph->Release();
	m_pVidWin->Release();
	CoUninitialize();
	/*
	tagMSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		DispatchMessage(&msg);
		if (WAIT_OBJECT_0 == WaitForSingleObject(pThread->m_hThread, INFINITE))
		{
			delete   pThread;
			pThread = NULL;
			break;
		}
	}
	*/
	DWORD exitcode;
	while (GetExitCodeThread(pThread->m_hThread, &exitcode))
	{
		if (exitcode == 1000)break;
	}
    hge->System_Start();
} 

//各种释放（载入了就一定要释放！！！)
FontPool::GetFontPool().Exit();
RenderPool::GetRenderPool().Exit();
ObjectPool::GetObjPool().Exit();
SFXManager::GetSFXManager().Exit();
NoticeManager::GetNoticeManager().Exit();
BuffManager::GetBuffManager().Exit();
delete fontforalpha;
delete fontfornumbers;
delete mouse;
delete BG;
delete Frame;
delete menubg;
delete menumouse;
delete blackin;
delete selectlayer1;
delete selectlayer2;
delete pausemenu;
delete resultbg;
delete TokenL;
hge->System_Shutdown();
hge->Release();


return 0;
} 