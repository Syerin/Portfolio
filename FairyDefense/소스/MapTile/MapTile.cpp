#include "MapTile.h"
#include "Mob.h"
#include "GameManager.h"
MapTile::MapTile()
{
}


MapTile::~MapTile()
{
}

void MapTile::Init(Layer* pLayerMainScene,Vec2 vec2Pos)
{
	m_pLayerMainScene = pLayerMainScene;
	m_vec2Pos = vec2Pos;
	m_iWidth = vec2Pos.x;
	m_iHeight = vec2Pos.y;
	m_bCharacterOnTile = false;
}
void MapTile::SetTileType(int Type,int zOrder)
{
	m_iSpriteType = Type;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Map/MapTileArea1.plist");
	switch (m_iSpriteType)
	{
	case TILE_TYPE_GRASS_00:
		m_pSprTile = GameManager::GetInstance()->GetSpriteWithFrameFileName("Tilegrass_00.png");
		break;
	case TILE_TYPE_GRASS_01:
		m_pSprTile = GameManager::GetInstance()->GetSpriteWithFrameFileName("Tilegrass_01.png");
		break;
	case TILE_TYPE_GRASS_02:
		m_pSprTile = GameManager::GetInstance()->GetSpriteWithFrameFileName("Tilegrass_02.png");
		break;
	case TILE_TYPE_GRASS_03:
		m_pSprTile = GameManager::GetInstance()->GetSpriteWithFrameFileName("Tilegrass_03.png");
		break;
	case TILE_TYPE_GRASS_04:
		m_pSprTile = GameManager::GetInstance()->GetSpriteWithFrameFileName("Tilegrass_04.png");
		break;
	case TILE_TYPE_LOAD_00:
		m_pSprTile = GameManager::GetInstance()->GetSpriteWithFrameFileName("Mob_Load_00.png");
		break;
	case TILE_TYPE_LOAD_01:
		m_pSprTile = GameManager::GetInstance()->GetSpriteWithFrameFileName("Mob_Load_01.png");
		break;
	case TILE_TYPE_LOAD_CORNER_00:
		m_pSprTile = GameManager::GetInstance()->GetSpriteWithFrameFileName("Mob_Load_Corner_00.png");
		break;
	case TILE_TYPE_LOAD_CORNER_01:
		m_pSprTile = GameManager::GetInstance()->GetSpriteWithFrameFileName("Mob_Load_Corner_01.png");
		break;
	case TILE_TYPE_LOAD_CORNER_02:
		m_pSprTile = GameManager::GetInstance()->GetSpriteWithFrameFileName("Mob_Load_Corner_02.png");
		break;
	case TILE_TYPE_LOAD_CORNER_03:
		m_pSprTile = GameManager::GetInstance()->GetSpriteWithFrameFileName("Mob_Load_Corner_03.png");
		break;
	case TILE_TYPE_LOAD_CENTER:
		m_pSprTile = GameManager::GetInstance()->GetSpriteWithFrameFileName("Mob_Load_Center.png");
		break;
	case TILE_TYPE_CORE_STAET_00:
		m_pSprTile = GameManager::GetInstance()->GetSpriteWithFrameFileName("Core.png");
		break;
	case TILE_TYPE_END:
		break;
	default:
		break;
	}
	if (TILE_TYPE_LOAD_00 <= m_iSpriteType && m_iSpriteType <= TILE_TYPE_LOAD_01)
		m_eTileProperty = TILE_PROPERTY_LOAD;
	else if (TILE_TYPE_LOAD_CORNER_00 <= m_iSpriteType && m_iSpriteType <= TILE_TYPE_LOAD_CORNER_03)
	{

			int ConerDir = (m_iSpriteType - TILE_TYPE_LOAD_CORNER_00)+2;
			m_eTileProperty = (TILE_PROPERTY)ConerDir;
		
	}
	else if (TILE_TYPE_CORE_STAET_00 == m_iSpriteType)
		m_eTileProperty = TILE_PROPERTY_CORE;


	m_pSprTile->getBoundingBox();
	m_pSprTile->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	m_vec2Pos.x *= m_pSprTile->getContentSize().width;
	m_vec2Pos.y *= m_pSprTile->getContentSize().height;
	m_vec2Pos.y = visibleSize.height - m_vec2Pos.y- m_pSprTile->getContentSize().height; 
	m_pSprTile->setPosition(m_vec2Pos);
	m_pLayerMainScene->addChild(m_pSprTile, zOrder);
}
void MapTile::SetCharacterOnTile(bool OnCharcter)
{
	m_bCharacterOnTile = OnCharcter;
}
bool MapTile::GetCharacterOnTile()
{
	return m_bCharacterOnTile;
}
void MapTile::AdjustScale(int Width,int Height)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	float curSizeWidth = m_pSprTile->getContentSize().width;
	float curSizeHeight = m_pSprTile->getContentSize().height;
	m_pSprTile->setPosition(m_vec2Pos);
	m_rcRect.setRect(m_vec2Pos.x, m_vec2Pos.y, curSizeWidth,curSizeHeight);

	char szTemp[128];
	sprintf(szTemp, "[x %d , y %d ]", m_iWidth, m_iHeight);
}
bool MapTile::SelectTile(Vec2 vec2Mouse)
{
	if (vec2Mouse.x > m_rcRect.getMinX() && vec2Mouse.x < m_rcRect.getMaxX()  &&  vec2Mouse.y >m_rcRect.getMinY() && vec2Mouse.y < m_rcRect.getMaxY()
		&& m_iSpriteType >= TILE_TYPE_GRASS_00 && m_iSpriteType <= TILE_TYPE_GRASS_04)
	{
		return true;
	}

	return false;
}
Sprite* MapTile::GetSprite()
{
	return m_pSprTile;
}

DIRECTION MapTile::GetDirection()
{
	return m_eDir;
}
int MapTile::GetTileType()
{
	return m_iSpriteType;
}

Vec2 MapTile::GetCenterPosition()
{
	Vec2 Middle = Vec2(m_rcRect.getMidX(),m_rcRect.getMidY());
	return Middle;
}
bool MapTile::ConerInMob(Mob* pMob)
{
	if (TILE_PROPERTY_CONER_LB <= m_eTileProperty&&  m_eTileProperty <= TILE_PROPERTY_CONER_LT)
	{
		if (m_rcRect.getMinX() < pMob->GetVec2Pos().x && pMob->GetVec2Pos().x < m_rcRect.getMaxX() && 
			m_rcRect.getMinY() < pMob->GetSpriteMob()->getBoundingBox().getMinY() && pMob->GetSpriteMob()->getBoundingBox().getMinY() <m_rcRect.getMaxY())
		{
			return true;
		}
	}
	return false;
}
TILE_PROPERTY MapTile::GetTileProperty()
{
	return m_eTileProperty;
}
Rect MapTile::GetRect()
{
	return m_pSprTile->getBoundingBox();
}