#include "MapTileManager.h"
#include "StageInfo.h"
#include "Mob.h"
#include "GameManager.h"
MapTileManager* MapTileManager::m_pThis = NULL;;

MapTileManager::MapTileManager()
{
}


MapTileManager::~MapTileManager()
{
		for (int i = 0; i < m_iWidth; i++)
		{
			delete[] m_pMapTile[i];
			m_pMapTile[i] = NULL;
		}
		delete[] m_pMapTile;
}

void MapTileManager::Init(Layer* pLayerMainScene,int StageNum)
{
	m_pLayerMainScene = pLayerMainScene;
	Stage* pStage = NULL;
	pStage = StageInfo::GetInstance()->GetStage(StageNum);
	m_iWidth = pStage->GetWidth();
	m_iHeight = pStage->GetHeight();
	m_iMap = pStage->GetStage();

	m_pSprSelectTile = GameManager::GetInstance()->GetSpriteWithFrameFileName("SelectTile.png");
	m_pLayerMainScene->addChild(m_pSprSelectTile, 50);

	m_pSprSelectTile->setVisible(false);
	m_bSelectTile = false;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	m_pSelectMapTile = NULL;
	m_bMenuVisible = false;
	for (int i = 0; i < m_iWidth; i++)
	{
		m_pMapTile = new MapTile*[m_iWidth];
	}
	
	for (int i = 0; i < m_iWidth; i++)
	{
		m_pMapTile[i] = new MapTile[m_iHeight];
	}

	for (int j = 0; j < m_iHeight; j++)
	{
		for (int i = 0; i < m_iWidth; i++)
		{
			Vec2 vec2 = Vec2(i, j);
			m_pMapTile[i][j].Init(m_pLayerMainScene,vec2);
			m_pMapTile[i][j].SetTileType(m_iMap[i][j], j);
			m_pMapTile[i][j].AdjustScale(m_iWidth, m_iHeight);
		}
	}
}

bool MapTileManager::SelectMapTile(Vec2 vec2Mouse)
{
	if (m_bSelectTile == false && m_bMenuVisible ==false)
	{
		for (int j = 0; j < m_iHeight; j++)
		{
			for (int i = 0; i < m_iWidth; i++)
			{
				if (j == 0 && i < m_iWidth)
					continue;
				if (i == 0 && j < m_iHeight)
					continue;
				if (m_pMapTile[i][j].SelectTile(vec2Mouse) && m_pMapTile[i][j].GetCharacterOnTile() ==false)
				{
					Sprite* tileInfo = m_pMapTile[i][j].GetSprite();
					m_pSprSelectTile->setPosition(tileInfo->getPosition());
					m_pSprSelectTile->setAnchorPoint(tileInfo->getAnchorPoint());
					m_pSprSelectTile->setOpacity(100.0f);
					m_pSprSelectTile->setVisible(true);
					m_pSprSelectTile->setLocalZOrder(tileInfo->getLocalZOrder()+1);
					m_bSelectTile = true;
					m_pSelectMapTile = &m_pMapTile[i][j];

					return true;
				}
			}
		}
	}
	return false;
}
void MapTileManager::SetMenuVisible(bool visible)
{
	m_bMenuVisible = visible;
}
bool MapTileManager::GetMenuVisible()
{
	return m_bMenuVisible;
}
void MapTileManager::SetCharacterOnTile(bool OnCharacter)
{
	m_pSelectMapTile->SetCharacterOnTile(OnCharacter);
}

Vec2 MapTileManager::GetSelectTilePos()
{
	return m_pSprSelectTile->getPosition();
}

void MapTileManager::SetSelectMapTile(bool ExitcurSelectile)
{
	m_bSelectTile = ExitcurSelectile;
	if (m_bSelectTile == false)
	{
		m_pSprSelectTile->setVisible(false);
	}
}
bool MapTileManager::GetSelectMapTile()
{
	return m_bSelectTile;
}
Sprite* MapTileManager::GetSelectTileSprite()
{
	return m_pSprSelectTile;
}

MapTile* MapTileManager::GetSelectCurTile()
{
	return m_pSelectMapTile;
}

