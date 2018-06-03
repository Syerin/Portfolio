#include "MobManager.h"
#include "Orc.h"
#include "MathValue.h"
#include "StageInfo.h"
#include "GameManager.h"
#include "MobInfo.h"
#include "CallLabel.h"
#include "CallLabelManager.h"
#include "GenInfo.h"
#include "DarkElf.h"
#include "Golem.h"
#include "Puppet.h"
#include "SwordElf.h"
#include "Rabbit.h"
#include "WizardElf.h"
#include "MathValue.h"
MobManager* MobManager::m_pThis = NULL;


MobManager::MobManager()
{
}


MobManager::~MobManager()
{

}

void MobManager::Init(Layer* pLayerMainScene)
{
	m_pLayerMainScene = pLayerMainScene;
	for (auto iter = m_vecMob.begin(); iter != m_vecMob.end(); iter++)
	{
		CC_SAFE_DELETE((*iter));
	}
	m_vecMob.clear();

	m_pStage = StageInfo::GetInstance()->GetStage(StageInfo::GetInstance()->GetCurStage());
	m_fNextSurmonInterval = 6.0f;
	m_iSurmonCount = 0;
	m_fSurmonTimer = 0.0f;

	m_iCurWaveNumber = 0;
	m_iCoundDieMob = 0;

	for (int i = 0; i < MOE_TYPE_END; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			GerateMob((MOB_TYPE)i);
		}
	}
}

void MobManager::Update(float fElapse)
{
	if (m_vecMob.size() >= 1)
	{
		for (auto iter = m_vecMob.begin(); iter != m_vecMob.end(); iter++)
		{
			(*iter)->Update(fElapse);
		}
	} 


	m_fSurmonTimer += fElapse;
	SurmonTimer();


	for (auto iter = m_vecMob.begin(); iter != m_vecMob.end(); iter++)
	{
		if ((*iter)->GetAlive())
		{
			MapTile* pMapTile = MapTileManager::GetInstance()->GetStepOnTile((*iter));
		

			if ((*iter)->DieCoreInMob(MapTileManager::GetInstance()->GetCoreMapTile()))
			{
				GameManager::GetInstance()->CountDieMob();
				m_iCoundDieMob++;
				
			}
		}
	}
}
void MobManager::DieMobCount()
{
	m_iCoundDieMob++;
}
void MobManager::SurmonTimer()
{
	if (GameManager::GetInstance()->GetGameWaveUpVisible()==true)
	{
		return;
	}

	if (m_fNextSurmonInterval < m_fSurmonTimer)
	{
		int* pGenNumber = m_pStage->GetGenOrderNumber(m_iCurWaveNumber);
		if (pGenNumber == NULL)
			return;


		GenInfo* pGenInfo = StageInfo::GetInstance()->GetGenInfo((*pGenNumber));

		GENINFO* pStGenInfo;
		pStGenInfo = pGenInfo->GetGenInfo(m_iSurmonCount);

		if (m_iCoundDieMob == pGenInfo->GetGenAllMobCount())
		{

			m_iCurWaveNumber++;
			m_iSurmonCount = 0;
			m_iCoundDieMob = 0;
			if (GameManager::GetInstance()->GetWaveMax() > m_iCurWaveNumber)
			{
				GameManager::GetInstance()->GameWaveUpVisible(true);
			}
			
			GameManager::GetInstance()->SetCurWave(m_iCurWaveNumber);			
			return;
		}
		else if (pStGenInfo == NULL)
		{
			
			return;
		}

		int GenMobInfoTypeNumber = pStGenInfo->MobTypeNumber;
		MobNumberInfo* GenOrderMobType = MobInfo::GetInstance()->GetZenOrderMobType(GenMobInfoTypeNumber);
		
		float SpawnInterval = pStGenInfo->NextInterval;

		float fSpeed = GenOrderMobType->fSpeed;
		int Hp = GenOrderMobType->HealthPoint;
		int ZenMoney = GenOrderMobType->ZenMoney;

		MOB_TYPE mobType = GenOrderMobType->MobType;
		Mob* pMob = GetMob(mobType);

		int RespawnWidth = m_pStage->GetRespawnWidth();
		int RespawnHeight = m_pStage->GetRespawnHeight();
		pMob->SetPosition(MapTileManager::GetInstance()->GetTile(RespawnWidth, RespawnHeight));

		pMob->SetAlive(true);
		pMob->SetStat(fSpeed, Hp, ZenMoney);
		pMob->GetSpriteMob()->setVisible(true);

		m_fNextSurmonInterval = SpawnInterval;
		m_iSurmonCount++;
		m_fSurmonTimer = 0.0f;
	}
}

