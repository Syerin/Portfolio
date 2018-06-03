#pragma once
#include "Character.h"
class Offense;
class Caitsith : public Character
{
private:

	float m_fMagicAttackTime;
	float m_fMagicAttackTimer;

	Vec2 m_vec2Area;

	Offense* m_pOffenseLightning;

	bool m_bCharging;
	Animate* m_pAnimCharging;
public:
	virtual void Init(Layer* pLayerMainScene);
	virtual void Update(float fElapse);
	virtual void Attack(DIRECTION direction, Mob* pMob);

	Caitsith();
	~Caitsith();
};

