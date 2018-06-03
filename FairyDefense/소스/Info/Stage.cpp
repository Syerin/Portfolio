#include "Stage.h"
#include "GenInfo.h"


Stage::Stage()
{
	m_iStageNumber = 0;
}


Stage::~Stage()
{
}

void Stage::Init(int StageNumber, char* lpFileAddress)
{
	m_iStageNumber = StageNumber;
	m_iAddressLen = strlen(lpFileAddress);
	LoadMap(lpFileAddress);
	LoadGen(lpFileAddress);
	LoadStageInfo(lpFileAddress);
}
int* Stage::GetGenOrderNumber(int Wave)
{
	if (Wave < m_vecGenOrder.size())
	{
		return m_vecGenOrder[Wave];
	}
	else
		return NULL;
}

int Stage::GetWaveMaxCount()
{
	return m_vecGenOrder.size();
}
void Stage::LoadGen(char* lpFileAddress)
{
	sprintf(m_chSubAddress, "\\Stage\\Stage%d\\GenOrder.csv", m_iStageNumber);
	strcpy(&lpFileAddress[m_iAddressLen], m_chSubAddress);


	FILE* pFile = fopen(lpFileAddress, "r");
	if (pFile == NULL)
	{
		log("%s이 없습니다.", lpFileAddress);
		return;
	}
	else
	{
		log("%s을 찾았습니다..", lpFileAddress);

	}
	char FirstLine[512];

	fgets(FirstLine, 512, pFile);

	int* pInt = NULL;

	while (true)
	{

		if (feof(pFile))
			break;
		pInt = new int;
		fscanf(pFile, "%d\n", pInt);

		m_vecGenOrder.push_back(pInt);
	}
	fclose(pFile);
}

void Stage::LoadStageInfo(char* lpFileAddress)
{
	sprintf(m_chSubAddress, "\\Stage\\Stage%d\\StageInfo%.2d.csv", m_iStageNumber, m_iStageNumber);
	strcpy(&lpFileAddress[m_iAddressLen], m_chSubAddress);

	FILE* pFile = fopen(lpFileAddress, "r");
	if (pFile == NULL)
	{
		log("%s이 없습니다.", lpFileAddress);
		return;
	}
	else
	{
		log("%s을 찾았습니다..", lpFileAddress);
	}
	char firstLine[512];
	fgets(firstLine, 512, pFile);
	while (true)
	{
		if (feof(pFile))
		{
			break;
		}
		fscanf(pFile,"%d,%d,%d\n",&m_iStageFirstGold, &m_iStageClearGetGold,&m_iStageLimitStar);
	}
	fclose(pFile);


}
void Stage::LoadMap(char* lpFileAddress)
{
	sprintf(m_chSubAddress, "\\Stage\\Stage%d\\Stage%.2d.txt", m_iStageNumber, m_iStageNumber);
	strcpy(&lpFileAddress[m_iAddressLen], m_chSubAddress);


	FILE* pFile = fopen(lpFileAddress, "r");
	if (pFile == NULL)
	{
		log("%s이 없습니다.", lpFileAddress);
		return;
	}
	else
	{
		log("%s을 찾았습니다..", lpFileAddress);

	}

	fscanf(pFile, "%d %d ", &m_iWidth, &m_iHeight);
	fscanf(pFile, "%d %d ", &m_iRespawnWidth, &m_iRespawnHeight);

	m_piStage = new int*[m_iWidth];

	for (int i = 0; i < m_iWidth; i++)
	{
		m_piStage[i] = new int[m_iHeight];
	}

	for (int j = 0; j < m_iHeight; j++)
	{
		for (int i = 0; i < m_iWidth; i++)
		{
			fscanf(pFile, "%d ", &m_piStage[i][j]);
		}
	}
	fclose(pFile);

}
