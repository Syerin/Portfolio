#pragma once
#include "MapTile.h"
class Mob;
class MapTileManager
{
private:
	Layer* m_pLayerMainScene;
	Sprite* m_pSprSelectTile;
	bool m_bSelectTile;
	MapTile** m_pMapTile;
	int** m_iMap;
	int m_iWidth;
	int m_iHeight;
	MapTile* 	m_pSelectMapTile;
	bool m_bMenuVisible;

	static MapTileManager* m_pThis;
	MapTileManager();
public:
	void Init(Layer* pLayerMainScene,int StageNum);
	bool SelectMapTile(Vec2 vec2Mouse);
	Vec2 GetSelectTilePos();
	void SetSelectMapTile(bool ExitcurSelectile);
	MapTile* GetStepOnTile(Mob* pMob);
	Sprite* GetSelectTileSprite();
	MapTile* GetTile(int x, int y);
	MapTile* GetCoreMapTile();

	MapTile* GetSelectCurTile();
	void SetCharacterOnTile(bool OnCharacter);
	bool GetSelectMapTile();

	bool GetMenuVisible();
	void SetMenuVisible(bool visible);

	static MapTileManager* GetInstance()
	{
		if (m_pThis == NULL)
		{
			m_pThis = new MapTileManager;
		}
		return m_pThis;
	}
	~MapTileManager();
};

