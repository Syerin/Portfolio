#include "GenInfo.h"



GenInfo::GenInfo()
{
}


GenInfo::~GenInfo()
{
}

void GenInfo::Init(int GenNumber,char* lpFileAddress)
{
	char szFileAddress[512];
	char szFileName[128];
	strcpy(szFileAddress, lpFileAddress);

	sprintf(szFileName, "\\Stage\\Gen%.2d.csv", GenNumber);
	strcat(szFileAddress, szFileName);

	char FirstLine[128];

	GENINFO* pStGenInfo = NULL;
	FILE* pFile = fopen(szFileAddress, "r");
	fgets(FirstLine, 128, pFile);
	while (true)
	{
		if (feof(pFile))
			break;
		pStGenInfo = new GENINFO;
		fscanf(pFile, "%d,%f\n", &pStGenInfo->MobTypeNumber, &pStGenInfo->NextInterval);
		m_vecGenInfo.push_back(pStGenInfo);
	}
	fclose(pFile);
}

int GenInfo::GenInfoMaxCount()
{
	return m_vecGenInfo.size();
}

GENINFO* GenInfo::GetGenInfo(int count)
{
	if (count < m_vecGenInfo.size())
	{
		return m_vecGenInfo[count];
	}
	return NULL;
}
int GenInfo::GetGenAllMobCount()
{
	return  m_vecGenInfo.size();
}
