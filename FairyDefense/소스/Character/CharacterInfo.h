#pragma once
#include "Character.h"
#include "cocos2d.h"
USING_NS_CC;
class CharacterInfo
{
private:
	Layer* m_pLayerMainScene;
	char m_FilePathBase[256];
	std::map<int, Character*> m_mapCharacterInfo;
	static CharacterInfo* m_pThis;
	CharacterInfo();
public:
	void Init(Layer* pLayerMainScene);
	void LoadCharacterInfo();
	void Release();

	Character* GetCharacterInfo(int TypeNum);
	void WCharToChar(const wchar_t* pwstrSrc, char pstrDest[]);
	void SetCharacterInfo(Character* pCharacter);
	Character* GetCharacterType(CHARACTER_TYPE type);
	static CharacterInfo* GetInstance()
	{
		if (m_pThis == NULL)
		{
			m_pThis = new CharacterInfo();
		}
		return m_pThis;
	}
	~CharacterInfo();
};

