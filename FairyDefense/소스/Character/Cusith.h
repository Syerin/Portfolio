#pragma once
#include "Character.h"


class Cusith : public Character
{
private:
	float m_fSynchronizeAttackAnim;
	float m_fSynchronizeTimer;

public:	
	virtual void Init(Layer* pLayerMainScene);
	virtual void Update(float fElapse);
	virtual void Attack(DIRECTION direction, Mob* pMob);


	Cusith();
	~Cusith();
};

