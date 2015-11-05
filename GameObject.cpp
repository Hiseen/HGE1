#include "GameObject.h"


GameObject::GameObject(float x1, float y1, double speed1,
	int HP1, int MP1, int DEF1,int DMG1,
	bool reachable1, double size1 , bool alive1,
	errstate errstate2, SceneNumber scenenumber1 )
{
	x = x1;
	y = y1;
	HP = HP1;
	MP = MP1; 
	DEF = DEF1;
	speed = speed1;
	errvar_freeze = 0;
	size = size1;
	alive = alive1;
	reachable = reachable1;
	errstate1 = errstate2;
	avlid = true;
	finishb = false;
	finishd = false;
	angle = 0;
	attacked = false;
	varimgh = 1;
	varimgw = 1;
	DoNotSpin = false;
	Targeted = false;
	DMG = DMG1;
	scenenumber = scenenumber1;
	deadanimover = false;
	MAX_HP = HP;
	reverseDMG = false;
	force_to_die = false;
	needchangecolor = false;
	CA = 0;
	CR = CG = CB = 0;
	invulnerable_time = 2;
	errlast_time_freeze = 0;
	back_speed = 0;
	errvar_toxic = 0;
	errlast_time_toxic = 0;
	errvar_vertigo = 0;
	errlast_time_vertigo = 0;
	angleforimage = 0;
	errvar_heal = 0;
	errlast_time_heal = 0;
	Z = 0;
	delayrender = false;
	canhaveerrorstate = true;
	dashtocenter = false;
	CritDMG = 2;
	Crit = 0;
	Dodge = 0;
	islocked = false;
	hasshield = false;
	PenetrateArmor = 0;
	speedrate = 1;
	innertype = 0;
}


GameObject::~GameObject()
{
}

bool GameObject::IsCoincide(GameObject *obj,bool force_to_10)
{
	if (!IsSkill())
	{
		if (force_to_10)
		{
			if ((abs(x - obj->x)) <= 10 && (abs(y - obj->y) <= 10))
				return true;
			else
				return false;
		}
		else
		{
			if ((abs(x - obj->x)) <= obj->GetCollisionRange() + GetCollisionRange() && (abs(y - obj->y) <= obj->GetCollisionRange() + GetCollisionRange()))
				return true;
			else
				return false;
		}
	}
	else
	{
		if ((abs(x - obj->x)) <= 10 && (abs(y - obj->y) <= 10))
			return true;
		else
			return false;
	}
}

double GameObject::GetAngle(GameObject *obj, bool return1)
{

	float dx, dy;
	float distance, cos = 0, rad;
	dx = x - obj->x;
	dy = y - obj->y;
	distance = sqrt((float)(dx*dx + dy*dy));
	if (!return1)
	{
		if ((dx <= 0) && (dy > 0))
		{
			cos = abs((float)dy / distance);
			angle = acos(cos);
		}
		if ((dx < 0) && (dy <= 0))
		{
			cos = abs((float)dx / distance);
			angle = acos(cos) + 1.57;
		}
		if ((dx >= 0) && (dy<0))
		{
			cos = abs((float)dy / distance);
			angle = acos(cos) + 3.14;
		}
		if ((dx>0) && (dy >= 0))
		{
			cos = abs((float)dx / distance);
			angle = acos(cos) - 1.57;
		}
		return 0;
	}
	else
	{
		if ((dx <= 0) && (dy > 0))
		{
			cos = abs((float)dy / distance);
			return acos(cos);
		}
		if ((dx < 0) && (dy <= 0))
		{
			cos = abs((float)dx / distance);
			return acos(cos) + 1.57;
		}
		if ((dx >= 0) && (dy<0))
		{
			cos = abs((float)dy / distance);
			return acos(cos) + 3.14;
		}
		if ((dx>0) && (dy >= 0))
		{
			cos = abs((float)dx / distance);
			return acos(cos) - 1.57;
		}
		return 0;
	}
	
}

void GameObject::GoToAnotherObject(GameObject *obj)
{
	HGE *hge = hgeCreate(HGE_VERSION);
	float dt = hge->Timer_GetDelta();
	if (alive)
	{
			GetAngle(obj);
			if (x >= obj->x && y > obj->y){ x -= speed*speedrate*dt*cos(angle + 1.57); y -= speed*speedrate*dt*sin(angle + 1.57); }
			if (x < obj->x && y >= obj->y){ x += speed*speedrate*dt*sin(angle); y -= speed*speedrate*dt*cos(angle); }
			if (x > obj->x && y <= obj->y){ x -= speed*speedrate*dt*sin(angle - 3.14); y += speed*speedrate*dt*cos(angle - 3.14); }
			if (x <= obj->x && y < obj->y){ x += speed*speedrate*dt*cos(angle - 1.57); y += speed*speedrate*dt*sin(angle - 1.57); }
	}

}

