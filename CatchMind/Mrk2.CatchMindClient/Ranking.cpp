#include "Ranking.h"
#include "ResourceManager.h"
#include "Bitmap.h"

Ranking::Ranking()
{
}


Ranking::~Ranking()
{
	for (auto iter = m_vecUserSocre.begin(); iter != m_vecUserSocre.end(); iter++)
	{
		SAFE_DETE_ARRAY(*iter);
		iter = m_vecUserSocre.erase(iter);
	}
	m_vecUserSocre.clear();
}

void Ranking::Init(HDC hdc)
{
	m_hMemDC = CreateCompatibleDC(hdc);
	for (int i = 0; i < MAXPLAYER; i++)
	{
		USER_SCORE* pUserScore = new USER_SCORE;
		pUserScore->Score = -1;
		pUserScore->Rank = MAXPLAYER;
		pUserScore->isPlayer = false;
		pUserScore->strUserName = "";
		m_vecUserSocre.push_back(pUserScore);
	}
	m_hBrush = CreateSolidBrush(RGB(63, 72, 204));
	m_hOld = (HBRUSH)SelectObject(m_hMemDC, m_hBrush);
	m_iCurPlayer = 0;
	for (int i = 0; i < MAXPLAYER; i++)
	{
		m_pBitmap[i]= ResourceManager::GetInstance()->GetBitmap(RES_TYPE_RANK_LINE);
	}
}


void Ranking::InitChart()
{
	for (int i = 0; i < MAXPLAYER; i++)
	{
		m_vecUserSocre[i]->Score = -1;
		m_vecUserSocre[i]->Rank = MAXPLAYER;
		m_vecUserSocre[i]->strUserName = "";
		m_vecUserSocre[i]->isPlayer = false;
	}
}
void Ranking::SetPlayer(string strUserName)
{
	USER_SCORE* pUserScore = new USER_SCORE;
	pUserScore->Score = 0;
	pUserScore->Rank = MAXPLAYER;
	pUserScore->isPlayer = true;
	pUserScore->strUserName = strUserName;

	m_vecUserSocre.push_back(pUserScore);
}
void Ranking::SetScore(int Score, string strUserName)
{
	USER_SCORE* pUserScore = new USER_SCORE;
	pUserScore->Score = Score;
	pUserScore->Rank = MAXPLAYER;
	pUserScore->isPlayer = true;
	pUserScore->strUserName = strUserName;
	m_vecUserSocre.push_back(pUserScore);
}

void Ranking::Draw(HDC hdc)
{
	FillRect(hdc, &m_rcRank, m_hBrush);
	DrawRanking(hdc);
}
void Ranking::ArrayScore()
{
	USER_SCORE* pUserTemp = NULL;

	for (int i = 0; i < m_vecUserSocre.size() - 1; i++)
	{
		for (int j = 0; j < m_vecUserSocre.size() - 1; j++)
		{
			if (m_vecUserSocre[j]->Score < m_vecUserSocre[j + 1]->Score)
			{
				pUserTemp = m_vecUserSocre[j];
				m_vecUserSocre[j] = m_vecUserSocre[j + 1];
				m_vecUserSocre[j + 1] = pUserTemp;
			}
		}
	}
	for (int i = 0; i < MAXPLAYER; i++)
	{
		m_vecUserSocre[i]->Rank = i;

		if (i > 0)
		{
			if (m_vecUserSocre[i]->Score == m_vecUserSocre[i - 1]->Score)
			{
				m_vecUserSocre[i]->Rank = m_vecUserSocre[i - 1]->Rank;
			}
		}
		

		m_vecUserSocre[i]->rcRect[0].left;
		m_vecUserSocre[i]->rcRect[0].left = RANK_POS_X + 5;
		m_vecUserSocre[i]->rcRect[0].top = RANK_POS_Y + (i*RANK_OFFSET_Y);
		m_vecUserSocre[i]->rcRect[0].right = m_vecUserSocre[i]->rcRect[0].left + RANK_WIDTH;
		m_vecUserSocre[i]->rcRect[0].bottom = m_vecUserSocre[i]->rcRect[0].top + RANK_HEIGHT;

		m_vecUserSocre[i]->rcRect[1].left = m_vecUserSocre[i]->rcRect[0].right + RANK_OFFSET_X;
		m_vecUserSocre[i]->rcRect[1].top = RANK_POS_Y + (i*RANK_OFFSET_Y);
		m_vecUserSocre[i]->rcRect[1].right = m_vecUserSocre[i]->rcRect[1].left + RANK_NAME_WIDTH;
		m_vecUserSocre[i]->rcRect[1].bottom = m_vecUserSocre[i]->rcRect[1].top + RANK_HEIGHT;

		m_vecUserSocre[i]->rcRect[2].left = m_vecUserSocre[i]->rcRect[1].right + RANK_OFFSET_X;
		m_vecUserSocre[i]->rcRect[2].top = RANK_POS_Y + (i*RANK_OFFSET_Y);
		m_vecUserSocre[i]->rcRect[2].right = m_vecUserSocre[i]->rcRect[2].left + RANK_SCORE_WIDTH;
		m_vecUserSocre[i]->rcRect[2].bottom = m_vecUserSocre[i]->rcRect[2].top + RANK_HEIGHT;
	}
}

void Ranking::DrawRanking(HDC hdc)
{
	int PlayerCount = 0;
	string strTemp;
	for (int i = 0; i < MAXPLAYER; i++)
	{
		if (m_vecUserSocre[i]->isPlayer == false)
			break;

		m_pBitmap[i]->DrawTransparent(hdc, RANK_POS_X, RANK_POS_Y + (i*RANK_OFFSET_Y),25,0);
		
		strTemp = to_string(m_vecUserSocre[i]->Rank+1)+"À§";
		DrawText(hdc, strTemp.c_str(), -1, &m_vecUserSocre[i]->rcRect[0], DT_CENTER | DT_NOCLIP);

		strTemp = m_vecUserSocre[i]->strUserName;
		DrawText(hdc, strTemp.c_str(), -1, &m_vecUserSocre[i]->rcRect[1], DT_CENTER | DT_NOCLIP);

		strTemp = to_string(m_vecUserSocre[i]->Score)+"°³";
		DrawText(hdc, strTemp.c_str(), -1, &m_vecUserSocre[i]->rcRect[2], DT_CENTER | DT_NOCLIP);
		PlayerCount++;
	}
	m_rcRank.left = RANK_POS_X;
	m_rcRank.top = RANK_POS_Y;
	m_rcRank.right = m_vecUserSocre[0]->rcRect[2].right;
	m_rcRank.bottom = m_rcRank.top + (PlayerCount*RANK_OFFSET_Y);
}