#pragma once
#include "cocos2d.h"
USING_NS_CC;
class PlayerInfo
{
private:
	std::map<int, int> m_mapStageStarCount;

	int m_iGold;
	int m_iAbilityPoint;
	int m_iClearStageEndNumber;

	int m_iAllStageStar;

	char m_FilePathBase[256];

	PlayerInfo();
	static 	PlayerInfo* m_pThis;
public:
	void Init();
	void LoadPlayerInfo();
	void LoadPlayerStageInfo();

	void WCharToChar(const wchar_t* pwstrSrc, char pstrDest[]);

	int GetGold();
	int GetAbilityPoint();
	int GetTopClearStage();

	void PlusAbilityPoint(int buyApPrice);
	
	void SaveAbilityPoint();

	void SetStageStar(int StageNumber,int Star);

	void SetStageClearInfo(int StageNumber, int Star, int Gold);
	
	int GetStageStar(int StageNumber);
	int GetAllStageStar();

	static PlayerInfo* GetInstace()
	{
		if (m_pThis == NULL)
		{
			m_pThis = new PlayerInfo();
		}
		return m_pThis;
	}
	~PlayerInfo();
};

