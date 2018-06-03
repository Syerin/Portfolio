#include "UnitStatMenu.h"
#include "CallLabelManager.h"
#include "CallLabel.h"
#include "Character.h"
#include "CharacterInfo.h"
#include "UnitLab.h"
#include "GameManager.h"
UnitStatMenu::UnitStatMenu()
{
}


UnitStatMenu::~UnitStatMenu()
{
}
void UnitStatMenu::Init(Layer* pLayerMainScene,UPGRADE_STATTYPE StatType,Character* pCharacter, UnitLab* pUnitLab)
{
	m_pLayerMainScene = pLayerMainScene;
	m_pUnitLab = pUnitLab;
	m_pCharacter = pCharacter;
	m_eMyStatType = StatType;

	m_pSprStatGageBar = GameManager::GetInstance()->GetSpriteWithFrameFileName("StatGagebar.png");
	m_pLayerMainScene->addChild(m_pSprStatGageBar, 100);

	m_iMaxStat = 10;
	m_iStat = 0;

	Sprite* pSpriteUpButton = GameManager::GetInstance()->GetSpriteWithFrameFileName("StatPlusButton.png");
	Sprite* pSpriteUpButtonDown = GameManager::GetInstance()->GetSpriteWithFrameFileName("StatPlusButton.png");

	BlendFunc blendFunc1 = { GL_SRC_COLOR,GL_ONE };
	pSpriteUpButtonDown->setBlendFunc(blendFunc1);

	Sprite* pSpriteDownButton = GameManager::GetInstance()->GetSpriteWithFrameFileName("StatMinusButton.png");
	Sprite* pSpriteDownButtonDown = GameManager::GetInstance()->GetSpriteWithFrameFileName("StatMinusButton.png");
	pSpriteDownButtonDown->setBlendFunc(blendFunc1);

	m_pMenuSprItemUp = MenuItemSprite::create(pSpriteUpButton, pSpriteUpButtonDown,CC_CALLBACK_0(UnitStatMenu::CallBackUpButton,this));
	m_pMenuSprItemDown = MenuItemSprite::create(pSpriteDownButton, pSpriteDownButtonDown, CC_CALLBACK_0(UnitStatMenu::CallBackDownButton, this));

	m_pMenu = Menu::create(m_pMenuSprItemDown, m_pMenuSprItemUp,NULL);

	m_bVisible = false;
	m_pSprStatGageBar->addChild(m_pMenu,100);
	m_pMenu->alignItemsHorizontallyWithPadding(m_pSprStatGageBar->getContentSize().width);
	m_pMenu->setPosition(m_pSprStatGageBar->getContentSize().width / 2, m_pSprStatGageBar->getContentSize().height/2);

	CallLabel* pCallLabel = CallLabelManager::GetInstance()->GetCallLabel();
	pCallLabel->SetEnable(true);
	m_pLabelTTF = pCallLabel->GetLabel();
	m_pLabelTTF->setSystemFontSize(m_pSprStatGageBar->getContentSize().height/2);
	m_pLabelTTF->setPosition(m_pSprStatGageBar->getBoundingBox().getMidX(), m_pSprStatGageBar->getBoundingBox().getMidY());
	m_pLabelTTF->setZOrder(120);
	m_pLabelTTF->setColor(Color3B::BLACK);
	m_pLabelTTF->setVisible(false);
	switch (StatType)
	{
	case UPGRADE_STATTYPE_DAMAGE:
		m_pLabelTTF->setString("Damage");
		m_iStat = CharacterInfo::GetInstance()->GetCharacterInfo(m_pCharacter->GetCharacterType())->GetiUpGradeDamageCount();
		break;
	}
	float fStat = (float)m_iStat / 10;
	m_pSprFillStatbar = GameManager::GetInstance()->GetSpriteWithFrameFileName("FillStatbar.png");
	m_pSprStatGageBar->addChild(m_pSprFillStatbar, 9999);
	m_pSprFillStatbar->setPosition(0, m_pSprStatGageBar->getContentSize().height/2);
	m_pSprFillStatbar->setVisible(false);
	m_pSprFillStatbar->setScaleX(fStat);
	m_pSprFillStatbar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	m_iprevStat = 0;
}
void UnitStatMenu::SetActiveStat(bool Act)
{
	m_bVisible = Act;
	m_pSprStatGageBar->setVisible(m_bVisible);
	m_pLabelTTF->setVisible(m_bVisible);
	m_pSprFillStatbar->setVisible(m_bVisible);
	m_iStat += m_iprevStat;
	m_iprevStat = 0;
	SetCharacterStat();
	float fStat = (float)m_iStat / 10;
	m_pSprFillStatbar->setScaleX(fStat);

}
void UnitStatMenu::SetPos(Vec2 pos)
{
	m_vec2Pos = pos;
	m_pSprStatGageBar->setPosition(pos);
	m_pLabelTTF->setPosition(m_pSprStatGageBar->getBoundingBox().getMidX(), m_pSprStatGageBar->getBoundingBox().getMidY());
	m_pSprFillStatbar->setPosition(0, m_pSprStatGageBar->getContentSize().height / 2);
}


Sprite* UnitStatMenu::GetGageBarSprite()
{
	return m_pSprStatGageBar;
}
void UnitStatMenu::CallBackUpButton()
{
	if (m_pUnitLab->GetRemainingAP() <= 0)
		return;

	if (m_iStat >= m_iMaxStat)
	{
		m_iStat = m_iMaxStat;
		return;
	}
	m_iStat++;
	float fStat = (float)m_iStat/10;
	m_pSprFillStatbar->setScaleX(fStat);
	SetCharacterStat();
	m_iprevStat--;
	char szTemp[128];
	sprintf(szTemp, "%.3d", m_pUnitLab->GetRemainingAP());
	m_pLabelRemaningAp->setString(szTemp);
}

void UnitStatMenu::SetCurRemainingAPLabel(Label* pLabelCurTitleRemaningAP)
{
	m_pLabelRemaningAp = pLabelCurTitleRemaningAP;
}
void UnitStatMenu::CallBackDownButton()
{
	if (m_iStat <= 0)
	{
		m_iStat = 0;
		return;
	}

	m_iStat--;
	float fStat = (float)m_iStat / 10;
	m_pSprFillStatbar->setScaleX(fStat);
	SetCharacterStat();
	m_iprevStat++;

		char szTemp[128];
		sprintf(szTemp, "%.3d", m_pUnitLab->GetRemainingAP());
		m_pLabelRemaningAp->setString(szTemp);
}

void UnitStatMenu::SetCharacterStat()
{
	if (m_pCharacter == NULL)
		return;
	switch (m_eMyStatType)
	{
	case UPGRADE_STATTYPE_DAMAGE:
		m_pCharacter->SetiUpGradeDamageCount(m_iStat);
		m_iStat = m_pCharacter->GetiUpGradeDamageCount();
		break;
	case UPGRADE_STATTYPE_END:
		break;
	default:
		break;
	}

}