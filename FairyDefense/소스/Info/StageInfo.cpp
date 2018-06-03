#include "StageInfo.h"
#include "GenInfo.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <Windows.h>
#endif

StageInfo* StageInfo::m_pThis = NULL;

StageInfo::StageInfo()
{
	m_iStageIndex = 0;
	m_iCurStage = 0;
}


StageInfo::~StageInfo()
{
	for (auto iter = m_mapStage.begin(); iter != m_mapStage.end(); iter++)
	{
		CC_SAFE_DELETE((*iter).second);
	}
	m_mapStage.clear();
}
void StageInfo::SetCurStage(int StageNum)
{
	m_iStageIndex = StageNum;
}
int StageInfo::GetCurStage()
{
	return m_iStageIndex;
}
void StageInfo::Init()
{	
	for (int i = 1; i <= MAXSTAGENUM; i++)
	{
		GetStage(i);
	}

	char FileAddress[512];
	
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	WCHAR lpStrFileFath[512];
	GetCurrentDirectory(MAX_PATH + 1, lpStrFileFath);
	WCharToChar(lpStrFileFath, FileAddress);

	for (int i = 1; i <= 7; i++)
	{
		GenInfo* pGenInfo = new GenInfo();

		pGenInfo->Init(i,FileAddress);

		m_mapGenInfo.insert(std::make_pair(i, pGenInfo));
	}

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	std::string FilePath = FileUtils::sharedFileUtils()->getWritablePath();

	strcpy(FileAddress, FilePath.c_str());
	for (int i = 1; i <= 7; i++)
	{
		GenInfo* pGenInfo = new GenInfo();

		pGenInfo->Init(i, FileAddress);

		m_mapGenInfo.insert(std::make_pair(i, pGenInfo));
	}
#endif
}

void StageInfo::WCharToChar(const wchar_t* pwstrSrc, char pstrDest[])
{
	int nLen = (int)wcslen(pwstrSrc);
	wcstombs(pstrDest, pwstrSrc, nLen + 1);
}

Stage* StageInfo::GetStage(int StageNumber)
{
	auto iter = m_mapStage.find(StageNumber);

	if(iter == m_mapStage.end())
	{
		m_iStageIndex++;
		char FileAddress[256];
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		WCHAR lpStrFileFath[512];
		GetCurrentDirectory(MAX_PATH + 1, lpStrFileFath);
		WCharToChar(lpStrFileFath, FileAddress);
#elif(CC_TARGET_PLATROMF == CC_PLATFORM_ANDROID)
		std::string FilePath = FileUtils::sharedFileUtils()->getWritablePath();
		strcpy(FileAddress, FilePath.c_str());
#endif
		Stage* pStage = NULL;
		pStage = new Stage();
		pStage->Init(m_iStageIndex,FileAddress);
		m_mapStage.insert(std::make_pair(StageNumber, pStage));
		return pStage;
	}
	else
	return iter->second;
}

GenInfo* StageInfo::GetGenInfo(int GenNumber)
{
	auto iter = m_mapGenInfo.find(GenNumber);

	if (iter != m_mapGenInfo.end())
	{
		return iter->second;
	}
	else
		return NULL;
}