#include "Range.h"

void Range::GetInfo(GameObject linkwho, double mutisize)
{
	SetPos(linkwho.GetX(), linkwho.GetY());
	if (changesize < mutisize)
		changesize += (mutisize - changesize) / 10;
	if (changesize > mutisize)
		changesize -= (changesize - mutisize) / 10;
}