void GameObject::AwayFromOneObject(GameObject *obj)
{
	HGE *hge = hgeCreate(HGE_VERSION);
	float dt = hge->Timer_GetDelta();
	if (alive)
	{
		GetAngle(obj);
		if (x >= obj->x && y > obj->y){ x += speed*speedrate*dt*cos(angle + 1.57); y += speed*speedrate*dt*sin(angle + 1.57); }
		if (x < obj->x && y >= obj->y){ x -= speed*speedrate*dt*sin(angle); y += speed*speedrate*dt*cos(angle); }
		if (x > obj->x && y <= obj->y){ x += speed*speedrate*dt*sin(angle - 3.14); y -= speed*speedrate*dt*cos(angle - 3.14); }
		if (x <= obj->x && y < obj->y){ x -= speed*speedrate*dt*cos(angle - 1.57); y -= speed*speedrate*dt*sin(angle - 1.57); }
		SetRot(GetRot() - 3.14);
	}

}

void GameObject::SetAlive(bool alive1)
{
	alive = alive1;
}

void GameObject::Swell(bool faster)
{
	double delta = 0.05;
	if (faster)delta = 0.1;
	if (size >= 1.0)
	{
		size -= delta;
	}
	else
	{
		size = 1.0;
		alive = true;
		finishb = true;
	}
}

void GameObject::Swell2()
{
	double delta = 0.05;
	if (size <= 1.0)
	{
		size += delta;
	}
	else
	{
		size = 1.0;
		alive = true;
		finishb = true;
	}
}


void GameObject::Contract(bool instant)
{
	if (!instant)
	{
		if (HP <= 0 && finishd == false)
		{
			alive = false;
			size -= 0.5;
		}
		if (size <= 0)
		{
			avlid = false;
			finishd = true;
		}
	}
	else
	{
		if (HP <= 0 && finishd == false)
		{
			alive = false;
			size = 0;
			avlid = false;
			finishd = true;
		}
	}
}

float GameObject::GetX()const
{
	return x;
}

float GameObject::GetY()const
{
	return y;
}

double GameObject::GetSize()const
{
	return size;
}

double GameObject::GetRot()const
{
	return angle;
}

void GameObject::SetPos(float x1, float y1)
{
	x = x1;
	y = y1;
}



double GameObject::GetSpeed()const
{
	return speed;
}

void GameObject::SetSpeed(double speed1)
{
	speed = speed1;
}

double GameObject::GetDistance(GameObject *obj)
{
	return sqrt((x - obj->GetX())*(x - obj->GetX()) + (y - obj->GetY())*(y - obj->GetY()));
}

void GameObject::DmgDispose(int &damage,int DEFbreak)
{
	HGE *hge = hgeCreate(HGE_VERSION);
		if (!reverseDMG)
		{
			if (hge->Random_Int(0, 100) <= Dodge)
				damage = 0;
			else
			{
				damage += DEF-DEFbreak;
				if (damage > -1)
					damage = -1;
			}
		}
		else
			damage = -damage;
}


bool GameObject::varyHP(int dHP)
{

	if (HP + dHP > MAX_HP)
	{
		HP = MAX_HP;
		return false;
	}
	else
	{
		HP += dHP;
		return true;
	}
}

void GameObject::varyMP(int dMP)
{
	MP += dMP;
}

void GameObject::SetDEF(int newDEF)
{
	DEF = newDEF;
}

int GameObject::GetDEF()const
{
	return DEF;
}


void GameObject::BoomEffect(GameObject *obj, float dt, int effect)
{
	if (alive)
	{
		float newangle = GetAngle(obj,true);
		newangle -= 3.14;
		speed *= effect;
		if (x > obj->x && y > obj->y){ x -= speed*dt*cos(newangle + 1.57); y -= speed*dt*sin(newangle + 1.57); }
		if (x < obj->x && y > obj->y){ x += speed*dt*sin(newangle); y -= speed*dt*cos(newangle); }
		if (x > obj->x && y < obj->y){ x -= speed*dt*sin(newangle - 3.14); y += speed*dt*cos(newangle - 3.14); }
		if (x < obj->x && y < obj->y){ x += speed*dt*cos(newangle - 1.57); y += speed*dt*sin(newangle - 1.57); }
		speed /= effect;
	}
}




void GameObject::GoWithAnAngle(float dt,double angle)
{
	if (alive)
	{
		x += speed*speedrate*dt*cos(angle);
		y += speed*speedrate*dt*sin(angle);
	}
}


void GameObject::SpinAroundObject(GameObject *obj)
{
	if (alive)
	{
		x = (x - obj->x)*cos(0.1) - (y - obj->y)*sin(0.1) + obj->x;
		y = (x - obj->x)*sin(0.1) + (y - obj->y)*cos(0.1) + obj->y;
	}
}

