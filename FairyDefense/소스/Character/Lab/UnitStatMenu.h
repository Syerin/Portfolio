#pragma once
#include "defines.h"
#include "cocos2d.h"
USING_NS_CC;
class Character;
class UnitLab;
class UnitStatMenu
{
private:
	Layer* m_pLayerMainScene;
	Sprite* m_pSprStatGageBar;
	Sprite* m_pSprFillStatbar;
	MenuItemSprite* m_pMenuSprItemUp;
	MenuItemSprite* m_pMenuSprItemDown;

	Menu* m_pMenu;
	UPGRADE_STATTYPE m_eMyStatType;
	Label* m_pLabelTTF;

	Character* m_pCharacter;

	Vec2 m_vec2Pos;

	int m_iMaxStat; 
	int m_iStat;
	int m_iprevStat;
	bool m_bVisible;


	UnitLab* m_pUnitLab;
	Label* m_pLabelRemaningAp;
public:
	void Init(Layer* pLayerMainScene,UPGRADE_STATTYPE MyStatType, Character* pCharacter, UnitLab* pUnitLab);
	void SetActiveStat(bool Act); 

	void SetPos(Vec2 pos);

	void SetCharacterStat();

	void CallBackUpButton();
	void CallBackDownButton();
	Sprite* GetGageBarSprite();

	void SetCurRemainingAPLabel(Label* pLabelRemaningAP);
	inline int GetStat()
	{
		return m_iStat;
	}
	inline void SetPrevStatus(int i)
	{
		m_iprevStat = i;
	}
	UnitStatMenu();
	~UnitStatMenu();
};

