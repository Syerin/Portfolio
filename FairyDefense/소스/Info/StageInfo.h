#pragma once
#include "Stage.h"
#include <map>
#include "cocos2d.h"
USING_NS_CC;
class StageInfo 
{
private:
	int m_iStageIndex;
	int m_iCurStage;
	std::map<int,Stage*> m_mapStage;

	std::map<int, GenInfo*> m_mapGenInfo;

	StageInfo();
	static StageInfo* m_pThis;
public:
	void Init();
	void WCharToChar(const wchar_t* pwstrSrc, char pstrDest[]);
	Stage* GetStage(int StageNumber);
	GenInfo* GetGenInfo(int GenNumber);
	void SetCurStage(int StageNum);
	int GetCurStage();

	static StageInfo* GetInstance()
	{
		if (m_pThis == NULL)
		{
			m_pThis = new StageInfo();
		}
		return m_pThis;
	}
	~StageInfo();
};

