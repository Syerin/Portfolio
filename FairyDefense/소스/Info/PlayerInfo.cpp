#include "PlayerInfo.h"
#include "StageInfo.h"
PlayerInfo* PlayerInfo::m_pThis = NULL;

PlayerInfo::PlayerInfo()
{
}


PlayerInfo::~PlayerInfo()
{
	m_mapStageStarCount.clear();
}


void PlayerInfo::Init()
{

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	WCHAR lpStrFileFath[512];
	GetCurrentDirectory(MAX_PATH + 1, lpStrFileFath);
	WCharToChar(lpStrFileFath, m_FilePathBase);
#elif(CC_TARGET_PLATROMF == CC_PLATFORM_ANDROID)
	std::string FilePath = FileUtils::sharedFileUtils()->getWritablePath();

	strcpy(m_FilePathBase, FilePath.c_str());
#endif


	LoadPlayerInfo();
	LoadPlayerStageInfo();
}


void PlayerInfo::LoadPlayerInfo()
{
	char FileAddress[512];

	strcpy(FileAddress, m_FilePathBase);

	strcat(FileAddress, "\\Player\\PlayerInfo.csv");

	FILE* pFile = fopen(FileAddress, "r");
	if (pFile == NULL)
	{
		log("%s 파일이 없습니다.", FileAddress);
		return;
	}

	log("%s 파일을 찾았습니다.", FileAddress);

	char ReadLine[512];
	fgets(ReadLine, 512, pFile);

	while (true)
	{
		if (feof(pFile) != 0)
		{
			break;
		}
		fscanf(pFile, "%d,%d,%d,%d\n", &m_iAllStageStar, &m_iGold, &m_iClearStageEndNumber, &m_iAbilityPoint);
	}
	fclose(pFile);

}
void PlayerInfo::LoadPlayerStageInfo()
{
	char FileAddress[512];

	strcpy(FileAddress, m_FilePathBase);

	strcat(FileAddress, "\\Player\\PlayerStageStar.csv");

	FILE* pFile = fopen(FileAddress, "r");
	if (pFile == NULL)
	{
		log("%s 파일이 없습니다.", FileAddress);
		return;
	}

	log("%s 파일을 찾았습니다.", FileAddress);

	char ReadLine[512];
	fgets(ReadLine, 512, pFile); 
	int count = 0;
	while (true)
	{
		if (feof(pFile) != 0)
		{
			break;
		}
		if (count == MAXSTAGENUM)
			break;
		count++;
		int Star = 0;
		fscanf(pFile, "%d,",&Star);
		m_mapStageStarCount.insert(std::make_pair(count, Star));

	}
	fclose(pFile);
}

void PlayerInfo::PlusAbilityPoint(int buyApPrice)
{
	m_iAbilityPoint++;
	m_iGold -= buyApPrice;
	SaveAbilityPoint();
}

void PlayerInfo::SaveAbilityPoint()
{
	char FileAddress[512];

	strcpy(FileAddress, m_FilePathBase);

	strcat(FileAddress, "\\Player\\PlayerInfo.csv");


	FILE* pFile = fopen(FileAddress, "r");
	char FirstLine[512] = { '\0' };
	fgets(FirstLine, 512, pFile);
	fclose(pFile);


	pFile = fopen(FileAddress, "w");

	int len = strlen(FirstLine);
	strcpy(&FirstLine[len - 1], ",");
	FirstLine[len] = '\n';
	fprintf(pFile, "%s", FirstLine);

	m_iAllStageStar = GetAllStageStar();

	fprintf(pFile, "%d,%d,%d,%d\n", m_iAllStageStar, m_iGold, m_iClearStageEndNumber, m_iAbilityPoint);
	fclose(pFile);

}
int PlayerInfo::GetStageStar(int StageNumber)
{
	auto iter = m_mapStageStarCount.find(StageNumber);
	if (iter != m_mapStageStarCount.end())
	{
		return iter->second;
	}
	else
	{
		log("존재하지않는 스테이지의 Star갯수를 호출");
	}
	return 0;
}

int PlayerInfo::GetAllStageStar()
{
	m_iAllStageStar = 0;
	for (auto iter = m_mapStageStarCount.begin(); iter != m_mapStageStarCount.end(); iter++)
	{
		m_iAllStageStar += iter->second;
	}
	return m_iAllStageStar;
}


void PlayerInfo::SetStageClearInfo(int StageNumber, int Star, int Gold)
{
	char FileAddress[512];

	strcpy(FileAddress, m_FilePathBase);
	strcat(FileAddress, "\\Player\\PlayerInfo.csv");


	FILE* pFile = fopen(FileAddress, "r");
	char FirstLine[512] = { '\0' };
	fgets(FirstLine, 512, pFile);
	fclose(pFile);


	pFile = fopen(FileAddress, "w");

	int len = strlen(FirstLine);
	strcpy(&FirstLine[len - 1], ",");
	FirstLine[len] = '\n';
	fprintf(pFile, "%s", FirstLine);

	m_iGold += Gold;
	auto iter = m_mapStageStarCount.find(StageNumber);
	if (iter->second < Star)
	{
		iter->second = Star;
		SetStageStar(StageNumber,Star);
	}
	if (StageNumber > m_iClearStageEndNumber)
	{
		m_iClearStageEndNumber = StageNumber;
	}

	m_iAllStageStar = GetAllStageStar();

	fprintf(pFile, "%d,%d,%d,%d\n", m_iAllStageStar,m_iGold,m_iClearStageEndNumber,m_iAbilityPoint);
	fclose(pFile);
}
int PlayerInfo::GetTopClearStage()
{
	return m_iClearStageEndNumber;
}
void PlayerInfo::SetStageStar(int StageNumber,int Star)
{
	char FileAddress[512];

	strcpy(FileAddress, m_FilePathBase);
	strcat(FileAddress, "\\Player\\PlayerStageStar.csv");


	FILE* pFile = fopen(FileAddress, "r");
	char FirstLine[512] = { '\0' };
	fgets(FirstLine, 512, pFile);
	fclose(pFile);

	pFile = fopen(FileAddress, "w");

	int len = strlen(FirstLine);
	strcpy(&FirstLine[len - 1], ",");
	FirstLine[len] = '\n';
	fprintf(pFile, "%s",FirstLine);

	int count = 0;
	for (auto iter = m_mapStageStarCount.begin(); iter != m_mapStageStarCount.end(); iter++)
	{
		count++;
		if (m_mapStageStarCount.size() == count)
			fprintf(pFile, "%d\n", (*iter).second);
		else
			fprintf(pFile, "%d,", (*iter).second);		
	}
	fclose(pFile);
}
int PlayerInfo::GetGold()
{
	return m_iGold;
}
int PlayerInfo::GetAbilityPoint()
{
	return m_iAbilityPoint;
}

void PlayerInfo::WCharToChar(const wchar_t* pwstrSrc, char pstrDest[])
{
	int nLen = (int)wcslen(pwstrSrc);
	wcstombs(pstrDest, pwstrSrc, nLen + 1);
}