#pragma once
#include "ImageObject.h"

//Ҫ����mask ������2���ݳ���
//
class Mask 
{
public:
	Mask(float x, float y, float w, float h, float centerx=0, float centery=0);
	~Mask();


private:
	ImageObject *pic;
	HTARGET target;
};