MapTile* MapTileManager::GetStepOnTile(Mob* pMob)
{
	TILE_PROPERTY TileProperty;
	for (int j = 0; j < m_iHeight; j++)
	{
		for (int i = 0; i < m_iWidth; i++)
		{
			TileProperty = (TILE_PROPERTY)m_pMapTile[i][j].GetTileProperty();

			if (TILE_PROPERTY_LOAD == TileProperty)
				continue;

			if (m_pMapTile[i][j].ConerInMob(pMob))
			{
				if (TileProperty == TILE_PROPERTY_CONER_LB)
				{
					if (m_pMapTile[i][j].GetCenterPosition().x >= pMob->GetVec2Pos().x && pMob->GetDirection() == DIRECTION_L)
					{
						pMob->SetPosition(m_pMapTile[i][j].GetCenterPosition().x, m_pMapTile[i][j].GetCenterPosition().y + (pMob->GetSpriteMob()->getContentSize().height / 2));
						pMob->SetDirection(DIRECTION_T);
					}
					else if (m_pMapTile[i][j].GetCenterPosition().y >= pMob->GetRect().getMinY() && pMob->GetDirection() == DIRECTION_B)
					{
						pMob->SetPosition(m_pMapTile[i][j].GetCenterPosition().x, m_pMapTile[i][j].GetCenterPosition().y + (pMob->GetSpriteMob()->getContentSize().height / 2));
						pMob->SetDirection(DIRECTION_R);
					}
				}
				else if (TileProperty == TILE_PROPERTY_CONER_RB)
				{
					if (m_pMapTile[i][j].GetCenterPosition().x <= pMob->GetVec2Pos().x && pMob->GetDirection() == DIRECTION_R)
					{
						pMob->SetPosition(m_pMapTile[i][j].GetCenterPosition().x, m_pMapTile[i][j].GetCenterPosition().y + (pMob->GetSpriteMob()->getContentSize().height / 2));
						pMob->SetDirection(DIRECTION_T);
					}
					else if (m_pMapTile[i][j].GetCenterPosition().y >= pMob->GetRect().getMinY() && pMob->GetDirection() == DIRECTION_B)
					{
						pMob->SetPosition(m_pMapTile[i][j].GetCenterPosition().x, m_pMapTile[i][j].GetCenterPosition().y + (pMob->GetSpriteMob()->getContentSize().height / 2));
						pMob->SetDirection(DIRECTION_L);
					}

				}
				else if (TileProperty == TILE_PROPERTY_CONER_RT)
				{
					float y = m_pMapTile[i][j].GetCenterPosition().y;
					float moby = pMob->GetVec2Pos().y - (pMob->GetSpriteMob()->getContentSize().height / 2);

					if (m_pMapTile[i][j].GetCenterPosition().x <= pMob->GetVec2Pos().x && pMob->GetDirection() == DIRECTION_R)
					{
						pMob->SetPosition(m_pMapTile[i][j].GetCenterPosition().x, m_pMapTile[i][j].GetCenterPosition().y + (pMob->GetSpriteMob()->getContentSize().height / 2));
						pMob->SetDirection(DIRECTION_B);
					}
					else if (m_pMapTile[i][j].GetCenterPosition().y <= pMob->GetVec2Pos().y - (pMob->GetSpriteMob()->getContentSize().height/2) && pMob->GetDirection() == DIRECTION_T) 
					{
						pMob->SetPosition(m_pMapTile[i][j].GetCenterPosition().x, m_pMapTile[i][j].GetCenterPosition().y + (pMob->GetSpriteMob()->getContentSize().height / 2));
						pMob->SetDirection(DIRECTION_L);
					}
				}
				else if (TileProperty == TILE_PROPERTY_CONER_LT)
				{
					if (m_pMapTile[i][j].GetCenterPosition().x >= pMob->GetVec2Pos().x && pMob->GetDirection() == DIRECTION_L)
					{
						pMob->SetPosition(m_pMapTile[i][j].GetCenterPosition().x, m_pMapTile[i][j].GetCenterPosition().y + (pMob->GetSpriteMob()->getContentSize().height / 2));
						pMob->SetDirection(DIRECTION_B);
					}
					else if (m_pMapTile[i][j].GetCenterPosition().y <= pMob->GetRect().getMinY()&& pMob->GetDirection() == DIRECTION_T)
					{
						pMob->SetPosition(m_pMapTile[i][j].GetCenterPosition().x, m_pMapTile[i][j].GetCenterPosition().y + (pMob->GetSpriteMob()->getContentSize().height / 2));
						pMob->SetDirection(DIRECTION_R);
					}

				}
			}
		}
	}
	return NULL;
}
MapTile* MapTileManager::GetTile(int x, int y)
{
	return &m_pMapTile[x][y];
}
MapTile* MapTileManager::GetCoreMapTile()
{
		for (int j = 0; j < m_iHeight; j++)
		{
			for (int i = 0; i < m_iWidth; i++)
			{
				if ((TILE_PROPERTY)m_pMapTile[i][j].GetTileProperty() == TILE_PROPERTY_CORE)
					return &m_pMapTile[i][j];
			}
		}
	return NULL;
}