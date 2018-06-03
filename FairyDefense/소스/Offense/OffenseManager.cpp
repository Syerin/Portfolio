#include "OffenseManager.h"
#include "Bullet.h"
#include "Lightning.h"
OffenseManager* OffenseManager::m_pThis = NULL;

OffenseManager::OffenseManager()
{
}


OffenseManager::~OffenseManager()
{
}

void OffenseManager::Init(Layer* pLayerMainScene)
{
	m_pLayerMainScene = pLayerMainScene;
	for (auto iter = m_vecOffense.begin(); iter != m_vecOffense.end(); iter++)
	{
		CC_SAFE_DELETE((*iter));
	}
	m_vecOffense.clear();

	for (int j = 0; j < OFFENSE_TYPE_END; j++)
	{
		for (int i = 0; i < 40; i++)
		{
			GenerateOffense(j);
		}
	}
}
void OffenseManager::Update(float fElapse)
{
	for (auto iter = m_vecOffense.begin(); iter != m_vecOffense.end(); iter++)
	{
		if ((*iter)->GetAlive() == true)
		{
			(*iter)->Update(fElapse);
		}
	}
}

void OffenseManager::visibleOffense(bool visible)
{
	for (auto iter = m_vecOffense.begin(); iter != m_vecOffense.end(); iter++)
	{
			(*iter)->SetAlive(visible);
	}
}

Offense* OffenseManager::GenerateOffense(int type)
{
	Offense* pOffense = NULL;
	switch (type)
	{
	case OFFENSE_TYPE_BULLET:
		pOffense = new Bullet();
		break;
	case OFFENSE_TYPE_LIGHTNING:
		pOffense = new Lightning();
		break;
	default:
		break;
	}
	pOffense->Init(m_pLayerMainScene);
	m_vecOffense.push_back(pOffense);

	return pOffense;
}
Offense* OffenseManager::GetOffense(int type)
{
	Offense* pOffense = NULL;
	for (auto iter = m_vecOffense.begin(); iter != m_vecOffense.end(); iter++)
	{
		if ((*iter)->GetAlive() == false && (*iter)->GetOffenseType() == (OFFENSE_TYPE)type)
		{
			pOffense = (*iter);

			return pOffense;
		}
	}
	pOffense = GenerateOffense(type);

	return pOffense;
}
