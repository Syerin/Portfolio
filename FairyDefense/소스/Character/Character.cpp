#include "Character.h"
#include "GameManager.h"
#include "CallLabel.h"
#include "CallLabelManager.h"
#include "MapTile.h"
#include "StageInfo.h"
Character::Character()
{


	m_bAlive = false;
	m_iUpGradeCount = 1;
	m_bSelectCharacter = false;

	m_pCallLabelbuyPrice = NULL;
	m_pCallLabelSalePrice = NULL;

}


Character::~Character()
{
	CC_SAFE_DELETE(m_pUnitLab);

}


CHARACTER_TYPE Character::GetCharacterType()
{
	return m_eCharacterType;
}
void Character::SetUpMenu()
{
	Sprite* Upgrade = GameManager::GetInstance()->GetSpriteWithFrameFileName("UpGrade.png");
	Sprite* ClickUpGrade = GameManager::GetInstance()->GetSpriteWithFrameFileName("UpGrade.png");
	Sprite* DisableUpGrade = GameManager::GetInstance()->GetSpriteWithFrameFileName("UpGrade.png");
	ClickUpGrade->setColor(Color3B(99, 99, 99));
	DisableUpGrade->setColor(Color3B(99, 99, 99));
	Sprite* Sale = GameManager::GetInstance()->GetSpriteWithFrameFileName("Sale.png");
	Sprite* ClickSale = GameManager::GetInstance()->GetSpriteWithFrameFileName("Sale.png");
	Sprite* DisableSale = GameManager::GetInstance()->GetSpriteWithFrameFileName("Sale.png");
	ClickSale->setColor(Color3B(99, 99, 99));
	DisableSale->setColor(Color3B(99, 99, 99));

	m_pUpGradeItem = MenuItemSprite::create(Upgrade, ClickUpGrade, DisableUpGrade, CC_CALLBACK_0(Character::UpGradeItem, this));
	m_pUpGradeItem->setPosition(0, 0);
	m_pSaleItem = MenuItemSprite::create(Sale, ClickSale, DisableSale, CC_CALLBACK_0(Character::SaleItem, this));
	m_pSaleItem->setPosition(0, 0);

	m_pMenu = NULL;
	m_pMenu = Menu::create(m_pUpGradeItem, m_pSaleItem, NULL);
	m_pMenu->setVisible(false);
	m_pLayerMainScene->addChild(m_pMenu, 300);



	m_pSprUpgarde_one = GameManager::GetInstance()->GetSpriteWithFrameFileName("oneUpgrade.png");
	m_pSprUpgarde_one->setVisible(false);
	m_pLayerMainScene->addChild(m_pSprUpgarde_one,280);
	m_pSprUpgarde_two = GameManager::GetInstance()->GetSpriteWithFrameFileName("doubleUpgrade.png");
	m_pSprUpgarde_two->setVisible(false);
	m_pLayerMainScene->addChild(m_pSprUpgarde_two, 280);
}
void Character::SetiUpGradeDamageCount(int Stat)
{
	m_iUpGradeDamageCount = Stat;
}
void Character::SetAlive(bool alive)
{
	m_bAlive = alive;
}