void GameObject::GoWithAnArc(GameObject *end,bool fangxiang)
{
	float centerx = abs((x + end->x) / 2), centery = abs((y + end->y) / 2);
	x = (x - centerx)*cos((10 - 20 * fangxiang) / centerx) - (y - centery)*sin((10 - 20 * fangxiang) / centery) + centerx;
	y = (x - centerx)*sin((10 - 20 * fangxiang) / centerx) + (y - centery)*cos((10 - 20 * fangxiang) / centery) + centery;
	GoToAnotherObject(end);
}


bool GameObject::SetErrorState(errstate err, double var, int last_time)
{
	if (canhaveerrorstate && x<ACT_END_X && x>ACT_START_X && y<ACT_END_Y && y>ACT_START_Y)
	{
		int newerr = (int)errstate1 | (int)err;
		errstate1 = (errstate)newerr;
		if (errstate1 & freeze && !errvar_freeze)
		{
			    speedrate += var;
				if (speedrate < 0)
					speedrate = 0;
				errvar_freeze = var;
			errlast_time_freeze = last_time;
		}
		if (errstate1 & toxic && !errvar_toxic)
		{
			errlast_time_toxic = last_time;
			errvar_toxic = var;
		}
		if (errstate1 & vertigo && !errvar_vertigo)
		{
			errvar_vertigo = speedrate;
			speedrate = 0;
			errlast_time_vertigo = last_time;
		}
		if (errstate1 & heal && !errvar_heal)
		{
			errlast_time_heal = last_time;
			errvar_heal = var;
		}
		switch (err)
		{
		case freeze:
			if (last_time > errlast_time_freeze)
				errlast_time_freeze = last_time;
			break;
		case toxic:
			if (last_time > errlast_time_toxic && errlast_time_toxic<25)
				errlast_time_toxic = last_time;
			break;
		case vertigo:
			if (last_time > errlast_time_vertigo)
				errlast_time_vertigo = last_time;
			break;
		case heal:
			if (last_time > errlast_time_heal && errlast_time_heal < 5)
				errlast_time_heal = last_time;
			break;
		}
		return true;
	}
	else
		return false;
}

void GameObject::ErrorState()
{
		if (errstate1 & freeze)
		{
			if (errlast_time_freeze > 0)
			{
				/*
				SetNeedChangeColor();
				CA |= 255;
				CR |= 46;
				CG |= 187;
				CB |= 245;
				*/
				errlast_time_freeze--;
			}
			else
			{
				speedrate = 1;
				/*
				SetNeedChangeColor(false);
				CA ^= 255;
				CR ^= 46;
				CG ^= 187;
				CB ^= 245;
				*/
				errvar_freeze = 0;
				int newerr = (int)errstate1 ^ (int)freeze;
				errstate1 = (errstate)newerr;
			}
		}
		if (errstate1 & toxic)
		{
			if (errlast_time_toxic > 0)
			{
				/*
				SetNeedChangeColor();
				CA |= 255;
				CR |= 131;
				CG |= 112;
				CB |= 233;
				*/
				if (errlast_time_toxic % 25 == 0)
				{
					if (varyHP(-errvar_toxic))
						FontPool::GetFontPool().InsertFont(x, y, -errvar_toxic, Passive, NeedRenderFont());
				}
				errlast_time_toxic--;
			}
			else
			{
				/*
				SetNeedChangeColor(false);
				CA ^= 255;
				CR ^= 131;
				CG ^= 112;
				CB ^= 233;
				*/
				errvar_toxic = 0;
				int newerr = (int)errstate1 ^ (int)toxic;
				errstate1 = (errstate)newerr;
			}
		}



		if (errstate1 & vertigo)
		{
			if (errlast_time_vertigo > 0)
				errlast_time_vertigo--;
			else
			{
				speedrate = errvar_vertigo;
				errvar_vertigo = 0;
				int newerr = (int)errstate1 ^ (int)vertigo;
				errstate1 = (errstate)newerr;
			}
		}
		if (errstate1 & heal)
		{
			if (errlast_time_heal > 0)
			{
				if (errlast_time_heal % 10 == 0)
				{
					if (errstate1 & toxic)
					{
						if (errvar_heal/2!=0 && varyHP(errvar_heal / 2))
							FontPool::GetFontPool().InsertFont(x, y, errvar_heal / 2, Heal, NeedRenderFont());
					}
					else
					{
						if (varyHP(errvar_heal))
							FontPool::GetFontPool().InsertFont(x, y, errvar_heal, Heal, NeedRenderFont());
					}
				}
				errlast_time_heal--;
			}
			else
			{
				errvar_heal = 0;
				int newerr = (int)errstate1 ^ (int)heal;
				errstate1 = (errstate)newerr;
			}
		}


}

int GameObject::GetErrStateTime(errstate err)
{
	switch (err)
	{
	case freeze:
		return errlast_time_freeze;
	case toxic:
		return errlast_time_toxic;
	case vertigo:
		return errlast_time_vertigo;
	case heal:
		return errlast_time_heal;
	}
}
