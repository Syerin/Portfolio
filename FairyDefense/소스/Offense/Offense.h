#pragma once
#include "defines.h"
#include "cocos2d.h"
USING_NS_CC;
class Character;
class Mob;
class Offense
{
public:
	Layer* m_pLayerMainScene;
	Sprite* m_pSprOffense;
	float m_fSpeed;
	float m_fElapse;
	float m_fArea;

	bool m_bAlive;
	Vec2 m_vec2Pos;
	Mob* m_pMob;
	Rect m_rcRect;
	Vec2 m_vec2Dir;
	Character* m_pShootCharacter;
	OFFENSE_TYPE m_eOffenseType;
public:
	virtual void Init(Layer* pLayerMainScene) = 0;
	virtual void Update(float fElapse) = 0;
	virtual void Move() = 0;
	virtual void HitMob() = 0;

	virtual	void SetShootCharacter(Character* pCharacter);
	virtual void SetTaget(Mob* pMob);
	virtual void SetAlive(bool alive);

	virtual bool GetAlive();
	virtual void SetPosition(Vec2 CharacterVec2);

	virtual OFFENSE_TYPE GetOffenseType();
	Offense();
	~Offense();
};

