#pragma once
#include "defines.h"
#include "GameScreen.h"
struct PACKET_INFO
{
	char buf[BUFSIZE];
	int Sendbyte;
	int Recvbyte;
	int Len;
};

class Bitmap;
class PlayerManager;
class Player;
class GameScreen;
class Ranking;
class GameManager
{
private:
	HDC m_hMemDC;
	HWND m_hWnd;
	HBITMAP m_hBitmap;
	HBITMAP m_hOld;
	SOCKET m_Sock;
	HFONT m_hFont[FONT_TYPE_END];
	HFONT m_hOldFont[FONT_TYPE_END];

	bool m_bAbleReady;

	CLIENT_STATE m_eCurState;
	Bitmap* m_pBitmapLogin[RES_TYPE_LOGIN_ID- RES_TYPE_NAMEBLOCK];
	Bitmap* m_pBitmapReady[2];
	Bitmap* m_pBitmapStart;

	Ranking* m_pRanking;
	Player* m_pMyPlayer;
	PlayerManager* m_pPlayerManager;
	GameScreen* m_pGameScreen;
	PACKET_INFO m_stPacket;
	Bitmap* m_pBitmap;
	Bitmap* m_pBitmap_Explain;

	HWND m_hEdit;
	HWND m_hChat;

	string m_strName;
	string m_strNameCheck;
	string m_strNameLimit;

	string m_strQuestion;
	Bitmap* m_pBitmap_Question;
	RECT m_rcQuestion;
	SIZE m_Questionsize;

	RECT m_rcNameCheck;
	RECT m_rcNameLimit;
	RECT m_rcButton;
	RECT m_rcRound;
	string m_strRound;

	int m_iRoundEnd;
	int m_icurRound;

	POINT ptMouse;


	float m_fTimer;
	bool m_bTimer;
	RECT m_rcChat;
	string m_strChat;
	bool m_bDrawChat;
	GameManager();
	void DrawMyPlayer();
	DWORD m_dwLastTime;
	DWORD m_dwCurTime;
	float m_fElapsTime;
	bool m_bLbutton;
	bool m_bAnswer;

	float m_fGameOver;
	bool m_bGameOver;
	bool m_bTimerOut;

	string m_strTimer;
	RECT m_rcTimer;
	int m_iMin;
	int m_iSecon;
	int m_iMilSecon;
	float m_fMilisecond;
	float m_fTimerSecond;
	void SetTimeTimer();
	static GameManager* m_pThis;
public:
	void Init(HDC hdc,HWND hWnd,SOCKET sock, HINSTANCE hInstance);
	void Update();

	void Draw();
	void PlayerButton(POINT ptMouse);
	LRESULT SettingWindowOption(WPARAM wParam, LPARAM lParam);

	bool ProcessPacket(SOCKET sock);

	void SendLoginIdCheck(HWND hWnd, SOCKET sock);
	void SendChat(SOCKET sock);
	void SetRecvPacketInfo(char* buf, int len);

	bool GetCompletePacketProcess();
	void TimerTime(float fElapse);

	int GetSend(SOCKET sock, char* buf, int len);

	inline void SetSocket(SOCKET sock)
	{
		m_Sock = sock;
	}
	inline HWND GetWindowEdit()
	{
		return m_hEdit;
	}
	inline HWND GetWindowChat()
	{
		return m_hChat;
	}
	static GameManager* GetInstance()
	{
		if (m_pThis == NULL)
		{
			m_pThis = new GameManager();
		}
		return m_pThis;
	}

	~GameManager();
};

