#include "Lightning.h"
#include "MobManager.h"
#include "GameManager.h"

Lightning::Lightning()
{
}


Lightning::~Lightning()
{
}
void Lightning::Init(Layer* pLayerMainScene)
{
	m_pLayerMainScene = pLayerMainScene;
	m_pSprOffense = GameManager::GetInstance()->GetSpriteWithFrameFileName("Lightning.png");
	m_pLayerMainScene->addChild(m_pSprOffense, 300);

	m_pSprOffense->setVisible(false);
	m_pSprOffense->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	m_fSpeed = 0.0f;
	m_fArea = 100.0f;
	m_pSprOffense->setOpacity(200);
	m_bAlive = false;
	m_vec2Pos = Vec2(0, 0);

	m_pMob = NULL;
	m_pShootCharacter = NULL;

	m_rcRect.setRect(m_pSprOffense->getBoundingBox().getMinX(), m_pSprOffense->getBoundingBox().getMinY(), m_pSprOffense->getBoundingBox().getMaxX(), m_pSprOffense->getBoundingBox().getMaxY());
	m_vec2Dir = Vec2(0,0);
	m_eOffenseType = OFFENSE_TYPE_LIGHTNING;
	m_fvisibleTimer = 0.0f;
}
void Lightning::Update(float fElapse)
{
	m_fElapse = fElapse;

	if (m_bAlive == true)
	{
		m_fvisibleTimer += m_fElapse;
		if (m_fvisibleTimer > 1.0f)
		{
			m_bAlive = false;
			m_fvisibleTimer = 0.0f;
			m_pSprOffense->setVisible(false);
		}
	}
}
void Lightning::Move()
{
	
}
void Lightning::HitMob()
{
	m_fvisibleTimer = 0.0f;
	m_pSprOffense->setVisible(true);
	MobManager::GetInstance()->HitMobArea(m_pShootCharacter,m_vec2Pos,m_fArea);
}
