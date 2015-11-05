#pragma once
#include "Hero.h"
#include "hge.h"
#include "hgesprite.h"

#define STARTX 12
#define STARTY 81
#define BUFFSIZE 32
class BuffManager
{
public:
	static BuffManager& GetBuffManager()
	{
		static  BuffManager ObjPool;
		ObjPool.AssureInitialized();
		return ObjPool;
	}
	void Update();
	bool GetDeBuff(errstate err);
	int GetDeBuffTime(errstate err);
	void Exit()
	{
		delete toxicdebuff;
		delete freezedebuff;
		delete vertigodebuff;
		delete mprecover;
		delete extraattack;
		delete font;
	}
	~BuffManager();
private:
	BuffManager();
	void AssureInitialized()
	{
		while (!m_bHasInitialized)
			Sleep(0);
	};
	bool m_bHasInitialized;
	hgeSprite *toxicdebuff;
	hgeSprite *freezedebuff;
	hgeSprite *vertigodebuff;
	hgeSprite *mprecover;
	hgeSprite *extraattack;
	hgeFont *font;
};

