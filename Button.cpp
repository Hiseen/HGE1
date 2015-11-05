#include "Button.h"
#include "Hero.h"
Button::Button(float x, float y, SceneNumber scenenumber, int buttonnum1, float buttonwidth, float buttonheight) :GameObject(x, y, 0, 1, 0, 0, 0, false, 0, 0, none, scenenumber)
{
	buttonnum = buttonnum1;
	state = MouseOff;
	height = buttonheight;
	width = buttonwidth;
	buttondown = false;
	finishchangecolor = false;
	finishmove = false;
	count = 0;
	SetNeedChangeColor();
	SetChangeColor(255,0, 0, 0);
	colorreverse = false;
	countdelay = 0;
	countalpha = 255;
	playsound = false;
	flag = false;
	switch (buttonnum)
	{
	case BUTTON_1_OFF:
		RenderPool::GetRenderPool().InstantRender(x, y, BUTTON_1_OFF);
		SetDelayrender(true);
		SetZ(0.7);
		break;
	case BUTTON_2_OFF:
		RenderPool::GetRenderPool().InstantRender(x, y, BUTTON_1_OFF);
		SetZ(0.7);
		SetDelayrender(true);
		break;
	case BUTTON_3_OFF:
		RenderPool::GetRenderPool().InstantRender(x, y, BUTTON_1_OFF);
		SetZ(0.7);
		SetDelayrender(true);
		break;
	case BUTTON_4_OFF:
		RenderPool::GetRenderPool().InstantRender(x, y, BUTTON_1_OFF);
		SetZ(0.7);
		SetDelayrender(true);
		break;
	case BUTTON_9_OFF:
		countalpha = 0;
		SetChangeColor(countalpha, 0, 0, 0);
		break;
	case BUTTON_10_OFF:
		countalpha = 0;
		SetChangeColor(countalpha, 0, 0, 0);
		break;
	case BUTTON_11_OFF:
		countalpha = 0;
		SetChangeColor(countalpha, 0, 0, 0);
		break;
	case BUTTON_12_OFF:
		countalpha = 0;
		SetChangeColor(countalpha, 0, 0, 0);
		break;
	case BUTTON_13_OFF:
		countalpha = 0;
		SetChangeColor(countalpha, 0, 0, 0);
		break;
	case BUTTON_14_OFF:
		countalpha = 0;
		SetChangeColor(countalpha, 0, 0, 0);
		break;
	case BUTTON_15_OFF:
		countalpha = 0;
		SetChangeColor(countalpha, 0, 0, 0);
		break;
	case BUTTON_17_OFF:
		countalpha = 0;
		SetChangeColor(countalpha, 0, 0, 0);
		break;
	case BUTTON_18_OFF:
		countalpha = 0;
		SetChangeColor(countalpha, 0, 0, 0);
		break;
	case BUTTON_19_OFF:
		countalpha = 0;
		SetChangeColor(countalpha, 0, 0, 0);
		break;
	}
}


Button::~Button()
{
}


