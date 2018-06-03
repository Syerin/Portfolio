#include "CharacterInfo.h"
#include "ELF.h"
#include "Caitsith.h";
#include "Cusith.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <Windows.h>
#endif


CharacterInfo* CharacterInfo::m_pThis = NULL;

CharacterInfo::CharacterInfo()
{
}


CharacterInfo::~CharacterInfo()
{

}

void CharacterInfo::Init(Layer* pLayerMainScene)
{
	m_pLayerMainScene = pLayerMainScene;

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	WCHAR lpStrFileFath[256];
	GetCurrentDirectory(MAX_PATH + 1, lpStrFileFath);
	WCharToChar(lpStrFileFath, m_FilePathBase);
#elif(CC_TARGET_PLATROMF == CC_PLATFORM_ANDROID)
	std::string FilePath = FileUtils::sharedFileUtils()->getWritablePath();

	strcpy(FileAddress, FilePath.c_str());

#endif


	LoadCharacterInfo();
}
void CharacterInfo::LoadCharacterInfo()
{
	char FileAddress[256];
	
	strcpy(FileAddress, m_FilePathBase);

	strcat(FileAddress, "\\Character\\CharacterInfo.csv");
	FILE* pFile = fopen(FileAddress, "r");
	if (pFile == NULL)
	{
		log("[%s에 파일이 없습니다.]", FileAddress);
		return;
	}


	log("[%s 찾았습니다.]", FileAddress);
	Character* pCharacter = NULL;
	char sztemp[512];
	fgets(sztemp, 512, pFile);
	int count = 0;

	CHARACTER_TYPE CharacterType = CHARACTER_TYPE_END;
	float  fAttackDamage = 0;
	float  fAttackDelay = 0;
	int ibuyPrice = 0;
	int iSalePirce = 0;
	int iUpgradePrice = 0;
	int iUpGradeDamageCount = 0;
	while (true)
	{
		if (feof(pFile))
			break;

		fscanf(pFile, "%d,%f,%f,%d,%d,%d,%d\n", &CharacterType, &fAttackDamage, &fAttackDelay, &ibuyPrice, &iSalePirce, &iUpgradePrice, &iUpGradeDamageCount);

		pCharacter = GetCharacterType(CharacterType);
		if (pCharacter == NULL)
			break;
		pCharacter->SetCharacterInfo(CharacterType, fAttackDamage, fAttackDelay, ibuyPrice, iSalePirce, iUpgradePrice, iUpGradeDamageCount);

		m_mapCharacterInfo.insert(std::make_pair(count, pCharacter));

		count++;
	}
	fclose(pFile);

}
Character* CharacterInfo::GetCharacterType(CHARACTER_TYPE type)
{
	Character* pCharacter = NULL;
	switch (type)
	{
	case CHARACTER_TYPE_ELF:
		pCharacter = new ELF();
		break;
	case CHARACTER_TYPE_CAITSITH:
		pCharacter = new Caitsith();
		break;
	case CHARACTER_TYPE_CUSITH:
		pCharacter = new Cusith();
		break;

	case CHARACTER_TYPE_END:
		break;
	default:
		break;
	}
	return pCharacter;
}

