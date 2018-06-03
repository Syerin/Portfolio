#include "MobInfo.h"

MobInfo* MobInfo::m_pThis = NULL;

MobInfo::MobInfo()
{
}


MobInfo::~MobInfo()
{
	for (auto iter = m_mapMobStat.begin(); iter != m_mapMobStat.end(); iter++)
	{
		CC_SAFE_DELETE((*iter).second);
	}
	m_mapMobStat.clear();
}
void MobInfo::WCharToChar(const wchar_t* pwstrSrc, char pstrDest[])
{
	int nLen = (int)wcslen(pwstrSrc);
	wcstombs(pstrDest, pwstrSrc, nLen + 1);
}

void MobInfo::Init()
{
	char FileAddress[256];

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	WCHAR lpStrFileFath[512];
		GetCurrentDirectory(MAX_PATH + 1, lpStrFileFath);
		WCharToChar(lpStrFileFath, FileAddress);
#elif(CC_TARGET_PLATROMF == CC_PLATFORM_ANDROID)
	std::string FilePath = FileUtils::sharedFileUtils()->getWritablePath();
	strcpy(FileAddress, FilePath.c_str());

#endif

	LoadMobStat(FileAddress);
	LoadMobType(FileAddress);
}
void MobInfo::LoadMobStat(char* lpFilePath)
{
	char FilePath[512];
	strcpy(FilePath, lpFilePath);
	strcat(FilePath, "\\Mob\\MobStatus.csv");
	FILE* pFile = fopen(FilePath, "r");
	if (pFile == NULL)
	{
		log("[%s에 파일이 없습니다.]", FilePath);
		return;
	}


	log("[%s 찾았습니다.]", FilePath);

	float fSpeed = 0.0f;
	int ZenMoney = 0;
	int TypeNumber = 0;
	int HealthPoint = 0;


	MobNumberInfo* pMobNumberInfo;
	char sztemp[512];
	fgets(sztemp, 512, pFile);
	while (true)
	{

		if (feof(pFile))
			break;
		fscanf(pFile, "%d,%f,%d,%d\n", &TypeNumber, &fSpeed, &HealthPoint, &ZenMoney);
		pMobNumberInfo = new MobNumberInfo();
		pMobNumberInfo->InfoNumber = TypeNumber;
		pMobNumberInfo->fSpeed = fSpeed;
		pMobNumberInfo->HealthPoint = HealthPoint;
		pMobNumberInfo->ZenMoney = ZenMoney;

		m_mapMobStat.insert(std::make_pair(TypeNumber, pMobNumberInfo));
		TypeNumber++;
	}
	fclose(pFile);

}
void MobInfo::LoadMobType(char* lpFileAddress)
{
	char FilePath[512];
	strcpy(FilePath, lpFileAddress);

	strcat(FilePath, "\\Mob\\MobType.csv");
	FILE* pFile = fopen(FilePath, "r");
	if (pFile == NULL)
	{
		log("[%s에 파일이 없습니다.]", FilePath);
		return;
	}


	log("[%s 찾았습니다.]", FilePath);

	MobNumberInfo* pMobTypeInfo;
	char sztemp[512];
	fgets(sztemp, 512, pFile);
	MOB_TYPE MobType;
	int MobStatNumber;
	int MobNumber;
	while (true)
	{
		if (feof(pFile))
			break;
		fscanf(pFile, "%d,%d,%d\n",&MobNumber,&MobStatNumber,&MobType);
		pMobTypeInfo = GetMobInfo(MobStatNumber);
		pMobTypeInfo->MobType = MobType;
		m_mapMobType.insert(std::make_pair(MobNumber, pMobTypeInfo));
	}
	fclose(pFile);
}

int MobInfo::GetMobTypeMaxNum()
{
	return m_mapMobType.size();
}
MobNumberInfo* MobInfo::GetMobInfo(int TypeNum)
{
	MobNumberInfo* pMobNumberInfo = NULL;
	auto iter = m_mapMobStat.find(TypeNum);
	if (iter != m_mapMobStat.end())
	{
		pMobNumberInfo = iter->second;
		return pMobNumberInfo;
	}
	else
	{
		return pMobNumberInfo;
	}
}


MobNumberInfo* MobInfo::GetZenOrderMobType(int TypeNum)
{
	MobNumberInfo* pMobNumberInfo = NULL;
	auto iter = m_mapMobType.find(TypeNum);
	if (iter != m_mapMobType.end())
	{
		pMobNumberInfo = iter->second;
		return pMobNumberInfo;
	}
	else
	{
		return pMobNumberInfo;
	}

}