Mob* MobManager::GetMob(MOB_TYPE type)
{
	Mob* pMob = NULL;

	for (auto iter = m_vecMob.begin(); iter != m_vecMob.end(); iter++)
	{
		if ((*iter)->GetMobType() == type && (*iter)->GetAlive() == false)
		{
			(*iter)->Init(m_pLayerMainScene);
			pMob = (*iter);
			return pMob;
		}
	}
	if (pMob == NULL)
	{
		switch (type)
		{
		case MOB_TYPE_ORC:
			pMob = new Orc();
			break;
		case MOB_TYPE_PUPPET:
			pMob = new Puppet();
			break;

		case MOB_TYPE_DARKELF:
			pMob = new DarkElf();
			break;
		case MOB_TYPE_GOLEM:
			pMob = new Golem();
			break;
		case MOB_TYPE_SWORDELF:
			pMob = new SwordElf();
			break;
		case MOB_TYPE_RABIT:
			pMob = new Rabbit();
			break;
		case MOB_TYPE_WIZARDELF:
			pMob = new WizardElf();
			break;
		}
		pMob->Init(m_pLayerMainScene);
		m_vecMob.push_back(pMob);
	}
	return pMob;
}

void MobManager::GerateMob(MOB_TYPE type)
{

	Mob* pMob = NULL;
	switch (type)
	{
	case MOB_TYPE_ORC:
		pMob = new Orc();
		break;
	case MOB_TYPE_PUPPET:
		pMob = new Puppet();
		break;

	case MOB_TYPE_DARKELF:
		pMob = new DarkElf();
		break;
	case MOB_TYPE_GOLEM:
		pMob = new Golem();
		break;
	case MOB_TYPE_SWORDELF:
		pMob = new SwordElf();
		break;
	case MOB_TYPE_RABIT:
		pMob = new Rabbit();
		break;
	case MOB_TYPE_WIZARDELF:
		pMob = new WizardElf();
		break;
	}
	if (pMob != NULL)
	{
		pMob->Init(m_pLayerMainScene);
		m_vecMob.push_back(pMob);
	}
}
void MobManager::HitMobArea(Character* pShootCharacter, Vec2 vec2Offense, float Area)
{
	Mob* pMob;
	Vec2 vec2PosMob;
	float fDistance = 0.0f;
	for (auto iter = m_vecMob.begin(); iter != m_vecMob.end(); iter++)
	{

		if ((*iter)->GetAlive() == true)
		{
			vec2PosMob = Vec2((*iter)->GetRect().getMidX(), (*iter)->GetRect().getMinY());
			fDistance = MathValue::GetDistance(vec2Offense,vec2PosMob);
			if (Area > fDistance)
			{
				(*iter)->HittiingMob(pShootCharacter);
			}
		}
	}
}
Mob* MobManager::GetNearMob(Vec2 CharacterPos)
{
	float MinDistance = 10000;
	float curDistance = 0;
	Mob* pMob = NULL;
	for (auto iter = m_vecMob.begin(); iter != m_vecMob.end(); iter++)
	{
		if ((*iter)->GetAlive() == true)
		{
			float MobMidX = (*iter)->GetSpriteMob()->getBoundingBox().getMidX();
			float MobBottomY = (*iter)->GetSpriteMob()->getBoundingBox().getMinY();

			curDistance = MathValue::GetDistance(Vec2(MobMidX, MobBottomY), CharacterPos);

			if (MinDistance >= curDistance)
			{
				MinDistance = curDistance;
				pMob = (*iter); 
			}
			if (pMob == NULL)
				MinDistance = curDistance;
		}
	}

	if (pMob != NULL)
	{
		return pMob;
	}
	return NULL;
}