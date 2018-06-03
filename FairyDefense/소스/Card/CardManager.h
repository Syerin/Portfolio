#pragma once
#include "cocos2d.h"
#include "Character.h"
#include "Card.h"
#include "MapTileManager.h"
#include "CharacterManager.h"
#include <vector>
USING_NS_CC;

class CardManager
{
private:
	Layer* m_pLayerMainScene;

	Sprite* m_pSprCancelButton;
	Sprite* m_pSprSetButton;
	Menu* m_pMenu;
	std::vector<Card*> m_vecCard;

	Menu* m_pMenuCardToggle;

	Character* m_pSelectCharacter;

	cocos2d::Vector<MenuItem*> m_vecMenuItem;
	int m_iCardCount;
	bool m_bActive;

	static CardManager* m_pThis;
	CardManager();
public:
	void Init(Layer* pLayerMainScene);

	void Update(float fElapse);

	void InsertCard(Character* pCharacter);
	
	void DrawActionCard(Vec2 Vec2Mouse);

	void CancelCardMenu();
	void ItemCancel();
	void ItemSet();

	void SelectCard(Vec2 vec2Mouse);

	bool GetCardActiveState();

	static CardManager* GetInstance()
	{
		if (m_pThis == NULL)
		{
			m_pThis = new CardManager;
		}
		return m_pThis;
	}


	~CardManager();
};

