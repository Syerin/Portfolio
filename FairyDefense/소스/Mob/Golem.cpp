#include "Golem.h"
#include "GameManager.h"

Golem::Golem()
{
}


Golem::~Golem()
{
}
void Golem::Init(Layer* pLayerMainScene)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Mob/Golem/GolemAnim.plist");

	m_pLayerMainScene = pLayerMainScene;

	m_pSprCharacter = GameManager::GetInstance()->GetSpriteWithFrameFileName("Golem_00.png");
	m_pLayerMainScene->addChild(m_pSprCharacter, 99);



	m_eDirection = DIRECTION_L;
	m_vec2Pos.x = visibleSize.width;
	m_vec2Pos.y = 200;
	m_bAlive = false;
	m_eMobType = MOB_TYPE_GOLEM;



	m_pSprCharacter->setVisible(false);
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
			for (int i = 0; i <= RES_GOLEM_02; i++)
			{

				sprintf(szFileName, "Golem_%.2d.png", i);
				frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFileName);
				vecFrame.pushBack(frame);
			}
			frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("Golem_01.png");
			vecFrame.pushBack(frame);

			pAnimation = Animation::createWithSpriteFrames(vecFrame, 0.6f);
			m_pAnimate_Left = Animate::create(pAnimation);
			m_pAnimate_Left->retain();
		}
		else
		{
			for (int i = RES_GOLEM_03; i <RES_GOLEM_END; i++)
			{

				sprintf(szFileName, "Golem_%.2d.png", i);
				frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFileName);
				vecFrame.pushBack(frame);
			}
			frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("Golem_04.png");
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


	m_pSpriteOrigin = Sprite::create("Origin.png");
	m_pLayerMainScene->addChild(m_pSpriteOrigin, 777);

	m_pSpriteOrigin->setAnchorPoint(m_pSprCharacter->getAnchorPoint());
	m_pSpriteOrigin->setPosition(m_vec2Pos);
	m_pSpriteOrigin->setVisible(false);
}

void Golem::Update(float fElapse)
{
	if (m_bAlive == false)
	{
		m_pSprCharacter->setVisible(false);

	}
	else
	{
		Mob::Move(fElapse);
	}
	m_pSpriteOrigin->setPosition(m_vec2Pos.x, m_pSprCharacter->getBoundingBox().getMinY());

}