#include "Card.h"
#include "CallLabelManager.h"
#include "CallLabel.h"
#include "CharacterInfo.h"
#include "GameManager.h"
Card::Card()
{
}


Card::~Card()
{
	CC_SAFE_RELEASE(m_pSequence);
}

void Card::Init(Layer* pLayerMainScene,Character* CardCharacter)
{
	m_pLayerMainScene = pLayerMainScene;
	m_pCharacter = CardCharacter;
	m_pCharacter->Init(m_pLayerMainScene);

	m_pSprSelectCard = GameManager::GetInstance()->GetSpriteWithFrameFileName("Card.png");
	m_pLayerMainScene->addChild(m_pSprSelectCard, 330);

 	m_pSprSelectCard->setPosition(0, 0);
	m_pSprSelectCard->setVisible(false);
	m_pSprSelectCard->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

	m_pSprCard = GameManager::GetInstance()->GetSpriteWithFrameFileName("Card.png");
	m_pLayerMainScene->addChild(m_pSprCard, 330);

	m_pSprCard->setColor(Color3B(122, 122, 122));
	m_rcRect.setRect(m_pSprCard->getBoundingBox().getMinX(), m_pSprCard->getBoundingBox().getMinY(), m_pSprCard->getContentSize().width, m_pSprCard->getContentSize().height);
	m_pSprCard->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	m_pSprCard->setPosition(0, 0);
	m_pSprCard->setVisible(false);

	m_pSprCharacter = m_pCharacter->GetSpriteCharacter();
	m_pSprCharacter->setLocalZOrder(331);
	m_pSprCharacter->setColor(Color3B(122, 122, 122));
	m_pSprCharacter->setVisible(true);
	m_iCharacterPrice = CharacterInfo::GetInstance()->GetCharacterInfo(m_pCharacter->GetCharacterType())->GetiBuyPrice();

	m_pCallLabelPrice = CallLabelManager::GetInstance()->GetCallLabel();
	m_pCallLabelPrice->SetEnable(true);
	m_pCallLabelPrice->SetColor(Color3B::WHITE);
	m_pCallLabelPrice->SetColor(Color3B(122, 122, 122));
	m_pCallLabelPrice->GetLabel()->setSystemFontSize(40);

	Label* pLabelTTFPrice = m_pCallLabelPrice->GetLabel();
	char Price[32];
	sprintf(Price, "%d", m_iCharacterPrice);
	pLabelTTFPrice->setString(Price);
	pLabelTTFPrice->setLocalZOrder(331);
}
void Card::Update(float fElapse)
{
	if (m_pCharacter == NULL)
		return;

	m_rcRect = m_pSprCard->getBoundingBox();
	m_pSprCharacter->setPosition(m_pSprCard->getBoundingBox().getMidX(), m_pSprCard->getBoundingBox().getMidY() +m_pSprCard->getContentSize().height / 5);
	Label* pLabelTTFPrice = m_pCallLabelPrice->GetLabel();
	pLabelTTFPrice->setPosition(m_pSprCard->getBoundingBox().getMidX(), m_pSprCard->getBoundingBox().getMinY() + 30);

	if (!m_pSprCard->getNumberOfRunningActions())
	{
		bool card = m_pSprCard->isVisible();
		bool card2 = m_pSprSelectCard->isVisible();

		m_pSprSelectCard->setPosition(m_pSprCard->getPosition());
	}
}
Character* Card::GetCardCharacter()
{
	return m_pCharacter;
}
void Card::SetPosition(int index)
{
	m_vec2Pos.x = index*m_pSprCard->getContentSize().width;
	m_vec2Pos.y = -m_pSprCard->getContentSize().height;
	m_pSprCard->setPosition(m_vec2Pos);

	MoveBy* pMoveBy = MoveBy::create(0.4f, Vec2(0, m_pSprCard->getContentSize().height));
	DelayTime* pDelayTime = DelayTime::create((index-1)* 0.1f);
	m_pSequence = Sequence::create(pDelayTime, pMoveBy, NULL);
	m_pSequence->retain();
	m_pSprCharacter->setPosition(m_pSprCard->getBoundingBox().getMidX(), m_pSprCard->getBoundingBox().getMidY() + m_pSprCard->getContentSize().height/5 );
}
void Card::VisbleActionCard(bool act)
{
	m_pCharacter->SetAnimIdle();
	m_pSprCard->setVisible(act);
	m_pSprCharacter->setVisible(act);
	m_pSprSelectCard->setVisible(act);
	m_pSprCharacter->setColor(Color3B(122, 122, 122));
	m_pCallLabelPrice->GetLabel()->setColor(Color3B(122, 122, 122));

	m_pCallLabelPrice->GetLabel()->setVisible(act);

	if (act)
	{
		m_pSprCard->runAction(m_pSequence);
	}
	else
	{
		m_pSprCard->stopAllActions();
		m_pSprCard->setPosition(m_vec2Pos);
		m_pSprCharacter->setPosition(m_pSprCard->getBoundingBox().getMidX(), m_pSprCard->getBoundingBox().getMidY() + m_pSprCard->getContentSize().height / 5);
	}
}
Rect Card::GetRect()
{
	return m_rcRect;
}

Sprite* Card::GetCardCharacterSprite()
{
	return m_pSprCharacter;
}

void Card::CardAction()
{
	m_pCharacter->GetAnimDirectionCharacter(DIRECTION_B)->setDuration(0.6f);
	m_pSprCharacter->runAction(RepeatForever::create(m_pCharacter->GetAnimDirectionCharacter(DIRECTION_B)));
}
Sprite* Card::GetSpriteCard()
{
	return m_pSprCard;
}
Sprite* Card::GetSpriteSelectCard()
{
	return m_pSprSelectCard;
}
Label* Card::GetLabelPrice()
{
	return m_pCallLabelPrice->GetLabel();
}