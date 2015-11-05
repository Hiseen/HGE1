#include "FontPool.h"


FontPool::FontPool()
{
	HGE *hge = hgeCreate(HGE_VERSION);
	delay = 5;
	for (int i = 0; i < 32; i++)
	{
		CString str = ".png",before;
		before.Format("%d", i);
		CString finalstr = before + str;
		HTEXTURE a = hge->Texture_Load(finalstr);
		hgeSprite *font = new hgeSprite(a, 0, 0, hge->Texture_GetWidth(a), hge->Texture_GetHeight(a));
		if (i < 30)font->SetHotSpot(6, 8.5);
		if (i == 30)font->SetHotSpot(16.5, 7);
		if (i == 31)font->SetHotSpot(32, 7.5);
		fontpool.push_back(font);
	}
	m_bHasInitialized = true;
}


FontPool::~FontPool()
{
}


void FontPool::InsertFont(float x, float y, int dmg, Type type, bool Need)
{
	if (Need)
	{
		DMGFont font(dmg, x, y, type);
		delaypool.push_back(font);
		delaypool.shrink_to_fit();
	}
}

void FontPool::RenderFont()
{
	for (int i = 0; i < pool.size(); i++)
	{
		if (pool[i].IsAlive())
		{
			pool[i].Action();
			int number = pool[i].GetNum().GetLength();
			if (atoi(pool[i].GetNum()) != 0)
			{
				for (int j = 0; j < number; j++)
				{
					int pos = atoi(pool[i].GetNum().Mid(j, 1));
					switch (pool[i].GetDMGType())
					{
					case Damage:
						break;
					case Heal:
						pos += 10;
						break;
					case Passive:
						pos += 20;
						break;
					}
					fontpool[pos]->SetColor(ARGB(pool[i].GetAlpha(), 255, 255, 255));
					fontpool[pos]->RenderEx(pool[i].GetX() - (number / 2) * 12 + 12 * j, pool[i].GetY(), 0, pool[i].GetSize(), pool[i].GetSize());
					fontpool[pos]->SetColor(ARGB(255, 255, 255, 255));
				}
				if (pool[i].GetDMGType() == Crit)
				{
					fontpool[31]->SetColor(ARGB(pool[i].GetAlpha(), 255, 255, 255));
					fontpool[31]->RenderEx(pool[i].GetX(), pool[i].GetY() - 17, 0, pool[i].GetSize(), pool[i].GetSize());
					fontpool[31]->SetColor(ARGB(255, 255, 255, 255));
				}
			}
			else
			{
				fontpool[30]->SetColor(ARGB(pool[i].GetAlpha(), 255, 255, 255));
				fontpool[30]->RenderEx(pool[i].GetX(), pool[i].GetY(), 0, pool[i].GetSize(), pool[i].GetSize());
				fontpool[30]->SetColor(ARGB(255, 255, 255, 255));
			}
		}
		
	}
}

void FontPool::Delete()
{
	if (delay>0)
		delay--;
	else
	{
		delay = 5;
		for (int i = 0; i < delaypool.size(); i++)
			pool.push_back(delaypool[i]);
		delaypool.clear();
	}
	vector<DMGFont>::iterator it;
	for (it = pool.begin(); it != pool.end();)
	if (IsDead(*it))
	{
		it = pool.erase(it);
	}
	else
		++it;
}


void FontPool::InsertFont(GameObject *attackobj,int dmg,GameObject *fontobj, Type type)
{	
	if (fontobj->NeedRenderFont())
	{
		if (type==Damage && -dmg > attackobj->GetDMG(false))
		{
			DMGFont font(dmg, fontobj->GetX(), fontobj->GetY(), Crit);
			delaypool.push_back(font);
			delaypool.shrink_to_fit();
		}
		else
		{
			DMGFont font(dmg, fontobj->GetX(), fontobj->GetY(), type);
			delaypool.push_back(font);
			delaypool.shrink_to_fit();
		}

	}
}