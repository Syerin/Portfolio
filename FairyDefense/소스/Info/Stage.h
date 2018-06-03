#pragma once
#include "defines.h"
#include "cocos2d.h"
#include <string>
USING_NS_CC;

struct ZENORDER
{
	int ZenMobTypeNumber;
	float NextSpawnInterval;
};
class GenInfo;
class Stage
{
private:
	int m_iWidth;
	int m_iHeight;
	int m_iAddressLen;
	int** m_piStage;

	int m_iRespawnWidth;
	int m_iRespawnHeight;

	int m_iStageNumber;
	char m_chSubAddress[512];

	int m_iStageFirstGold;
	int m_iStageClearGetGold;
	int m_iStageLimitStar;

	std::vector<int*> m_vecGenOrder;
public:
	void Init(int StageNumber,char* lpFileAddress);

	void LoadMap(char* lpFileAddress);
	

	void LoadStageInfo(char* lpFileAddress);

	void LoadGen(char* lpFileAddress);
	int* GetGenOrderNumber(int Wave);
	int GetWaveMaxCount();

	inline int  GetStageLimitStar()
	{
		return m_iStageLimitStar;
	}


	inline int GetRespawnWidth()
	{
		return m_iRespawnWidth;
	}
	inline int GetRespawnHeight()
	{
		return m_iRespawnHeight;
	}

	inline int GetStageFirstGold()
	{
		return m_iStageFirstGold;
	}
	inline int GetStageClearGold()
	{
		return m_iStageClearGetGold;
	}

	inline int** GetStage()
	{
		return m_piStage;
	}
	inline int GetWidth()
	{
		return m_iWidth;
	}
	inline int GetHeight()
	{
		return m_iHeight;
	}
	Stage();
	~Stage();
};

