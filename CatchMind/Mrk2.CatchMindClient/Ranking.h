#pragma once
#include "defines.h"

class USER_SCORE
{
public:
	int Score;
	int Rank;
	bool isPlayer;
	RECT rcRect[3];
	string strUserName;

	bool operator <(const USER_SCORE* &a) const
	{
		return this->Score < a->Score;
	}
};

class Bitmap;
class Ranking
{
private:

	HDC m_hMemDC;
	vector<USER_SCORE*> m_vecUserSocre;

	HBRUSH m_hBrush;
	HBRUSH m_hOld;

	RECT m_rcRank;

	Bitmap* m_pBitmap[MAXPLAYER];

	int m_iCurPlayer;

	void DrawRanking(HDC hdc);
public:
	void Init(HDC hdc);
	void InitChart();
	void SetPlayer(string strUserName);

	void SetScore(int Score, string strUserName);
	void Draw(HDC hdc);
	void ArrayScore();
	Ranking();
	~Ranking();
};

