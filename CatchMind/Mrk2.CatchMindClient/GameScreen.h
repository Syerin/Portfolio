#pragma once
#include "defines.h"
class Bitmap;
class Player;
class GameScreen
{
private:
	//새로운DC
	HDC m_hMemDC;
	HWND m_hWnd;
	HBITMAP m_hBitmap;
	HBITMAP m_hOld;

	Bitmap* m_pBitmap;
	Bitmap* m_pBitmapReady;
	Bitmap* m_pBitmapStart;
	Bitmap* m_pBitmapSelectColor;
	Bitmap* m_pBitmapNextTurn;
	Bitmap* m_pBitmapCleanButton;
	Bitmap* m_pBitmapTrepass;

	RECT m_rcGameScreen;
	POINT m_ptMouse;

	RECT m_rcCleanButton;
	bool m_bCleanButton;

	DWORD m_curColor;

	GAME_STATE m_eCurGameState;

	HPEN m_hLbPen[COLOR_TYPE_END];
	HPEN m_hRbPen[COLOR_TYPE_END];
	HPEN m_hOldLbPen[COLOR_TYPE_END];
	HPEN m_hOldRbPen[COLOR_TYPE_END];


	HBRUSH m_hBrush[COLOR_TYPE_END];
	HBRUSH m_hOldBrush[COLOR_TYPE_END];

	RECT m_rcPaint[COLOR_TYPE_END];
	bool m_bLButtonDown;
	bool m_bSelectColor;

	string m_curDrawPlayer;
	string m_NextDrawPlayer;
	RECT m_rcCurDrawPlayer;
	RECT m_rcNextDrawPlayer;

	SOCKET m_Sock;
	bool m_bEffectStart;
	float m_fEffectStart;

	float m_fEffectNext;
	bool m_bEffectNext;

	float m_fEffectAnswer;
	bool m_bEffectAnswer;

	float m_fEffectTimeOut;
	bool m_bEffectTimeOut;

	int m_iPrevMouseX;
	int m_iPrevMouseY;

	int m_iOriginX;
	int m_iOriginY;
	bool m_bStrate;


	int m_iRound;
	string m_strRound;
	RECT m_rcCurRound;
	Bitmap* m_pBitmap_AllReady;

public:
	void Init(HDC hdc,HWND hWnd);

	void Draw(HDC hdc);
	
	void DrawingMouse(POINT ptMouse);
	void SendDrawing(SOCKET sock, POINT ptMouse);

	void RecvDrawMouseDown(int color, int type, int posX, int posY);
	void RecvDrawMouseMove(int color, int type, int posX, int posY);

	void Update(float fElapse);
	bool GetNextEffectEnd();
	bool GetAnswerEffectEnd();


	void DrawGameStateWait();
	void DrawGameStateGame();
	void DrawGameNextTurn();//순서
	void DrawGameAnswer();
	//void DrawGameTimeOut();

	void DrawCleanButton(HDC hdc);
	void DrawAllReady();
	void DrawTrepass();

	bool ClenaButton(POINT ptMouse);

	void SelectColor(POINT ptMouse);

	void SetGameState(GAME_STATE SetGameState);
	void SetNextTurn(string curPlayer, string NextPlayer);

	void ScreenClear();

	void SetRound(int Round);

	inline GAME_STATE GetGameState() { return m_eCurGameState; }

	inline void SetEffectNext(bool act)
	{
		m_bEffectNext = act;
	}

	GameScreen();
	~GameScreen();
};

