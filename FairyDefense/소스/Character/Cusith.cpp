#include "Cusith.h"
#include "Mob.h"
#include "MobManager.h"
#include "GameManager.h"

Cusith::Cusith()
{
}


Cusith::~Cusith()
{
	for (int i = 0; i < DIRECTION_END; i++)
	{
		CC_SAFE_RELEASE(m_pAnimate[i]);
	}

}

void Cusith::Init(Layer* pLayerMainScene)
{
	m_pLayerMainScene = pLayerMainScene;

	Character::SetUpMenu();

	m_bAlive = false;
	m_fAnimDuration = 0.3f;
	m_fDelayTimer = 0.0f;
	m_fRange = 150.0f;

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Character/CU.SITH/CusithAnim.plist");
	m_eCharacterType = CHARACTER_TYPE_CUSITH;

	char szFileName[256] = { 0 };
	Animate* animate;
	Animation* animation;
	m_pSprFrameCharacterBottom = SpriteFrameCache::getInstance()->getSpriteFrameByName("CUSITH_00.png");
	m_pSprCharacter = GameManager::GetInstance()->GetSpriteWithFrameFileName("CUSITH_00.png");
	m_pLayerMainScene->addChild(m_pSprCharacter, 110);

	m_pSprCharacter->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_pSprCharacter->setVisible(false);
	m_pSprCharacter->setPosition(400, 400);
	m_vec2Pos.x = 400;
	m_vec2Pos.y = 400;

	m_pSprRange = GameManager::GetInstance()->GetSpriteWithFrameFileName("Range.png");
	m_pLayerMainScene->addChild(m_pSprRange, 250);

	m_pSprRange->setPosition(m_vec2Pos);
	m_pSprRange->setOpacity(40);
	m_pSprRange->setVisible(false);
	m_pSprRange->setScale((m_fRange / m_pSprRange->getContentSize().width) * 2);

	m_pSpriteOrigin = GameManager::GetInstance()->GetSpriteWithFrameFileName("Origin.png");
	m_pLayerMainScene->addChild(m_pSpriteOrigin, 777);

	m_pSpriteOrigin->setPosition(m_vec2Pos);

	SpriteFrame* frame;
	for (int i = 0; i < DIRECTION_END; i++)
	{
		Vector<SpriteFrame*> vecFrame;

		switch (i)
		{
		case DIRECTION_B:
			for (int j = RES_CUSITH_B_01; j <= RES_CUSITH_B_02; j++)
			{
				sprintf(szFileName, "CUSITH_%02d.png", j);
				frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFileName);
				vecFrame.pushBack(frame);
			}

			sprintf(szFileName, "CUSITH_00.png");
			frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFileName);
			vecFrame.pushBack(frame);

			break;
		case DIRECTION_LB:
			for (int j = RES_CUSITH_LB_00; j <= RES_CUSITH_LB_02; j++)
			{
				sprintf(szFileName, "CUSITH_%02d.png", j);
				frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFileName);
				vecFrame.pushBack(frame);
			}
			sprintf(szFileName, "CUSITH_03.png");
			frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFileName);
			vecFrame.pushBack(frame);

			break;
		case DIRECTION_L:
			for (int j = RES_CUSITH_L_00; j <= RES_CUSITH_L_02; j++)
			{
				sprintf(szFileName, "CUSITH_%02d.png", j);
				frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFileName);
				vecFrame.pushBack(frame);
			}
			sprintf(szFileName, "CUSITH_06.png");
			frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFileName);
			vecFrame.pushBack(frame);

			break;
		case DIRECTION_LT:
			for (int j = RES_CUSITH_LT_00; j <= RES_CUSITH_LT_02; j++)
			{
				sprintf(szFileName, "CUSITH_%02d.png", j);
				frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFileName);
				vecFrame.pushBack(frame);
			}
			sprintf(szFileName, "CUSITH_09.png");
			frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFileName);
			vecFrame.pushBack(frame);
			break;
		case DIRECTION_T:
			for (int j = RES_CUSITH_T_00; j <= RES_CUSITH_T_02; j++)
			{
				sprintf(szFileName, "CUSITH_%02d.png", j);
				frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFileName);
				vecFrame.pushBack(frame);
			}
			sprintf(szFileName, "CUSITH_12.png");
			frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFileName);
			vecFrame.pushBack(frame);
			break;
		case DIRECTION_RT:
			for (int j = RES_CUSITH_RT_00; j <= RES_CUSITH_RT_02; j++)
			{
				sprintf(szFileName, "CUSITH_%02d.png", j);
				frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFileName);
				vecFrame.pushBack(frame);
			}
			sprintf(szFileName, "CUSITH_15.png");
			frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFileName);
			vecFrame.pushBack(frame);
			break;
		case DIRECTION_R:
			for (int j = RES_CUSITH_R_00; j <= RES_CUSITH_R_02; j++)
			{
				sprintf(szFileName, "CUSITH_%02d.png", j);
				frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFileName);
				vecFrame.pushBack(frame);
			}
			sprintf(szFileName, "CUSITH_18.png");
			frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFileName);
			vecFrame.pushBack(frame);
			break;
		case DIRECTION_RB:
			for (int j = RES_CUSITH_RB_00; j <= RES_CUSITH_RB_02; j++)
			{
				sprintf(szFileName, "CUSITH_%02d.png", j);
				frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFileName);
				vecFrame.pushBack(frame);
			}
			sprintf(szFileName, "CUSITH_21.png");
			frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFileName);
			vecFrame.pushBack(frame);
			break;
		case DIRECTION_END:
			break;
		default:
			break;
		}

		animation = Animation::createWithSpriteFrames(vecFrame, m_fAnimDuration);
		animate = Animate::create(animation);
		m_pAnimate[i] = Animate::create(animation);
		m_pAnimate[i]->retain();
	}

	m_isAttacking = false;
	m_eDirection = DIRECTION_R;
	m_pMob = NULL;
	m_pMenu->alignItemsHorizontallyWithPadding((m_pSprCharacter->getBoundingBox().getMaxX() - m_pSprCharacter->getBoundingBox().getMinX()));
	m_pUnitLab = new UnitLab();
	m_pUnitLab->Init(this,m_pLayerMainScene);
	m_pUnitLab->SetActiveStat();
	m_pUnitLab->SetVisible(false);

	m_pSpriteOrigin->setVisible(false);
	m_fSynchronizeAttackAnim = 1.0f * (m_pAnimate[m_eDirection]->getAnimation()->getReferenceCount()*(m_fAnimDuration));
	m_fSynchronizeTimer = 0.0f;
}

