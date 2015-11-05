#pragma once
#include "ObjectPool.h"
class SceneManager
{
public:
	
	~SceneManager();
	SceneNumber GetCurrentScene()const
	{
		return currentscene;
	};
	GameMode GetCurrentMode()const
	{
		return mode;
	}
	void SetGameMode(GameMode newmode)
	{
		mode = newmode;
	}
	//感觉好像一点用都没有的函数：
	//void GoToNext(){ currentscene++; };
	//void GoToLast(){ currentscene--; };
	//这个比较重要：
	bool IsExit()const{ return exitgame; };
	void ExitGame(){ exitgame = true; };
	void GoToOneScene(SceneNumber scenenumber,  bool isneedclean=true,bool needexceptclean=false);
	void CleanOneScene(bool &initclean);
	int ReturnMenuStage()const{ return MenuStage; };
	void TitleInitFinish(bool finish){ inittitle = finish; };
	bool GetTitleFinish()const{ return inittitle; };
	void SetMenuStage(int stage){ MenuStage = stage; };
	bool ReturnAllclean()const{ return needclean; };
	bool ReturnPartClean()const{ return needexceptclean; };
	void WaveClear(){ waves++; };
	void WaveInit(){ waves = 1; };
	void SetWaveDelay(bool delay)
	{
		wavedelay = delay;
	}
	bool GetWaveDelay()const
	{
		return wavedelay;
	}
	int GetWave()const{ return waves; };
	static SceneManager& GetSceneManager()
	{
		static SceneManager SM;
		SM.AssureInitialized();
		return SM;
	}
	void AssureInitialized()
	{
		while (!m_bHasInitialized)
			Sleep(0);
	};
private:
	SceneNumber currentscene;
	GameMode mode;
	bool m_bHasInitialized;
	bool needclean;
	int MenuStage;
	bool inittitle;
	bool needexceptclean;
	bool exitgame;
	int waves;
	bool wavedelay;
	SceneManager()
	{ 
		inittitle = false;
		needexceptclean = false;
		MenuStage = -1;
		wavedelay = true;
	currentscene =LogoScene;
	mode = Undecided;
	needclean = false;
	exitgame = false;
	m_bHasInitialized=true;
	};
};

