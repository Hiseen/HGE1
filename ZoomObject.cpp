#include "ZoomObject.h"





ZoomObject::~ZoomObject()
{
}


void ZoomObject::action()
{
	switch (num)
	{
	case TITLE:
		if(alpha < 255)
		{
			alpha *= 1.06;
			SetSize(GetSize()*0.98);
		}
		else
		{
			SceneManager::GetSceneManager().TitleInitFinish(true);
			SetSize(1);
			alpha = 255;
		}
		if (alpha>255)alpha = 255;
		SetChangeColor(alpha, 255, 255, 255);
		break;
	default:
		break;
	}
}