void Button::action()
{
	if (countdelay != 50)
		countdelay++;
	else
		SetDelayrender(false);
	if (!IsDelayrender())
	{
		HGE *hge = hgeCreate(HGE_VERSION);
		float mousex, mousey;
		hge->Input_GetMousePos(&mousex, &mousey);
		if (countalpha<255)
		{
			countalpha += 5;
			SetChangeColor(countalpha, 0, 0, 0);
		}
		else
			SetChangeColor(255, 0, 0, 0);
		if (state != MouseDown)
		if (mousex > GetX() && mousex<GetX() + width && mousey>GetY() && mousey < GetY() + height)
		{
			state = MouseOver;
			if (!playsound)
			{
				playsound = true;
				SFXManager::GetSFXManager().PlaySFX(BUTTONSFX);
			}
			if (state == MouseOver && hge->Input_KeyDown(HGEK_LBUTTON))
			{
				state = MouseDown;
				SFXManager::GetSFXManager().PlaySFX(BUTTONSFX2);
			}
		}
		else
		{
			state = MouseOff;
			playsound = false;
		}
		if (state == MouseDown)
		{
			if (!finishchangecolor)
			{
				if (!colorreverse)
					count += 50;
				else
					count -= 50;
				if (count == 250)
					colorreverse = true;
				SetChangeColor(count + 5, 255, 255, 255);
				if (count == 0 && colorreverse)
				{
					finishchangecolor = true;
					SetChangeColor(255, 0, 0, 0);
					finishmove = false;
				}
			}
			else
			{
				switch (buttonnum)
				{
				case BUTTON_1_OFF:
					if (!flag)
					{
						SceneManager::GetSceneManager().SetMenuStage(1);
						flag = true;
					}
					if (!finishmove && ObjectPool::GetObjPool().ChangeAllPos(Left, 853))
					{
						finishmove = true;
						flag = false;
						state = MouseOff;
						finishchangecolor = false;
						buttondown = false;
						colorreverse = false;
					}
					break;
				case BUTTON_2_OFF:
					state = MouseOff;
					finishchangecolor = false;
					buttondown = false;
					colorreverse = false;
					break;
				case BUTTON_3_OFF:
					state = MouseOff;
					finishchangecolor = false;
					buttondown = false;
					colorreverse = false;
					break;
				case BUTTON_4_OFF:
					SceneManager::GetSceneManager().ExitGame();
					state = MouseOff;
					finishchangecolor = false;
					buttondown = false;
					colorreverse = false;
					break;
				case BUTTON_5_OFF:
						if (!flag && SceneManager::GetSceneManager().ReturnMenuStage() == 2)
						{
							SceneManager::GetSceneManager().SetMenuStage(0);
							flag = true;
						}
						if (!finishmove &&SceneManager::GetSceneManager().ReturnMenuStage() == 0 && ObjectPool::GetObjPool().ChangeAllPos(Right, 853))
						{
							flag = false;
							finishmove = true;
							state = MouseOff;
							finishchangecolor = false;
							buttondown = false;
							colorreverse = false;
						}
					break;
				case BUTTON_6_OFF:
						if (!flag&&SceneManager::GetSceneManager().ReturnMenuStage() == 2)
						{
							SceneManager::GetSceneManager().SetMenuStage(4);
							flag = true;
						}
						if (!finishmove&&SceneManager::GetSceneManager().ReturnMenuStage()==4 && ObjectPool::GetObjPool().ChangeAllPos(Left, 853))
						{
							flag = false;
							finishmove = true;
							state = MouseOff;
							finishchangecolor = false;
							buttondown = false;
							colorreverse = false;
						}
					break;
				case BUTTON_7_OFF:
					if (!flag && SceneManager::GetSceneManager().ReturnMenuStage() == 2)
					{
						SceneManager::GetSceneManager().SetMenuStage(3);
						flag = true;
					}
					if (!finishmove &&SceneManager::GetSceneManager().ReturnMenuStage()==3 && ObjectPool::GetObjPool().ChangeAllPos(Left, 853))
					{
						flag = false;
						finishmove = true;
						state = MouseOff;
						finishchangecolor = false;
						buttondown = false;
						colorreverse = false;
					}
					break;
				case BUTTON_8_OFF:
					state = MouseOff;
					finishchangecolor = false;
					buttondown = false;
					colorreverse = false;
					break;
				case BUTTON_9_OFF:
					state = MouseOff;
					finishchangecolor = false;
					buttondown = false;
					colorreverse = false;
					break;
				case BUTTON_10_OFF:
					state = MouseOff;
					finishchangecolor = false;
					buttondown = false;
					colorreverse = false;
					break;
				case BUTTON_11_OFF:
					state = MouseOff;
					finishchangecolor = false;
					buttondown = false;
					colorreverse = false;
					break;
				case BUTTON_12_OFF:
					state = MouseOff;
					finishchangecolor = false;
					buttondown = false;
					colorreverse = false;
					SceneManager::GetSceneManager().GoToOneScene(GameScene, false, true);
					break;
				case BUTTON_13_OFF:
					state = MouseOff;
					finishchangecolor = false;
					buttondown = false;
					colorreverse = false;
					SceneManager::GetSceneManager().GoToOneScene(GameScene);
					break;
				case BUTTON_14_OFF:
					state = MouseOff;
					finishchangecolor = false;
					buttondown = false;
					colorreverse = false;
					SceneManager::GetSceneManager().GoToOneScene(MenuScene);
					break;
				case BUTTON_15_OFF:
					state = MouseOff;
					finishchangecolor = false;
					buttondown = false;
					colorreverse = false;
					break;
				case BUTTON_16_OFF:
					state = MouseOff;
					finishchangecolor = false;
					buttondown = false;
					colorreverse = false;
					break;
				case BUTTON_17_OFF:
					state = MouseOff;
					finishchangecolor = false;
					buttondown = false;
					colorreverse = false;
					if (Hero::GetHero().AddStats(ADD_ATK))
						SceneManager::GetSceneManager().GoToOneScene(GameScene, false, true);
					break;
				case BUTTON_18_OFF:
					state = MouseOff;
					finishchangecolor = false;
					buttondown = false;
					colorreverse = false;
					if (Hero::GetHero().AddStats(ADD_DEX))
						SceneManager::GetSceneManager().GoToOneScene(GameScene, false, true);
					break;
				case BUTTON_19_OFF:
					state = MouseOff;
					finishchangecolor = false;
					buttondown = false;
					colorreverse = false;
					if (Hero::GetHero().AddStats(ADD_INT))
						SceneManager::GetSceneManager().GoToOneScene(GameScene, false, true);
					break;
				}
			}
		}

	}
}