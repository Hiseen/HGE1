#pragma once
#include "hge.h"
/*
using namespace std;
DWORD WINAPI ThreadProc(LPVOID);
LPCTSTR EVENT_NAME = _T("TEST");
*/


 struct BWpic
{
	bool **bools;
	int number;
	int width;
	int height;
};
static void getRange(int *arr, int size, int *max, int *min)
{
	int *p = arr, *endp = arr + size;
	if (size % 2 != 0)
	{
		*max = *min = *p++;
	}
	else
	{
		if (*p > *(p + 1))
		{
			*max = *p;
			*min = *(p + 1);
		}
		else
		{
			*max = *(p + 1);
			*min = *p;
		}
		p += 2;
	}
	while (p != endp)
	{
		if (*p > *(p + 1))
		{
			if (*p > *max) *max = *p;
			if (*(p + 1) < *min) *min = *(p + 1);
		}
		else
		{
			if (*(p + 1) > *max) *max = *(p + 1);
			if (*(p) < *min) *min = *p;
		}
		p += 2;
	}
};

static bool Collision(Point *mPoint, const int count, const float angle5, float center_x, float center_y, float px, float py)
{
    int count_point =count;
	Point cmpoint[10];
	memcpy(cmpoint, mPoint, count_point * sizeof(Point));
	for (int i = 0; i < count_point; i++)
	{
		float xx1 = cmpoint[i].x, yy1 = cmpoint[i].y;
		if (angle5>-4.71 || angle5 < -7.85) yy1 = -yy1;
		cmpoint[i].x = cos(angle5)*xx1 - sin(angle5)*yy1 + center_x;
		cmpoint[i].y = cos(angle5)*yy1 + sin(angle5)*xx1 + center_y;
	}
	/*
	HGE *hge = hgeCreate(HGE_VERSION);
	hge->Gfx_BeginScene();
	for (int i = 0; i < count_point; i++)
	{
		if (i != count_point - 1)
			hge->Gfx_RenderLine(cmpoint[i].x, cmpoint[i].y, cmpoint[i + 1].x, cmpoint[i + 1].y, ARGB(255, 255, 0, 0));
		else
			hge->Gfx_RenderLine(cmpoint[i].x, cmpoint[i].y, cmpoint[0].x, cmpoint[0].y,ARGB(255, 255, 0, 0));
	}
	hge->Gfx_EndScene();
	*/
	for (int i = 0; i < count_point; i++)
	{
		if (i != count_point - 1)
		{
			float k = 0;
			if (cmpoint[i].x - cmpoint[i + 1].x != 0) k = (cmpoint[i].y - cmpoint[i + 1].y) / (cmpoint[i].x - cmpoint[i + 1].x);
			else  k = 10000000;
			int bcs[10], maxs, mins;
			float dx = py - px*k;
			for (int j = 0; j < count_point; j++)
				bcs[j] = cmpoint[j].y - cmpoint[j].x*k;
			getRange(bcs, count_point, &maxs, &mins);
			if (dx>maxs || dx < mins)	return false;
		}
		else
		{
			float k = 0;
			if (cmpoint[i].x - cmpoint[0].x != 0) k = (cmpoint[i].y - cmpoint[0].y) / (cmpoint[i].x - cmpoint[0].x);
			else  k = 10000000;
			int bcs[10], maxs, mins;
				float dx = py - px*k;
			for (int j = 0; j < count_point; j++)
				bcs[j] = cmpoint[j].y - cmpoint[j].x*k;
			getRange(bcs, count_point, &maxs, &mins);
			if (maxs > dx && dx > mins) return true;
			else return false;
		}
	}
}

static bool CollisionBW(BWpic bools,float Targetx, float Targety, double angle, float myselfx, float myselfy,float size)
{
	if (bools.bools != nullptr)
	{
		double angle1 = -angle;
		//HGE *hge = hgeCreate(HGE_VERSION);
		//HWND hwnd=hge->System_GetState(HGE_HWND);
		//HDC dc = GetDC(hwnd);
		//for (int i = 0; i < bools.width;i++)
		//for (int j = 0; j < bools.height;j++)
		//if (bools.bools[i][j])SetPixel(dc, i, j, RGB(0, 0, 0));
		//else
		//	SetPixel(dc, i, j, RGB(255, 255, 255));
		int fx, fy;
		int x = (Targetx - myselfx);// +bools.width / 2;
		int y = (Targety - myselfy);// +bools.height / 2;
		fx = x*cos(angle1) - y*sin(angle1);
		fy = y*cos(angle1) + x*sin(angle1);
		fx /= size;
		fy /= size;
		fx += (bools.width / 2);
		fy += (bools.height / 2);
		//SetPixel(dc, fx, fy, RGB(50, 50, 50));
		if (fx<0 || fy<0 || fx>bools.width-1 || fy>bools.height-1)
			return false;
		else
			return bools.bools[fx][fy];
	}
	else
		return false;
}
static bool ballcrash(float center_x, float center_y, int r, float px, float py)
{
	int d1 = px - center_x;
	int d2 = py - center_y;
	if (r*r < d1*d1 + d2*d2)return false;
	else return true;
}
/*
DWORD WINAPI ThreadProc(LPVOID)
{
	HANDLE hEvent = ::OpenEvent(EVENT_ALL_ACCESS, FALSE, EVENT_NAME);
	::WaitForSingleObject(hEvent, INFINITE);
	::CloseHandle(hEvent);
	return 0L;
}
*/

static void AddUntil(int &stage, int &charge, int max,bool &finish)
{
		if (charge < max)
		{
			charge++;
		}
		else
		{
			charge = 0;
			finish = true;
		}
}

static void random(int a[], int n)
{
	int index, tmp, i;
	srand(time(NULL));
	for (i = 0; i <n; i++)
	{
		index = rand() % (n - i) + i;
		if (index != i)
		{
			tmp = a[i];

			a[i] = a[index];

			a[index] = tmp;
		}
	}
}