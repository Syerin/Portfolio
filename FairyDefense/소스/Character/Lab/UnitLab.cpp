#include "UnitLab.h"
#include "Character.h"
#include "UnitStatMenu.h"
#include "PlayerInfo.h"
UnitLab::UnitLab()
{
}


UnitLab::~UnitLab()
{
}

void UnitLab::Init(Character* pCharacter,Layer* pLayerMainScene)
{
	m_pLayerMainScene = pLayerMainScene;
	UnitStatMenu* pUnitStatMenu = new UnitStatMenu();
	pUnitStatMenu->Init(m_pLayerMainScene,UPGRADE_STATTYPE_DAMAGE, pCharacter,this);
	m_mapUnitStatMenu.insert(std::make_pair((int)UPGRADE_STATTYPE_DAMAGE, pUnitStatMenu));
	

	m_iPlayerAbilityPoint = PlayerInfo::GetInstace()->GetAbilityPoint();

	m_iCharacterAbilityPoint = m_iPlayerAbilityPoint;
}
void UnitLab::SetRemainingAPLabel(Label* pLabel)
{
	for (auto iter = m_mapUnitStatMenu.begin(); iter != m_mapUnitStatMenu.end(); iter++)
	{
		iter->second->SetCurRemainingAPLabel(pLabel);
	}
}
int UnitLab::GetRemainingAP()
{
	m_iCharacterAbilityPoint = PlayerInfo::GetInstace()->GetAbilityPoint();

	for (auto iter = m_mapUnitStatMenu.begin(); iter != m_mapUnitStatMenu.end(); iter++)
	{
		m_iCharacterAbilityPoint -= iter->second->GetStat();
	}
	return m_iCharacterAbilityPoint;
}
void UnitLab::SetActiveStat(bool Damage)
{
	Size visiblesize = Director::getInstance()->getVisibleSize();

	float fFirstLineX = visiblesize.width / 5;
	float fMidLineX = visiblesize.width / 2;
	float fEndLineX = (visiblesize.width / 2) + fFirstLineX;

	
	float fFirstLineY = (visiblesize.height/2) + (visiblesize.height/ 5) ;
	float fMidLineY = visiblesize.height / 2;
	float fEndLineY = (visiblesize.height / 5);



	auto iterDamage = m_mapUnitStatMenu.find(UPGRADE_STATTYPE_DAMAGE);
	if (iterDamage != m_mapUnitStatMenu.end())
	{
		iterDamage->second->SetActiveStat(Damage);
		iterDamage->second->SetPos(Vec2(fFirstLineX, fFirstLineY));
	}

}

void UnitLab::SetVisible(bool visible)
{
	for (auto iter = m_mapUnitStatMenu.begin(); iter != m_mapUnitStatMenu.end(); iter++)
	{
		(*iter).second->SetActiveStat(visible);
	}
}
void UnitLab::SetPrevStat()
{
	for (auto iter = m_mapUnitStatMenu.begin(); iter != m_mapUnitStatMenu.end(); iter++)
	{
		(*iter).second->SetPrevStatus(0);
	}
}