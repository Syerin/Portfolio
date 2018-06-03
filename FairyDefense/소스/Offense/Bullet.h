#pragma once
#include "Offense.h"
class Mob;
class Character;
class Bullet : public Offense
{
private:
	Sprite* m_pSprBulletOutline;

public:
	virtual void Init(Layer* pLayerMainScene);
	virtual void Update(float fElapse);
	virtual void Move();
	virtual void HitMob();
	Bullet();
	~Bullet();
};

