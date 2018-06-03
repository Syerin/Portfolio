#pragma once
#include "cocos2d.h"
#include "defines.h"
USING_NS_CC;
class MapTile;
class Character;
class Mob
{
protected:
	Layer* m_pLayerMainScene;
	Sprite* m_pSprCharacter;
	Sprite* m_pSprNextTargetTile;
	Vec2 m_vec2Pos;
	Animate* m_pAnimate_Left;
	Animate* m_pAnimate_right;
	MOB_TYPE m_eMobType;
	float m_fMoveSpeed;
	bool m_bAlive;
	DIRECTION m_eDirection;
	Rect m_rcRect;

	Sprite* m_pSprHealthBar;
	Sprite* m_pSprHealthBarBackground;
	
	int m_iMobZenMoeny;
	int m_iHealthPoint;
	int m_iHealthPointMax;

	int m_iMobInfoTypeNum;

	Sprite* m_pSpriteOrigin;
public:
	virtual void Init(Layer* pLayerMainScene) = 0;
	virtual void Update(float fElapse) = 0;
	virtual void SetNextTile();
	virtual Vec2 GetVec2Pos();
	virtual MOB_TYPE GetMobType();
	virtual bool GetAlive();
	virtual void SetAlive(bool live);
	virtual void SetDirection(DIRECTION MapConerTiledir);
	virtual void Move(float fEapse);
	virtual void SetPosition(MapTile* pMapTile);
	virtual bool DieCoreInMob(MapTile* pMapTile);
	virtual Rect GetRect();
	virtual void HittiingMob(Character* pCharacter);
	virtual int GetHealthPoint();
	virtual int GetMobZenMoney();
	virtual DIRECTION GetDirection();

	virtual void SetPosition(int x, int y);

	virtual Sprite* GetSpriteMob();
	virtual void SetMobInfoTypeNum(int TypeNumber);
	virtual int GetMobInfoTypeNum();
	virtual void SetStat(float MoveSpeed, int HP, int ZenMoney);
	virtual float GetMoveSpeed();
	virtual int GetHPMax();
	virtual int GetZenMoney();
	Mob();
	virtual ~Mob();
};