bool Character::Surmon(MapTile* pMapTile)
{
	m_pOnMapTile = pMapTile;
	int Zorder = pMapTile->GetSprite()->getLocalZOrder();
	Vec2 AnchorPoint = pMapTile->GetSprite()->getAnchorPoint();

	m_vec2Pos = Vec2(pMapTile->GetRect().getMidX(), pMapTile->GetRect().getMinY());
	if (m_pCallLabelbuyPrice == NULL)
	{
		m_pCallLabelbuyPrice = CallLabelManager::GetInstance()->GetCallLabel();
		m_pCallLabelbuyPrice->SetEnable(true);
	}
	if (m_pCallLabelSalePrice == NULL)
	{
		m_pCallLabelSalePrice = CallLabelManager::GetInstance()->GetCallLabel();
		m_pCallLabelSalePrice->SetEnable(true);
	}


	if (GameManager::GetInstance()->GetMoney() >= m_iBuyPrice)
	{
		int curStageNum = StageInfo::GetInstance()->GetCurStage();
		int StageWidth = StageInfo::GetInstance()->GetStage(curStageNum)->GetWidth();
		int StageHeight = StageInfo::GetInstance()->GetStage(curStageNum)->GetHeight();

		m_pSprCharacter->setVisible(true);
		m_pSprCharacter->setZOrder(Zorder+1); 
		m_pSprCharacter->setPosition(m_vec2Pos.x , m_vec2Pos.y +m_pSprCharacter->getContentSize().height/2);
		m_rcRect.setRect(m_pSprCharacter->getBoundingBox().getMinX(), m_pSprCharacter->getBoundingBox().getMinY(),
			m_pSprCharacter->getBoundingBox().getMaxX() - m_pSprCharacter->getBoundingBox().getMinX(), m_pSprCharacter->getBoundingBox().getMaxY() - m_pSprCharacter->getBoundingBox().getMinY());
		
		m_bAlive = true;
		GameManager::GetInstance()->PlusAndMinusMoney(-m_iBuyPrice);

		Vec2 vec2Mid = Vec2(m_pOnMapTile->GetRect().getMidX(), m_pOnMapTile->GetRect().getMidY());
		m_pSpriteOrigin->setPosition(vec2Mid);
		m_pSprRange->setPosition(vec2Mid);
		m_vec2Pos = vec2Mid;

		Vec2 vec2Menu;
		Vec2 vec2SalePositon;
		Vec2 vec2UpPosition;
		if (pMapTile->GetMapTileWidth() == 0)
		{
			vec2Menu = Vec2(m_pSprCharacter->getBoundingBox().getMidX(), m_pSprCharacter->getBoundingBox().getMidY() -200);
			vec2UpPosition = Vec2(m_pUpGradeItem->getContentSize().width / 2, m_pSprCharacter->getContentSize().height / 2);
			vec2SalePositon = Vec2(-(m_pSprCharacter->getContentSize().width / 2) + m_pSaleItem->getContentSize().width / 2, 0);
		}
		else if (pMapTile->GetMapTileWidth() >= StageWidth-1)
		{
			vec2Menu = Vec2(m_pSprCharacter->getBoundingBox().getMidX(), m_pSprCharacter->getBoundingBox().getMidY()-50);
			vec2UpPosition = Vec2(-m_pUpGradeItem->getContentSize().width, 0);
			vec2SalePositon = Vec2(0, m_pSprCharacter->getContentSize().height/2 + m_pSaleItem->getContentSize().height/2 + 30);
		}
		else if (pMapTile->GetMapTileHeight() >= StageHeight-1)
		{
			vec2Menu = Vec2(m_pSprCharacter->getBoundingBox().getMidX(), m_pSprCharacter->getBoundingBox().getMinY());
			vec2UpPosition = Vec2((-m_pSprCharacter->getContentSize().width / 2) - m_pUpGradeItem->getContentSize().width / 2, m_pUpGradeItem->getContentSize().height / 2);
			vec2SalePositon = Vec2((m_pSprCharacter->getContentSize().width / 2) + m_pSaleItem->getContentSize().width / 2, m_pSaleItem->getContentSize().height/2);
		}
		else
		{
			vec2Menu = Vec2(m_pSprCharacter->getBoundingBox().getMidX(), m_pSprCharacter->getBoundingBox().getMidY() - 100);
			vec2UpPosition = Vec2((-m_pSprCharacter->getContentSize().width / 2) - m_pUpGradeItem->getContentSize().width / 2, 0);
			vec2SalePositon = Vec2((m_pSprCharacter->getContentSize().width / 2) + m_pSaleItem->getContentSize().width / 2, 0);
		}

		m_pMenu->setPosition(vec2Menu);
		m_pUpGradeItem->setPosition(vec2UpPosition);
		m_pSaleItem->setPosition(vec2SalePositon);


		Label* pLabelTTFbuyPrice = m_pCallLabelbuyPrice->GetLabel();
		pLabelTTFbuyPrice->setColor(Color3B::WHITE);
		pLabelTTFbuyPrice->setSystemFontSize(25);

		Vec2 vec2UpPrice = Vec2(m_pMenu->getPosition().x + m_pUpGradeItem->getPosition().x, m_pMenu->getPosition().y + 15+ m_pUpGradeItem->getPosition().y - (m_pUpGradeItem->getContentSize().height / 2));
		pLabelTTFbuyPrice->setPosition(vec2UpPrice);
		pLabelTTFbuyPrice->setLocalZOrder(330);



		Label* pLabelTTFSalePrice = m_pCallLabelSalePrice->GetLabel();
		pLabelTTFSalePrice->setColor(Color3B::WHITE);
		pLabelTTFSalePrice->setSystemFontSize(25);
		pLabelTTFSalePrice->setLocalZOrder(330);

		Vec2 vec2SalePrice = Vec2(m_pMenu->getPosition().x + m_pSaleItem->getPosition().x, m_pMenu->getPosition().y + 15 + m_pSaleItem->getPosition().y- (m_pSaleItem->getContentSize().height / 2));
		pLabelTTFSalePrice->setPosition(vec2SalePrice);

		m_pSprUpgarde_one->setPosition(Vec2(m_pSprCharacter->getBoundingBox().getMaxX(), m_pSprCharacter->getBoundingBox().getMaxY() - m_pSprCharacter->getContentSize().height / 10));
		m_pSprUpgarde_two->setPosition(Vec2(m_pSprCharacter->getBoundingBox().getMaxX(), m_pSprCharacter->getBoundingBox().getMaxY() - m_pSprCharacter->getContentSize().height/10));

		return true;
	}
	else
	{
		log("돈이 부족합니다");
		char szTemp[128] = "";
		sprintf(szTemp, "돈이 부족합니다.");
		Size visibleSize = Director::getInstance()->getVisibleSize();
		CallLabel* pCallLabel = NULL;
		pCallLabel  = CallLabelManager::GetInstance()->GetCallLabel();
		pCallLabel->SetVerticalAnim(true);
		pCallLabel->GetLabel()->enableShadow(Color4B::BLACK, Size(2, 2), 100);
		pCallLabel->GetLabel()->setOpacity(200);
		pCallLabel->GetLabel()->setLocalZOrder(996);
		pCallLabel->SetPos(Vec2(visibleSize / 2));
		pCallLabel->SetString(szTemp);
		pCallLabel->GetLabel()->setLocalZOrder(996);
		pCallLabel->GetLabel()->setColor(Color3B::YELLOW);
		pCallLabel->GetLabel()->setSystemFontSize(120);
		return false;

	}
	return false;

}
void Character::SetRect()
{
	
	float Width = m_pSprCharacter->getBoundingBox().getMaxX() - m_pSprCharacter->getBoundingBox().getMinX();
	float Height = m_pSprCharacter->getBoundingBox().getMaxY() - m_pSprCharacter->getBoundingBox().getMinY();

	m_rcRect.setRect(m_pSprCharacter->getBoundingBox().getMinX(), m_pSprCharacter->getBoundingBox().getMinY(), Width, Height);
}
Rect Character::GetRect()
{
	return m_rcRect;
}
void Character::SetAnimateDuration(float Speed)
{
	float AttackAnimEndTime = m_pAnimate[m_eDirection]->getAnimation()->getReferenceCount() * m_pAnimate[m_eDirection]->getAnimation()->getDuration();//갯수*개당시간
	m_pAnimate[m_eDirection]->setDuration((AttackAnimEndTime / m_iUpGradeCount)/Speed);//공격에걸리는시간 업그레이드수만큼 배속
}