void Cusith::Update(float fElapse)
{

	m_fElapse = fElapse * m_iUpGradeCount;
	if (m_pMob != NULL && m_pMob->GetAlive() == false)
	{
		m_isAttacking = false;
		m_fSynchronizeTimer = 0.0f;
		m_fDelayTimer = 0.0f;
		m_pMob = NULL;
	}
	if (m_isAttacking)
	{
		m_fDelayTimer = 0.0f;
		m_fSynchronizeTimer += m_fElapse;
		if (m_fSynchronizeTimer > m_fSynchronizeAttackAnim)
		{
			m_pMob->HittiingMob(this);
			m_fSynchronizeTimer = 0.0f;
			m_isAttacking = false;
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
}
void Cusith::Attack(DIRECTION direction, Mob* pMob)
{
	if (m_isAttacking == false && m_pMob ==NULL)
	{
		if (!m_pSprCharacter->getNumberOfRunningActions() && m_fDelayTimer >= m_fDelayAttack && pMob->GetAlive() == true) 
		{
			m_fSynchronizeTimer = 0.0f;
			m_eDirection = direction;
			m_pSprCharacter->stopAllActions();
			m_pSprCharacter->runAction(m_pAnimate[m_eDirection]);
			m_isAttacking = true;
			m_pMob = pMob;
		}
	}
}