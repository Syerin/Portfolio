#pragma once
#include "cocos2d.h"
#include "Mob.h"
USING_NS_CC;

struct MobNumberInfo
{
	float fSpeed;
	int ZenMoney;
	int InfoNumber;
	int HealthPoint;

	MOB_TYPE MobType;
};
class MobInfo
{
private:
	std::map<int, MobNumberInfo*> m_mapMobStat;
	std::map<int, MobNumberInfo*> m_mapMobType;

	static MobInfo* m_pThis;
	void WCharToChar(const wchar_t* pwstrSrc, char pstrDest[]);
	void LoadMobType(char* lpFileAddress);
	void LoadMobStat(char* lpFileAddress);
	MobInfo();
public:
	void Init();

	MobNumberInfo* GetMobInfo(int TypeNum);
	MobNumberInfo* GetZenOrderMobType(int TypeNum);


	int GetMobTypeMaxNum();
	static MobInfo* GetInstance()
	{
		if (m_pThis == NULL)
		{
			m_pThis = new MobInfo();
		}
		return m_pThis;
	}
	~MobInfo();
};

