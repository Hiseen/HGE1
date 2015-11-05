#pragma once
#include "Item.h"
#include "SFXManager.h"
class ItemEnemy :
	public GameObject
{
public:
	ItemEnemy(float x,float y);
	virtual void action();
	virtual void born();
	virtual void death();
	void SetType(int newtype)
	{
		type = newtype;
	}
	void SetEnemy(){ isenemy = true; };
	virtual int TellType(){ return ITEMENEMY1; };
	virtual int GetCollisionRange(){ return 0; };
	virtual bool IsEnemy(){ return isenemy; };
	virtual bool NeedRenderFont(){ return false; };
	virtual bool NeedToDestroyWhenBossArrive(){ return false; };
	~ItemEnemy();
private:
	bool effective;
	int type;
	bool isenemy;
	//�����֣�
	//1.��Ѫҩ
	//0.get����
	//�������ֻᰴ��ʱ��仯 2->3->2..���Ժ��ٿ��ǣ�
	//2.get�ӣ��죩
	//3.get��������
	//���Կ���ֻ��������ʾģʽ��1��2��3һ�֣�0һ�֣�
};

