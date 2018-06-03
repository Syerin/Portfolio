#include "Offense.h"
#include "Mob.h"
#include "MobManager.h"


Offense::Offense()
{
}


Offense::~Offense()
{
}
bool Offense::GetAlive()
{
	return m_bAlive;
}
void Offense::SetPosition(Vec2 charahcterVec2)
{
	m_vec2Pos = charahcterVec2;
	m_pSprOffense->setPosition(m_vec2Pos);
	m_bAlive = true;

}
void Offense::SetShootCharacter(Character* pCharacter)
{
	m_pShootCharacter = pCharacter;
}

void Offense::SetAlive(bool alive)
{
	m_bAlive = alive;
	if (m_bAlive == false)
	{
		m_pSprOffense->setVisible(false);
	}
}

void Offense::SetTaget(Mob* pMob)
{
	m_pMob = pMob;
}

OFFENSE_TYPE Offense::GetOffenseType()
{
	return m_eOffenseType;
}