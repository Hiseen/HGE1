#include "FloatingObject.h"


void FloatingObject::action()
{
	HGE *hge = hgeCreate(HGE_VERSION);
	SetImageAngle(GetImageAngle()+GetSpeed()/3000);
	GoWithAnAngle(hge->Timer_GetDelta(), GetRot()-1.57);
	if (GetY()<-100)
		Suicide(true);
};

FloatingObject::~FloatingObject()
{
}
