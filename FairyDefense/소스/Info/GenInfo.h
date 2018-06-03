#pragma once
#include "cocos2d.h"
USING_NS_CC;

struct GENINFO
{
	int MobTypeNumber;
	float NextInterval;
};

class GenInfo
{
private:
	std::vector<GENINFO*> m_vecGenInfo;
public:
	void Init(int Number, char* lpFileAddress);

	GENINFO* GetGenInfo(int count);
	int GenInfoMaxCount();
	int GetGenAllMobCount();
	GenInfo();
	~GenInfo();
};

