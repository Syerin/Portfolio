#pragma once
#include "defines.h"
#include "../Mrk2.CatchMindClient/defines.h"
enum PACKET_INDEX
{
	PACKET_INDEX_LOGIN = 1,
	PACKET_INDEX_LOGIN_ID_CHECK,//아이디확인
	PACKET_INDEX_LOGIN_ID_GENERATE,//아이디생성


	PACKET_INDEX_ROOM_WAIT,
	PACKET_INDEX_ROOM_IN,
	PACKET_INDEX_ROOM_PLAYER_INFO,

	PACKET_INDEX_ROOM_READY,
	PACKET_INDEX_ROOM_START,
	PACKET_INDEX_REQUEST_QUESTION,
	PACKET_INDEX_ROOM_CHAT,
	PAKCET_INDEX_ROOM_ANSWER,

	PACKET_INDEX_ROOM_NEXTTURN,

	PACKET_INDEX_DRAWDOWN,
	PACKET_INDEX_DRAWMOVE,

	PACKET_INDEX_ROOM_RANK,
	PACKET_INDEX_ROOM_GAMEOVER,
	PACKET_INDEX_ROOM_INITIALIZED,

	PACKET_INDEX_ROOM_OUT,
	PACKET_INDEX_TIMER,
	PACKET_INDEX_ROOM_SHUTDOWN_PLAYER,
	PACKET_INDEX_ROOM_SHUTDOWN_DRAW_PLYAER,
	PACKET_INDEX_ROOOM_SHUTDOWN_NOMAL_PLAYER,

	PACKET_INDEX_END,
};

#pragma pack(1)

struct PACKET_HEADER
{
	DWORD wIndex;
	DWORD wLen;
};
struct PACKET_ID
{
	PACKET_HEADER header;
	char Name[BUFSIZE];
};
struct PACKET_ID_GENERATE
{
	PACKET_HEADER header;
	bool avaliableId;
};
struct PACKET_LOGIN
{
	PACKET_HEADER header;
	int index;
};

struct PACKET_ROOM_IN
{
	PACKET_HEADER header; 
	int index;
	int type;
	PLAYER_STATE state;
	int curRound;
	int RoundEnd;

	char name[BUFSIZE];
};
struct PACKET_ROOM_PLAYER_INFO
{
	PACKET_HEADER header;
	int index; //몇번자리에
	PLAYER_TYPE type;
	PLAYER_STATE State;
	char name[BUFSIZE]; //이런사람이있다.
};
struct PACKET_ROOM_PLAYER_READY
{
	PACKET_HEADER header;
	int PlayerNumber;
	bool m_bReady;
};
struct PACKET_ROOM_START
{
	PACKET_HEADER header;
	bool bStart;
	int CurDrawPlayer;//지금 그릴 사람을 보내줌.
	int NextDrawPlayer;//다음 그릴 사람을 보내줌.
	int curRound;
};

struct PACKET_RANKING
{
	PACKET_HEADER header;
	int Socre;
	char PlayerName[BUFSIZE];
};

struct PACKET_TIMER
{
	PACKET_HEADER header;
	float ElapseTime;
	int curRound;
	int CurDrawPlayer;
	int NextDrawPlayer;
};
struct PACKET_ROOM_CHAT
{
	PACKET_HEADER header;
	int index;
	char Chat[BUFSIZE];
};
struct PACKET_ROOM_ANSWER
{
	PACKET_HEADER header;
	int index;//맞춘사람넘버.
	char buf[BUFSIZE];
};
struct PACKET_REQUEST_QUESTION
{
	PACKET_HEADER header;
	char Question[BUFSIZE];
};
struct PACKET_ROOM_NEXTTURN
{
	PACKET_HEADER header;
	int curDrawPlayer;
	int NextDrawPlayer;
	int curRound;
};
struct PACKET_ROOM_DRAWDOWN
{
	PACKET_HEADER header;
	int Color;//색상
	int BrushType;//좌,우,
	int OriginX; //찍은곳
	int OriginY;
};
struct PACKET_ROOM_DRAWMOVE
{
	PACKET_HEADER header;
	int Color;//색상
	int BrushType;
	WORD OriginX; //찍은곳
	WORD OriginY;
};
struct PACKET_ROOM_SHUTDOWN_DRAW_PLYAER
{
	PACKET_HEADER header;
	int index;
	int NextHost;
	bool GameOver;
};
struct PACKET_ROOM_SHUTDOWN_NOMAL_PLAYER
{
	PACKET_HEADER header;
	int index;
	int NextHost;
	bool actTrespass;
};
struct PACKET_ROOM_SHUTDOWN_PLAYER
{
	PACKET_HEADER header;
	int index;
	int NextHost;
};
struct PACKET_ROOM_INITIALIZED
{
	PACKET_HEADER header;
	PLAYER_STATE PlayerState;
};
struct PACKET_ROOM_GAME_OVER
{
	PACKET_HEADER header;
};


#pragma pack()
