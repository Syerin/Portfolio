#include "Orc.h"
#include "GameManager.h"

Orc::Orc()
{
}


Orc::~Orc()
{
	CC_SAFE_RELEASE(m_pAnimate_Left);
	CC_SAFE_RELEASE(m_pAnimate_right);
}

void Orc::Init(Layer* pLayerMainScene)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();


	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Mob/Orc/Orc.plist");

	m_pLayerMainScene = pLayerMainScene;

	m_pSprCharacter = GameManager::GetInstance()->GetSpriteWithFrameFileName("Orc_00.png");
	m_pLayerMainScene->addChild(m_pSprCharacter, 99);




	m_iMobZenMoeny = 6;
	m_iHealthPoint = 30;
	m_iHealthPointMax = m_iHealthPoint;

	m_eDirection = DIRECTION_L;
	m_vec2Pos.x = visibleSize.width;
	m_vec2Pos.y = 200;
	m_bAlive = false;
	m_eMobType = MOB_TYPE_ORC;

	m_pSprCharacter->setVisible(true);
	m_pSprCharacter->setPosition(m_vec2Pos);
	m_pSprCharacter->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_fMoveSpeed = 100.0f;
	Animation* pAnimation;
	SpriteFrame* frame;
	char szFileName[256];
	for (int i = 0; i < 2; i++)
	{
		Vector<SpriteFrame*> vecFrame;
		if (i == 0)
		{
			for (int i = 0; i <= RES_ORC_02; i++)
			{

				sprintf(szFileName, "Orc_%.2d.png", i);
				frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFileName);
				vecFrame.pushBack(frame);
			}
			frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("Orc_01.png");
			vecFrame.pushBack(frame);

			pAnimation = Animation::createWithSpriteFrames(vecFrame, 0.6f);
			m_pAnimate_Left = Animate::create(pAnimation);
			m_pAnimate_Left->retain();
		}
		else
		{
			for (int i = RES_ORC_03; i <RES_ORC_END; i++)
			{

				sprintf(szFileName, "Orc_%.2d.png", i);
				frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFileName);
				vecFrame.pushBack(frame);
			}
			frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("Orc_04.png");
			vecFrame.pushBack(frame);

			pAnimation = Animation::createWithSpriteFrames(vecFrame, 0.6f);
			m_pAnimate_right = Animate::create(pAnimation);
			m_pAnimate_right->retain();

		}
	}


	m_pSprCharacter->runAction(RepeatForever::create(m_pAnimate_Left));

	m_pSprHealthBarBackground = GameManager::GetInstance()->GetSpriteWithFrameFileName("HealthPointBackground.png");
	m_pSprCharacter->addChild(m_pSprHealthBarBackground);
	m_pSprHealthBarBackground->setAnchorPoint(Vec2::ANCHOR_MIDDLE);


	m_pSprHealthBar = GameManager::GetInstance()->GetSpriteWithFrameFileName("HealthPoint.png");
	m_pSprCharacter->addChild(m_pSprHealthBar);
	m_pSprHealthBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE);


	m_pSpriteOrigin = GameManager::GetInstance()->GetSpriteWithFrameFileName("Origin.png");
	m_pLayerMainScene->addChild(m_pSpriteOrigin, 777);

	m_pSpriteOrigin->setAnchorPoint(m_pSprCharacter->getAnchorPoint());
	m_pSpriteOrigin->setPosition(m_vec2Pos);
	m_pSpriteOrigin->setVisible(false);
}
void Orc::Update(float fElapse)
{
	if (m_bAlive == false)
	{
		m_pSprCharacter->setVisible(false);

	}
	else
	{
		Mob::Move(fElapse);
	}
	m_pSpriteOrigin->setPosition(m_vec2Pos);

}