#pragma once
#include "defines.h"
#include "../Mrk2.CatchMindClient/defines.h"
struct QUESTION_INFO
{
	bool prevSend;
	string strQuestion;
};
struct USER_INFO
{
	SOCKET sock;
	string strName;
	PLAYER_TYPE PlayerType; //호스트,일반,게스트
	PLAYER_STATE PlayerState;
	int PlayerNumber;
	bool m_bReady;
};
struct USERRANK
{
	SOCKET sock;
	string strName;
	int Score;
};
class Room
{
private:
	vector<USERRANK*> m_vecUserRank;
	vector<USER_INFO*> m_vecUser;
	map<int, QUESTION_INFO*> m_mapQuestion;
	int m_iPlayerCount;
	GAME_STATE m_eCurGameState;
	float m_fTimer;
	float m_fEffect;
	int CurDrawPlayer;
	int NextDrawPlayer;
	bool m_bPlayerNumber[MAXPLAYER];
	string m_curQuestion;

	int m_iQuestionCount;
public:
	void Init();
	void InitRoom();
	void EnterPlayer(SOCKET sock, char* buf);

	void JoinPlayer(SOCKET sock);
	void NoticeOtherPlayer(SOCKET sock);
	void ReadyPlayer(SOCKET sock, char* buf);

	void SendQuestion(SOCKET sock,char* buf);


	void StartGame(SOCKET sock,char* buf);
	void ChatPlayer(SOCKET sock, char* buf, int len);
	void AnswerPlayer(SOCKET sock, char* buf, int len);

	void NextTurn(SOCKET sock);

	void DrawMouseDown(SOCKET sock, char* buf);
	void DrawMouseMove(SOCKET sock, char* buf);

	bool actTressPass();
	bool GameOver();

	int GetNextRound();

	void ShutdonwDrawPlayer();
	void NextDrawingPlayer();

	int GetNextDrawing();

	void ExitPlayer(SOCKET sock, char* buf);
	void ShutDownPlayer(SOCKET sock, char* buf);

	void TransTresPassToGame();

	inline GAME_STATE GetGameState()
	{
		return m_eCurGameState;
	}
	inline int GetCurDrawPlayer()
	{
		CurDrawPlayer = NextDrawPlayer;
		return CurDrawPlayer;
	}
	inline int GetNextDrawPlayer()
	{
		return NextDrawPlayer;
	}

	Room();
	~Room();
};

