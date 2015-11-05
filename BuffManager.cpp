#include "BuffManager.h"


BuffManager::BuffManager()
{
	HGE *hge = hgeCreate(HGE_VERSION);
	HTEXTURE f = hge->Texture_Load("freeze.png");
	HTEXTURE v = hge->Texture_Load("vertigo.png");
	HTEXTURE t = hge->Texture_Load("toxic.png");
	HTEXTURE m = hge->Texture_Load("MPrecover.png");
	HTEXTURE e = hge->Texture_Load("extraattack.png");
	toxicdebuff = new hgeSprite(t, 0, 0, BUFFSIZE, BUFFSIZE);
	vertigodebuff = new hgeSprite(v, 0, 0, BUFFSIZE, BUFFSIZE);
	freezedebuff = new hgeSprite(f, 0, 0, BUFFSIZE, BUFFSIZE);
	mprecover = new hgeSprite(m, 0, 0, BUFFSIZE, BUFFSIZE);
	extraattack = new hgeSprite(e, 0, 0, BUFFSIZE, BUFFSIZE);
	extraattack->SetHotSpot(1, 1);
	mprecover->SetHotSpot(1, 1);
	toxicdebuff->SetHotSpot(1, 1);
	vertigodebuff->SetHotSpot(1, 1);
	freezedebuff->SetHotSpot(1, 1);
	font = new hgeFont("font.fnt");
	m_bHasInitialized = true;
}


void BuffManager::Update()
{
	int numnow = 0;
	if (Hero::GetHero().GetExtraDMG() != 0)
	{
		extraattack->Render(STARTX + numnow * BUFFSIZE, STARTY);
		font->printf(STARTX + numnow * BUFFSIZE + 28, STARTY + 17, HGETEXT_RIGHT, "%d", Hero::GetHero().GetExtraDMG());
		numnow++;
	}
	if (Hero::GetHero().GetRecoverMP() != 0)
	{
		mprecover->Render(STARTX + numnow * BUFFSIZE, STARTY);
		font->printf(STARTX + numnow * BUFFSIZE + 28, STARTY + 17, HGETEXT_RIGHT, "%d", Hero::GetHero().GetRecoverMP());
		numnow++;
	}
	if (GetDeBuff(toxic))
	{
		if (GetDeBuffTime(toxic) < 250)
			toxicdebuff->SetColor(ARGB((float)(GetDeBuffTime(toxic) % 50) / 50 * 255, 255, 255, 255));
		toxicdebuff->Render(STARTX + numnow * BUFFSIZE, STARTY);
		numnow++;
	}
	if (GetDeBuff(freeze))
	{
		if (GetDeBuffTime(freeze) < 250)
			freezedebuff->SetColor(ARGB((float)(GetDeBuffTime(freeze) % 50) / 50 * 255, 255, 255, 255));
		freezedebuff->Render(STARTX + numnow * BUFFSIZE, STARTY);
		numnow++;
	}
	if (GetDeBuff(vertigo))
	{
		if (GetDeBuffTime(vertigo) < 250)
			vertigodebuff->SetColor(ARGB((float)(GetDeBuffTime(vertigo) % 50) / 50 * 255, 255, 255, 255));
		vertigodebuff->Render(STARTX + numnow * BUFFSIZE, STARTY);
		numnow++;
	}

}

bool BuffManager::GetDeBuff(errstate err)
{
	if (Hero::GetHero().GetErrorState() & err)
		return true;
	else
		return false;
}


int BuffManager::GetDeBuffTime(errstate err)
{
	return Hero::GetHero().GetErrStateTime(err);
}


BuffManager::~BuffManager()
{
}
