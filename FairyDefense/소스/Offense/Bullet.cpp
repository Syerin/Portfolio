#include "Bullet.h"
#include "Mob.h"
#include "defines.h"
#include "MathValue.h"
#include "GameManager.h"
#include "CallLabel.h"
#include "CallLabelManager.h"
Bullet::Bullet()
{
}


Bullet::~Bullet()
{
}

void Bullet::Init(Layer* pLayerMainScene)
{
	m_pLayerMainScene = pLayerMainScene;
	m_pSprOffense = GameManager::GetInstance()->GetSpriteWithFrameFileName("ELF_Bullet_center.png");
	m_pLayerMainScene->addChild(m_pSprOffense, 70);

	m_pSprBulletOutline = GameManager::GetInstance()->GetSpriteWithFrameFileName("ELF_Bullet_OutLine.png");
	m_pSprOffense->addChild(m_pSprBulletOutline);

	m_pSprOffense->setVisible(false);
	m_pSprBulletOutline->setVisible(false);
	m_vec2Pos.x = 100;
	m_vec2Pos.y = 100;
	m_pSprOffense->setPosition(m_vec2Pos);
	m_pSprBulletOutline->setPosition(m_vec2Pos/2);
	m_pSprBulletOutline->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_pSprOffense->setColor(Color3B(255, 255, 255));
	m_pSprBulletOutline->setColor(Color3B(255, 255, 255));

	m_rcRect.setRect(m_pSprBulletOutline->getBoundingBox().getMinX(), m_pSprBulletOutline->getBoundingBox().getMinY(), m_pSprBulletOutline->getBoundingBox().getMaxX(), m_pSprBulletOutline->getBoundingBox().getMaxY());
	m_fSpeed = 400.0f;
	m_bAlive = false;
	m_pMob = NULL;
	m_pShootCharacter = NULL;

	m_eOffenseType = OFFENSE_TYPE_BULLET;
}
void Bullet::Update(float fElapse)
{
	m_fElapse = fElapse;
	m_fArea = m_pSprBulletOutline->getContentSize().width / 2;
	Move();
	HitMob();
}
void Bullet::Move()
{
	if (m_pMob != NULL)
	{
		if (m_pMob->GetAlive() == true && m_bAlive ==true)
		{
			m_pSprOffense->setVisible(true);
			m_pSprBulletOutline->setVisible(true);
			m_vec2Pos.x += m_vec2Dir.x * m_fSpeed * m_fElapse;
			m_vec2Pos.y += m_vec2Dir.y * m_fSpeed * m_fElapse;

			m_pSprOffense->setPosition(m_vec2Pos);
			m_rcRect.setRect(m_pSprBulletOutline->getBoundingBox().getMinX(), m_pSprBulletOutline->getBoundingBox().getMinY(),
				(m_pSprBulletOutline->getBoundingBox().getMaxX()- m_pSprBulletOutline->getBoundingBox().getMinX()), (m_pSprBulletOutline->getBoundingBox().getMaxY()- m_pSprBulletOutline->getBoundingBox().getMinY()));
				m_vec2Dir = MathValue::GetDirection(m_vec2Pos.x, m_pMob->GetVec2Pos().x, m_vec2Pos.y, m_pMob->GetVec2Pos().y);
		}
		else if(m_pMob->GetAlive() == false)
		{
			m_bAlive = false;
			m_pSprOffense->setVisible(false);
			m_pSprBulletOutline->setVisible(false);
			m_pShootCharacter = NULL;
		}
	}
}

void Bullet::HitMob()
{
	if (m_pShootCharacter == NULL)
	{
		m_bAlive = false;
		m_pSprOffense->setVisible(false);
		m_pSprBulletOutline->setVisible(false);
		m_pMob = NULL;
		return;
	}

	if (m_pMob->GetRect().intersectsCircle(m_vec2Pos, (m_pSprBulletOutline->getBoundingBox().getMaxX() - m_pSprBulletOutline->getBoundingBox().getMinX()) / 2))
	{
		m_bAlive = false;
		m_pSprOffense->setVisible(false);
		m_pSprBulletOutline->setVisible(false);

		m_pMob->HittiingMob(m_pShootCharacter);

		m_pShootCharacter = NULL;
	}
}