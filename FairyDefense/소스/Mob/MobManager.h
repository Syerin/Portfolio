#pragma once
#include <vector>
#include "Mob.h"
#include "defines.h"
#include "MapTileManager.h"
class Stage;
class Character;
class MobManager
{
private:
	Layer* m_pLayerMainScene;
	Stage* m_pStage;
	std::vector<Mob*> m_vecMob;


	int m_iZenOrderMax;

	int* m_iNextMobNumber;
	
	int m_iSurmonCount;
	float m_fSurmonTimer;
	float m_fNextSurmonInterval;


	int m_iCurWaveNumber;

	int m_iCoundDieMob;
	void GerateMob(MOB_TYPE type);
	static MobManager* m_pThis;
	MobManager();
public:
	void Init(Layer* pLayerMainScene);
	void Update(float fElapse);
	void SurmonTimer();
	Mob* GetMob(MOB_TYPE type);
	Mob* GetNearMob(Vec2 CharacterPos);
	void HitMobArea(Character* pShootCharacter,Vec2 vec2Offense, float Area);

	void DieMobCount();
	static MobManager* GetInstance()
	{
		if (m_pThis == NULL)
		{
			m_pThis = new MobManager;
		}
		return m_pThis;
	}
	~MobManager();
};

