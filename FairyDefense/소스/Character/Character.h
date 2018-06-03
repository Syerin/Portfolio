#pragma once
#include "defines.h"
#include "cocos2d.h"
#include "UnitLab.h"

USING_NS_CC;
class Mob;
class MapTile;
class CallLabel;
class Character
{
protected:
	Sprite* m_pSprCharacter;
	CHARACTER_TYPE m_eCharacterType;
	Sprite* m_pSprCurTile;

	MapTile* m_pOnMapTile;
	Vec2 m_vec2Pos;
	Animate* m_pAnimate[DIRECTION_END];
	DIRECTION m_eDirection;
	UnitLab* m_pUnitLab;
	bool m_bAlive;

	float m_fAnimDuration;
	float m_fDelayAttack;
	float m_fDelayTimer;
	float m_fRange;

	int m_iBuyPrice;

	Rect m_rcRect;
	int m_iUpGradePrice;
	int m_iSalePrice;
	int m_iUpGradeCount; 

	Menu* m_pMenu;
	MenuItem* m_pUpGradeItem;
	MenuItem* m_pSaleItem;
	bool m_bSelectCharacter;

	float m_fDamage;

	int m_iUpGradeDamageCount;

	Sprite* m_pSprRange;
	Sprite* m_pSpriteOrigin;
	Sprite* m_pSprUpgarde_one;
	Sprite* m_pSprUpgarde_two;


	CallLabel* m_pCallLabelbuyPrice;
	CallLabel* m_pCallLabelSalePrice;

	SpriteFrame* m_pSprFrameCharacterBottom;

	Mob* m_pMob;
	float m_fElapse;
	bool m_isAttacking;

	Layer* m_pLayerMainScene;
public:
	virtual void Init(Layer* pLayerMainScene) = 0;
	virtual void SetUpMenu();
	virtual void Update(float fElapse) = 0;
	virtual void Attack(DIRECTION direction,Mob* pMob) = 0;

	virtual CHARACTER_TYPE GetCharacterType();
	virtual bool Surmon(MapTile* pMapTile);
	virtual Vec2 GetVec2Postion();

	virtual bool SelectCharacterMouse(Vec2 vec2Mouse);
	virtual bool GetSelectCharacter();
	virtual float GetRange();
	virtual bool GetAlive();
	virtual void UpGradeItem();
	virtual void SaleItem();
	virtual void SetMenuVisible(bool MenuVisible);
	virtual Sprite* GetSpriteCharacter();
	virtual Animate* GetAnimDirectionCharacter(DIRECTION dir);
	virtual Rect GetRect();
	virtual void SetRect();
	virtual int GetAttackDamage();

	virtual void SetCharacterInfo(CHARACTER_TYPE CharacterType,float fAttackDamage, float fAttackDelay, int ibuyPrice, int iSalePrice, int iUpgradePrice,int UpGradeDamageCount);
	virtual void SetAlive(bool alive);


	virtual void SetAnimateDuration(float Speed);

	virtual void SetAnimIdle();

	virtual float GetfAttackDelay();
	virtual float GetfAttackDamage();
	virtual int GetiBuyPrice();
	virtual int GetiSalePrice();
	virtual int GetiUpGradePrice();
	virtual int GetiUpGradeDamageCount();
	virtual MapTile* GetOnTile();
	virtual void SetiUpGradeDamageCount(int UpCount);

	virtual UnitLab* GetUnitLab();
	Character();
	virtual ~Character();
};