void CharacterInfo::WCharToChar(const wchar_t* pwstrSrc, char pstrDest[])
{
	int nLen = (int)wcslen(pwstrSrc);
	wcstombs(pstrDest, pwstrSrc, nLen + 1);
}
void CharacterInfo::SetCharacterInfo(Character* pCharacter)
{
	auto iter = m_mapCharacterInfo.find(pCharacter->GetCharacterType());
	if (iter == m_mapCharacterInfo.end())
	{
		return;
	}

	char FileAddress[256];

	strcpy(FileAddress, m_FilePathBase);

	strcat(FileAddress, "\\Character\\CharacterInfo.csv");
	FILE* pFile = fopen(FileAddress, "r");
	if (pFile == NULL)
	{
		log("[%s에 파일이 없습니다.]", FileAddress);
		return;
	}
	char szFileFirstLine[512] = { '\0' };
	fgets(szFileFirstLine, 512, pFile);

	fclose(pFile);
	pFile = fopen(FileAddress, "w");

	log("[%s 찾았습니다.]", FileAddress);
	int len = strlen(szFileFirstLine);
	strcpy(&szFileFirstLine[len - 1], ",");
	szFileFirstLine[len] = '\n';

	fprintf(pFile,"%s", szFileFirstLine);
	float  fAttackDamage;
	float  fAttackDelay;
	int Type;
	int ibuyPrice;
	int iSalePirce;
	int iUpgradePrice;
	int iUpgradeDamageCount;


	for (auto iter = m_mapCharacterInfo.begin(); iter != m_mapCharacterInfo.end(); iter++)
	{

		if (iter->second->GetCharacterType() == pCharacter->GetCharacterType())
		{
			iter->second = pCharacter;
		}
		fAttackDamage = (*iter).second->GetAttackDamage();
		fAttackDelay = (*iter).second->GetfAttackDelay();
		Type = (int)(*iter).second->GetCharacterType();
		ibuyPrice = (*iter).second->GetiBuyPrice();
		iSalePirce = (*iter).second->GetiSalePrice();
		iUpgradePrice = (*iter).second->GetiUpGradePrice();
		iUpgradeDamageCount = (*iter).second->GetiUpGradeDamageCount();


		fprintf(pFile, "%d,%f,%f,%d,%d,%d,%d\n", Type, fAttackDamage, fAttackDelay, ibuyPrice, iSalePirce, iUpgradePrice, iUpgradeDamageCount);
	}
	fclose(pFile);

}
Character* CharacterInfo::GetCharacterInfo(int TypeNum)
{
	Character* pCharacter = NULL;
	auto iter = m_mapCharacterInfo.find(TypeNum);

	if (iter != m_mapCharacterInfo.end()) 
	{
		float  fAttackDamage = 0;
		float  fAttackDelay = 0;
		int ibuyPrice = 0;
		int iSalePirce = 0;
		int iUpgradePrice = 0;
		int iUpgradeDamageCount= 0;
		CHARACTER_TYPE type = CHARACTER_TYPE_END;
		pCharacter = GetCharacterType((CHARACTER_TYPE)TypeNum);
		fAttackDamage = iter->second->GetAttackDamage();
		fAttackDelay = iter->second->GetfAttackDelay();
		ibuyPrice = iter->second->GetiBuyPrice();
		iSalePirce = iter->second->GetiSalePrice();
		iUpgradePrice = iter->second->GetiUpGradePrice();
		iUpgradeDamageCount = iter->second->GetiUpGradeDamageCount();
		type = pCharacter->GetCharacterType();
		pCharacter->SetCharacterInfo(type,fAttackDamage, fAttackDelay, ibuyPrice, iSalePirce, iUpgradePrice, iUpgradeDamageCount);
		return pCharacter;
	}
	else
	{
		return pCharacter;
	}
}

void CharacterInfo::Release()
{
	char FileAddress[256];

	strcpy(FileAddress, m_FilePathBase);

	strcat(FileAddress, "\\Character\\CharacterInfo.csv");
	FILE* pFile = fopen(FileAddress, "r");
	if (pFile == NULL)
	{
		log("[%s에 파일이 없습니다.]", FileAddress);
		return;
	}
	char szFileFirstLine[512] = { '\0' };
	fgets(szFileFirstLine, 512, pFile);

	fclose(pFile);
	 pFile = fopen(FileAddress, "w");

	log("[%s 찾았습니다.]", FileAddress);
	int len = strlen(szFileFirstLine);
	strcpy(&szFileFirstLine[len-1], ",");
	szFileFirstLine[len] = '\n';
	
	
	fprintf(pFile, "%s",szFileFirstLine);

	for (auto iter = m_mapCharacterInfo.begin(); iter != m_mapCharacterInfo.end(); iter++)
	{
		float  fAttackDamage = (*iter).second->GetAttackDamage();
		float  fAttackDelay = (*iter).second->GetfAttackDelay();
		int Type = (int)(*iter).second->GetCharacterType();
		int ibuyPrice = (*iter).second->GetiBuyPrice();
		int iSalePirce = (*iter).second->GetiSalePrice();
		int iUpgradePrice = (*iter).second->GetiUpGradePrice();
		int iUpgradeDamageCount = (*iter).second->GetiUpGradeDamageCount();

		fprintf(pFile, "%d,%f,%f,%d,%d,%d,%d\n", Type, fAttackDamage, fAttackDelay, ibuyPrice, iSalePirce, iUpgradePrice, iUpgradeDamageCount);
	}
	fclose(pFile);

	auto iter = m_mapCharacterInfo.begin();
	while (iter != m_mapCharacterInfo.end())

	{
		if (iter->second != NULL)
			m_mapCharacterInfo.erase(iter++); 
		else
			++iter;
	}
}