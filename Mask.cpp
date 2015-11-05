#include "Mask.h"


Mask::Mask(float x, float y, float w, float h,float centerx,float centery)
{
	HGE *hge = hgeCreate(HGE_VERSION);
	target = hge->Target_Create(1024, 1024,0);
	HTEXTURE a = hge->Texture_Create(w, h);
	pic = new ImageObject(a, centerx, centery);
	pic->CopyData(x, y, target);
}


Mask::~Mask()
{
}
