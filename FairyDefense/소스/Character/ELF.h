#pragma once
#include "Character.h"
#include "defines.h"
class Offense;
class Mob;
class ELF : public Character
{
private:
	float m_fSynchronizeAttackAnim;
	float m_fSynchronizeTimer;

	Offense* m_pOffenseBullet;
public:

	virtual void Init(Layer* pLayerMainScene);
	virtual void Update(float fElapse);
	virtual void Attack(DIRECTION direction, Mob* pMob);


	ELF();
	virtual ~ELF();
};

