#pragma once
#include "Character.h"
#include "cocos2d.h"
USING_NS_CC;
class MapTile;
class Mob;
class MobManager;
class CharacterManager
{
private:
	Layer* m_pLayerMainScene;
	std::vector<Character*> m_vecCharacter;
	
	static CharacterManager* m_pThis;
	CharacterManager();
public:
	void Init(Layer* pLayerMainScene);

	Character* GetCharacter(CHARACTER_TYPE type);
	bool SurmonCharacter(CHARACTER_TYPE type, MapTile* pMapTile);
	void Update(float fElapse);
	void SearchRangeMob();
	void Attack(Character* pCharacter,Mob* pMob,float distance);
	void SelectFieldCharacter(Vec2 vec2Mouse);
	bool GetSelectCharacter();
	void CancleCharacterMenu();


	void SetAnimDuration(float Speed);

	static CharacterManager* GetInstance()
	{
		if (m_pThis == NULL)
		{
			m_pThis = new CharacterManager;
		}
		return m_pThis;
	}

	~CharacterManager();
};

