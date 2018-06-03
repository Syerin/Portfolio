#include "ELF.h"
#include "Bullet.h"
#include "Mob.h"
#include "UnitLab.h"
#include "OffenseManager.h"
#include "GameManager.h"

ELF::ELF()
{
}


ELF::~ELF()
{
	for (int i = 0; i < DIRECTION_END; i++)
	{
		CC_SAFE_RELEASE(m_pAnimate[i]);
	}

}

void ELF::Init(Layer* pLayerMainScene)
{
	m_pLayerMainScene = pLayerMainScene;

	Character::SetUpMenu();

	m_bAlive = false;
	m_pOffenseBullet = NULL;
	m_fAnimDuration = 0.3f;
	m_fDelayTimer = 0.0f;
	m_fRange = 300.0f;
	m_eCharacterType = CHARACTER_TYPE_ELF;

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Character/ELF/ELF_Anim.plist");

	char szFileName[256] = { 0 };
	Animate* animate;
	Animation* animation;
	m_pSprFrameCharacterBottom= SpriteFrameCache::getInstance()->getSpriteFrameByName("ELF_00.png");
	m_pSprCharacter = GameManager::GetInstance()->GetSpriteWithFrameFileName("ELF_00.png");
	m_pLayerMainScene->addChild(m_pSprCharacter, 110);

	m_pSprCharacter->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_pSprCharacter->setVisible(false);
	m_pSprCharacter->setPosition(400, 400);
	m_vec2Pos.x = 400;
	m_vec2Pos.y = 400;

	m_pSprRange  = GameManager::GetInstance()->GetSpriteWithFrameFileName("Range.png");
	m_pSprRange->setPosition(m_vec2Pos);
	m_pSprRange->setOpacity(40);
	m_pSprRange->setVisible(false);
	m_pSprRange->setScale((m_fRange /m_pSprRange->getContentSize().width)*2);
	m_pLayerMainScene->addChild(m_pSprRange, 250);

	m_pSpriteOrigin = GameManager::GetInstance()->GetSpriteWithFrameFileName("Origin.png");
	m_pSpriteOrigin->setPosition(m_vec2Pos);
	m_pLayerMainScene->addChild(m_pSpriteOrigin, 777);

	SpriteFrame* frame;
	for (int i = 0; i < DIRECTION_END; i++)
	{
		Vector<SpriteFrame*> vecFrame;

		switch (i)
		{
		case DIRECTION_B:
			for (int j = RES_ELF_B_00; j <= RES_ELF_B_01; j++)
			{
				sprintf(szFileName, "ELF_%02d.png", j);
				frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFileName);
				vecFrame.pushBack(frame);
			}

			sprintf(szFileName, "ELF_00.png");
			frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFileName);
			vecFrame.pushBack(frame);

			break;
		case DIRECTION_LB:
			for (int j = RES_ELF_LB_00; j <= RES_ELF_LB_01; j++)
			{
				sprintf(szFileName, "ELF_%02d.png", j);
				frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFileName);
				vecFrame.pushBack(frame);
			}
			sprintf(szFileName, "ELF_02.png");
			frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFileName);
			vecFrame.pushBack(frame);

			break;
		case DIRECTION_L:
			for (int j = RES_ELF_L_00; j <= RES_ELF_L_01; j++)
			{
				sprintf(szFileName, "ELF_%02d.png", j);
				frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFileName);
				vecFrame.pushBack(frame);
			}
			sprintf(szFileName, "ELF_04.png");
			frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFileName);
			vecFrame.pushBack(frame);

			break;
		case DIRECTION_LT:
			for (int j = RES_ELF_LT_00; j <= RES_ELF_LT_01; j++)
			{
				sprintf(szFileName, "ELF_%02d.png", j);
				frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFileName);
				vecFrame.pushBack(frame);
			}
			sprintf(szFileName, "ELF_06.png");
			frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFileName);
			vecFrame.pushBack(frame);
			break;
		case DIRECTION_T:
			for (int j = RES_ELF_T_00; j <= RES_ELF_T_01; j++)
			{
				sprintf(szFileName, "ELF_%02d.png", j);
				frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFileName);
				vecFrame.pushBack(frame);
			}
			sprintf(szFileName, "ELF_08.png");
			frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFileName);
			vecFrame.pushBack(frame);
			break;
		case DIRECTION_RT:
			for (int j = RES_ELF_RT_00; j <= RES_ELF_RT_01; j++)
			{
				sprintf(szFileName, "ELF_%02d.png", j);
				frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFileName);
				vecFrame.pushBack(frame);
			}
			sprintf(szFileName, "ELF_10.png");
			frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFileName);
			vecFrame.pushBack(frame);
			break;
		case DIRECTION_R:
			for (int j = RES_ELF_R_00; j <= RES_ELF_R_01; j++)
			{
				sprintf(szFileName, "ELF_%02d.png", j);
				frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFileName);
				vecFrame.pushBack(frame);
			}
			sprintf(szFileName, "ELF_12.png");
			frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFileName);
			vecFrame.pushBack(frame);
			break;
		case DIRECTION_RB:
			for (int j = RES_ELF_RB_00; j <= RES_ELF_RB_01; j++)
			{
				sprintf(szFileName, "ELF_%02d.png", j);
				frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFileName);
				vecFrame.pushBack(frame);
			}
			sprintf(szFileName, "ELF_14.png");
			frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFileName);
			vecFrame.pushBack(frame);
			break;
		case DIRECTION_END:
			break;
		default:
			break;
		}
		
		animation = Animation::createWithSpriteFrames(vecFrame, 1.0f);
		animate = Animate::create(animation);
		m_pAnimate[i] = Animate::create(animation);
		m_pAnimate[i]->retain();
	}

	m_isAttacking = false;
	m_eDirection = DIRECTION_R;
	m_pMob = NULL;
	m_pMenu->alignItemsHorizontallyWithPadding((m_pSprCharacter->getBoundingBox().getMaxX() - m_pSprCharacter->getBoundingBox().getMinX()));
	m_pUnitLab = new UnitLab();
	m_pUnitLab->Init(this, m_pLayerMainScene);
	m_pUnitLab->SetActiveStat();
	m_pUnitLab->SetVisible(false);

	m_pSpriteOrigin->setVisible(false);

	m_fSynchronizeAttackAnim = 1.0f * (m_pAnimate[m_eDirection]->getAnimation()->getReferenceCount()*(m_fAnimDuration));
	m_fSynchronizeTimer = 0.0f;

}
void ELF::Update(float fElapse)
{
	m_fElapse = fElapse*m_iUpGradeCount;
	if (m_pMob != NULL)
	{
		if (m_pMob->GetAlive() == false && m_isAttacking == true)
		{
			m_isAttacking = false;
			m_fSynchronizeTimer = 0.0f;
			m_isAttacking = false;
			m_pOffenseBullet = NULL;
			m_pMob = NULL;
		}
	}
	if (m_isAttacking)
	{
		m_fDelayTimer = 0.0f;		
		m_fSynchronizeTimer += m_fElapse;
		if (m_fSynchronizeTimer > m_fSynchronizeAttackAnim)
		{
			m_pOffenseBullet->SetPosition(Vec2(m_rcRect.getMidX(), m_rcRect.getMidY()));
			m_fSynchronizeTimer = 0.0f;
			m_isAttacking = false;
			m_pOffenseBullet = NULL;
			m_pMob = NULL;
		}

	}
	else
	{
		m_fDelayTimer += m_fElapse;
	}


	if (m_fDelayTimer > 3.0f)
	{
		m_pSprCharacter->setSpriteFrame(m_pSprFrameCharacterBottom);
	}



	if (m_pOffenseBullet != NULL && m_isAttacking == false)
	{
		if (m_pOffenseBullet->GetAlive() == false)
		{
			m_isAttacking = false;
			m_pOffenseBullet = NULL; 
			m_pMob = NULL;
		}
	}

}
void ELF::Attack(DIRECTION direction, Mob* pMob)
{
	if (m_isAttacking == false)
	{
		if (!m_pSprCharacter->getNumberOfRunningActions() && m_fDelayTimer >= m_fDelayAttack &&pMob->GetAlive() == true) 
		{
			m_eDirection = direction;
			m_pSprCharacter->stopAllActions();
			m_pAnimate[m_eDirection]->setDuration(m_fAnimDuration);
			m_pSprCharacter->runAction(m_pAnimate[m_eDirection]);
			m_isAttacking = true;
			m_pMob = pMob;
			m_pOffenseBullet = OffenseManager::GetInstance()->GetOffense(OFFENSE_TYPE_BULLET);
			m_pOffenseBullet->SetShootCharacter(this);
			m_pOffenseBullet->SetTaget(pMob);
		}
	}
}