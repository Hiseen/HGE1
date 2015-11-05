#include "Sword.h"

Sword::~Sword()
{

}


void Sword::effect(GameObject *obj)
{
	if (!obj->IsAttacked())
	{
		int damage = -Hero::GetHero().ToAttack() - GetATK();
		obj->SetDEF(obj->GetDEF() - Hero::GetHero().GetBreakArmor());
		obj->DmgDispose(damage);
		obj->varyHP(damage);
		HGE *hge = hgeCreate(HGE_VERSION);
		obj->BoomEffect(MakePosObject(GetX(), GetY()), hge->Timer_GetDelta(),5);
		FontPool::GetFontPool().InsertFont(obj->GetX(), obj->GetY(), damage);
	}
}

void Sword::Start()
{
	HGE *hge = hgeCreate(HGE_VERSION);
	float x, y;
	hge->Input_GetMousePos(&x, &y);
	GameObject *a=GameObject::MakePosObject(x, y);
	SetRot(atan2f(GetY()-a->GetY(),GetX()-a->GetX()));
}


void Sword::Move()
{
	SetPos(Hero::GetHero().GetX(), Hero::GetHero().GetY());
}