bool Character::SelectCharacterMouse(Vec2 vec2Mouse)
{
	if (m_rcRect.containsPoint(vec2Mouse))
	{
		m_pSprRange->setVisible(true);

		char szTemp[512];
		sprintf(szTemp, "%d", m_iUpGradePrice*m_iUpGradeCount);
		Label* pLabelTTFbuyPrice = m_pCallLabelbuyPrice->GetLabel();
		pLabelTTFbuyPrice->setString(szTemp);

		sprintf(szTemp, "%d", m_iSalePrice*m_iUpGradeCount);


		Label* pLabelTTFSalePrice = m_pCallLabelSalePrice->GetLabel();
		pLabelTTFSalePrice->setString(szTemp);

		m_pMenu->setVisible(true);
		m_pSprRange->setVisible(true);

		m_pCallLabelbuyPrice->SetEnable(true);
		m_pCallLabelSalePrice->SetEnable(true);

		if (m_iUpGradeCount == 3)
		{
			m_pUpGradeItem->setEnabled(false);
			pLabelTTFbuyPrice->setVisible(false);
			pLabelTTFSalePrice->setVisible(true);
		}
		else
		{
			m_pUpGradeItem->setEnabled(true);
			pLabelTTFbuyPrice->setVisible(true);
			pLabelTTFSalePrice->setVisible(true);
		}
		m_bSelectCharacter = true;
		return true;
	}
	return false;
}
bool Character::GetSelectCharacter()
{
	return m_bSelectCharacter;
}

