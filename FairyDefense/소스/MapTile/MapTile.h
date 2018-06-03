#pragma once
#include "cocos2d.h"
#include "defines.h"
USING_NS_CC;
class Mob;
class MapTile
{
private:
	Layer* m_pLayerMainScene;
	Rect m_rcRect;
	Sprite* m_pSprTile;
	int m_iSpriteType;
	Vec2 m_vec2Pos;
	int m_iWidth;
	int m_iHeight;
	LabelTTF* m_pLabelTTF;
	DIRECTION m_eDir;
	bool m_bCharacterOnTile;
	TILE_PROPERTY m_eTileProperty;
public:
	void Init(Layer* pLayerMainScene,Vec2 vec2Pos);
	void SetTileType(int Type, int zOrder);
	void AdjustScale(int Width,int Height);
	bool SelectTile(Vec2 vec2Mouse);
	void SetCharacterOnTile(bool OnCharacter);
	bool GetCharacterOnTile();
	Sprite* GetSprite();
	DIRECTION GetDirection();
	int GetTileType();
	Vec2 GetCenterPosition();
	bool ConerInMob(Mob* pMob);
	TILE_PROPERTY GetTileProperty();
	Rect GetRect();

	inline int GetMapTileWidth()
	{
		return m_iWidth;
	}

	inline int GetMapTileHeight()
	{
		return m_iHeight;
	}


	MapTile();
	~MapTile();
};

