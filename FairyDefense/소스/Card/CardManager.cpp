#include "CardManager.h"
#include "ELF.h"
#include "Caitsith.h"
#include "Cusith.h"
#include "GameManager.h"

CardManager* CardManager::m_pThis = NULL;


CardManager::CardManager()
{
}


CardManager::~CardManager()
{
}

void CardManager::Init(Layer* pLayerMainScene)
{
	m_pLayerMainScene = pLayerMainScene;
	for (auto iter = m_vecCard.begin(); iter != m_vecCard.end(); iter++)
	{
		CC_SAFE_DELETE((*iter));
	}
	m_vecCard.clear();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Sprite* pSpriteCancle = GameManager::GetInstance()->GetSpriteWithFrameFileName("Cancel.png");
	Sprite* pSpriteCancleDown = GameManager::GetInstance()->GetSpriteWithFrameFileName("Cancel.png");
	pSpriteCancleDown->setColor(Color3B(99, 99, 99));

	Sprite* pSpriteSet = GameManager::GetInstance()->GetSpriteWithFrameFileName("Set.png");
	Sprite* pSpriteSetDown = GameManager::GetInstance()->GetSpriteWithFrameFileName("Set.png");
	pSpriteSetDown->setColor(Color3B(99, 99, 99));

	MenuItemSprite* pItemCancel = MenuItemSprite::create(pSpriteCancle, pSpriteCancleDown, CC_CALLBACK_0(CardManager::ItemCancel, this));
	MenuItemSprite* pItemSet = MenuItemSprite::create(pSpriteSet, pSpriteSetDown, CC_CALLBACK_0(CardManager::ItemSet, this));

	m_pMenu = Menu::create(pItemSet, pItemCancel,NULL);
	m_pLayerMainScene->addChild(m_pMenu, 997);

	m_pMenu->setPosition(visibleSize.width - pItemSet->getBoundingBox().size.width*4, visibleSize.height / 2);
	m_pMenu->alignItemsHorizontallyWithPadding(pItemSet->getBoundingBox().size.width);
	m_pMenu->setVisible(false);
	m_iCardCount = 0;

	InsertCard(new ELF());
	InsertCard(new Caitsith());
	InsertCard(new Cusith());
}
void CardManager::InsertCard(Character* pCharacter)
{
	m_iCardCount++;
	Card* pCard = new Card();
	pCard->Init(m_pLayerMainScene,pCharacter);
	pCard->SetPosition(m_iCardCount);
	pCard->VisbleActionCard(false);
	m_vecCard.push_back(pCard);

}

void CardManager::DrawActionCard(Vec2 Vec2Mouse)
{
	m_pSelectCharacter = NULL;

	for (auto iter = m_vecCard.begin(); iter != m_vecCard.end(); iter++)
	{
		(*iter)->VisbleActionCard(true);

		if ((*iter)->GetRect().containsPoint(Vec2Mouse))
		{

			for (auto iterAnim = m_vecCard.begin(); iterAnim != m_vecCard.end(); iterAnim++)
			{
				(*iterAnim)->GetCardCharacterSprite()->stopAllActions();
				(*iterAnim)->GetCardCharacter()->SetAnimIdle();
			}

			(*iter)->CardAction();
		}
	}
	m_bActive = true;
	m_pMenu->setVisible(true);
	MapTileManager::GetInstance()->SetMenuVisible(true);
}
void CardManager::CancelCardMenu()
{
	for (auto iter = m_vecCard.begin(); iter != m_vecCard.end(); iter++)
	{
		(*iter)->VisbleActionCard(false);
	}
	m_pMenu->setVisible(false);
	MapTileManager::GetInstance()->SetSelectMapTile(false);
	MapTileManager::GetInstance()->SetMenuVisible(false);
	m_bActive = false;

}

void CardManager::SelectCard(Vec2 vec2Mouse)
{
	for (auto iter = m_vecCard.begin(); iter != m_vecCard.end(); iter++)
	{
		if ((*iter)->GetRect().containsPoint(vec2Mouse))
		{
			for (auto iterAnim = m_vecCard.begin(); iterAnim != m_vecCard.end(); iterAnim++)
			{
				(*iterAnim)->GetSpriteCard()->setVisible(true);
				(*iterAnim)->GetSpriteSelectCard()->setVisible(false);
				(*iterAnim)->GetLabelPrice()->setColor(Color3B(122,122,122));
				(*iterAnim)->GetCardCharacter()->GetSpriteCharacter()->setColor(Color3B(122, 122, 122));
				(*iterAnim)->GetCardCharacter()->SetAnimIdle();
			}
			m_pSelectCharacter = (*iter)->GetCardCharacter();
			(*iter)->GetLabelPrice()->setColor(Color3B::WHITE);
			Sprite* pSpriteCharacter = m_pSelectCharacter->GetSpriteCharacter();
			pSpriteCharacter->setColor(Color3B::WHITE);
			Animate* pAnimateCharacter = m_pSelectCharacter->GetAnimDirectionCharacter(DIRECTION_B);
			pSpriteCharacter->runAction(RepeatForever::create(pAnimateCharacter));

			(*iter)->GetSpriteCard()->setVisible(false);
			(*iter)->GetSpriteSelectCard()->setVisible(true);
			break;
		}		
	}
	if (m_pSelectCharacter == NULL)
	{
		for (auto iter = m_vecCard.begin(); iter != m_vecCard.end(); iter++)
		{
			(*iter)->VisbleActionCard(false);
		}

		m_pMenu->setVisible(false);
		MapTileManager::GetInstance()->SetSelectMapTile(false);
		MapTileManager::GetInstance()->SetMenuVisible(false);
		m_bActive = false;
	}
}
void CardManager::Update(float fElapse)
{
	for (auto iter = m_vecCard.begin(); iter != m_vecCard.end(); iter++)
	{
		(*iter)->Update(fElapse);
	}

}
void CardManager::ItemSet()
{
	if (m_pSelectCharacter == NULL)
		return;
	if (CharacterManager::GetInstance()->SurmonCharacter(m_pSelectCharacter->GetCharacterType(), MapTileManager::GetInstance()->GetSelectCurTile()))
	{
		MapTileManager::GetInstance()->SetCharacterOnTile(true);
	}
	for (auto iter = m_vecCard.begin(); iter != m_vecCard.end(); iter++)
	{
		(*iter)->VisbleActionCard(false);
	}
	m_pMenu->setVisible(false);
	MapTileManager::GetInstance()->SetSelectMapTile(false);
	MapTileManager::GetInstance()->SetMenuVisible(false);
	m_bActive = false;
}

void CardManager::ItemCancel()
{
	for (auto iter = m_vecCard.begin(); iter != m_vecCard.end(); iter++)
	{
		(*iter)->VisbleActionCard(false);
	}
	m_pMenu->setVisible(false);
	MapTileManager::GetInstance()->SetSelectMapTile(false);
	m_bActive = false;
}

bool CardManager::GetCardActiveState()
{
	return m_bActive;
}