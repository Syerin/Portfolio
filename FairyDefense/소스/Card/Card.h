#pragma once
#include "cocos2d.h"
#include "Character.h"
USING_NS_CC;
class CallLabel;
class Card
{
private:
	Layer* m_pLayerMainScene;
	Sprite* m_pSprCard;
	Sprite* m_pSprSelectCard;

	Sprite* m_pSprCharacter;

	Character* m_pCharacter;
	Vec2 m_vec2Pos;
	Rect m_rcRect;
	float m_fActionDelay; 
	MenuItemSprite* m_pMenuCardItem;

	CallLabel* m_pCallLabelPrice;
	int m_iCharacterPrice;

	Sequence* m_pSequence;
public:
	void Init(Layer* pLayerMainScene,Character* CardCharacter);
	void Update(float fElapse);
	void SetPosition(int index);
	void VisbleActionCard(bool act);

	void CardAction();
	Sprite* GetCardCharacterSprite();
	Rect GetRect();
	Character* GetCardCharacter();

	Sprite* GetSpriteCard();
	Sprite* GetSpriteSelectCard();
	Label* GetLabelPrice();

	Card();
	~Card();
};