void Character::SetAnimIdle()
{
	m_pSprCharacter->stopAllActions();
	m_pSprCharacter->setSpriteFrame(m_pSprFrameCharacterBottom);
}

Vec2 Character::GetVec2Postion()
{
	Vec2 vec2Mid = Vec2(m_pSprCharacter->getBoundingBox().getMidX(), m_pSprCharacter->getBoundingBox().getMidY());
	return vec2Mid;
}
float Character::GetRange()
{
	float Range = 0.0f;
	if (m_iUpGradeCount > 1)
		Range = m_fRange + ((m_fRange*((float)m_iUpGradeCount / 10)));
	else
		Range = m_fRange;
	return Range;
}
bool Character::GetAlive()
{
	return m_bAlive;
}
Animate* Character::GetAnimDirectionCharacter(DIRECTION dir)
{
	return m_pAnimate[dir];
}
Sprite* Character::GetSpriteCharacter()
{
	return m_pSprCharacter;
}
void Character::UpGradeItem()
{
	m_pMenu->setVisible(false);
	m_bSelectCharacter = false;
	m_pSprRange->setVisible(false);

	m_pCallLabelbuyPrice->GetLabel()->setVisible(false);
	m_pCallLabelSalePrice->GetLabel()->setVisible(false);

	if (GameManager::GetInstance()->GetMoney() > m_iUpGradePrice*m_iUpGradeCount)
	{
		if (m_iUpGradeCount == 1)
		{
			m_pSprUpgarde_one->setVisible(true);
		}
		else
		{
			m_pSprUpgarde_one->setVisible(false);
			m_pSprUpgarde_two->setVisible(true);
		}

		char UpGradePrice[128];
		int UpPrice = m_iUpGradePrice*m_iUpGradeCount;

		sprintf(UpGradePrice, "-%d", UpPrice);

		CallLabel* pCallLabel = CallLabelManager::GetInstance()->GetCallLabel();
		pCallLabel->GetLabel()->setString(UpGradePrice);
		pCallLabel->SetVerticalAnim(true);
		pCallLabel->GetLabel()->setColor(Color3B::BLUE);
		pCallLabel->GetLabel()->enableShadow(Color4B::BLACK,Size(2,2),100);
		pCallLabel->SetPos(Vec2(m_rcRect.getMidX(), m_rcRect.getMaxY()));
		GameManager::GetInstance()->PlusAndMinusMoney(-UpPrice);
		m_iUpGradeCount++; 
		m_pSprRange->setScale( (m_fRange + (m_fRange * m_iUpGradeCount / 10)) / m_pSprRange->getContentSize().width * 2);
	}
	else
	{
		Size visibleSize = Director::getInstance()->getVisibleSize();
		CallLabel* pCallLabel = NULL;
		pCallLabel = CallLabelManager::GetInstance()->GetCallLabel();
		pCallLabel->SetVerticalAnim(true);
		pCallLabel->GetLabel()->enableShadow(Color4B::BLACK, Size(2, 2), 100);
		pCallLabel->GetLabel()->setOpacity(200);
		pCallLabel->SetPos(Vec2(visibleSize / 2));
		pCallLabel->SetString("돈이 부족합니다.");
		pCallLabel->GetLabel()->setLocalZOrder(996);
		pCallLabel->GetLabel()->setColor(Color3B::YELLOW);
		pCallLabel->GetLabel()->setSystemFontSize(120);

	}
}
void Character::SaleItem()
{
	m_pMenu->setVisible(false);
	m_bSelectCharacter = false;
	m_pCallLabelbuyPrice->GetLabel()->setVisible(false);
	m_pCallLabelSalePrice->GetLabel()->setVisible(false);
	m_pSprRange->setVisible(false);
	m_pSprUpgarde_one->setVisible(false);
	m_pSprUpgarde_two->setVisible(false);


	char SalePrice[128];
	int iSale = m_iSalePrice * m_iUpGradeCount;

	sprintf(SalePrice, "+%d", iSale);

	CallLabel* pCallLabel = CallLabelManager::GetInstance()->GetCallLabel();
	pCallLabel->GetLabel()->setString(SalePrice);
	pCallLabel->SetVerticalAnim(true);
	pCallLabel->GetLabel()->setColor(Color3B::BLUE);
	pCallLabel->GetLabel()->enableShadow(Color4B::BLACK, Size(2, 2), 100);
	pCallLabel->SetPos(Vec2(m_rcRect.getMidX(), m_rcRect.getMaxY()));
	GameManager::GetInstance()->PlusAndMinusMoney(iSale);
	m_pSprCharacter->stopAllActions();
	m_pSprCharacter->setVisible(false);
	m_pSprCharacter->setPosition(-100, -100);
	m_pOnMapTile->SetCharacterOnTile(false);
	m_pOnMapTile = NULL;
	m_pSprRange->setVisible(false);
	m_bAlive = false;
	m_iUpGradeCount = 1;

}
void Character::SetMenuVisible(bool MenuVisible)
{
	m_bSelectCharacter = false;
	m_pMenu->setVisible(MenuVisible);
	m_pCallLabelbuyPrice->GetLabel()->setVisible(MenuVisible);
	m_pCallLabelSalePrice->GetLabel()->setVisible(MenuVisible);
	m_pSprRange->setVisible(MenuVisible);
}

