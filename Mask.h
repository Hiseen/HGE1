#pragma once
#include "ImageObject.h"

//要创建mask 必须用2次幂长宽
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

