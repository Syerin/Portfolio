#pragma once
#include "cocos2d.h"
USING_NS_CC;
//ĳ���ʹ� 1���� ��� ����.
class Character;
class UnitStatMenu;
class UnitLab
{
private:
	Layer* m_pLayerMainScene;
	Sprite* m_pSprGage;
	MenuItemSprite* m_pMenuSprItemUp;
	MenuItemSprite* m_pMenuSprItemDown;

	Character* m_pCharacter;

	int m_iPlayerAbilityPoint;

	int m_iCharacterAbilityPoint;

	std::map<int, UnitStatMenu*> m_mapUnitStatMenu;
public:
	void Init(Character* pCharacter,Layer* pLayerMainScene);
	void SetActiveStat(bool Damage = true); // ����Ȱ��ȭ.
	void SetVisible(bool visible);
	void SetRemainingAPLabel(Label* pLabel);
	void SetPrevStat();
	int GetRemainingAP();
	
	UnitLab();
	~UnitLab();
};