int Character::GetAttackDamage()
{
	return (int)(m_fDamage*m_iUpGradeCount);
}

MapTile* Character::GetOnTile()
{
	return m_pOnMapTile;
}
void Character::SetCharacterInfo(CHARACTER_TYPE CharacterType,float fAttackDamage, float fAttackDelay, int ibuyPrice, int iSalePrice, int iUpgradePrice,int UpGradeDamageCount)
{
	m_eCharacterType = CharacterType;
	m_fDamage = fAttackDamage;
	m_fDelayAttack = fAttackDelay;
	m_iBuyPrice = ibuyPrice;
	m_iSalePrice = iSalePrice;
	m_iUpGradePrice = iUpgradePrice;
	m_iUpGradeDamageCount = UpGradeDamageCount;
}


float Character::GetfAttackDelay()
{
	return m_fDelayAttack;
}
float Character::GetfAttackDamage()
{
	return m_fDamage;
}
int Character::GetiBuyPrice()

{
	return m_iBuyPrice;
}
int Character::GetiSalePrice()
{
	return m_iSalePrice;
}
int Character::GetiUpGradePrice()
{
	return m_iUpGradePrice;
}

int Character::GetiUpGradeDamageCount()
{
	return (m_iUpGradeDamageCount*m_iUpGradeCount);
}

UnitLab* Character::GetUnitLab()
{
	return m_pUnitLab;
}